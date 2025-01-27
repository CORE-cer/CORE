#include "core_server/library/server.hpp"

#include <chrono>
#include <exception>
#include <iostream>
#include <ostream>
#include <thread>
#include <tracy/Tracy.hpp>
#include <utility>

#include "core_server/library/server_config.hpp"

using namespace CORE;

int main(int argc, char** argv) {
  FrameMark;
  try {
    Library::ServerConfig server_config = Library::ServerConfig::from_args(argc, argv);
    Library::OnlineServer server{std::move(server_config)};

    while (true) {
      std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    }

    return 0;
  } catch (std::exception& e) {
    std::cout << "Exception: " << e.what() << std::endl;
    return 1;
  }
}
