#pragma once

#include <atomic>
#include <cassert>
#include <cstdint>
#include <utility>
#include <vector>

#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "node.hpp"
#include "node_manager.hpp"
#include "shared/datatypes/eventWrapper.hpp"
#include "time_reservator.hpp"

namespace CORE::Internal::tECS {

class tECS {
  // tECS manages nodes and union lists, by default all operations take
  // ownership of the node passed.
  using UnionList = std::vector<Node*>;

 public:
  TimeReservator* time_reservator;

 private:
  NodeManager node_manager;

 public:
  tECS(std::atomic<uint64_t>& event_time_of_expiration)
      : node_manager(MEMORY_POOL_STARTING_SIZE, event_time_of_expiration) {
    time_reservator = &node_manager.get_time_reservator();
  }

  void pin(Node* node) { node_manager.increase_ref_count(node); }

  void pin(UnionList& ulist) {
    for (auto node : ulist) {
      pin(node);
    }
  }

  /**
   * If a node is no longer used, it should be unpinned, this allows the
   * memory manager to recycle the memory allocated to it.
   */
  void unpin(Node* node) { node_manager.decrease_ref_count(node); }

  void unpin(UnionList& ulist) {
    for (auto& node : ulist) {
      unpin(node);
    }
  }

  /**
   * The bottom node, also known as the terminal node, has no children and
   * tells us that we reached the end of an output
   */
  [[nodiscard]] Node* new_bottom(RingTupleQueue::Tuple& tuple,
                                 Types::EventWrapper&& event,
                                 uint64_t timestamp) {
    auto out = node_manager.alloc(tuple, std::move(event), timestamp);
    assert(out != nullptr);
    return out;
  }

  /**
   * Extend nodes, also known as content nodes store the opened and closed
   * variables and the position in the document that this annotation is
   * referring to.
   */
  [[nodiscard]] Node* new_extend(Node* node,
                                 RingTupleQueue::Tuple& tuple,
                                 Types::EventWrapper& event,
                                 uint64_t timestamp) {
    return node_manager.alloc(node, tuple, std::move(event.clone()), timestamp);
  }

  /**
   * Union nodes allow us to enumerate multiple outputs starting from a
   * single node.
   */
  [[nodiscard]] Node* new_union(Node* node_1, Node* node_2) {
    assert(node_1 != nullptr && node_2 != nullptr);
    assert(node_1->node_type != Node::NodeType::DEAD);
    assert(node_2->node_type != Node::NodeType::DEAD);
    assert(node_1->max() == node_2->max());
    if (!node_1->is_union()) {
      return new_direct_union(node_1, node_2);
    } else if (!node_2->is_union()) {
      return new_direct_union(node_2, node_1);
    } else {
      return create_union_of_two_union_nodes(node_1, node_2);
    }
  }

  [[nodiscard]] Node* new_direct_union(Node* node_1, Node* node_2) {
    assert(node_1 != nullptr && node_2 != nullptr);
    assert(node_1->node_type != Node::NodeType::DEAD);
    assert(node_2->node_type != Node::NodeType::DEAD);
    return node_manager.alloc(node_1, node_2);
  }

  /**
   * If the node is going to be used in another scope, it is necessary to
   * pin it so that the memory manager does not recycle its memory location.
   */
  UnionList new_ulist(Node* node) {
    assert(!node->is_union());
    pin(node);
    return {node};
  }

  /// Inserts the node in the ulist, maintaining the max-sorted invariant.
  [[nodiscard]] UnionList insert(UnionList&& ulist, Node* node) {
    assert_required_properties_of_union_list(ulist);
    assert(node->max() <= ulist[0]->max());
    if (ulist.size() == 1) {
      ulist.push_back(node);
      pin(node);
      assert_required_properties_of_union_list(ulist);
      return std::move(ulist);
    }
    // TODO: binary search would be better on large lists.
    int64_t i;
    for (i = 1; i < ulist.size(); i++) {
      if (ulist[i]->max() == node->max()) {
        Node* union_node = new_union(ulist[i], node);
        pin(union_node);
        unpin(ulist[i]);
        ulist[i] = union_node;
        break;
      }
      if (ulist[i]->max() < node->max()) {
        pin(node);
        ulist.insert(ulist.begin() + i, node);
        break;
      }
    }
    if (i == ulist.size()) {
      pin(node);
      ulist.push_back(node);
    }

    assert_required_properties_of_union_list(ulist);
    return std::move(ulist);
  }

  /*       _\|/_
           (o o)
   +----oOO-{_}-OOo---------------------------------------+
   |From the union list: n0, n1, ... nk creates this node:|
   |     u                                                |
   |    / \                                               |
   |  n0   u1                                             |
   |      /  \                                            |
   |     n1   u2                                          |
   |         /  \                                         |
   |        n2   \                                        |
   |              uk-1                                    |
   |             /   \                                    |
   |           nk-1   nk                                  |
   +-----------------------------------------------------*/
  Node* merge(UnionList& ulist) {
    assert_required_properties_of_union_list(ulist);
    Node* tail = ulist.back();
    for (auto rit = ulist.rbegin() + 1; rit != ulist.rend(); ++rit) {
      assert(*rit != nullptr && tail != nullptr);
      Node* node_1 = *rit;
      assert(node_1->node_type != Node::NodeType::DEAD);
      assert(tail->node_type != Node::NodeType::DEAD);
      tail = node_manager.alloc(node_1, tail);
    }
    assert(tail != nullptr);
    return tail;
  }

 private:
  void assert_required_properties_of_union_list(UnionList& union_list) {
    assert(union_list.size() >= 1);
    assert(union_list[0] != nullptr);
    assert(!union_list[0]->is_union());
    for (int i = 0; i < union_list.size(); i++) {
      assert(union_list[i] != nullptr);
      assert(union_list[0]->max() >= union_list[i]->max());
    }
    for (int i = 2; i < union_list.size(); i++) {
      assert(union_list[i - 1]->max() > union_list[i]->max());
    }
  }

  /*!       _\|/_
            (o o)
    +----oOO-{_}-OOo---------------------------------------+
    |           u                                          |
    |         /   \            The union of two nodes that |
    |        /    u1           are non output nodes creates|
    |       /      |\          the union that is shown on  |
    |      /       | u2        the left. If max(right(n1)) |
    |     /        |/  \       is >= than max(right(n2))   |
    |    /         |    \      then r(n1) is the left child|
    |   /         /|     \     of u2, if not, it is the    |
    |  /         / |      \    right child.                |
    | /  n1     /  |   n2  |                               |
    || /   \   /   | /    \|                               |
    |l(n1) r(n1)  l(n2)   r(n2)                            |
    +----------------------------------------------------- +
  */
  Node* create_union_of_two_union_nodes(Node* node_1, Node* node_2) {
    /// Because the creation of the union gives ownership of the nodes,
    /// the children of n1 and n2 are the ones that are referenced.
    /// n1 and n2 are not going to be used, therefore they are unpined.
    assert(node_1 != nullptr);
    assert(node_2 != nullptr);
    Node* u2 = create_first_intermediate_union_node(node_1, node_2);
    assert(u2 != nullptr);
    Node* u1 = create_second_intermediate_union_node(node_2, u2);
    assert(u1 != nullptr);
    Node* new_node = create_union_of_output_and_intermediate_node(node_1, u1);
    assert(new_node != nullptr);
    return new_node;
  }

  Node* create_first_intermediate_union_node(Node* node_1, Node* node_2) {
    assert(node_1 != nullptr);
    assert(node_2 != nullptr);
    assert(node_1->right->node_type != Node::NodeType::DEAD);
    assert(node_2->right->node_type != Node::NodeType::DEAD);
    Node* u2;
    if (node_1->max() >= node_2->max()) {
      u2 = node_manager.alloc(node_1->right, node_2->right);
    } else {
      u2 = node_manager.alloc(node_2->right, node_1->right);
    }
    assert(u2 != nullptr);
    return u2;
  }

  Node* create_second_intermediate_union_node(Node* node_2, Node* u2) {
    assert(node_2 != nullptr);
    assert(u2 != nullptr);
    assert(node_2->left != nullptr);
    assert(node_2->left->node_type != Node::NodeType::DEAD);
    assert(u2->node_type != Node::NodeType::DEAD);
    Node* u1 = node_manager.alloc(node_2->left, u2);
    assert(u1 != nullptr);
    return u1;
  }

  Node* create_union_of_output_and_intermediate_node(Node* node_1, Node* u2) {
    assert(node_1 != nullptr);
    assert(u2 != nullptr);
    assert(node_1->left != nullptr);
    assert(node_1->left->node_type != Node::NodeType::DEAD);
    assert(u2->node_type != Node::NodeType::DEAD);
    Node* new_node = node_manager.alloc(node_1->left, u2);
    assert(new_node != nullptr);
    return new_node;
  }
};
}  // namespace CORE::Internal::tECS
