#pragma once

#include <atomic>
#include <exception>
#include <iostream>
#include <ostream>
#include <string>
#include <thread>

#include "core_server/internal/interface/backend.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "core_server/library/components/event_handler.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/stream.hpp"
#include "shared/networking/message_receiver/zmq_message_receiver.hpp"
#include "shared/networking/message_sender/zmq_message_sender.hpp"
#include "shared/serializer/cereal_serializer.hpp"

namespace CORE::Library::Components {

template <typename ResultHandlerFactoryT>
class OnlineStreamsListener {
  using Backend = CORE::Internal::Interface::Backend<ResultHandlerFactoryT>;

 private:
  EventHandler& event_handler;
  Backend& backend;
  Types::PortNumber receiver_port;
  Internal::ZMQMessageReceiver receiver;
  std::thread worker_thread;
  std::atomic<bool> stop_condition;

 public:
  OnlineStreamsListener(EventHandler& event_handler,
                        Backend& backend,
                        Types::PortNumber port_number)
      : event_handler(event_handler),
        backend(backend),
        receiver_port(port_number),
        receiver("tcp://*:" + std::to_string(port_number)) {
    start();
  }

  ~OnlineStreamsListener() { stop(); }

  // Delete Copy constructor and assigment as that should not be done with the stream listener
  OnlineStreamsListener(const OnlineStreamsListener&) = delete;
  OnlineStreamsListener& operator=(const OnlineStreamsListener&) = delete;

 private:
  void start() {
    stop_condition = false;
    worker_thread = std::thread([&]() {
      while (!stop_condition) {
        std::string s_message = receiver.receive();
        Types::Stream stream = Internal::CerealSerializer<Types::Stream>::deserialize(
          s_message);
        for (auto& event : stream.events) {
          RingTupleQueue::Tuple tuple = event_handler.event_to_tuple(event);
          backend.send_event_to_queries(tuple);
        }
      }
    });
  }

  void stop() {
    try {
      Internal::ZMQMessageSender sender("tcp://localhost:"
                                        + std::to_string(receiver_port));
      stop_condition = true;
      sender.send(
        Internal::CerealSerializer<Types::Stream>::serialize(Types::Stream(0, {})));
      worker_thread.join();
    } catch (std::exception& e) {
      std::cout << "Exception: " << e.what() << std::endl;
    }
  }
};

}  // namespace CORE::Library::Components
