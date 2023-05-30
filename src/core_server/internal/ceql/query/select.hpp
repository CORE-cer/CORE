#pragma once
#include <string>
#include <set>

namespace InternalCORECEQL {

struct Select {
  enum class Strategy {
    ALL,
    MAX,
    NEXT,
    LAST,
    STRICT,
    DEFAULT = Strategy::ALL
  };

  Strategy strategy;
  std::set<std::string> variable_names;
};
}
