#ifndef TUPLE_HPP
#define TUPLE_HPP

#include <atomic>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <functional>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace RingTupleQueue {

#ifndef SupportedTypes_HPP
#define SupportedTypes_HPP

struct StructType {
  enum Type { INT64, DOUBLE, BOOL, STRING_VIEW, DATE };

  static size_t type_size(StructType::Type type) {
    size_t size_in_bytes;
    switch (type) {
      case StructType::Type::INT64:
        size_in_bytes = sizeof(int64_t);
        break;
      case StructType::Type::DOUBLE:
        size_in_bytes = sizeof(double);
        break;
      case StructType::Type::BOOL:
        size_in_bytes = sizeof(bool);
        break;
      case StructType::Type::STRING_VIEW:
        // For std::string_view we store pointer to the start and end of the string,
        // hence 2 * sizeof(uint64_t)
        size_in_bytes = 2 * sizeof(uint64_t);
        break;
      case StructType::Type::DATE:
        size_in_bytes = sizeof(std::time_t);
        break;
      default:
        throw std::invalid_argument("Unsupported type");
    }
    return (size_in_bytes + sizeof(uint64_t) - 1) / sizeof(uint64_t);  // Ceiling division
  }
};

using SupportedTypes = StructType::Type;
using Type = StructType;

#endif

#ifndef TUPLE_SCHEMA_HPP
#define TUPLE_SCHEMA_HPP

class TupleSchemas {
 private:
  std::vector<std::vector<SupportedTypes>> schemas;
  std::vector<std::vector<uint64_t>> relative_positions;

 public:
  TupleSchemas() {}

  uint64_t add_schema(std::vector<SupportedTypes>& schema) {
    schemas.push_back(schema);
    relative_positions.push_back(get_positions(schemas.size() - 1));
    return schemas.size() - 1;
  }

  uint64_t add_schema(std::vector<SupportedTypes>&& schema) {
    schemas.push_back(std::move(schema));
    relative_positions.push_back(get_positions(schemas.size() - 1));
    return schemas.size() - 1;
  }

  const std::vector<SupportedTypes>& get_schema(uint64_t id) const {
    if (id >= schemas.size()) {
      throw std::out_of_range("TupleSchemas::get_schema: id: " + std::to_string(id)
                              + " out of range. (size = " + std::to_string(schemas.size())
                              + ")");
    }  // In the future we just return the id, no checks to increase efficiency.
    return schemas[id];
  }

  uint64_t size() const { return schemas.size(); }

  const std::vector<uint64_t>& get_relative_positions(uint64_t id) const {
    if (id >= schemas.size()) {
      throw std::out_of_range(
        "TupleSchemas::get_relative_positions: id: " + std::to_string(id)
        + " out of range. (size = " + std::to_string(schemas.size()) + ")");
    }  // OPTIMIZE: In the future we just return the id, no checks to increase efficiency.
    return relative_positions[id];
  }

  uint64_t get_constant_section_size(uint64_t id) const {
    auto& relative_positions_of_id = relative_positions[id];
    uint64_t last_position = relative_positions_of_id.back();
    auto& schema = get_schema(id);
    uint64_t size_of_last_element = Type::type_size(schema.back());
    return last_position + size_of_last_element;
  }

 private:
  std::vector<uint64_t> get_positions(uint64_t id) const {
    if (id >= schemas.size()) {
      throw std::out_of_range("TupleSchemas::get_positions: id out of range");
    }

    // First transform the types to their respective sizes
    std::vector<uint64_t> sizes;
    sizes.reserve(schemas[id].size());
    for (size_t i = 0; i < schemas[id].size(); i++) {
      sizes.push_back(Type::type_size(schemas[id][i]));
    }
    // Then calculate the cumulative sizes in place
    for (int i = 1; i < sizes.size(); i++) {
      sizes[i] += sizes[i - 1];
    }

    std::vector<uint64_t> positions;
    positions.reserve(sizes.size());
    positions.push_back(3);  // Offset from id, system_time and date_time
    for (int i = 1; i < sizes.size(); i++) {
      positions.push_back(3 + sizes[i - 1]);  // Offset from previous sizes.
    }
    return positions;
  }
};

#endif  // TUPLE_SCHEMA_HPP

class Tuple {
 private:
  uint64_t* data;  // Span to hold tuple data.
  TupleSchemas* schemas;

 public:
  explicit Tuple(uint64_t* data, TupleSchemas* schemas) : data(data), schemas(schemas) {}

  uint64_t* get_data() const { return data; }

  uint64_t id() const { return data[0]; }

  const std::vector<SupportedTypes>& get_schema() const {
    return schemas->get_schema(id());
  }

  uint64_t hash() const { return reinterpret_cast<uint64_t>(data); }

  std::chrono::system_clock::time_point system_timestamp() const {
    // Note: Assuming timestamp is stored as the second element of the data span.
    return std::chrono::system_clock::time_point(
      std::chrono::system_clock::duration(data[1]));
  }

  std::chrono::system_clock::time_point data_timestamp() const {
    // Note: Assuming timestamp is stored as the second element of the data span.
    return std::chrono::system_clock::time_point(
      std::chrono::system_clock::duration(data[2]));
  }

  uint64_t system_nanoseconds() const {
    auto compile_time_point = get_compile_time_point();
    auto duration_since_compile = system_timestamp() - compile_time_point;

    return std::chrono::duration_cast<std::chrono::nanoseconds>(duration_since_compile)
      .count();
  }

  uint64_t data_nanoseconds() const {
    auto compile_time_point = get_compile_time_point();
    auto duration_since_compile = data_timestamp() - compile_time_point;

    return std::chrono::duration_cast<std::chrono::nanoseconds>(duration_since_compile)
      .count();
  }

  std::string serialize_data() const {
    // Use memcpy or some other way so that the data ptr is sent as a string, so that
    // it is then converted directly to a uint64_t* pos.
    std::atomic<uint64_t*> atomic_data;
    atomic_data.store(data, std::memory_order_release);
    std::string out(sizeof(uint64_t*), '\0');
    std::memcpy(&out[0], &data, sizeof(uint64_t*));
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

 private:
  const static std::chrono::system_clock::time_point get_compile_time_point() {
    static const auto compile_time_point = []() {
      std::istringstream compile_time_stream(__DATE__ " " __TIME__);
      std::tm compile_time_tm = {};
      compile_time_stream >> std::get_time(&compile_time_tm, "%b %d %Y %H:%M:%S");

      if (compile_time_stream.fail()) {
        assert(false
               && "The compiler should be able to give the date and time of "
                  "compilation!");
      }

      return std::chrono::system_clock::from_time_t(std::mktime(&compile_time_tm));
    }();

    return compile_time_point;
  }
};

}  // namespace RingTupleQueue

namespace std {
template <>
class hash<RingTupleQueue::Tuple> {
 public:
  std::uint64_t operator()(const RingTupleQueue::Tuple& tuple) const {
    return tuple.hash();
  }
};
}  // namespace std

#endif  // TUPLE_HPP
