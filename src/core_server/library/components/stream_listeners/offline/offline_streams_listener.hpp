#pragma once

#include <vector>

#include "core_server/internal/interface/backend.hpp"

namespace CORE::Library::Components {

using Backend = CORE::Internal::Interface::Backend;

class OfflineStreamsListener {
 private:
  Backend& backend;

 public:
  OfflineStreamsListener(Backend& backend, Types::PortNumber port_number);

  // Delete Copy constructor and assigment as that should not be done with the stream listener
  OfflineStreamsListener(const OfflineStreamsListener&) = delete;
  OfflineStreamsListener& operator=(const OfflineStreamsListener&) = delete;

  void receive_stream(const Types::Stream& stream);
};

}  // namespace CORE::Library::Components
