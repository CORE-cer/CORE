#include <atomic>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
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
  auto transformer = [](const std::string& message) {
    return "Transformed: " + message;
  };

  using TransformFunc = std::function<std::string(const std::string&)>;
  ZMQMessageRouter<TransformFunc> router("tcp://*:5555",
                                         std::move(transformer));

  std::thread router_thread([&router]() { router.start(); });

  std::this_thread::sleep_for(std::chrono::milliseconds(20));
  static const int amount_of_threads = 2;
  std::unique_ptr<std::thread> threads[amount_of_threads];
  // make an atomic int
  std::atomic<int> counter = 0;
  bool results[amount_of_threads];
  for (int i = 0; i < amount_of_threads; i++) {
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
        results[j] = false;
      }
      //std::cout << "DEALER thread received reply" << reply << std::endl;
      results[j] = reply == "Transformed: " + to_send;
    });
  }

  for (int i = 0; i < amount_of_threads; i++) {
    // join only if it is possible to join
    if (threads[i] && threads[i]->joinable()) threads[i]->join();
  }

  INFO("Shutting down...");

  router.stop();
  router_thread.join();
  for (int i = 0; i < amount_of_threads; i++) {
    REQUIRE(results[i]);
  }
}

TEST_CASE("A broadcast message is received exactly as it was sent.",
          "[zmq]") {
  std::string sent_message1 = "Hello World 1";
  std::vector<std::string> received_messages;

  ZMQMessageBroadcaster message_sender("tcp://*:5555");
  std::thread publisher_thread([&]() {
    // Receive all messages from 5555
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
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

TEST_CASE(
    "MessageRouterRequesterTest - messages are sent specifically to each "
    "listener: 100 listeners 1 router",
    "[zmq]") {
  auto transformer = [](const std::string& message) {
    return "Transformed: " + message;
  };

  using TransformFunc = std::function<std::string(const std::string&)>;
  ZMQMessageRouter<TransformFunc> router("tcp://*:5555",
                                         std::move(transformer));

  std::thread router_thread([&router]() { router.start(); });

  std::this_thread::sleep_for(std::chrono::milliseconds(20));
  static const int amount_of_threads = 100;
  std::unique_ptr<std::thread> threads[amount_of_threads];
  // make an atomic int
  std::atomic<int> counter = 0;
  bool results[amount_of_threads];
  for (int i = 0; i < amount_of_threads; i++) {
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
        results[j] = false;
      }
      //std::cout << "DEALER thread received reply" << reply << std::endl;
      results[j] = reply == "Transformed: " + to_send;
    });
  }

  for (int i = 0; i < amount_of_threads; i++) {
    // join only if it is possible to join
    if (threads[i] && threads[i]->joinable()) threads[i]->join();
  }

  INFO("Shutting down...");

  router.stop();
  router_thread.join();
  for (int i = 0; i < amount_of_threads; i++) {
    REQUIRE(results[i]);
  }
}

TEST_CASE(
    "A broadcast message is received exactly as it was sent. 100 "
    "receivers",
    "[zmq]") {
  std::string sent_message = "Hello World 1";
  std::vector<std::string> received_messages;

  ZMQMessageBroadcaster message_sender("tcp://*:5555");

  static const int amount_of_threads = 100;
  bool results[amount_of_threads];
  std::unique_ptr<std::thread> threads[amount_of_threads];
  std::atomic<int> counter = 0;

  for (int i = 0; i < amount_of_threads; i++) {
    //std::cout << "i = " << i << std::endl;
    threads[i] = std::make_unique<std::thread>([&]() {
      int j = counter.fetch_add(1);
      ZMQMessageSubscriber receiver("tcp://localhost:5555");
      std::string reply;
      try {
        reply = receiver.receive();
      } catch (std::runtime_error err) {
        results[j] = false;
      }
      results[j] = reply == sent_message;
    });
  }
  // Rerun the publisher_thread
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  message_sender.broadcast(sent_message);

  for (int i = 0; i < amount_of_threads; i++) {
    // join only if it is possible to join
    if (threads[i] && threads[i]->joinable()) threads[i]->join();
  }

  INFO("Shutting down...");

  for (int i = 0; i < amount_of_threads; i++) {
    REQUIRE(results[i]);
  }
}

TEST_CASE("A sent message is received exactly as it was sent, 100 senders",
          "[zmq]") {
  static const int amount_of_threads = 100;
  std::string sent_message = "Hello World";
  std::vector<std::string> received_messages;
  std::vector<std::string> sent_messages;
  ZMQMessageReceiver receiver("tcp://*:5555");

  std::thread receiver_thread([&]() {
    // Receive all messages from 5555
    for (int i = 0; i < amount_of_threads; i++) {
      received_messages.push_back(receiver.receive());
    }
  });

  bool results[amount_of_threads];
  std::unique_ptr<std::thread> threads[amount_of_threads];
  std::atomic<int> counter = 0;

  std::this_thread::sleep_for(std::chrono::milliseconds(20));
  for (int i = 0; i < amount_of_threads; i++) {
    threads[i] = std::make_unique<std::thread>([&]() {
      int j = counter.fetch_add(1);
      ZMQMessageSender message_sender("tcp://localhost:5555");
      std::string message_to_send = sent_message + std::to_string(j);
      message_sender.send(message_to_send);
    });
  }

  receiver_thread.join();
  for (int i = 0; i < amount_of_threads; i++) {
    // join only if it is possible to join
    if (threads[i] && threads[i]->joinable()) threads[i]->join();
    std::string message_to_send = sent_message + std::to_string(i);
    sent_messages.push_back(message_to_send);
  }

  REQUIRE_THAT(sent_messages,
               Catch::Matchers::UnorderedEquals(received_messages));
}

}  // namespace CerealThirdPartyTesting
