#include <set>
#include <stack>

#include "node.hpp"

namespace CORE::Internal::tECS {

class Enumerator {
  friend class iterator;
  std::stack<std::pair<Node*, std::set<uint64_t>>> stack;
  uint64_t original_pos;
  uint64_t last_time_to_consider;
  std::pair<std::pair<uint64_t, uint64_t>, std::set<uint64_t>> next_value;

  class iterator {
   private:
    Enumerator& enumerator;
    bool is_end;

   public:
    iterator(Enumerator& enumerator, bool is_end)
        : enumerator(enumerator), is_end(is_end) {}

    std::pair<std::pair<uint64_t, uint64_t>, std::set<uint64_t>>
    operator*() {
      return enumerator.next();
    }

    bool operator!=(const iterator& other) const {
      return is_end != other.is_end;
    }

    iterator& operator++() {
      if (!enumerator.has_next()) {
        is_end = true;
      }
      return *this;
    }
  };

 public:
  Enumerator(Node* node, uint64_t original_pos, int64_t time_window)
      : original_pos(original_pos),
        last_time_to_consider(
          (original_pos < time_window) ? 0 : original_pos - time_window) {
    if (node->max() >= last_time_to_consider) {
      stack.push({node, {}});
    } else {
      std::cout << "node max: " << node->max()
                << "Is less than: " << last_time_to_consider << std::endl;
    }
  }

  iterator begin() { return iterator(*this, !has_next()); }

  iterator end() { return iterator(*this, true); }

 private:
  bool has_next() {
    std::cout << "has_next Stack Size: " << stack.size() << std::endl;
    while (!stack.empty()) {
      std::cout << "tECS enumerator Stack is not empty" << std::endl;
      Node* current_node = stack.top().first;
      std::cout << "Enumerating the node:" << std::endl;
      std::cout << current_node->to_string() << std::endl;
      std::set<uint64_t> positions = stack.top().second;
      stack.pop();
      while (true) {
        if (current_node->is_bottom()) {
          next_value = {{current_node->pos(), original_pos}, positions};
          std::cout << "\nGOT AN OUTPUT TECS ENUMERATOR\n" << std::endl;
          std::cout << "Elements still in stack: " << stack.size()
                    << std::endl;
          return true;
        } else if (current_node->is_output()) {
          positions.insert(current_node->pos());
          current_node = current_node->next();
        } else if (current_node->is_union()) {
          if (current_node->right->max() >= last_time_to_consider) {
            stack.push({current_node->right, positions});
          }
          current_node = current_node->left;
        }
      }
    }
    std::cout << "\nNO OUTPUT TECS ENUMEARTOR\n" << std::endl;
    return false;
  }

  /// It requires has_next to be evaluated before.
  std::pair<std::pair<uint64_t, uint64_t>, std::set<uint64_t>> next() {
    return next_value;
  }
};
}  // namespace CORE::Internal::tECS
