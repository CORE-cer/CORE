#include "core_server/internal/coordination/mediator.hpp"

int main(int argc, char** argv) {
  InternalCORE::Mediator mediator(5000);
  mediator.start();
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
  }
}
