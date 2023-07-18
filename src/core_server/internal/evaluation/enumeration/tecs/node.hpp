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
    uint64_t stream_position;
  };

  enum class NodeType { BOTTOM = 0, UNION = 1, LABEL = 2 };
  NodeType node_type;

  uint64_t maximum_start;

  union {
    uint64_t ref_count{1};
    Node* next_free_node;
  };

 public:
  Node(uint64_t stream_position)
      : node_type(NodeType::BOTTOM),
        stream_position(stream_position),
        maximum_start(stream_position) {}

  void reset(uint64_t stream_position) {
    node_type = NodeType::BOTTOM;
    this->stream_position = stream_position;
    maximum_start = stream_position;
    uint64_t ref_count = 1;
  }

  Node(Node* node, uint64_t stream_position)
      : node_type(NodeType::LABEL),
        stream_position(stream_position),
        left(node) {
    assert(node != nullptr);
    maximum_start = std::max(left->maximum_start, stream_position);
  }

  void reset(Node* node, uint64_t stream_position) {
    node_type = NodeType::LABEL;
    this->stream_position = stream_position;
    left = node;
    uint64_t ref_count = 1;
  }

  Node(Node* left, Node* right)
      : node_type(NodeType::UNION), left(left), right(right) {
    assert(left != nullptr);
    assert(right != nullptr);
    maximum_start = std::max(left->maximum_start, right->maximum_start);
  }

  void reset(Node* left, Node* right) {
    node_type = NodeType::UNION;
    this->left = left;
    this->right = right;
    uint64_t ref_count = 1;
  }

  bool is_union() const { return node_type == NodeType::UNION; }

  bool is_output() const { return node_type == NodeType::LABEL; }

  bool is_bottom() const { return node_type == NodeType::BOTTOM; }

  uint64_t pos() const {
    assert(!is_union());
    return stream_position;
  }

  Node* next() const {
    assert(is_output());
    assert(left != nullptr);
    return left;
  }

  uint64_t max() const { return maximum_start; }
};
}  // namespace CORE::Internal::tECS
