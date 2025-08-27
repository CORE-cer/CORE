#pragma once

#include <quill/Frontend.h>
#include <quill/LogMacros.h>
#include <quill/Logger.h>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iterator>
#include <memory>
#include <stack>
#include <string>
#include <tracy/Tracy.hpp>
#include <utility>
#include <vector>

#include "complex_event.hpp"
#include "core_server/internal/evaluation/enumeration/tecs/time_reservator.hpp"
#include "node.hpp"
#include "shared/datatypes/eventWrapper.hpp"
#include "tecs.hpp"

namespace CORE::Internal::tECS {

class Enumerator {
  friend class iterator;

  class iterator {
   private:
    Enumerator& enumerator;
    bool is_end;
    uint64_t current_number_result = 0;

   public:
    iterator(Enumerator& enumerator, bool is_end)
        : enumerator(enumerator), is_end(is_end) {}

    ComplexEvent operator*() { return enumerator.next(); }

    bool operator!=(const iterator& other) const { return is_end != other.is_end; }

    iterator& operator++() {
      ZoneScopedN("Internal::Enumerator::iterator::operator++");
      current_number_result++;
      if (current_number_result == enumerator.enumeration_limit
          || !enumerator.has_next()) {
        is_end = true;
      }
      return *this;
    }
  };

  quill::Logger* logger = quill::Frontend::get_logger("root");
  std::stack<std::pair<Node*, std::vector<Types::EventWrapper>>> stack;
  uint64_t original_pos;
  uint64_t last_time_to_consider;
  std::pair<std::pair<uint64_t, uint64_t>, std::vector<Types::EventWrapper>> next_value;
  Node* original_node{nullptr};
  std::shared_ptr<tECS> tecs{nullptr};
  TimeReservator* time_reservator{nullptr};
  TimeReservator::Node* time_reserved_node{nullptr};
  int64_t enumeration_limit;

 public:
  Enumerator(Node* node,
             uint64_t original_pos,
             uint64_t time_window,
             std::shared_ptr<tECS> tecs,
             TimeReservator* time_reservator,
             int64_t enumeration_limit)
      : original_pos(original_pos),
        last_time_to_consider((original_pos < time_window) ? 0
                                                           : original_pos - time_window),
        original_node(node),
        tecs(tecs),
        time_reservator(time_reservator),
        enumeration_limit(enumeration_limit) {
    assert(time_reservator != nullptr);
    time_reserved_node = time_reservator->reserve(last_time_to_consider);
    assert(node != nullptr);
    if (node->max() >= last_time_to_consider) {
      stack.emplace(node, std::vector<Types::EventWrapper>());
    }
  }

  /// The copy constructor is deleted so that it does not inferfere with
  /// the delete operator of the enumerator.
  Enumerator(const Enumerator&) = delete;

  /// The copy assignment is deleted so that it does not inferfere with
  /// the delete operator of the enumerator.
  Enumerator& operator=(const Enumerator&) = delete;

  // Allow move constructor
  Enumerator(Enumerator&& other) noexcept
      : stack(std::move(other.stack)),
        original_pos(other.original_pos),
        last_time_to_consider(other.last_time_to_consider),
        next_value(std::move(other.next_value)),
        original_node(other.original_node),
        tecs(other.tecs),
        time_reservator(other.time_reservator),
        time_reserved_node(other.time_reserved_node),
        enumeration_limit(other.enumeration_limit) {
    other.tecs = nullptr;
    other.time_reservator = nullptr;
    other.time_reserved_node = nullptr;
  }

  // Allow move assignment
  Enumerator& operator=(Enumerator&& other) noexcept {
    if (this != &other) {
      // cleanup();
      stack = std::move(other.stack);
      original_pos = other.original_pos;
      last_time_to_consider = other.last_time_to_consider;
      next_value = std::move(other.next_value);
      original_node = other.original_node;
      tecs = other.tecs;
      time_reservator = other.time_reservator;
      time_reserved_node = other.time_reserved_node;
      enumeration_limit = other.enumeration_limit;
      other.tecs = nullptr;
      other.time_reservator = nullptr;
      other.time_reserved_node = nullptr;
    }
    return *this;
  }

  Enumerator()
      : original_node(nullptr),
        tecs(nullptr),
        time_reservator{nullptr},
        time_reserved_node{nullptr} {}  // Empty enumerator

  ~Enumerator() {
    if (tecs != nullptr) {
      cleanup();
    }
  }

  iterator begin() { return iterator(*this, !has_next() || enumeration_limit == 0); }

  iterator end() { return iterator(*this, true); }

  void reset() {
    stack = {};
    if (original_node != nullptr && original_node->max() >= last_time_to_consider) {
      stack.emplace(original_node, std::vector<Types::EventWrapper>());
    }
  };

 private:
  bool has_next() {
    ZoneScopedN("Internal::Enumerator::has_next");
    while (!stack.empty()) {
      Node* current_node = stack.top().first;
      std::vector<Types::EventWrapper> events = std::move(stack.top().second);
      stack.pop();
      while (true) {
        if (current_node->is_bottom()) {
          next_value = std::make_pair(std::make_pair(current_node->pos(), original_pos),
                                      std::move(events));
          return true;
        } else if (current_node->is_output()) {
          assert(current_node->get_event_clone().marked_variables.has_value());
          events.push_back(current_node->get_event_clone());
          current_node = current_node->next();
        } else if (current_node->is_union()) {
          if (current_node->get_right()->max() >= last_time_to_consider) {
            std::vector<Types::EventWrapper> events_copy = {};
            std::transform(events.begin(),
                           events.end(),
                           std::back_inserter(events_copy),
                           [](auto& event) { return event.clone(); });
            stack.push({current_node->get_right(), std::move(events_copy)});
          }
          current_node = current_node->get_left();
        }
      }
    }
    return false;
  }

  /// It requires has_next to be evaluated before.
  ComplexEvent next() {
    ZoneScopedN("Internal::Enumerator::next");
    std::reverse(next_value.second.begin(), next_value.second.end());
    auto complex_event = ComplexEvent(std::move(next_value));
#if QUILL_ACTIVE_LOG_LEVEL <= QUILL_LOG_LEVEL_TRACE_L2
    std::string complex_event_string = complex_event.to_string<true>();
    LOG_TRACE_L2(logger, "Returning complex event: {}", complex_event_string);
#endif
    return complex_event;
  }

  inline void cleanup() {
    if (tecs != nullptr) {
      assert(tecs != nullptr);
      tecs->unpin(original_node);
      assert(time_reservator != nullptr);
      assert(time_reserved_node != nullptr);
      time_reservator->remove_node(time_reserved_node);
      time_reserved_node = nullptr;
      tecs = nullptr;
    }
  }
};
}  // namespace CORE::Internal::tECS
