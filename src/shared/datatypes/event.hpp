#pragma once

#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#define QUILL_ROOT_LOGGER_ONLY
#include <quill/Quill.h>             // NOLINT
#include <quill/detail/LogMacros.h>  // NOLINT

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
  std::vector<std::unique_ptr<Types::Value>> attributes;
  /**
   * Primary is usually shared with one of the attributes
   * however we do not use a shared pointer here to remove
   * a common jump in memory.
   */
  std::optional<Types::IntValue> primary_time;

  Event() noexcept {}

  Event(UniqueEventTypeId event_type_id,
        std::vector<std::shared_ptr<Types::Value>> attributes,
        std::optional<Types::IntValue> primary_time = {})
      : event_type_id(event_type_id), primary_time(primary_time) {
    this->attributes.reserve(attributes.size());
    for (auto& attr : attributes) {
      this->attributes.push_back(attr->clone());
    }
  }

  Event(UniqueEventTypeId event_type_id,
        std::vector<std::unique_ptr<Types::Value>>&& attributes,
        std::optional<Types::IntValue> primary_time = {}) noexcept
      : event_type_id(event_type_id), primary_time(primary_time) {
    this->attributes = std::move(attributes);
  }

  // TODO: Add this log in the server sider:
  // LOG_TRACE_L3("Destroying Event with id {}", event_type_id);
  ~Event() {}

  Event(const Event& other)
      : event_type_id(other.event_type_id), primary_time(other.primary_time) {
    attributes.reserve(other.attributes.size());
    for (const auto& attr : other.attributes) {
      attributes.push_back(std::unique_ptr<Types::Value>(attr->clone()));
    }
  }

  Event& operator=(const Event& other) {
    if (this == &other) {
      return *this;
    }
    event_type_id = other.event_type_id;
    primary_time = other.primary_time;

    attributes.clear();
    attributes.reserve(other.attributes.size());

    for (const auto& attr : other.attributes) {
      attributes.push_back(std::unique_ptr<Types::Value>(attr->clone()));
    }

    return *this;
  }

  std::string to_string() const {
    std::string out = "(id: " + std::to_string(event_type_id) + " attributes: [";
    for (auto& val : attributes) {
      out += val->to_string() + " ";
    }
    return out + "])";
  }

  std::string to_json() const {
    std::string out = "{";
    out += "\"event_type_id\": " + std::to_string(event_type_id) + ", ";
    out += "\"attributes\": [";
    auto it = attributes.begin();
    while (it != attributes.end()) {
      out += (*it)->to_json();
      ++it;
      if (it != attributes.end()) {
        out += ",";
      }
    }

    out += "]}";

    return out;
  }

  std::string
  to_json_with_attribute_projection(std::vector<bool> attribute_projection) const {
    std::string out = "{";
    out += "\"event_type_id\": " + std::to_string(event_type_id) + ", ";
    out += "\"attributes\": [";
    for (size_t i = 0; i < attributes.size(); ++i) {
      if (attribute_projection[i]) {
        out += attributes[i]->to_json() + ",";
      }
    }
    if (out.back() == ',') {
      out.pop_back();  // Remove the last comma
    }

    out += "]}";

    return out;
  }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(event_type_id, attributes, primary_time);
  }
};

}  // namespace CORE::Types
