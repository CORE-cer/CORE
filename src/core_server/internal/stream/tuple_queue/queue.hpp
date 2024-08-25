#pragma once

#include <cstdint>
#include <list>

#include "page.hpp"

namespace TupleQueue {
static const int64_t initial_page_count = 10;
static const int64_t initial_page_size = 10;

class Queue {
  std::list<Page> used_pages = {};
  std::list<Page> recyclable_pages = {};
  int64_t new_page_count = 10;
  int64_t initial_page_size = 1000;

 public:
  explicit Queue() {
    for (int64_t i = 0; i < initial_page_count; ++i) {
      recyclable_pages.emplace_back(initial_page_size);
    }
  }
};

}  // namespace TupleQueue
