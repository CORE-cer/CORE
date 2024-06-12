#ifndef RINGTUPLEQUEUE_HPP
#define RINGTUPLEQUEUE_HPP

#include <atomic>
#include <cassert>
#include <chrono>
#include <csignal>
#include <cstdint>
#include <cstring>
#include <queue>
#include <type_traits>
#include <vector>

#include "tuple.hpp"

namespace RingTupleQueue {

class Queue {
  // TODO: Check if memory is being overwriten with start index.
 private:
  std::vector<std::vector<uint64_t>> buffers;
  std::vector<std::chrono::system_clock::time_point> last_updated;

  // Oldest buffer that has not yet been overwritten.
  uint64_t start_buffer_index;

  uint64_t constant_section_buffer_index;
  uint64_t constant_section_index;

  uint64_t current_buffer_index;
  uint64_t current_index;

  uint64_t buffer_size;

  TupleSchemas* schemas;
  std::chrono::system_clock::time_point overwrite_timepoint;
  std::chrono::system_clock::time_point start_timepoint;

 public:
  Tuple get_tuple(uint64_t* data) { return Tuple(data, schemas); }

  explicit Queue(uint64_t buffer_size, TupleSchemas* schemas)
      : buffer_size(buffer_size),
        buffers({std::vector<uint64_t>(buffer_size)}),
        last_updated(1),
        start_buffer_index(0),
        constant_section_buffer_index(0),
        constant_section_index(0),
        current_buffer_index(0),
        current_index(0),
        schemas(schemas),
        overwrite_timepoint(std::chrono::system_clock::now()),
        start_timepoint(std::chrono::system_clock::now()) {}

  uint64_t* start_tuple(uint64_t tuple_type_id) {
    uint64_t minimum_size = schemas->get_constant_section_size(tuple_type_id);
    assert(minimum_size < buffer_size);

    if (current_index < buffers[current_buffer_index].size() - minimum_size) {
      // Current buffer does have enough size.
      constant_section_buffer_index = current_buffer_index;
      constant_section_index = current_index;
      current_index += minimum_size;
    } else {
      // Current buffer does not have enough size.
      increase_size_if_necessary();
      current_buffer_index = (current_buffer_index + 1) % buffers.size();
      constant_section_buffer_index = current_buffer_index;
      constant_section_index = 0;
      current_index = minimum_size;
    }

    // Add tuple_type_id and date
    auto& current_buffer = buffers[constant_section_buffer_index];
    current_buffer[constant_section_index++] = tuple_type_id;
    // get current time_point and cast it to uint64_t
    auto now = std::chrono::system_clock::now();
    // use memcpy to place it inside the buffer:
    memcpy(&current_buffer[constant_section_index++],
           &now,
           sizeof(std::chrono::system_clock::time_point));
    memcpy(&current_buffer[constant_section_index++],
           &now,
           sizeof(std::chrono::system_clock::time_point));
    static_assert(sizeof(std::chrono::system_clock::time_point) <= sizeof(uint64_t));
    last_updated[constant_section_buffer_index] = now;

    return &current_buffer[constant_section_index - 3];
  }

  // Note that Substitution is not a failure is used in these functions.
  // The std::enable_if<A, T> will become T if A is true and would be
  // a failure if A is false. So, this will pattern match for the
  // applicable case at 0 runtime cost.
  template <typename T>
  auto writer() ->
    typename std::enable_if<std::is_trivially_copyable<T>::value, T*>::type {
    auto& current_buffer = buffers[constant_section_buffer_index];
    auto ptr = &current_buffer[constant_section_index];
    // Advance, ceiling so that we can actually fit if not multiple of sizeof(uin64_t)
    constant_section_index += (sizeof(T) + sizeof(uint64_t) - 1) / sizeof(uint64_t);
    return reinterpret_cast<T*>(ptr);
  }

  template <typename T>
  auto writer(uint64_t size_in_bytes) ->
    typename std::enable_if<!std::is_trivially_copyable<T>::value
                              && std::is_convertible<T, std::string>::value,
                            char*>::type {
    // Update the pointer positions fo the constant sized section
    auto* current_buffer = &(buffers[current_buffer_index]);
    uint64_t size = (size_in_bytes + 7) / 8;  // Ceiling
    uint64_t index_to_write_in;
    if (current_index < current_buffer->size() - size) {
      index_to_write_in = current_index;
      current_index += size;
    } else {
      increase_size_if_necessary();
      current_buffer_index = (current_buffer_index + 1) % buffers.size();
      current_buffer = &(buffers[current_buffer_index]);
      current_index = size;
      index_to_write_in = 0;
    }
    auto start_ptr = reinterpret_cast<char*>(&((*current_buffer)[index_to_write_in]));
    auto end_ptr = &(start_ptr[size_in_bytes]);

    auto& constant_section_buffer = buffers[constant_section_buffer_index];
    char** start_ptr_storage = reinterpret_cast<char**>(
      &constant_section_buffer[constant_section_index++]);
    *start_ptr_storage = start_ptr;
    char** end_ptr_storage = reinterpret_cast<char**>(
      &constant_section_buffer[constant_section_index++]);
    *end_ptr_storage = end_ptr;

    return start_ptr;
  }

  template <typename T>
  auto writer(uint64_t size_in_bytes) ->
    typename std::enable_if<!std::is_trivially_copyable<T>::value
                              && !std::is_convertible<T, std::string>::value,
                            uint64_t*>::type {
    throw std::logic_error("Non-string non-const sized writer is not implemented yet");
    //auto& buffer = buffers[constant_section_buffer_index];
    //uint64_t size = (size_in_bytes + 7) / 8;  // Ceiling
    //int64_t index_to_write_in;
    //index_to_write_in = constant_section_index;
    //constant_section_index += size;
    //auto ptr = &buffer[index_to_write_in];
    //return ptr;
  }

  void update_overwrite_timepoint(std::chrono::system_clock::time_point timepoint) {
    overwrite_timepoint = timepoint;
  }

  void update_overwrite_timepoint(uint64_t ns_from_compile_time) {
    auto timepoint = start_timepoint + std::chrono::nanoseconds(ns_from_compile_time);
    overwrite_timepoint = timepoint;
  }

 private:
  void increase_size_if_necessary() {
    uint64_t next_buffer_index = (current_buffer_index + 1) % buffers.size();
    if (next_buffer_index == start_buffer_index) {
      update_indices();
      if (next_buffer_index == start_buffer_index) {
        insert_a_buffer_after(current_buffer_index);
      }
    }
  }

  void insert_a_buffer_after(uint64_t buffer_index) {
    assert(buffer_index < buffers.size());
    // NOTE: Added due to buffers.begin not being uint64_t. To remove need to refactor.
    if (buffer_index
        <= static_cast<size_t>(std::numeric_limits<std::ptrdiff_t>::max() - 1)) {
      std::ptrdiff_t buffer_index_ptrdiff = static_cast<std::ptrdiff_t>(buffer_index);

      buffers.insert(buffers.begin() + buffer_index_ptrdiff + 1,
                     std::vector<uint64_t>(buffer_size));

      last_updated.insert(last_updated.begin() + buffer_index_ptrdiff + 1,
                          overwrite_timepoint);
    } else {
      std::raise(SIGINT);
    }
    assert(last_updated.size() == buffers.size());

    if (start_buffer_index > buffer_index) {
      start_buffer_index++;
    }

    assert(current_buffer_index <= buffer_index
           && constant_section_buffer_index <= buffer_index);
  }

  /**
   * If any tuple was added in a time lower than the overwrite_timepoint,
   * it is fair play to remove it. The last_updated shows the last time a
   * new tuple was added to it, therefore, it shows when it is fair play
   * to recycle all the memory in that buffer.
   */
  void update_indices() {
    if (last_updated[start_buffer_index] <= overwrite_timepoint) {
      start_buffer_index = (start_buffer_index + 1) % buffers.size();
    }
  }
};

}  // namespace RingTupleQueue

#endif  // RINGTUPLEQUEUE_HPP
