#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "shared/datatypes/catalog/attribute_info.hpp"

namespace CORE::Types {

struct EventInfoParsed {
  std::string name;
  std::vector<AttributeInfo> attributes_info;

  // In tested circumstances faster then unordered_map
  std::map<std::string, size_t> attribute_names_to_ids;

  EventInfoParsed() noexcept {}

  EventInfoParsed(std::string name, std::vector<AttributeInfo>&& attributes_info)
      : name(name), attributes_info(std::move(attributes_info)) {
    for (size_t id = 0; id < this->attributes_info.size(); id++) {
      attribute_names_to_ids[this->attributes_info[id].name] = id;
    }
  }

  bool operator==(const EventInfoParsed& other) const {
    bool out = name == other.name
               && attributes_info.size() == other.attributes_info.size()
               && attribute_names_to_ids.size() == other.attribute_names_to_ids.size();
    if (!out) {
      return false;
    }
    for (uint64_t i = 0; i < attributes_info.size(); i++) {
      out = out && (attributes_info.at(i) == other.attributes_info.at(i));
    }
    for (const auto& [key, value] : attribute_names_to_ids) {
      out = out && other.attribute_names_to_ids.at(key) == value;
    }
    return out;
  }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(name, attributes_info);
  }
};

}  // namespace CORE::Types
