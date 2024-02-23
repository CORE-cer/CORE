#pragma once

namespace CORE::Types {
enum struct ClientRequestType {
  EventDeclaration,
  EventInfoFromId,
  EventInfoFromName,
  ListEvents,
  StreamDeclarationOld,
  StreamDeclaration,
  StreamInfoFromId,
  StreamInfoFromName,
  ListStreams,
  AddQuery
};
}  // namespace CORE::Types
