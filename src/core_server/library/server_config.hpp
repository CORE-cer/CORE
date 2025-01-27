#pragma once

#include <algorithm>
#include <mutex>
#include <vector>

#include "shared/datatypes/aliases/port_number.hpp"

namespace CORE::Library {

class ServerConfig {
 public:
  struct FixedPorts {
    Types::PortNumber webserver;
    Types::PortNumber router;
    Types::PortNumber stream_listener;
  };

 private:
  std::mutex server_config_mutex;
  FixedPorts fixed_ports;
  // Corresponds to the next open port available
  Types::PortNumber next_open_port;
  std::vector<Types::PortNumber> used_ports;

 public:
  ServerConfig(FixedPorts fixed_ports, Types::PortNumber next_open_port)
      : fixed_ports(fixed_ports),
        next_open_port(next_open_port),
        used_ports{
          fixed_ports.webserver,
          fixed_ports.router,
          fixed_ports.stream_listener,
        } {}

  ServerConfig& operator=(const ServerConfig& other) = delete;
  ServerConfig(const ServerConfig& other) = delete;
  ServerConfig& operator=(ServerConfig&& other) = delete;
  ServerConfig(ServerConfig&& other) = delete;

  [[nodiscard]] FixedPorts& get_fixed_ports() {
    std::lock_guard lock(server_config_mutex);
    return fixed_ports;
  }

  [[nodiscard]] Types::PortNumber get_next_open_port() {
    std::lock_guard lock(server_config_mutex);
    Types::PortNumber proposed_port = next_open_port++;

    while (std::find(used_ports.begin(), used_ports.end(), proposed_port)
           != used_ports.end()) {
      proposed_port = next_open_port++;
    }

    used_ports.push_back(proposed_port);

    return proposed_port;
  }
};

}  // namespace CORE::Library
