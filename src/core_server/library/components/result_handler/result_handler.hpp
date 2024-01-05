#pragma once

#include <cassert>

#include "core_server/internal/coordination/catalog.hpp"
#include "shared/datatypes/enumerator.hpp"

namespace CORE::Library::Components {

template <class Derived>
class ResultHandler {
 public:
  void operator()(Types::Enumerator enumerator) {
    static_cast<Derived*>(this)->handle_complex_event(enumerator);
  }
};

class OfflineResultHandler : public ResultHandler<OfflineResultHandler> {
  uint_fast64_t test{0};

 public:
  void handle_complex_event(Types::Enumerator enumerator) {
    for (auto& complex_event : enumerator) {
      std::cout << test++ << std::endl;
      std::cout << complex_event.to_string() << std::endl;
    }
  }
};

}  // namespace CORE::Library::Components
