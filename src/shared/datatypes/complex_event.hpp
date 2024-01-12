#pragma once

#include <memory>
#include <vector>

#include "shared/datatypes/event.hpp"

namespace CORE::Types {
/**
 * Formally a complex event is a pair C = ([i,j], D) where i <= j and D
 * is a subset of {i,...j} representing the data-tuples that are relevant
 * for C. This class represents the complex event but instead, given
 * the corresponding stream S = ti...tj, it returns C[S]
 */
struct ComplexEvent {
  uint64_t start;
  uint64_t end;
  std::vector<Event> events;

  ComplexEvent() : start(0), end(0), events() {}

  ComplexEvent(uint64_t start, uint64_t end, std::vector<Event>&& events) noexcept
      : start(start), end(end), events(std::move(events)) {}

  ComplexEvent(uint64_t start, uint64_t end, std::initializer_list<Event>&& events) noexcept
      : start(start), end(end), events(std::move(events)) {}

  ComplexEvent(uint64_t start, uint64_t end, std::vector<Event>& events) noexcept
      : start(start), end(end), events(events) {}

  std::string to_string() const {
    std::string out = "[" + std::to_string(start) + "," + std::to_string(end) + "], [";
    for (auto& event : events) {
      out += event.to_string() + " ";
    }
    return out + "]";
  }

  std::string to_string_java() const {
    std::string out = "";
    for (auto& event : events) {
      out += event.to_string_java() + " ";
    }
    return out;
  }

  ~ComplexEvent() noexcept = default;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(start, end, events);
  }
};
}  // namespace CORE::Types
