#pragma once

#include <memory>
#include <vector>

#include "shared/datatypes/complex_event.hpp"
#include "shared/datatypes/event.hpp"

namespace CORE::Types {
/**
 * Formally a complex event is a pair C = ([i,j], D) where i <= j and D
 * is a subset of {i,...j} representing the data-tuples that are relevant
 * for C. This class represents the complex event but instead, given
 * the corresponding stream S = ti...tj, it returns C[S]
 *
 * TODO: Think aboout how to enumerate this correctly
 */
struct Enumerator {
  std::vector<ComplexEvent> complex_events{};  // TODO: Create it with tecs.

  Enumerator() noexcept = default;

  Enumerator(std::vector<ComplexEvent>&& events) noexcept
      : complex_events(events) {}

  ~Enumerator() noexcept = default;

  auto begin() noexcept { return complex_events.begin(); }

  auto end() noexcept { return complex_events.end(); }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(complex_events);
  }
};
}  // namespace CORE::Types
