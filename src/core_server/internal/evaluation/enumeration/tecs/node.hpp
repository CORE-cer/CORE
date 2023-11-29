#pragma once
#include <stdint.h>

#include <cassert>
#include <cwchar>

#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"

namespace CORE::Internal::tECS {
class Node {
  friend class NodeManager;
  friend class TimeListManager;
  friend class tECS;

 private:
  enum class NodeType {
    BOTTOM,
    UNION,
    OUTPUT,
    DEAD,
    TIME_LIST_HEAD,
    TIME_LIST_TAIL,
  };
  NodeType node_type;

  Node* left = nullptr;

  union {
    Node* right = nullptr;
    RingTupleQueue::Tuple tuple;
  };

  union {
    uint64_t ref_count{0};
    Node* next_free_node;
  };

  Node* time_list_left;
  Node* time_list_right;

 public:
  uint64_t maximum_start;
  uint64_t timestamp;

  /**
   * The timestamp does not need to be the timestamp in the tuple. It
   * could be for example the stream position.
   */

  /* BOTTOM Node */
  Node(RingTupleQueue::Tuple tuple, uint64_t timestamp) {
    reset(tuple, timestamp);  //Se llama a reset para evitar repetir codigo
  }

  // TODO: Check if I really need a tuple.

  void reset(RingTupleQueue::Tuple tuple, uint64_t timestamp) {
    left = nullptr;
    this->tuple = tuple;
    this->timestamp = timestamp;
    this->node_type = NodeType::BOTTOM;
    this->maximum_start = timestamp;
    this->ref_count = 0;  // Ahora parte con ref_count 0 (lo mismo para los demas casos)
  }

  /* OUTPUT Node */
  Node(Node* node, RingTupleQueue::Tuple tuple, uint64_t timestamp) {
    assert(node != nullptr);
    reset(node,
          tuple,
          timestamp);  //Se llama a reset para evitar repetir codigo
  }

  void reset(Node* node, RingTupleQueue::Tuple tuple, uint64_t timestamp) {
    this->left = node;
    this->tuple = tuple;
    this->timestamp = timestamp;
    this->node_type = NodeType::OUTPUT;
    assert(left != nullptr);
    node->ref_count += 1;  // Se suma la referencia al nodo hijo para no reciclarla (como antes)
    maximum_start = left->maximum_start;
    this->ref_count = 0;
  }

  /* UNION Node */
  Node(Node* left, Node* right) {
    assert(left != nullptr);
    assert(right != nullptr);
    reset(left, right);
  }

  void reset(Node* left, Node* right) {
    this->node_type = NodeType::UNION;
    assert(left != nullptr);
    assert(right != nullptr);
    left->ref_count += 1;  // Se suma la referencia a cada hijo para no reciclarlas
    right->ref_count += 1;
    this->left = left;
    this->right = right;
    timestamp = {};  // TODO: this is not neccessary right?
    assert(this->left->maximum_start >= this->right->maximum_start);
    maximum_start = this->left->maximum_start;
    this->ref_count = 0;
  }

  /* TIME_LIST_HEAD/TAIL Nodes */
  Node(NodeType node_type) : node_type(node_type) { reset(node_type); }

  void reset(NodeType node_type) {
    assert(node_type == NodeType::TIME_LIST_HEAD
           || node_type == NodeType::TIME_LIST_TAIL);
    this->node_type = node_type;
    maximum_start = UINT64_MAX;
    this->ref_count = 1;  // Se mantiene en 1 dado que en teoria se ocupa siempre
  }

  bool is_union() const { return node_type == NodeType::UNION; }

  bool is_output() const { return node_type == NodeType::OUTPUT; }

  bool is_bottom() const { return node_type == NodeType::BOTTOM; }

  bool is_dead() const { return node_type == NodeType::DEAD; }

  uint64_t pos() const {
    assert(!is_union());
    return timestamp;
  }

  RingTupleQueue::Tuple get_tuple() const {
    assert(!is_union());
    return tuple;
  }

  Node* next() const {
    assert(is_output());
    assert(left != nullptr);
    return left;
  }

  Node* get_left() const {
    assert(is_union());
    return left;
  }

  Node* get_right() const {
    assert(is_union());
    return right;
  }

  uint64_t max() const { return maximum_start; }

  std::string to_string(size_t depth = 0) const {
    std::string out = "";
    for (size_t i = 0; i < depth; i++) {
      out += "    ";
    }
    if (is_bottom()) {
      out += "Bottom(" + std::to_string(tuple.id()) + ")";
    } else if (is_output()) {
      out += "Output(" + std::to_string(tuple.id()) + ")\n";
      out += left->to_string(depth + 1);
    } else {
      out += "Union\n";
      out += left->to_string(depth + 1);
      out += "\n";
      out += right->to_string(depth + 1);
    }
    return out;
  }
};
}  // namespace CORE::Internal::tECS
