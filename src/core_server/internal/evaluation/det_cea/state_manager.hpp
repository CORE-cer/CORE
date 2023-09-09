#pragma once

#include <cstdlib>

#include "core_server/internal/evaluation/minipool/minipool.hpp"
#include "state.hpp"

namespace CORE::Internal::CEA::Det {

const size_t STATE_MANAGER_STARTING_SIZE = 1;
const size_t STATE_MANAGER_MAX_SIZE = 2;

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

  template <class... Args>
  State* alloc(const uint64_t current_iteration, Args&&... args) {
    State* new_state;
    new_state = allocate_state((args)..., current_iteration);
    if (new_state == nullptr) {
      new_state = evictable_state_head;
      if (new_state != nullptr) {
        unset_evictable_state(new_state);
        new_state->reset((args)..., current_iteration);
      } else {
        size_t amount_force_added_states = increase_mempool_size();
        amount_of_allowed_states += amount_force_added_states;
        std::cout << "Forcing memory pool increase, increasing allowed "
                     "states to "
                  << amount_of_allowed_states << std::endl;
        new_state = allocate_state((args)..., current_iteration);
      }
    }
    assert(new_state != nullptr);
    return new_state;
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

  void update_evicated_states(const std::vector<State*>& evicted_states,
                              const uint64_t& current_iteration) {
    for (State* state : evicted_states) {
      std::cout << "Trying to evict state: " << state->id << std::endl;
      if (state->is_evictable(current_iteration)) {
        std::cout << "Evicting state: " << state->id << std::endl;
        state->set_evictable(this->evictable_state_head);
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

 private:
  template <class... Args>
  State* allocate_state(Args&&... args) {
    std::cout << "Amount of used states: " << amount_of_used_states
              << std::endl;
    std::cout << "Amount of allowed states: " << amount_of_allowed_states
              << std::endl;
    if (amount_of_used_states < amount_of_allowed_states) {
      if (minipool_head->is_full()) {
        increase_mempool_size();
      }
      amount_of_used_states++;
      return minipool_head->alloc((args)...);
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
    if (evictable_state_head == nullptr) {
      evictable_state_head = state;
    }
    if (evictable_state_tail != nullptr) {
      evictable_state_tail->prev_evictable_state = state;
    }
    state->set_evictable(evictable_state_tail);
    evictable_state_tail = state;
  }

  void unset_evictable_state(State* const& state) {
    if (evictable_state_head == state) {
      evictable_state_head = state->prev_evictable_state;
    }
    if (evictable_state_tail == state) {
      evictable_state_tail = nullptr;
    }
    state->unset_evictable();
  }
};
}  // namespace CORE::Internal::CEA::Det
