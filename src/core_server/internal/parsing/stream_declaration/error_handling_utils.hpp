#pragma once

#include <set>
#include <string>
#include <vector>

#include "shared/datatypes/catalog/attribute_info.hpp"
#include "shared/datatypes/parsing/event_info_parsed.hpp"

namespace CORE::Internal::Parsing {

void check_if_stream_already_exists(const std::set<std::string>& stream_names,
                                    std::string stream);
void check_if_attribute_already_exists(std::vector<Types::AttributeInfo>& attributes_info,
                                       Types::AttributeInfo& attribute);
void check_if_event_already_exists(
  std::vector<Types::EventInfoParsed>& event_info_parsed_vector,
  Types::EventInfoParsed& event_info_parsed);

}  // namespace CORE::Internal::Parsing
