#pragma once
#include <stdint.h>

#include <cassert>
#include <cwchar>

#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"

namespace CORE::Internal::tECS {
struct Node {
  //RingTupleQueue::Tuple tuple;
  friend class NodeManager;

  Node* left = nullptr;

  union {
    Node* right = nullptr;
    RingTupleQueue::Tuple tuple;
  };

  uint64_t timestamp;

  enum class NodeType { BOTTOM = 0, UNION = 1, LABEL = 2 };
  NodeType node_type;

  // Change to templating and have TimeType maximum_start;
  uint64_t maximum_start;

  union {
    uint64_t ref_count{1};
    Node* next_free_node;
  };

 public:
  /// The timestamp does not need to be the timestamp in the tuple. It
  /// could be for example the stream position.
  Node(RingTupleQueue::Tuple tuple, uint64_t timestamp)
      : node_type(NodeType::BOTTOM),
        tuple(tuple),
        timestamp(timestamp),
        maximum_start(timestamp) {}

  void reset(RingTupleQueue::Tuple tuple, uint64_t timestamp) {
    node_type = NodeType::BOTTOM;
    this->tuple = tuple;
    this->timestamp = timestamp;
    maximum_start = timestamp;
    uint64_t ref_count = 1;
  }

  Node(Node* node, RingTupleQueue::Tuple tuple, uint64_t timestamp)
      : node_type(NodeType::LABEL),
        tuple(tuple),
        left(node),
        timestamp(timestamp) {
    assert(node != nullptr);
    maximum_start = left->maximum_start;
  }

  void reset(Node* node, RingTupleQueue::Tuple tuple, uint64_t timestamp) {
    node_type = NodeType::LABEL;
    this->tuple = tuple;
    left = node;
    this->timestamp = timestamp;
    assert(left != nullptr);
    maximum_start = left->maximum_start;
    uint64_t ref_count = 1;
  }

  Node(Node* left, Node* right) : node_type(NodeType::UNION) {
    assert(left != nullptr);
    assert(right != nullptr);
    if (left->maximum_start >= right->maximum_start) {
      this->left = left;
      this->right = right;
    } else {
      this->left = right;
      this->right = left;
    }
    assert(this->left->maximum_start >= this->right->maximum_start);
    maximum_start = this->left->maximum_start;
  }

  void reset(Node* left, Node* right) {
    node_type = NodeType::UNION;
    assert(left != nullptr);
    assert(right != nullptr);
    if (left->maximum_start >= right->maximum_start) {
      this->left = left;
      this->right = right;
    } else {
      this->left = right;
      this->right = left;
    }
    assert(this->left->maximum_start >= this->right->maximum_start);
    maximum_start = this->left->maximum_start;
    uint64_t ref_count = 1;
  }

  bool is_union() const { return node_type == NodeType::UNION; }

  bool is_output() const { return node_type == NodeType::LABEL; }

  bool is_bottom() const { return node_type == NodeType::BOTTOM; }

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
