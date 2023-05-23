#pragma once

#include <string>
#include <vector>

#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/catalog/attribute_info.hpp"

namespace CORETypes {

struct EventInfo {
  EventTypeId id;
  std::string name;
  std::vector<AttributeInfo> attributes_info;

  EventInfo() noexcept {}

  EventInfo(EventTypeId event_type_id, std::string name,
            std::vector<AttributeInfo>&& attributes_info) noexcept
      : id(event_type_id), name(name), attributes_info(attributes_info) {}

  template <class Archive>
  void serialize(Archive& archive) {
    archive(id, name, attributes_info);
  }
};

}  // namespace CORETypes
