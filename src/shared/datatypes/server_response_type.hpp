#pragma once

namespace CORE::Types {
enum struct ServerResponseType {
  AttributeInfo,
  EventInfo,
  EventInfoVector,
  EventTypeId,
  PortNumber,
  QueryInfo,
  StreamInfo,
  StreamInfoVector,
  StreamTypeId,
  Error,
  Warning,
  SuccessEmpty,
};
}  // namespace CORE::Types
