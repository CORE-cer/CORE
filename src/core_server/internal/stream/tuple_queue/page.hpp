#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace TupleQueue {

class Page {
  std::size_t first_free_space;
  std::size_t page_size;

  std::vector<uint64_t> buffer;

  std::atomic<uint64_t> pins = 0;

 public:
  explicit Page(std::size_t page_size) : page_size(page_size) {}
};

}  // namespace TupleQueue
