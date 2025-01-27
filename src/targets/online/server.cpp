#include "core_server/library/server.hpp"

#include <chrono>
#include <exception>
#include <iostream>
#include <ostream>
#include <thread>
#include <tracy/Tracy.hpp>
#include "core_server/library/server_config.hpp"
#include "shared/datatypes/aliases/port_number.hpp"

using namespace CORE;

int main(int argc, char** argv) {
  FrameMark;
  try {
    Library::ServerConfig::FixedPorts fixed_ports{443, 5000, 5001};
    Types::PortNumber starting_port{5002};
    Library::OnlineServer server{fixed_ports, starting_port};

    while (true) {
      std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    }

    return 0;
  } catch (std::exception& e) {
    std::cout << "Exception: " << e.what() << std::endl;
    return 1;
  }
}
