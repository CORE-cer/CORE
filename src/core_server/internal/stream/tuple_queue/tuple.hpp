#pragma once

namespace TupleQueue {

class Tuple {
 private:
  uint64_t* data;  // Span to hold tuple data.
  TupleSchemas* schemas;

 public:
  explicit Tuple(uint64_t* data) : data(data), schemas(schemas) {}

  uint64_t* get_data() const { return data; }

  uint64_t id() const { return data[0]; }

  const std::vector<SupportedTypes>& get_schema() const {
    return schemas->get_schema(id());
  }

  uint64_t hash() const { return reinterpret_cast<uint64_t>(data); }

  std::chrono::system_clock::time_point timestamp() const {
    // Note: Assuming timestamp is stored as the second element of the data span.
    return std::chrono::system_clock::time_point(
      std::chrono::system_clock::duration(data[1]));
  }

  uint64_t nanoseconds() {
    auto compile_time_point = get_compile_time_point();
    auto duration_since_compile = timestamp() - compile_time_point;

    return std::chrono::duration_cast<std::chrono::nanoseconds>(duration_since_compile)
      .count();
  }

  std::string serialize_data() const {
    // Use memcpy or some other way so that the data ptr is sent as a string, so that
    // it is then converted directly to a uint64_t* pos.
    std::atomic<uint64_t*> atomic_data;
    atomic_data.store(data, std::memory_order_release);
    std::string out(sizeof(uint64_t*), '\0');
    std::memcpy(&out[0], &data, sizeof(uint64_t*));  // NOLINT
    return out;
  }

  // for set comparison.
  bool operator<(const RingTupleQueue::Tuple& other) const { return data < other.data; }

  // for set comparison.
  bool operator==(const RingTupleQueue::Tuple& other) const { return data == other.data; }

  uint64_t* operator[](uint64_t index) const {
    return &data[schemas->get_relative_positions(id())[index]];
  }

  uint64_t size() const { return schemas->get_relative_positions(id()).size(); }
}

}  // namespace TupleQueue
