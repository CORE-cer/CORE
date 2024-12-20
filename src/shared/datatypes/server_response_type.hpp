#pragma once

namespace CORE::Types {
enum struct ServerResponseType {
  AttributeInfo,
  EventInfo,
  EventInfoVector,
  EventTypeId,
  PortNumber,
  QueryInfo,
  QueryInfoVector,
  StreamInfo,
  StreamInfoVector,
  StreamTypeId,
  Error,
  Warning,
  SuccessEmpty,
  AttributeNameAlreadyDeclaredException,
  AttributeNotDefinedException,
  ClientException,
  EventNameAlreadyDeclaredException,
  EventNotDefinedException,
  EventNotInStreamException,
  ParsingSyntaxException,
  StreamNameAlreadyDeclaredException,
  StreamNotFoundException,
};
}  // namespace CORE::Types
