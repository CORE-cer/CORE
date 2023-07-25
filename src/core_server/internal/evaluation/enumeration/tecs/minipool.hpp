#pragma once

#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

#include "node.hpp"

namespace CORE::Internal::tECS {

/**
 * The NodeManager class stores its nodes through a linked list of MiniPools.
 */
class MiniPool {
 private:
  size_t capacity_;
  std::vector<Node> node_container;
  MiniPool* next_{nullptr};
  MiniPool* prev_{nullptr};

 public:
  MiniPool(size_t cap) : capacity_(cap) {
    node_container.reserve(capacity_);
  }

  size_t capacity() const { return node_container.capacity(); }

  size_t size() const { return node_container.size(); }

  bool is_full() const { return node_container.size() >= capacity_; }

  MiniPool* next() const { return next_; }

  void set_next(MiniPool* mp) { next_ = mp; }

  MiniPool* prev() const { return prev_; }

  void set_prev(MiniPool* mp) { prev_ = mp; }

  template <class... Args>
  Node* alloc(Args... args) {
    node_container.emplace_back(std::forward<Args>(args)...);
    return &node_container.back();
  }
};
}  // namespace CORE::Internal::tECS
