#pragma once

namespace CORE {
namespace Types {
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
}
}  // namespace CORE
