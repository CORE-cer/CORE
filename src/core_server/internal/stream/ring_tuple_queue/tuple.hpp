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

struct _Types {
  enum Types { INT64, DOUBLE, BOOL, STRING_VIEW, DATE };

  static size_t type_size(_Types::Types type) {
    size_t size_in_bytes;
    switch (type) {
      case _Types::Types::INT64:
        size_in_bytes = sizeof(int64_t);
        break;
      case _Types::Types::DOUBLE:
        size_in_bytes = sizeof(double);
        break;
      case _Types::Types::BOOL:
        size_in_bytes = sizeof(bool);
        break;
      case _Types::Types::STRING_VIEW:
        // For std::string_view we store pointer to the start and end of the string,
        // hence 2 * sizeof(uint64_t)
        size_in_bytes = 2 * sizeof(uint64_t);
        break;
      case _Types::Types::DATE:
        size_in_bytes = sizeof(std::chrono::system_clock::time_point);
        break;
      default:
        throw std::invalid_argument("Unsupported type");
    }
    return (size_in_bytes + sizeof(uint64_t) - 1) /
           sizeof(uint64_t);  // Ceiling division
  }
};

using SupportedTypes = _Types::Types;
using Types = _Types;

#endif

#ifndef TUPLE_SCHEMA_HPP
#define TUPLE_SCHEMA_HPP

class TupleSchema {
 private:
  std::vector<std::vector<SupportedTypes>> schemas;
  std::vector<std::vector<uint64_t>> relative_positions;

 public:
  TupleSchema() {}

  uint64_t add_schema(const std::vector<SupportedTypes>& schema) {
    schemas.push_back(schema);
    relative_positions.push_back(get_positions(schemas.size() - 1));
    return schemas.size() - 1;
  }

  std::vector<SupportedTypes> get_schema(uint64_t id) const {
    if (id >= schemas.size()) {
      throw std::out_of_range("TupleSchema::get_schema: id out of range");
    }  // In the future we just return the id, no checks to increase efficiency.
    return schemas[id];
  }

  uint64_t size() const { return schemas.size(); }

  const std::vector<uint64_t> get_relative_positions(uint64_t id) const {
    if (id >= schemas.size()) {
      throw std::out_of_range("TupleSchema::get_schema: id out of range");
    }  // In the future we just return the id, no checks to increase efficiency.
    return relative_positions[id];
  }

 private:
    std::vector<uint64_t> get_positions(uint64_t id) const {
    if (id >= schemas.size()) {
      throw std::out_of_range(
          "TupleSchema::get_positions: id out of range");
    }

    // First transform the types to their respective sizes
    std::vector<uint64_t> sizes;
    sizes.reserve(schemas[id].size());
    std::transform(
        schemas[id].begin(), schemas[id].end(), std::back_inserter(sizes),
        [](const SupportedTypes& a) { return Types::type_size(a); });

    // Then calculate the cumulative sizes in place
    sizes[0] += 1; // Add one to skip the timestamp
    for (int i = 1; i < sizes.size(); i++) {
      sizes[i] += sizes[i - 1];
    }
    return sizes;
  }
};

#endif  // TUPLE_SCHEMA_HPP

class Tuple {
 private:
  std::span<uint64_t> data;  // Span to hold tuple data.
  TupleSchema* schema;

 public:
  explicit Tuple(std::span<uint64_t> data, TupleSchema* schema)
      : data(data), schema(schema) {}

  uint64_t id() const { return data[0]; }

  std::chrono::system_clock::time_point timestamp() const {
    // Note: Assuming timestamp is stored as the second element of the data span.
    return std::chrono::system_clock::time_point(
        std::chrono::system_clock::duration(data[1]));
  }

  uint64_t* operator[](uint64_t index) {
    return &data[schema->get_relative_positions(id())[index]];
  }
};

}  // namespace RingTupleQueue

#endif  // TUPLE_HPP
