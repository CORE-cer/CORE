#ifndef RINGTUPLEQUEUE_HPP
#define RINGTUPLEQUEUE_HPP

#include <chrono>
#include <cstdint>
#include <vector>
#include <atomic>
#include <type_traits>

#include "tuple.hpp"

namespace RingTupleQueue {

class RingTupleQueue {
 private:
  std::vector<uint64_t> buffer;
  uint64_t start_index;
  uint64_t current_index;
  std::vector<std::pair<std::chrono::system_clock::time_point, uint64_t>> time_ring;
  TupleSchema* schema;
  std::chrono::system_clock::time_point overwrite_timepoint;

 public:
  explicit RingTupleQueue(uint64_t size, TupleSchema* schema)
      : buffer(size), start_index(0), current_index(0), schema(schema), overwrite_timepoint(std::chrono::system_clock::now()) {}

  uint64_t size() const { return buffer.size(); }

  void start_tuple(uint64_t tuple_type_id) {
    auto tuple_schema = schema->get_schema(tuple_type_id);
    // Start preparing the buffer and the index...
  }

  template <typename T>
  auto writer() -> typename std::enable_if<std::is_trivially_copyable<T>::value, T*>::type {
    auto ptr = &buffer[current_index];
    current_index += sizeof(T) / sizeof(uint64_t);
    return reinterpret_cast<T*>(ptr);
  }

  template <typename T>
  auto writer(uint64_t size_in_bytes) -> typename std::enable_if<!std::is_trivially_copyable<T>::value &&
                                           std::is_convertible<T, std::string>::value, char*>::type {
    auto ptr = reinterpret_cast<char*>(&buffer[current_index]);
    current_index += size_in_bytes / sizeof(uint64_t);
    return ptr;
  }

  template <typename T>
  auto writer(uint64_t size_in_bytes) -> typename std::enable_if<!std::is_trivially_copyable<T>::value &&
                                           !std::is_convertible<T, std::string>::value, uint64_t*>::type {
    auto ptr = &buffer[current_index];
    current_index += size_in_bytes / sizeof(uint64_t);
    return ptr;
  }

  void update_overwrite_timepoint(std::chrono::system_clock::time_point millis) {
    overwrite_timepoint = millis;
  }

  // Other methods...
};

}  // namespace RingTupleQueue

#endif  // RINGTUPLEQUEUE_HPP
