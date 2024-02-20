#pragma once

#include <map>
#include <optional>
#include <string>
#include <vector>

#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/catalog/attribute_info.hpp"

namespace CORE::Types {

struct EventInfo {
  std::string name;
  std::vector<AttributeInfo> attributes_info;
  std::map<std::string, size_t> attribute_names_to_ids;

  EventInfo() noexcept {}

  EventInfo(std::string name, std::vector<AttributeInfo>&& attributes_info) noexcept
      : name(name), attributes_info(attributes_info) {
    init_attribute_names_to_ids();
  }

  EventInfo(EventTypeId event_type_id,
            std::string name,
            std::vector<AttributeInfo>&& attributes_info) noexcept
      : name(name), attributes_info(attributes_info) {
    init_attribute_names_to_ids();
  }

  void init_attribute_names_to_ids() {
    for (size_t id = 0; id < attributes_info.size(); id++) {
      attribute_names_to_ids[attributes_info[id].name] = id;
    }
  }

  bool operator==(const EventInfo& other) const {
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

struct CatalogEventInfo {
  EventTypeId id;
  EventInfo event_info;

  CatalogEventInfo(EventTypeId event_type_id,
                   std::string name,
                   std::vector<AttributeInfo>&& attributes_info) noexcept
      : id(event_type_id), event_info(name, std::move(attributes_info)) {}

  CatalogEventInfo(EventTypeId event_type_id, EventInfo event_info) noexcept
      : id(event_type_id), event_info(event_info) {}

  CatalogEventInfo() noexcept : event_info() {}

  bool operator==(const CatalogEventInfo& other) const {
    bool out = event_info == other.event_info && id == other.id;
    return out;
  }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(id, event_info);
  }
};

}  // namespace CORE::Types
