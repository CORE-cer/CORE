#pragma once

#include <unistd.h>

#include <cstdlib>
#include <memory>

#include "node.hpp"
#include "time_reservator.hpp"

namespace CORE::Internal::tECS {

/**
 * The Node Manager class stores the pointers to all allocated
 * ECSNode's. When an ECSNode is no longer used, i.e, when the amount
 * of references to it has become 0, that memory is available to be recycled.
 */

class NodeManager;

class TimeListManager {
  NodeManager& node_manager;
  TimeReservator time_reservator;
  std::unique_ptr<Node> head;
  std::unique_ptr<Node> tail;

 public:
  TimeListManager(NodeManager& node_manager);

  void add_node(Node* node);
  bool remove_a_dead_node_if_possible(uint64_t maximum_start_limit);
  void remove_node(Node* node);

  TimeReservator& get_time_reservator() { return time_reservator; }
};
}  // namespace CORE::Internal::tECS
