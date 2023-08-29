#pragma once
#include <unistd.h>

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <mutex>

namespace CORE::Internal::tECS {

class TimeReservator {
 public:
  class Node {
    friend class TimeReservator;
    std::unique_ptr<Node> right{nullptr};
    Node* left{nullptr};
    uint64_t time_reservation;

   public:
    Node(uint64_t time_reservation) : time_reservation(time_reservation) {}
  };

 private:
  std::unique_ptr<Node> head;
  Node* tail{nullptr};
  std::mutex mtx;

 public:
  TimeReservator() : head(std::make_unique<Node>(UINT64_MAX)) {
    head->right = std::make_unique<Node>(UINT64_MAX);
    head->right->left = head.get();
    tail = head->right.get();
    assert(list_is_well_formed());
  }

  Node* reserve(uint64_t time) {
    auto new_node = std::make_unique<Node>(time);

    std::lock_guard<std::mutex> lock(mtx);
    assert(list_is_well_formed());
    new_node->right = std::move(head->right);
    assert(new_node->right != nullptr);
    new_node->right->left = new_node.get();

    new_node->left = head.get();
    head->right = std::move(new_node);
    auto out = head->right.get();
    assert(list_is_well_formed());
    assert(list_contains(out));
    return out;
  }

  uint64_t get_smallest_reserved_time() {
    std::lock_guard<std::mutex> lock(mtx);
    return tail->left->time_reservation;
  }

  void remove_node(Node* node) {
    std::lock_guard<std::mutex> lock(mtx);
    assert(list_is_well_formed());
    assert(list_contains(node));
    node->right->left = node->left;
    node->left->right = std::move(node->right);
    assert(list_is_well_formed());
  }

 private:
  bool list_is_well_formed() {
    if (head == nullptr || tail == nullptr) {
      std::cout << "Head or tail is nullptr" << std::endl;
      return false;
    }

    Node* current = head.get();
    Node* previous = nullptr;

    // Loop through the list from head to tail.
    while (current != nullptr) {
      if (current->right == nullptr && current != tail) {
        std::cout << "Right of a non-tail node is nullptr" << std::endl;
        return false;
      }

      if (current->left == nullptr && current != head.get()) {
        std::cout << "Left of a non-head node is nullptr" << std::endl;
        return false;
      }

      if (current->right != nullptr && current->right->left != current) {
        std::cout << "Right node's left does not point back to node"
                  << std::endl;
        return false;
      }

      if (current->left != nullptr && current->left->right.get() != current) {
        std::cout << "Left node's right does not point back to node"
                  << std::endl;
        return false;
      }

      previous = current;
      current = current->right.get();
    }

    if (previous != tail) {
      std::cout << "Did not reach tail starting from head" << std::endl;
      return false;
    }

    return true;
  }

  bool list_contains(Node* node) {
    Node* current = head.get();
    while (current != nullptr) {
      if (current == node) {
        return true;
      }
      current = current->right.get();
    }
    return false;
  }
};
}  // namespace CORE::Internal::tECS
