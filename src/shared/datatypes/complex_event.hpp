#pragma once

#include <memory>
#include <vector>

#include "shared/datatypes/event.hpp"

namespace CORETypes {
struct ComplexEvent {
  /**
   * Formally a complex event is a pair C = ([i,j], D) where i <= j and D
   * is a subset of {i,...j} representing the data-tuples that are relevant
   * for C. This class represents the complex event but instead, given
   * the corresponding stream S = ti...tj, it returns C[S]
   */

  std::vector<Event> events;

  ComplexEvent() noexcept = default;

  ComplexEvent(std::vector<Event>&& events) noexcept
      : events(std::move(events)) {}

  ComplexEvent(std::initializer_list<Event>&& events) noexcept
      : events(std::move(events)) {}

  ~ComplexEvent() noexcept = default;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(events);
  }
};
}  // namespace CORETypes
