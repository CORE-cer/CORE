#pragma once

#include <gmpxx.h>

#include <atomic>
#include <cassert>
#include <chrono>  // NOLINT
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#include "core_server/internal/ceql/query/consume_by.hpp"
#include "core_server/internal/ceql/query/limit.hpp"
#include "core_server/internal/evaluation/enumeration/tecs/enumerator.hpp"
#include "core_server/internal/evaluation/enumeration/tecs/node.hpp"
#include "det_cea/det_cea.hpp"
#include "det_cea/state.hpp"
#include "enumeration/tecs/tecs.hpp"
#include "predicate_evaluator.hpp"
#include "quill/Frontend.h"
#include "quill/LogMacros.h"
#include "quill/Logger.h"
#include "shared/datatypes/eventWrapper.hpp"
#include "tracy/Tracy.hpp"

namespace CORE::Internal::Evaluation {
class Evaluator {
 private:
  using UnionList = std::vector<tECS::Node*>;
  using State = CEA::Det::State;
  using Node = tECS::Node;
  quill::Logger* logger = quill::Frontend::get_logger("root");

  //                                   // Name in paper
  CEA::DetCEA& cea;                    // A
  PredicateEvaluator tuple_evaluator;  // t generator
  uint64_t time_window;                // ε

  std::unordered_map<State*, UnionList> historic_union_list_map;  // T
  std::vector<State*> historic_ordered_keys;
  std::unordered_map<State*, UnionList> current_union_list_map;  // T'
  std::vector<State*> current_ordered_keys;

  std::vector<State*> final_states;

  uint64_t actual_time;

  uint64_t current_iteration = 0;  // Current iteration of the algorithm as seen by next().

  /**
   * All events less than event_time_of_expiration can have it's children
   * recycled and marked as a dead node. Note that this uint64_t is passed
   * by reference to the tECS, and the tECS manages this behavior. This
   * uint64_t is a reference of the uint64_t stored at the mediator.
   */
  std::atomic<uint64_t>& event_time_of_expiration;

  // Other auxiliary objects

  // Use shared_ptr since enumerator may outlive the evaluator.
  std::shared_ptr<tECS::tECS> tecs{nullptr};

  CEQL::ConsumeBy::ConsumptionPolicy consumption_policy;
  CEQL::Limit enumeration_limit;

// Only in debug, check tuples are being sent in ascending order.
#ifdef CORE_DEBUG
  uint64_t last_tuple_time = 0;
#endif

 public:
  std::atomic<bool> should_reset = false;

  Evaluator(CEA::DetCEA& cea,
            PredicateEvaluator&& tuple_evaluator,
            uint64_t time_bound,
            std::atomic<uint64_t>& event_time_of_expiration,
            CEQL::ConsumeBy::ConsumptionPolicy consumption_policy,
            CEQL::Limit enumeration_limit)
      : cea(cea),
        tuple_evaluator(std::move(tuple_evaluator)),
        time_window(time_bound),
        event_time_of_expiration(event_time_of_expiration),
        tecs(std::make_shared<tECS::tECS>(event_time_of_expiration)),
        consumption_policy(consumption_policy),
        enumeration_limit(enumeration_limit) {}

  Evaluator(CEA::DetCEA& cea,
            const PredicateEvaluator& tuple_evaluator,
            uint64_t time_bound,
            std::atomic<uint64_t>& event_time_of_expiration,
            CEQL::ConsumeBy::ConsumptionPolicy consumption_policy,
            CEQL::Limit enumeration_limit)
      : cea(cea),
        tuple_evaluator(tuple_evaluator),
        time_window(time_bound),
        event_time_of_expiration(event_time_of_expiration),
        tecs(std::make_shared<tECS::tECS>(event_time_of_expiration)),
        consumption_policy(consumption_policy),
        enumeration_limit(enumeration_limit) {}

  std::optional<tECS::Enumerator>
  next(Types::EventWrapper&& event, uint64_t current_time) {
    ZoneScopedN("Evaluator::next");

    LOG_TRACE_L3(logger,
                 "Received tuple with timestamp {} in Evaluator::next",
                 event.get_primary_time().val);
#if QUILL_COMPILE_ACTIVE_LOG_LEVEL <= QUILL_LOG_LEVEL_TRACE_L2
    LOG_TRACE_L2(logger,
                 "Event type ID: {}",
                 event.get_event_reference().get_event_type_id());
    for (const auto& attr : event.get_event_reference().attributes) {
      LOG_TRACE_L2(logger, "Attribute: {}", attr->to_string());
    }
    auto start_time = std::chrono::steady_clock::now();
#endif
// If in debug, check tuples are being sent in ascending order.
#ifdef CORE_DEBUG
    if (current_time < last_tuple_time) {
      std::string attributes = event.get_event_reference().to_string();
      LOG_CRITICAL(logger,
                   "Received tuple with timestamp {} in Evaluator::next, "
                   "but the last tuple time was {}. Attributes: {}",
                   current_time,
                   last_tuple_time,
                   attributes);
      std::this_thread::sleep_for(std::chrono::nanoseconds(500000000));
      assert(false && "Received tuple out of order in Evaluator::next");
    }
    last_tuple_time = current_time;
#endif
    // current_time is j in the algorithm.
    event_time_of_expiration = current_time < time_window ? 0 : current_time - time_window;
    LOG_TRACE_L2(logger,
                 "Event time of expiration set to {}",
                 event_time_of_expiration.load());
    LOG_TRACE_L2(logger, "Time window is set to {}", time_window);

    if (should_reset.load()) {
      reset();
      should_reset.store(false);
    }

    mpz_class predicates_satisfied = tuple_evaluator(event);
    current_union_list_map = {};
    current_ordered_keys = {};
    final_states.clear();
    actual_time = current_time;
    UnionList ul = tecs->new_ulist(
      tecs->new_bottom(std::move(event.clone()), current_time));
    State* q0 = get_initial_state();
    exec_trans(event, q0, std::move(ul), predicates_satisfied, current_time);

    for (State* p : historic_ordered_keys) {
      assert(historic_union_list_map.contains(p));
      UnionList& actual_ul = historic_union_list_map[p];
      if (is_ul_out_time_window(actual_ul)) {
        tecs->unpin(actual_ul);
      } else {
        remove_out_of_time_nodes_ul(actual_ul);
        exec_trans(event,
                   p,
                   std::move(actual_ul),
                   predicates_satisfied,
                   current_time);  // Send the tuple in exec_trans.
      }
    }
    // Update the evicted states.
    cea.state_manager.unpin_states(historic_ordered_keys);
    historic_union_list_map = std::move(current_union_list_map);
    historic_ordered_keys = std::move(current_ordered_keys);
    current_iteration++;

    bool has_output = !final_states.empty();

    if (has_output) {
      LOG_TRACE_L2(logger, "Outputting in Evaluator");
      tECS::Enumerator enumerator = output();
      assert(enumeration_limit.result_limit == 0
             || (enumerator.begin() != enumerator.end() && (enumerator.reset(), true)));
      if (consumption_policy == CEQL::ConsumeBy::ConsumptionPolicy::ANY
          || consumption_policy == CEQL::ConsumeBy::ConsumptionPolicy::PARTITION) {
        LOG_TRACE_L3(logger,
                     "Setting should_reset to true due to consumption policy in "
                     "Evaluator");
        should_reset.store(true);
      }
#if QUILL_COMPILE_ACTIVE_LOG_LEVEL <= QUILL_LOG_LEVEL_TRACE_L2
      auto end_time = std::chrono::steady_clock::now();
#endif
      LOG_TRACE_L2(logger,
                   "Took {} seconds to process tuple with timestamp {}",
                   std::chrono::duration_cast<std::chrono::nanoseconds>(end_time
                                                                        - start_time)
                     .count(),
                   event.get_primary_time().val);
      return std::move(enumerator);
    }
#if QUILL_COMPILE_ACTIVE_LOG_LEVEL <= QUILL_LOG_LEVEL_TRACE_L2
    auto end_time = std::chrono::steady_clock::now();
#endif
    LOG_TRACE_L2(
      logger,
      "Took {} seconds to process tuple with timestamp {}",
      std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count(),
      event.get_primary_time().val);
    return {};
  }

 private:
  State* get_initial_state() { return cea.initial_state; }

  void reset() {
    LOG_TRACE_L3(logger, "Resetting historic states in Evaluator");
    cea.state_manager.unpin_states(historic_ordered_keys);
    historic_ordered_keys.clear();
    for (auto& [state, ul] : historic_union_list_map) {
      tecs->unpin(ul);
    }
  }

  bool is_ul_out_time_window(const UnionList& ul) {
    ZoneScopedN("Evaluator::is_ul_out_time_window");
    return (ul.at(0)->maximum_start < event_time_of_expiration);
  }

  void remove_out_of_time_nodes_ul(UnionList& ul) {
    ZoneScopedN("Evaluator::remove_dead_nodes_ul");
    for (auto it = ul.begin(); it != ul.end();) {
      Node* ul_node = *it;
      if (ul_node->maximum_start < event_time_of_expiration) {
        it = ul.erase(it);
        tecs->unpin(ul_node);
      } else {
        ++it;
      }
    }
  }

  void exec_trans(Types::EventWrapper& event,
                  State* p,
                  UnionList&& ul,
                  mpz_class& t,
                  uint64_t current_time) {
    // exec_trans places all the code of add into exec_trans.
    ZoneScopedN("Evaluator::exec_trans");
    LOG_TRACE_L3(logger,
                 "Received tuple with timestamp {} in Evaluator::exec_trans",
                 event.get_primary_time().val);
    assert(p != nullptr);
    State::TransitionTargetStatesWithMarkings
      next_states_with_markings = cea.next(p, t, current_iteration);
    bool recycle_ulist = false;
    mpz_class empty_marked_variables = mpz_class(0);
    for (auto& [state, marked_variables] :
         next_states_with_markings.state_marked_variables_pair) {
      assert(state != nullptr);
      if (state->is_empty) {
        continue;
      }
      if (marked_variables != empty_marked_variables) {
        Node* new_node = tecs->new_extend(tecs->merge(ul),
                                          event,
                                          current_time,
                                          marked_variables);
        if (current_union_list_map.contains(state)) {
          current_union_list_map[state] = tecs->insert(std::move(
                                                         current_union_list_map[state]),
                                                       new_node);
        } else {
          UnionList new_ulist = tecs->new_ulist(new_node);
          current_ordered_keys.push_back(state);
          cea.state_manager.pin_state(state);
          current_union_list_map[state] = new_ulist;
          if (state->is_final) {
            final_states.push_back(state);
          }
        }
      } else {
        if (current_union_list_map.contains(state)) {
          Node* new_node = tecs->merge(ul);
          current_union_list_map[state] = tecs->insert(std::move(
                                                         current_union_list_map[state]),
                                                       new_node);
        } else {
          current_ordered_keys.push_back(state);
          cea.state_manager.pin_state(state);
          current_union_list_map[state] = ul;
          recycle_ulist = true;
          if (state->is_final) {
            final_states.push_back(state);
          }
        }
      }
    }
    if (!recycle_ulist) {
      tecs->unpin(ul);
    }
  }

  // Change to tECS::Enumerator.
  tECS::Enumerator output() {
    ZoneScopedN("Evaluator::output");
    Node* out = nullptr;
    for (auto it = final_states.rbegin(); it != final_states.rend(); ++it) {
      State* p = *it;
      // If using ANY consumption policy, this assert will always fail due resetting state
      assert(historic_union_list_map.contains(p));
      Node* n = tecs->merge(historic_union_list_map[p]);
      // Aca hacer el union del nodo antiguo (si hay) con el nuevo nodo.
      if (out == nullptr) {
        out = n;
      } else {
        out = tecs->new_direct_union(n, out);
      }
    }
    // TODO: Take off the if statement when fixing online_query_evaluator empty enumerator problem
    if (out == nullptr) {
      return {};
    } else {
      tecs->pin(out);
      return {out,
              actual_time,
              time_window,
              tecs,
              tecs->time_reservator,
              enumeration_limit.result_limit};
    }
  }

  bool is_empty() const {
    return historic_union_list_map.empty() && current_union_list_map.empty();
  }
};

}  // namespace CORE::Internal::Evaluation
