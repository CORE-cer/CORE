#pragma once
#include <algorithm>
#include <stack>
#include <vector>

#include "node.hpp"
#include "tecs.hpp"

namespace CORE::Internal::tECS {

class Enumerator {
  friend class iterator;

  class iterator {
   private:
    Enumerator& enumerator;
    bool is_end;

   public:
    iterator(Enumerator& enumerator, bool is_end)
        : enumerator(enumerator), is_end(is_end) {}

    std::pair<std::pair<uint64_t, uint64_t>, std::vector<RingTupleQueue::Tuple>>
    operator*() {
      return enumerator.next();
    }

    bool operator!=(const iterator& other) const {
      return is_end != other.is_end;
    }

    iterator& operator++() {
      if (!enumerator.has_next()) {
        is_end = true;
      }
      return *this;
    }
  };

  std::stack<std::pair<Node*, std::vector<RingTupleQueue::Tuple>>> stack;
  uint64_t original_pos;
  uint64_t last_time_to_consider;
  std::pair<std::pair<uint64_t, uint64_t>, std::vector<RingTupleQueue::Tuple>>
    next_value;
  Node* original_node{nullptr};
  tECS* tecs{nullptr};
  TimeReservator* time_reservator{nullptr};
  TimeReservator::Node* time_reserved_node{nullptr};

 public:
  Enumerator(Node* node,
             uint64_t original_pos,
             uint64_t time_window,
             tECS& tecs,
             TimeReservator* time_reservator)
      : original_pos(original_pos),
        last_time_to_consider(
          (original_pos < time_window) ? 0 : original_pos - time_window),
        original_node(node),
        tecs(&tecs),
        time_reservator(time_reservator) {
    assert(time_reservator != nullptr);
    time_reserved_node = time_reservator->reserve(last_time_to_consider);
    assert(node != nullptr);
    if (node->max() >= last_time_to_consider) {
      stack.push({node, {}});
    }
  }

  /// The copy constructor is deleted so that it does not inferfere with
  /// the delete operator of the enumerator.
  Enumerator(const Enumerator&) = delete;

  Enumerator()
      : original_node(nullptr),
        tecs(nullptr),
        time_reservator{nullptr},
        time_reserved_node{nullptr} {}  // Empty enumerator

  ~Enumerator() {
    if (tecs != nullptr) {
      // It is not an empty enumerator
      tecs->unpin(original_node);
      assert(time_reservator != nullptr);
      assert(time_reserved_node != nullptr);
      time_reservator->remove_node(time_reserved_node);
      time_reserved_node = nullptr;
      tecs = nullptr;
    }
  }

  iterator begin() { return iterator(*this, !has_next()); }

  iterator end() { return iterator(*this, true); }

  void reset() {
    stack = {};
    if (original_node != nullptr) {
      stack.push({original_node, {}});
    }
  };

 private:
  bool has_next() {
    while (!stack.empty()) {
      Node* current_node = stack.top().first;
      std::vector<RingTupleQueue::Tuple> tuples = stack.top().second;
      stack.pop();
      while (true) {
        if (current_node->is_bottom()) {
          next_value = {{current_node->pos(), original_pos}, tuples};
          return true;
        } else if (current_node->is_output()) {
          tuples.push_back(current_node->get_tuple());
          current_node = current_node->next();
        } else if (current_node->is_union()) {
          if (current_node->get_right()->max() >= last_time_to_consider) {
            stack.push({current_node->get_right(), tuples});
          }
          current_node = current_node->get_left();
        }
      }
    }
    return false;
  }

  /// It requires has_next to be evaluated before.
  std::pair<std::pair<uint64_t, uint64_t>, std::vector<RingTupleQueue::Tuple>>
  next() {
    std::reverse(next_value.second.begin(), next_value.second.end());
    return next_value;
  }
};
}  // namespace CORE::Internal::tECS
