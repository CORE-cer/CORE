#pragma once

#include <map>
#include <string>
#include <vector>

#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/catalog/attribute_info.hpp"

namespace CORETypes {

struct EventInfo {
  EventTypeId id;
  std::string name;
  std::vector<AttributeInfo> attributes_info;
  std::map<std::string, size_t> attribute_names_to_ids;

  EventInfo() noexcept {}

  EventInfo(EventTypeId event_type_id,
            std::string name,
            std::vector<AttributeInfo>&& attributes_info) noexcept
      : id(event_type_id), name(name), attributes_info(attributes_info) {
    for (size_t id = 0; id < attributes_info.size(); id++) {
      attribute_names_to_ids[attributes_info[id].name] = id;
    }
  }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(id, name, attributes_info);
  }
};

}  // namespace CORETypes
