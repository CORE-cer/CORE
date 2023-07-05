#pragma once
#include <string>

namespace CORE::Internal::CEQL {

class Projection {
 public:
  Projection(std::string event, std::string stream) {
    this->event = event;
    this->stream = stream;
  }

  Projection(std::string event) {
    this->event = event;
    this->stream = "";
  }

  std::string event;
  std::string stream;
};

}  // namespace CORE::Internal::CEQL
