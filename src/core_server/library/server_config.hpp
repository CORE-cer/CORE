#pragma once

#include <algorithm>
#include <mutex>
#include <vector>

#include "shared/datatypes/aliases/port_number.hpp"

namespace CORE::Library {

class ServerConfig {
  std::mutex server_config_mutex;
  Types::PortNumber webserver_port;
  // Corresponds to the next open port available
  Types::PortNumber next_open_port;
  std::vector<Types::PortNumber> used_ports;

 public:
  ServerConfig(Types::PortNumber webserver_port, Types::PortNumber next_open_port)
      : webserver_port(webserver_port),
        next_open_port(next_open_port),
        used_ports{webserver_port, next_open_port} {}

  ServerConfig& operator=(const ServerConfig& other) = delete;
  ServerConfig(const ServerConfig& other) = delete;
  ServerConfig& operator=(ServerConfig&& other) = delete;
  ServerConfig(ServerConfig&& other) = delete;

  [[nodiscard]] Types::PortNumber get_webserver_port() {
    std::lock_guard lock(server_config_mutex);
    return webserver_port;
  }

  [[nodiscard]] Types::PortNumber get_next_open_port() {
    std::lock_guard lock(server_config_mutex);
    Types::PortNumber proposed_port = next_open_port++;

    while (std::find(used_ports.begin(), used_ports.end(), proposed_port)
           != used_ports.end()) {
      Types::PortNumber proposed_port = next_open_port++;
    }

    used_ports.push_back(proposed_port);

    return proposed_port;
  }
};

}  // namespace CORE::Library
