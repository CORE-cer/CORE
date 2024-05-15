#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "shared/datatypes/aliases/event_type_id.hpp"
#include "value.hpp"

namespace CORE::Types {

/**
 * An Event is what the base CORE paper defines as a data-tuple. Formally
 * a data-tuple is a partial mapping from attribute names to values. In
 * this case, the attribute names are not shown directly here, instead,
 * event_types_id's are given here, and through the schema the mapping
 * from attribute names to values can be obtained.
 */
struct Event {
  /**
   * An EventType is an id that is used to know what each index in the
   * attributes vector represent. To obtain this, it needs to be requested
   * to the Schema
   */
  UniqueEventTypeId event_type_id;
  std::optional<uint64_t> primary_time;
  /**
   * Shared pointers are used because it can be exploited in serialization,
   * officially from cereal (our current serialization provider):
   *   "cereal will make sure that the data pointed to by an
   *   std::shared_ptr is serialized only once, even if several
   *   std::shared_ptr (or std::weak_ptr) that point to the same data
   *   are serialized in the same archive. This means that when saving
   *   to an archive cereal will avoid duplication, and when loading
   *   from an archive, cereal will not allocate extraneous data."
   *   https://uscilab.github.io/cereal/pointers.html
   */
  std::vector<std::shared_ptr<Types::Value>> attributes;

  Event() noexcept {}

  Event(UniqueEventTypeId event_type_id,
        std::vector<std::shared_ptr<Types::Value>> attributes) noexcept
      : event_type_id(event_type_id), attributes(attributes) {}

  Event(UniqueEventTypeId event_type_id,
        std::vector<std::shared_ptr<Types::Value>> attributes,
        uint64_t primary_time) noexcept
      : Event(event_type_id, attributes) {
    primary_time = primary_time;
  }

  std::string to_string() const {
    std::string out = "(id: " + std::to_string(event_type_id) + " attributes: [";
    for (auto& val : attributes) {
      out += val->to_string() + " ";
    }
    return out + "])";
  }

  std::string to_string_java() const {
    std::string out = "";
    if (event_type_id == 0) {
      out += "BUY(";
    } else {
      out += "SELL(";
    }
    out += "id=";
    out += attributes[0]->to_string();
    out += ".0";
    out += ")";
    return out;
  }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(event_type_id, attributes);
  }
};

}  // namespace CORE::Types
