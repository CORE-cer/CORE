#pragma once
#include <string>

namespace CORE {
namespace Internal {
namespace CEQL {

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

}  // namespace CEQL
}  // namespace Internal
}  // namespace CORE
