#pragma once

namespace CORETypes {
enum struct ClientRequestType {
  EventDeclaration,
  EventInfoFromId,
  EventInfoFromName,
  ListEvents,
  StreamDeclaration,
  StreamInfoFromId,
  StreamInfoFromName,
  ListStreams,
  AddQuery
};
}  // namespace CORETypes
