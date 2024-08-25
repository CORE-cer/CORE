#pragma once

#include <cstdint>
#include <vector>
namespace TupleQueue {

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
    positions.push_back(2);  // Offset from id and time
    for (int i = 1; i < sizes.size(); i++) {
      positions.push_back(2 + sizes[i - 1]);  // Offset from previous sizes.
    }
    return positions;
  }
};

}  // namespace TupleQueue
