#include <string>
#include <vector>

#include "shared/catalog/event_type_id.hpp"

namespace CORECatalog {

class Schema {
 private:
  std::vector<std::vector<std::string>> schema_vector;

 public:
  [[nodiscard]] EventTypeId add_schema(
    std::vector<std::string>&& attributes) noexcept {
    schema_vector.push_back(std::move(attributes));
    return schema_vector.size() - 1;
  }

  [[nodiscard]] std::vector<std::string> get_attributes(
    const EventTypeId event_type_id) const noexcept {
    if (event_type_id < schema_vector.size()) {
      return schema_vector[event_type_id];
    } else {
      // EventTypeId not found. Return an empty vector
      // maybe in the future, we will want to raise an exception.
      return std::vector<std::string>();
    }
  }
};

}  // namespace CORECatalog
