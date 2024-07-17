#pragma once

namespace CORE::Types {
enum struct ClientRequestType {
  EventDeclaration,
  EventInfoFromId,
  EventInfoFromName,
  ListEvents,
  StreamDeclaration,
  StreamDeclarationFromString,
  StreamInfoFromId,
  StreamInfoFromName,
  ListStreams,
  AddQuery,
  SetOption
};
}  // namespace CORE::Types
