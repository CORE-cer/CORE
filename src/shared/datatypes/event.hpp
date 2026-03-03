#pragma once

#include <cereal/access.hpp>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
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
  std::vector<std::unique_ptr<Types::Value>> attributes;
  std::optional<Types::IntValue> primary_time;
  /**
   * Resolved marked-variable name from the QueryCatalog.  Set during
   * convert_enumerator() so that it survives serialisation over ZMQ.
   * Examples: "Sell", "s" (AS variable), "TICKER>Sell" (stream>event).
   */
  std::string variable_name;

 private:
  friend class cereal::access;
  std::optional<UniqueEventTypeId> event_type_id;

 public:
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
      : event_type_id(other.event_type_id),
        primary_time(other.primary_time),
        variable_name(other.variable_name) {
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
    variable_name = other.variable_name;

    attributes.clear();
    attributes.reserve(other.attributes.size());

    for (const auto& attr : other.attributes) {
      attributes.push_back(std::unique_ptr<Types::Value>(attr->clone()));
    }

    return *this;
  }

  UniqueEventTypeId get_event_type_id() const {
    if (!event_type_id.has_value()) {
      throw std::runtime_error("Event does not have an event_type_id set.");
    }
    return event_type_id.value();
  }

  std::string to_string() const {
    std::string out = "(id: " + std::to_string(get_event_type_id()) + " attributes: [";
    for (auto& val : attributes) {
      out += val->to_string() + " ";
    }
    return out + "])";
  }

  std::string to_json() const {
    std::string out = "{";
    out += "\"event_type_id\": " + std::to_string(get_event_type_id()) + ", ";
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

  template <class Archive>
  void serialize(Archive& archive) {
    archive(event_type_id, attributes, primary_time, variable_name);
  }
};

}  // namespace CORE::Types
