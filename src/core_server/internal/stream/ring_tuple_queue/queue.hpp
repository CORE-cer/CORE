#ifndef RINGTUPLEQUEUE_HPP
#define RINGTUPLEQUEUE_HPP

#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <type_traits>
#include <vector>

#include "tuple.hpp"

namespace RingTupleQueue {

class Queue {
  // TODO: Checks for if memory is being overwriten with start index.
 private:
  std::vector<uint64_t> buffer;
  uint64_t start_index;
  uint64_t constant_section_index;
  uint64_t current_index;
  std::vector<std::pair<std::chrono::system_clock::time_point, uint64_t>>
      time_ring;
  TupleSchemas* schemas;
  std::chrono::system_clock::time_point overwrite_timepoint;

 public:
  explicit Queue(uint64_t size, TupleSchemas* schemas)
      : buffer(size),
        start_index(0),
        current_index(0),
        schemas(schemas),
        overwrite_timepoint(std::chrono::system_clock::now()) {}

  uint64_t size() const { return buffer.size(); }

  std::span<uint64_t> start_tuple(uint64_t tuple_type_id) {
    int64_t minimum_size =
        schemas->get_constant_section_size(tuple_type_id);
    if (current_index < buffer.size() - minimum_size) {
      constant_section_index = current_index;
      current_index += minimum_size;
    } else {
      constant_section_index = 0;
      current_index = minimum_size;
    }
    // Add tuple_type_id and date
    buffer[constant_section_index++] = tuple_type_id;
    // get current time_point and cast it to uint64_t
    auto now = std::chrono::system_clock::now();
    // use memcpy to place it inside the buffer:
    memcpy(&buffer[constant_section_index++], &now,
           sizeof(std::chrono::system_clock::time_point));
    static_assert(sizeof(std::chrono::system_clock::time_point) <=
                  sizeof(uint64_t));
    std::chrono::system_clock::time_point overwrite_timepoint;

    return std::span<uint64_t>(&buffer[constant_section_index - 2],
                               &buffer[current_index - 1]);
  }

  // Note that Substitution is not a failure is used in these functions.
  // The std::enable_if<A, T> will become T if A is true and would be
  // a failure if A is false. So, this will pattern match for the
  // applicable case at 0 runtime cost.
  template <typename T>
  auto writer() ->
      typename std::enable_if<std::is_trivially_copyable<T>::value,
                              T*>::type {
    auto ptr = &buffer[constant_section_index];
    constant_section_index += sizeof(T) / sizeof(uint64_t);
    return reinterpret_cast<T*>(ptr);
  }

  template <typename T>
  auto writer(uint64_t size_in_bytes) -> typename std::enable_if<
      !std::is_trivially_copyable<T>::value &&
          std::is_convertible<T, std::string>::value,
      char*>::type {
    // Update the pointer positions fo the constant sized section

    uint64_t size = (size_in_bytes + 7) / 8;  // Ceiling
    int64_t index_to_write_in;
    if (current_index < buffer.size() - size) {
      index_to_write_in = current_index;
      current_index += size;
    } else {
      current_index = size;
      index_to_write_in = 0;
    }
    auto start_ptr = reinterpret_cast<char*>(&buffer[index_to_write_in]);
    auto end_ptr = &(start_ptr[size_in_bytes]);

    char** start_ptr_storage =
        reinterpret_cast<char**>(&buffer[constant_section_index++]);
    *start_ptr_storage = start_ptr;
    char** end_ptr_storage =
        reinterpret_cast<char**>(&buffer[constant_section_index++]);
    *end_ptr_storage = end_ptr;

    return start_ptr;
  }

  template <typename T>
  auto writer(uint64_t size_in_bytes) -> typename std::enable_if<
      !std::is_trivially_copyable<T>::value &&
          !std::is_convertible<T, std::string>::value,
      uint64_t*>::type {
    uint64_t size = (size_in_bytes + 7) / 8;  // Ceiling
    int64_t index_to_write_in;
    if (current_index < buffer.size() - size) {
      index_to_write_in = current_index;
      current_index += size;
    } else {
      current_index = size;
      index_to_write_in = 0;
    }
    auto ptr = &buffer[index_to_write_in];
    return ptr;
  }

  void update_overwrite_timepoint(
      std::chrono::system_clock::time_point millis) {
    overwrite_timepoint = millis;
  }

 private:
  int64_t compute_available_space() {
    // TODO
  }

  // Other methods...
};

}  // namespace RingTupleQueue

#endif  // RINGTUPLEQUEUE_HPP
