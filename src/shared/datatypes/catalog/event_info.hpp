#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/catalog/attribute_info.hpp"

namespace CORE::Types {

struct EventInfo {
  UniqueEventTypeId id;
  std::string name;
  std::vector<AttributeInfo> attributes_info;

  // In tested circumstances faster then unordered_map
  std::map<std::string, size_t> attribute_names_to_ids;

  EventInfo() noexcept {}

  EventInfo(UniqueEventTypeId event_type_id,
            std::string name,
            std::vector<AttributeInfo>&& attributes_info) noexcept
      : id(event_type_id), name(name), attributes_info(attributes_info) {
    for (size_t id = 0; id < attributes_info.size(); id++) {
      attribute_names_to_ids[attributes_info[id].name] = id;
    }
  }

  bool operator==(const EventInfo& other) const {
    bool out = id == other.id && name == other.name
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
    archive(id, name, attributes_info);
  }

  std::string to_json() const { return ""; }
};

}  // namespace CORE::Types
