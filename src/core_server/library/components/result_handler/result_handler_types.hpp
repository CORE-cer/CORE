#pragma once

#include "core_server/internal/evaluation/enumeration/tecs/enumerator.hpp"

namespace CORE::Library::Components {
enum ResultHandlerType {
  OFFLINE,
  ONLINE,
  CUSTOM,
};

struct QueryResult {
  enum class Kind {
    Exists,
    Enumerate,
    NoMatch
  };

  Kind kind = Kind::NoMatch;
  bool matched = false;
  std::optional<Internal::tECS::Enumerator> enumerator = std::nullopt;

  static QueryResult from_exists(bool matched) {
    return QueryResult{Kind::Exists, matched, std::nullopt};
  }

  static QueryResult from_enumerator(std::optional<Internal::tECS::Enumerator> enumerator) {
    return QueryResult{Kind::Enumerate, false, std::move(enumerator)};
  }

  static QueryResult from_no_match() {
    return QueryResult{Kind::NoMatch, false, std::nullopt};
  }
};

}  // namespace CORE::Library::Components
