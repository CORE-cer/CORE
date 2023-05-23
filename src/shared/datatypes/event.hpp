#pragma once

#include <vector>

#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/value/value.hpp"

namespace CORETypes {

/**
 * An Event is what the base CORE paper defines as a data-tuple. Formally
 * a data-tuple is a partial mapping from attribute names to values. In
 * this case, the attribute names are not shown directly here, instead,
 * event_types_id's are given here, and through the schema the mapping
 * from attribute names to values can be obtained.
 */
struct Event {
  time_t event_date;
  /**
   * An EventType is an id that is used to know what each index in the
   * attributes vector represent. To obtain this, it needs to be requested
   * to the Schema
   */
  EventTypeId event_type_id;
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
  std::vector<std::shared_ptr<Value>> attributes;

  Event() noexcept {}

  Event(time_t event_date, EventTypeId event_type_id,
        std::vector<std::shared_ptr<Value>> attributes) noexcept
      : event_date(event_date),
        event_type_id(event_type_id),
        attributes(attributes) {}

  template <class Archive>
  void serialize(Archive& archive) {
    archive(event_date, event_type_id, attributes);
  }
};

}  // namespace CORETypes
