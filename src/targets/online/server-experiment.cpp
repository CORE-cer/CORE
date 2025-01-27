#include <chrono>
#include <exception>
#include <iostream>
#include <ostream>
#include <thread>

#include "core_server/library/server.hpp"

using namespace CORE;
using namespace CORE::Internal;

int main(int argc, char** argv) {
  try {
    Library::OnlineServer server{443, 5000};
    while (true) {
      std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    }
  } catch (std::exception& e) {
    std::cout << "Exception: " << e.what() << std::endl;
  }
}
