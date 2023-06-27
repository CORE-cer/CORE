#ifndef TUPLE_HPP
#define TUPLE_HPP

#include <chrono>
#include <cstdint>
#include <numeric>
#include <span>
#include <string_view>
#include <vector>

namespace RingTupleQueue {

#ifndef SupportedTypes_HPP
#define SupportedTypes_HPP

struct _Type {
  enum Type { INT64, DOUBLE, BOOL, STRING_VIEW, DATE };

  static size_t type_size(_Type::Type type) {
    size_t size_in_bytes;
    switch (type) {
      case _Type::Type::INT64:
        size_in_bytes = sizeof(int64_t);
        break;
      case _Type::Type::DOUBLE:
        size_in_bytes = sizeof(double);
        break;
      case _Type::Type::BOOL:
        size_in_bytes = sizeof(bool);
        break;
      case _Type::Type::STRING_VIEW:
        // For std::string_view we store pointer to the start and end of the string,
        // hence 2 * sizeof(uint64_t)
        size_in_bytes = 2 * sizeof(uint64_t);
        break;
      case _Type::Type::DATE:
        size_in_bytes = sizeof(std::chrono::system_clock::time_point);
        break;
      default:
        throw std::invalid_argument("Unsupported type");
    }
    return (size_in_bytes + sizeof(uint64_t) - 1)
           / sizeof(uint64_t);  // Ceiling division
  }
};

using SupportedTypes = _Type::Type;
using Type = _Type;

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
      throw std::out_of_range(
        "TupleSchemas::get_schema: id: " + std::to_string(id)
        + " out of range. (size = " + std::to_string(schemas.size()) + ")");
    }  // In the future we just return the id, no checks to increase efficiency.
    return schemas[id];
  }

  uint64_t size() const { return schemas.size(); }

  const std::vector<uint64_t>& get_relative_positions(uint64_t id) const {
    if (id >= schemas.size()) {
      throw std::out_of_range(
        "TupleSchemas::get_relative_positions: id: " + std::to_string(id)
        + " out of range. (size = " + std::to_string(schemas.size()) + ")");
    }  // In the future we just return the id, no checks to increase efficiency.
    return relative_positions[id];
  }

  int64_t get_constant_section_size(uint64_t id) const {
    auto& relative_positions_of_id = relative_positions[id];
    int64_t last_position = relative_positions_of_id.back();
    auto& schema = get_schema(id);
    int64_t size_of_last_element = Type::type_size(schema.back());
    return last_position + size_of_last_element;
  }

 private:
  std::vector<uint64_t> get_positions(uint64_t id) const {
    if (id >= schemas.size()) {
      throw std::out_of_range(
        "TupleSchemas::get_positions: id out of range");
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
    positions.push_back(2);  // Offset from id and time
    for (int i = 1; i < sizes.size(); i++) {
      positions.push_back(2 + sizes[i - 1]);  // Offset from previous sizes.
    }
    return positions;
  }
};

#endif  // TUPLE_SCHEMA_HPP

class Tuple {
 private:
  std::span<uint64_t> data;  // Span to hold tuple data.
  TupleSchemas* schemas;

 public:
  explicit Tuple(std::span<uint64_t> data, TupleSchemas* schemas)
      : data(data), schemas(schemas) {}

  uint64_t id() const { return data[0]; }

  std::chrono::system_clock::time_point timestamp() const {
    // Note: Assuming timestamp is stored as the second element of the data span.
    return std::chrono::system_clock::time_point(
      std::chrono::system_clock::duration(data[1]));
  }

  uint64_t* operator[](uint64_t index) {
    return &data[schemas->get_relative_positions(id())[index]];
  }
};

}  // namespace RingTupleQueue

#endif  // TUPLE_HPP
