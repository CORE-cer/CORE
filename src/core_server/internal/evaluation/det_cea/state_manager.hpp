#pragma once

#include <cstdlib>

#include "core_server/internal/evaluation/minipool/minipool.hpp"
#include "state.hpp"

namespace CORE::Internal::CEA::Det {

const size_t STATE_MANAGER_STARTING_SIZE = 64;
const size_t STATE_MANAGER_MAX_SIZE = 512;

/**
 * The Node Manager class stores the pointers to all allocated
 * state nodes. When we need a state node, but we are out of memory,
 * we remove one of the existing nodes and reuse it.
 */
class StateManager {
  using StatePool = MiniPool::MiniPool<State>;
  using States = Det::State::States;

 private:
  size_t amount_of_used_states{0};
  size_t amount_of_allowed_states;
  StatePool* minipool_head = nullptr;
  State* evictable_state_head = nullptr;
  State* evictable_state_tail = nullptr;
  std::vector<State*> states;
  std::map<mpz_class, uint64_t> states_bitset_to_index;

 public:
  StateManager()
      : minipool_head(new StatePool(STATE_MANAGER_STARTING_SIZE)),
        amount_of_allowed_states(STATE_MANAGER_MAX_SIZE) {}

  StateManager(StateManager&& other) noexcept
      : amount_of_used_states(other.amount_of_used_states),
        amount_of_allowed_states(other.amount_of_allowed_states),
        minipool_head(other.minipool_head) {
    other.minipool_head = nullptr;
  }

  ~StateManager() {
    for (StatePool* mp = minipool_head; mp != nullptr;) {
      StatePool* next = mp->prev();
      delete mp;
      mp = next;
    }
  }

  // Update the last used iteration of the states in the transition
  void update_iteration_states(const States& next_states,
                               const uint64_t& current_iteration) {
    assert(next_states.marked_state != nullptr);
    assert(next_states.unmarked_state != nullptr);
    update_last_used_iteration_state(next_states.marked_state,
                                     current_iteration);
    update_last_used_iteration_state(next_states.unmarked_state,
                                     current_iteration);
  }

  void update_evicted_states(const std::vector<State*>& evicted_states,
                             const uint64_t& current_iteration) {
    for (State* state : evicted_states) {
      if (state->is_evictable(current_iteration)) {
        set_evictable_state(state);
        if (this->evictable_state_head == nullptr) {
          this->evictable_state_head = state;
        }
      }
    }
  }

  void update_last_used_iteration_state(State* const& state,
                                        const uint64_t& current_iteration) {
    state->last_used_iteration = current_iteration;
    unset_evictable_state(state);
  }

  std::string to_string() {
    std::string out = "";
    out += "Number of initialized states: " + std::to_string(states.size())
           + "\n";
    out += "Initialized States:\n";
    for (auto& state : states) {
      out += state->states.get_str(2);
    }
    return out;
  }

  State* create_or_return_existing_state(mpz_class bitset,
                                         const uint64_t& current_iteration,
                                         CEA& cea) {
    auto it = states_bitset_to_index.find(bitset);
    if (it != states_bitset_to_index.end()) {
      assert(it->second < states.size());
      return states[it->second];
    } else {
      State* state = alloc(current_iteration, bitset, cea);
      return state;
    }
  }

 private:
  template <class... Args>
  State* alloc(const uint64_t current_iteration, Args&&... args) {
    State* new_state;
    new_state = allocate_state(std::forward<Args>(args)...,
                               current_iteration);
    if (new_state == nullptr) {
      // Not enough memory, try to evict a state.
      new_state = evictable_state_head;
      if (new_state != nullptr) {
        // Successfully evicted a state, reset it and return it.
        reset_state(new_state,
                    current_iteration,
                    std::forward<Args>(args)...);
      } else {
        // Not enough memory, force increase the memory pool.
        size_t amount_force_added_states = increase_mempool_size();
        amount_of_allowed_states += amount_force_added_states;
        std::cout << "Forcing memory pool increase, increasing allowed "
                     "states to "
                  << amount_of_allowed_states << std::endl;
        new_state = allocate_state(std::forward<Args>(args)...,
                                   current_iteration);
      }
    } else {
    }
    assert(new_state != nullptr);
    return new_state;
  }

  template <class... Args>
  State* allocate_state(Args&&... args) {
    if (amount_of_used_states < amount_of_allowed_states) {
      if (minipool_head->is_full()) {
        increase_mempool_size();
      }
      amount_of_used_states++;
      State* state = minipool_head->alloc(std::forward<Args>(args)...);
      // Add the state to the list of states as is a new state.
      states.push_back(state);
      states_bitset_to_index[state->states] = states.size() - 1;
      return state;
    } else {
      return nullptr;
    }
  }

  /**
   * Increase the size of the memory pool by a factor of 2.
   *
   * @return The new size of the memory pool.
   */
  size_t increase_mempool_size() {
    size_t new_size = minipool_head->size() * 2;
    StatePool* new_minipool = new StatePool(new_size);
    minipool_head->set_next(new_minipool);
    new_minipool->set_prev(minipool_head);

    minipool_head = new_minipool;

    return new_size;
  }

  void set_evictable_state(State* const& state) {
    assert_state_list_consistency();
    if (evictable_state_head == nullptr) {
      evictable_state_head = state;
    }
    if (evictable_state_tail != nullptr) {
      evictable_state_tail->prev_evictable_state = state;
    }
    state->set_evictable(evictable_state_tail);
    evictable_state_tail = state;
    assert_state_list_consistency();
  }

  void unset_evictable_state(State* const& state) {
    assert_state_list_consistency();
    if (evictable_state_head == state) {
      evictable_state_head = state->prev_evictable_state;
    }
    if (evictable_state_tail == state) {
      evictable_state_tail = state->next_evictable_state;
    }
    state->unset_evictable();
    assert_state_list_consistency();
  }

  inline void assert_state_list_consistency() {
    // Check that either both are nullptr or both are not nullptr and evicted
    assert(
      (evictable_state_head == nullptr && evictable_state_tail == nullptr)
      || (evictable_state_head != nullptr
          && evictable_state_tail != nullptr));

    assert(amount_of_used_states <= amount_of_allowed_states);

    State* state = evictable_state_head;
#ifdef CORE_DEBUG
    while (state != nullptr) {
      if (state->prev_evictable_state != nullptr) {
        assert(state->prev_evictable_state->next_evictable_state == state);
      }
      if (state->next_evictable_state != nullptr) {
        assert(state->next_evictable_state->prev_evictable_state == state);
      }
      if (state->prev_evictable_state == nullptr) {
        assert(state == evictable_state_tail);
      }
      state = state->prev_evictable_state;
    }
#endif
  }

  template <class... Args>
  void reset_state(State* const& state,
                   const uint64_t current_iteration,
                   Args&&... args) {
    mpz_class old_states = state->states;
    unset_evictable_state(state);
    state->reset(std::forward<Args>(args)..., current_iteration);
    states_bitset_to_index[state->states] = states_bitset_to_index[old_states];
    states_bitset_to_index.erase(old_states);
  }
};
}  // namespace CORE::Internal::CEA::Det
