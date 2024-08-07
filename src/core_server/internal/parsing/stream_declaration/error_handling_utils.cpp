#include "error_handling_utils.hpp"

#include <algorithm>
#include <set>
#include <string>
#include <vector>

#include "shared/datatypes/catalog/attribute_info.hpp"
#include "shared/datatypes/parsing/event_info_parsed.hpp"
#include "shared/exceptions/parsing/attribute_name_already_declared_exception.hpp"
#include "shared/exceptions/parsing/event_name_already_declared.hpp"
#include "shared/exceptions/parsing/stream_name_already_declared_exception.hpp"

namespace CORE::Internal::Parsing {

void check_if_stream_already_exists(const std::set<std::string>& stream_names,
                                    std::string stream) {
  if (stream_names.contains(stream)) {
    throw StreamNameAlreadyDeclaredException("Stream name " + stream
                                             + " was already declared");
  }
}

void check_if_attribute_already_exists(std::vector<Types::AttributeInfo>& attributes_info,
                                       Types::AttributeInfo& attribute) {
  std::string attribute_name = attribute.name;
  auto it = std::find_if(attributes_info.begin(),
                         attributes_info.end(),
                         [&attribute_name](const Types::AttributeInfo& attr) {
                           return attr.name == attribute_name;
                         });
  if (it != attributes_info.end()) {
    throw AttributeNameAlreadyDeclared(
      "Attribute name " + attribute_name
      + " is defined more than one more time in the same event");
  }
}

void check_if_event_already_exists(
  std::vector<Types::EventInfoParsed>& event_info_parsed_vector,
  Types::EventInfoParsed& event_info_parsed) {
  std::string event_name = event_info_parsed.name;
  auto it = std::find_if(event_info_parsed_vector.begin(),
                         event_info_parsed_vector.end(),
                         [&event_name](const Types::EventInfoParsed& event) {
                           return event.name == event_name;
                         });
  if (it != event_info_parsed_vector.end()) {
    throw EventNameAlreadyDeclared("Event name " + event_name
                                   + " is defined more than one more time in the stream");
  }
}

}  // namespace CORE::Internal::Parsing
