#include <catch2/catch_test_macros.hpp>
#include <thread>
#include <vector>

#include "shared/message_receiver/zmq_message_receiver.hpp"
#include "shared/message_sender/zmq_message_sender.hpp"

namespace CerealThirdPartyTesting {
using namespace InternalCORE;

TEST_CASE("A sent message is received exactly as it was sent.", "[zmq]") {
  std::string sent_message = "Hello World";
  std::string received_message;

  std::thread receiver_thread([&]() {
    ZMQMessageReceiver receiver("tcp://localhost:5555");
    received_message = receiver.receive();
  });

  ZMQMessageSender message_sender("tcp://*:5555");
  message_sender.send(sent_message);

  receiver_thread.join();

  REQUIRE(received_message == sent_message);
}

}  // namespace CerealThirdPartyTesting

