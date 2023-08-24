#pragma once

#include <cstdlib>

#include "core_server/internal/evaluation/minipool/minipool.hpp"
#include "node.hpp"
#include "time_list_manager.hpp"

namespace CORE::Internal::tECS {

const size_t MEMORY_POOL_STARTING_SIZE = 2048;

/**
 * The Node Manager class stores the pointers to all allocated
 * ECSNode's. When an ECSNode is no longer used, i.e, when the amount
 * of references to it has become 0, that memory is available to be recycled.
 */
class NodeManager {
  typedef MiniPool::MiniPool<Node> NodePool;

 public:
  size_t amount_of_nodes_used{0};
  size_t amount_of_recycled_nodes{0};
  uint64_t& expiration_time;

 private:
  NodePool* minipool_head = nullptr;
  Node* recyclable_node_head = nullptr;
  TimeListManager time_list_manager;

 public:
  NodeManager(size_t starting_size, uint64_t& event_time_of_expiration)
      : minipool_head(new NodePool(starting_size)),
        recyclable_node_head(nullptr),
        time_list_manager(*this),
        expiration_time(event_time_of_expiration) {}

  ~NodeManager() {
    for (NodePool* mp = minipool_head; mp != nullptr;) {
      NodePool* next = mp->next();
      delete mp;
      mp = next;
    }
  }

  template <class... Args>
  Node* alloc(Args&&... args) {
    Node* out = get_node_to_recycle_or_increase_mempool_size_if_necessary();
    if (out != nullptr) {
      out->reset(std::forward<Args>(args)...);
    } else {
      out = allocate_a_new_node((args)...);
    }
    time_list_manager.add_node(out);
    return out;
  }

  size_t amount_of_nodes_allocated() const {
    size_t amount = 0;
    for (NodePool* mpool = minipool_head; mpool != nullptr;
         mpool = mpool->prev())
      amount += mpool->capacity();
    return amount;
  }

  void increase_ref_count(Node* node) { node->ref_count++; }

  void decrease_ref_count(Node* node) {
    assert(node != nullptr);
    node->ref_count--;
    try_to_mark_node_as_unused(node);
  }

  void mark_as_dead(Node* node) {
    assert(node != nullptr);
    assert(node->node_type != Node::NodeType::DEAD);
    switch (node->node_type) {
      case Node::NodeType::UNION:
        decrease_ref_count(node->right);
        node->right = nullptr;
        [[fallthrough]];
      case Node::NodeType::OUTPUT:
        decrease_ref_count(node->left);
        node->left = nullptr;
        [[fallthrough]];
      default:
        node->node_type = Node::NodeType::DEAD;
    }
  }

  size_t get_amount_of_nodes_used() const { return amount_of_nodes_used; }

 private:
  Node* get_node_to_recycle_or_increase_mempool_size_if_necessary() {
    if (!minipool_head->is_full()) {
      return nullptr;
    }
    if (recyclable_node_head == nullptr) {
      if (time_list_manager.remove_a_dead_node_if_possible(expiration_time)) {
        while (
          time_list_manager.remove_a_dead_node_if_possible(expiration_time))
          ;
        if (recyclable_node_head != nullptr) {
          return get_node_to_recycle();
        }
      };
      increase_mempool_size();
      return nullptr;
    }
    return get_node_to_recycle();
  }

  void increase_mempool_size() {
    NodePool* new_minipool = new NodePool(minipool_head->size() * 2);
    minipool_head->set_next(new_minipool);
    new_minipool->set_prev(minipool_head);

    minipool_head = new_minipool;
  }

  Node* get_node_to_recycle() {
    static int i = 1;
    static int bottom_nodes_recycled = 1;
    static int extend_nodes_recycled = 1;
    static int union_nodes_recycled = 1;
    Node* node_to_recycle = recyclable_node_head;
    advance_recyclable_nodes_list_head();
    if (node_to_recycle->is_union()) {
      decrease_ref_count(node_to_recycle->left);
      decrease_ref_count(node_to_recycle->right);
    } else if (node_to_recycle->is_output()) {
      decrease_ref_count(node_to_recycle->left);
    } else {
      assert(node_to_recycle->is_bottom() || node_to_recycle->is_dead());
    }
    time_list_manager.remove_node(node_to_recycle);
    return node_to_recycle;
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

  void add_to_list_of_free_memory(Node* node) {
    node->next_free_node = recyclable_node_head;
    recyclable_node_head = node;
  }
};
}  // namespace CORE::Internal::tECS
