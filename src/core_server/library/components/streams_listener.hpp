// #pragma once
//
// #include <thread>
// #include <vector>
//
// #include "shared/datatypes/aliases/port_number.hpp"
// #include "shared/networking/message_receiver/zmq_message_receiver.hpp"
// #include "shared/serializer/cereal_serializer.hpp"
//
// namespace CORE::Internal {
//
// class Mediator;
//
// class StreamsListener {
//  private:
//   Mediator* mediator;
//   Types::PortNumber receiver_port;
//   ZMQMessageReceiver receiver;
//   std::thread worker_thread;
//   bool stop_condition;
//
//  public:
//   StreamsListener(Mediator* mediator, Types::PortNumber port_number)
//       : mediator(mediator),
//         receiver_port(port_number),
//         receiver("tcp://*:" + std::to_string(port_number)) {}
//
//   void start();
//
//   void stop();
// };
//
// }  // namespace CORE::Internal
