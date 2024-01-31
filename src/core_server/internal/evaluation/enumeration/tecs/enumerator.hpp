#pragma once
#include <algorithm>
#include <stack>
#include <vector>

#include "complex_event.hpp"
#include "node.hpp"
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
      if (current_number_result >= enumerator.enumeration_limit
          || !enumerator.has_next()) {
        is_end = true;
      }
      current_number_result++;
      return *this;
    }
  };

  std::stack<std::pair<Node*, std::vector<RingTupleQueue::Tuple>>> stack;
  uint64_t original_pos;
  uint64_t last_time_to_consider;
  std::pair<std::pair<uint64_t, uint64_t>, std::vector<RingTupleQueue::Tuple>> next_value;
  Node* original_node{nullptr};
  tECS* tecs{nullptr};
  TimeReservator* time_reservator{nullptr};
  TimeReservator::Node* time_reserved_node{nullptr};
  uint64_t enumeration_limit;

 public:
  Enumerator(Node* node,
             uint64_t original_pos,
             uint64_t time_window,
             tECS& tecs,
             TimeReservator* time_reservator)
      : original_pos(original_pos),
        last_time_to_consider((original_pos < time_window) ? 0
                                                           : original_pos - time_window),
        original_node(node),
        tecs(&tecs),
        time_reservator(time_reservator),
        enumeration_limit(1000000000000000000) {
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
      cleanup();
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

  iterator begin() { return iterator(*this, !has_next()); }

  iterator end() { return iterator(*this, true); }

  void reset() {
    stack = {};
    if (original_node != nullptr && original_node->max() >= last_time_to_consider) {
      stack.push({original_node, {}});
    }
  };

 private:
  bool has_next() {
    ZoneScopedN("Internal::Enumerator::has_next");
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
  ComplexEvent next() {
    ZoneScopedN("Internal::Enumerator::next");
    std::reverse(next_value.second.begin(), next_value.second.end());
    return ComplexEvent(next_value);
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
