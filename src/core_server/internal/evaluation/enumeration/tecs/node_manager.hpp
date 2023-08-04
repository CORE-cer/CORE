#pragma once

#include <cstdlib>

#include "minipool.hpp"
#include "node.hpp"

namespace CORE::Internal::tECS {

const size_t MEMORY_POOL_STARTING_SIZE = 2048;

/**
 * The Node Manager class stores the pointers to all allocated
 * ECSNode's. When an ECSNode is no longer used, i.e, when the amount
 * of references to it has become 0, that memory is available to be recycled.
 */
class NodeManager {
 public:
  size_t amount_of_nodes_used{0};
  size_t amount_of_recycled_nodes{0};

 private:
  MiniPool* minipool_head;
  Node* recyclable_node_head;

 public:
  NodeManager(size_t starting_size)
      : minipool_head(new MiniPool(starting_size)),
        recyclable_node_head(nullptr) {}

  ~NodeManager() {
    for (MiniPool* mp = minipool_head; mp != nullptr;) {
      MiniPool* next = mp->next();
      delete mp;
      mp = next;
    }
  }

  template <class... Args>
  Node* alloc(Args&&... args) {
    Node*
      recycled_node = get_node_to_recycle_or_increase_mempool_size_if_necessary();
    if (recycled_node != nullptr) {
      recycled_node->reset(std::forward<Args>(args)...);
      return recycled_node;
    }
    return allocate_a_new_node((args)...);
  }

  size_t amount_of_nodes_allocated() const {
    size_t amount = 0;
    for (MiniPool* mpool = minipool_head; mpool != nullptr;
         mpool = mpool->prev())
      amount += mpool->capacity();
    return amount;
  }

  void decrease_ref_count(Node* node) {
    node->ref_count--;
    try_to_mark_node_as_unused(node);
  }

  void increase_ref_count(Node* node) { node->ref_count++; }

  void add_to_list_of_free_memory(Node* node) {
    node->next_free_node = recyclable_node_head;
    recyclable_node_head = node;
  }

  size_t get_amount_of_nodes_used() const { return amount_of_nodes_used; }

 private:
  Node* get_node_to_recycle_or_increase_mempool_size_if_necessary() {
    if (!minipool_head->is_full()) {
      return nullptr;
    }
    if (recyclable_node_head == nullptr) {
      increase_mempool_size();
      return nullptr;
    }
    return get_node_to_recycle();
  }

  void increase_mempool_size() {
    MiniPool* new_minipool = new MiniPool(minipool_head->size() * 2);
    minipool_head->set_next(new_minipool);
    new_minipool->set_prev(minipool_head);

    minipool_head = new_minipool;
  }

  Node* get_node_to_recycle() {
    Node* node_to_recycle = recyclable_node_head;
    Node* children_of_the_recycled_node[2] = {nullptr, nullptr};
    if (node_to_recycle->is_union()) {
      children_of_the_recycled_node[0] = recyclable_node_head->right;
      children_of_the_recycled_node[1] = recyclable_node_head->left;
    } else {
      children_of_the_recycled_node[0] = recyclable_node_head->left;
    }

    advance_recyclable_nodes_list_head();
    decrease_references_to_children(children_of_the_recycled_node);

    return node_to_recycle;
  }

  void decrease_references_to_children(Node* children[2]) {
    for (int i = 0; i < 2; i++) {
      Node* node = children[i];
      if (node != nullptr) decrease_ref_count(node);
    }
  }

  void advance_recyclable_nodes_list_head() {
    recyclable_node_head = recyclable_node_head->next_free_node;
    ++amount_of_recycled_nodes;
  }

  template <class... Args>
  Node* allocate_a_new_node(Args&&... args) {
    ++amount_of_nodes_used;
    return minipool_head->alloc(std::forward<Args>(args)...);
  }

  void try_to_mark_node_as_unused(Node* node) {
    if (node->ref_count == 0) {
      add_to_list_of_free_memory(node);
    }
  }
};
}  // namespace CORE::Internal::tECS
