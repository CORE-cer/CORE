#include "time_list_manager.hpp"

#include "node_manager.hpp"

namespace CORE::Internal::tECS {

/**
 * The Node Manager class stores the pointers to all allocated
 * ECSNode's. When an ECSNode is no longer used, i.e, when the amount
 * of references to it has become 0, that memory is available to be recycled.
 */

TimeListManager::TimeListManager(NodeManager& node_manager)
    : node_manager(node_manager),
      head(std::make_unique<Node>(Node::NodeType::TIME_LIST_HEAD)),
      tail(std::make_unique<Node>(Node::NodeType::TIME_LIST_TAIL)) {
  head->time_list_left = head.get();
  head->time_list_right = tail.get();
  tail->time_list_left = head.get();
  tail->time_list_right = tail.get();
}

void TimeListManager::add_node(Node* node) {
  // HEAD <-> previous_right               <- now
  // HEAD <-> node <-> previous_right      <- wanted
  Node* previous_right = head->time_list_right;

  head->time_list_right = node;
  node->time_list_left = head.get();

  node->time_list_right = previous_right;
  previous_right->time_list_left = node;
}

bool TimeListManager::remove_a_dead_node_if_possible(
  uint64_t maximum_start_limit) {
  std::cout << "Calling remove_a_dead_node_if_possible" << std::endl;
  if (tail->time_list_left->maximum_start < maximum_start_limit) {
    std::cout << "removed a node!" << std::endl;
    remove_node(tail->time_list_left);
    return true;
  }
  std::cout << "maximum start: " << tail->time_list_left->maximum_start
            << " limit: " << maximum_start_limit;
  std::cout << "Type: " << (uint64_t)tail->time_list_left->node_type
            << std::endl;
  std::cout << "Right of head:" << head->time_list_right->maximum_start
            << " limit: " << maximum_start_limit;
  std::cout << "Type: " << (uint64_t)head->time_list_right->node_type
            << std::endl;
  return false;
}

void TimeListManager::remove_node(Node* node) {
  assert(node != nullptr);
  if (node->time_list_left != nullptr) {
    assert(node->time_list_right != nullptr);

    // Prev <-> node <-> Post          <- current
    // Prev <-> Post                   <- wanted
    Node* prev = node->time_list_left;
    Node* post = node->time_list_right;
    node->time_list_left = nullptr;
    node->time_list_right = nullptr;

    prev->time_list_right = post;
    post->time_list_left = prev;

    node_manager.mark_as_dead(node);
  }
}
}  // namespace CORE::Internal::tECS
