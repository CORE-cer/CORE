#include <atomic>
#include <catch2/catch_test_macros.hpp>
#include <thread>
#include <vector>

#include "shared/networking/message_broadcaster/zmq_message_broadcaster.hpp"
#include "shared/networking/message_dealer/zmq_message_dealer.hpp"
#include "shared/networking/message_receiver/zmq_message_receiver.hpp"
#include "shared/networking/message_router/zmq_message_router.hpp"
#include "shared/networking/message_sender/zmq_message_sender.hpp"
#include "shared/networking/message_subscriber/zmq_message_subscriber.hpp"

namespace CerealThirdPartyTesting {
using namespace InternalCORE;

TEST_CASE("A sent message is received exactly as it was sent.", "[zmq]") {
  std::string sent_message1 = "Hello World 1";
  std::string sent_message2 = "Hello World 2";
  std::vector<std::string> received_messages;

  std::thread receiver_thread([&]() {
    // Receive all messages from 5555
    ZMQMessageReceiver receiver("tcp://*:5555");
    received_messages.push_back(receiver.receive());
    received_messages.push_back(receiver.receive());
  });

  // send to localhost:5555
  ZMQMessageSender message_sender("tcp://localhost:5555");
  message_sender.send(sent_message1);
  message_sender.send(sent_message2);

  receiver_thread.join();

  REQUIRE(received_messages[0] == sent_message1);
  REQUIRE(received_messages[1] == sent_message2);
}

TEST_CASE(
  "MessageRouterRequesterTest - messages are sent specifically to each "
  "listener",
  "[zmq]") {
  const std::string address = "inproc://test";
  const std::string test_message1 = "ping1";
  const std::string test_message2 = "ping2";

  auto transformer = [](const std::string& message) {
    return "Transformed: " + message;
  };

  ZMQMessageRouter router("tcp://*:5555", transformer);

  std::thread router_thread([&router]() { router.start(); });

  std::string received_message1;
  std::string received_message2;

  ZMQMessageDealer dealer1("tcp://localhost:5555");
  std::thread dealer1_thread([&dealer1]() {
    std::string reply = dealer1.send_and_receive("ping1");
    INFO("DEALER 1 received reply: " + reply);
    REQUIRE(reply == "Transformed: ping1");
  });

  ZMQMessageDealer dealer2("tcp://localhost:5555");
  std::thread dealer2_thread([&dealer2]() {
    std::string reply = dealer2.send_and_receive("ping2");
    INFO("DEALER 2 received reply: " + reply);
    REQUIRE(reply == "Transformed: ping2");
  });

  dealer1_thread.join();
  dealer2_thread.join();

  INFO("Shutting down...");

  router.stop();
  router_thread.join();
}

TEST_CASE(
  "MessageRouterRequesterTest - messages are sent specifically to each "
  "listener: 100 listeners 1 router",
  "[zmq]") {
  const std::string address = "inproc://test";
  const std::string test_message1 = "ping1";
  const std::string test_message2 = "ping2";

  auto transformer = [](const std::string& message) {
    return "Transformed: " + message;
  };

  ZMQMessageRouter router("tcp://*:5555", transformer);

  std::thread router_thread([&router]() { router.start(); });

  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  std::unique_ptr<std::thread> threads[200];
  // make an atomic int
  std::atomic<int> counter = 0;
  for (int i = 0; i < 100; i++) {
    //std::cout << "i = " << i << std::endl;
    threads[i] = std::make_unique<std::thread>([&]() {
      int j = counter.fetch_add(1);
      std::string to_send = "ping" + std::to_string(j);
      //std::cout << "DEALER thread started" << std::endl;
      ZMQMessageDealer dealer("tcp://localhost:5555");
      std::string reply;
      try {
        reply = dealer.send_and_receive(to_send);
      } catch (std::runtime_error err) {
        REQUIRE(false);
      }
      //std::cout << "DEALER thread received reply" << reply << std::endl;
      REQUIRE(reply == "Transformed: " + to_send);
    });
  }

  for (int i = 0; i < 100; i++) {
    // join only if it is possible to join
    if (threads[i] && threads[i]->joinable())
      threads[i]->join();
  }

  INFO("Shutting down...");

  router.stop();
  router_thread.join();
}

TEST_CASE("A broadcast message is received exactly as it was sent.",
          "[zmq]") {
  std::string sent_message1 = "Hello World 1";
  std::vector<std::string> received_messages;

  std::thread publisher_thread([&]() {
    // Receive all messages from 5555
    ZMQMessageBroadcaster message_sender("tcp://*:5555");
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    message_sender.broadcast(sent_message1);
  });

  std::thread receiver_thread1([&]() {
    // Receive all messages from 5555
    ZMQMessageSubscriber receiver("tcp://localhost:5555");
    REQUIRE(receiver.receive() == sent_message1);
  });

  std::thread receiver_thread2([&]() {
    // Receive all messages from 5555
    ZMQMessageSubscriber receiver("tcp://localhost:5555");
    REQUIRE(receiver.receive() == sent_message1);
  });

  publisher_thread.join();
  receiver_thread1.join();
  receiver_thread2.join();
}

}  // namespace CerealThirdPartyTesting
