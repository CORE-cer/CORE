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
  ClientException,
  AttributeNameAlreadyDeclaredException,
  AttributeNotDefinedException,
  EventNameAlreadyDeclaredException,
  EventNotDefinedException,
  EventNotInStreamException,
  ParsingSyntaxException,
  StreamNameAlreadyDeclaredException,
  StreamNotFoundException,
};
}  // namespace CORE::Types
