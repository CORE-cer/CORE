#pragma once

#include <atomic>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_set>
#include <utility>
#include <vector>

#include "message_handler.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/aliases/stream_type_id.hpp"
#include "shared/datatypes/catalog/event_info.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/client_request.hpp"
#include "shared/datatypes/client_request_type.hpp"
#include "shared/datatypes/complex_event.hpp"
#include "shared/datatypes/enumerator.hpp"
#include "shared/datatypes/parsing/event_info_parsed.hpp"
#include "shared/datatypes/parsing/stream_info_parsed.hpp"
#include "shared/datatypes/server_response.hpp"
#include "shared/datatypes/server_response_type.hpp"
#include "shared/exceptions/parsing/attribute_name_already_declared_exception.hpp"
#include "shared/exceptions/parsing/attribute_not_defined_exception.hpp"
#include "shared/exceptions/parsing/client_exception.hpp"
#include "shared/exceptions/parsing/event_name_already_declared_exception.hpp"
#include "shared/exceptions/parsing/event_not_defined_exception.hpp"
#include "shared/exceptions/parsing/event_not_in_stream_exception.hpp"
#include "shared/exceptions/parsing/parsing_syntax_exception.hpp"
#include "shared/exceptions/parsing/stream_name_already_declared_exception.hpp"
#include "shared/exceptions/parsing/stream_not_found_exception.hpp"
#include "shared/networking/message_dealer/zmq_message_dealer.hpp"
#include "shared/networking/message_subscriber/zmq_message_subscriber.hpp"
#include "shared/serializer/cereal_serializer.hpp"
#include "tracy/Tracy.hpp"

namespace CORE {
class Client {
  using SubscriptionId = uint64_t;
  using ClientReqSerializer = Internal::CerealSerializer<Types::ClientRequest>;
  using ServerResSerializer = Internal::CerealSerializer<Types::ServerResponse>;
  using EnumeratorSerializer = Internal::CerealSerializer<Types::Enumerator>;
  std::unordered_set<Types::PortNumber> known_query_evaluator_ports;  // TODO
  std::vector<std::thread> subscriber_threads;
  std::vector<std::unique_ptr<Internal::ZMQMessageSubscriber>> subscribers;
  std::vector<std::shared_ptr<std::atomic<bool>>> stop_conditions;

  Internal::ZMQMessageDealer dealer;

  std::string address;

 public:
  Client(std::string address, Types::PortNumber dealer_port)
      : dealer(address + ":" + std::to_string(dealer_port)), address(address) {}

  void declare_option(std::string option_declaration) {
    Types::ClientRequest option_declaration_req(
      Internal::CerealSerializer<std::string>::serialize(option_declaration),
      Types::ClientRequestType::SetOption);
    Types::ServerResponse res = send_request(option_declaration_req);
    assert(res.response_type == Types::ServerResponseType::SuccessEmpty);
  }

  Types::StreamInfo declare_stream(std::string stream_declaration) {
    Types::ClientRequest stream_declaration_req(
      Internal::CerealSerializer<std::string>::serialize(stream_declaration),
      Types::ClientRequestType::StreamDeclarationFromString);
    Types::ServerResponse res = send_request(stream_declaration_req);
    assert(res.response_type == Types::ServerResponseType::StreamInfo);
    return Internal::CerealSerializer<Types::StreamInfo>::deserialize(
      res.serialized_response_data);
  }

  Types::StreamInfo declare_stream(Types::StreamInfoParsed parsed_stream_info) {
    Types::ClientRequest stream_declaration_req(
      Internal::CerealSerializer<Types::StreamInfoParsed>::serialize(parsed_stream_info),
      Types::ClientRequestType::StreamDeclaration);
    Types::ServerResponse res = send_request(stream_declaration_req);
    assert(res.response_type == Types::ServerResponseType::StreamInfo);
    return Internal::CerealSerializer<Types::StreamInfo>::deserialize(
      res.serialized_response_data);
  }

  Types::EventInfo get_event_info(Types::UniqueEventTypeId id) {
    Types::ClientRequest
      request(Internal::CerealSerializer<Types::UniqueEventTypeId>::serialize(id),
              Types::ClientRequestType::EventInfoFromId);
    Types::ServerResponse response = send_request(request);
    assert(response.response_type == Types::ServerResponseType::EventInfo);
    auto event_info = Internal::CerealSerializer<Types::EventInfo>::deserialize(
      response.serialized_response_data);
    return event_info;
  }

  std::vector<Types::EventInfoParsed> get_all_event_types() {
    Types::ClientRequest request("", Types::ClientRequestType::EventInfoFromName);
    Types::ServerResponse response = send_request(request);
    assert(response.response_type == Types::ServerResponseType::EventInfo);
    auto events_info = Internal::CerealSerializer<
      std::vector<Types::EventInfoParsed>>::deserialize(response.serialized_response_data);
    return events_info;
  }

  Types::StreamInfo get_stream_info(Types::StreamTypeId id) {
    Types::ClientRequest
      request(Internal::CerealSerializer<Types::UniqueEventTypeId>::serialize(id),
              Types::ClientRequestType::StreamInfoFromId);
    Types::ServerResponse response = send_request(request);
    assert(response.response_type == Types::ServerResponseType::StreamInfo);
    auto stream_info = Internal::CerealSerializer<Types::StreamInfo>::deserialize(
      response.serialized_response_data);
    return stream_info;
  }

  Types::PortNumber add_query(std::string query) {
    Types::ClientRequest create_streamer{std::move(query),
                                         Types::ClientRequestType::AddQuery};
    Types::ServerResponse response = send_request(create_streamer);
    assert(response.response_type == Types::ServerResponseType::PortNumber);
    auto port_number = Internal::CerealSerializer<Types::PortNumber>::deserialize(
      response.serialized_response_data);
    return port_number;
  }

  template <class Handler>
  SubscriptionId subscribe_to_complex_event(Types::PortNumber port) {
    static_assert(std::is_base_of_v<StaticMessageHandler<Handler>, Handler>);

    auto subscription_id = create_subscribers_and_stop_conditions(port);
    subscriber_threads.emplace_back([&]() {
      while (*stop_conditions[subscription_id]) {
        std::string msg = subscribers[subscription_id]->receive();
        auto enumerator = EnumeratorSerializer::deserialize(msg);
        for (auto& complex_event : extract_complex_events(enumerator)) {
          Handler::static_eval(complex_event);
        }
      }
    });
    return subscription_id;
  }

  template <typename Handler>
  SubscriptionId subscribe_to_complex_event(Handler* handler, Types::PortNumber port) {
    static_assert(std::is_base_of_v<MessageHandler<Handler>, Handler>
                  || std::is_base_of_v<StaticMessageHandler<Handler>, Handler>);

    auto subscription_id = create_subscribers_and_stop_conditions(port);
    auto subscriber = subscribers[subscription_id].get();
    auto& stop_condition = stop_conditions[subscription_id];
    subscriber_threads.emplace_back([handler, subscriber, stop_condition]() {
      ZoneScopedN("Client::subscribe_to_complex_event::thread");  // NOLINT
      while (!*stop_condition && !handler->needs_to_stop()) {
        std::optional<std::string> message = subscriber->receive(100);
        if (message.has_value()) {
          handler->eval(EnumeratorSerializer::deserialize(message.value()));
        }
      }
    });
    return subscription_id;
  }

  void stop_subscription(SubscriptionId subscription_to_stop) {
    stop_conditions[subscription_to_stop]->store(true);
  }

  void stop_all_subscriptions() {
    for (SubscriptionId id = 0; id < stop_conditions.size(); id++) {
      stop_subscription(id);
    }
  }

  void join_all_threads() {
    for (SubscriptionId id = 0; id < stop_conditions.size(); id++) {
      subscriber_threads[id].join();
    }
  }

  std::string read_file(std::string path) {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
  }

 private:
  Types::ServerResponse send_request(Types::ClientRequest request) {
    std::string serialized_request = ClientReqSerializer::serialize(request);
    auto serialized_response = dealer.send_and_receive(serialized_request);
    auto res = ServerResSerializer::deserialize(serialized_response);
    switch (res.response_type) {
      case Types::ServerResponseType::AttributeNameAlreadyDeclaredException: {
        auto client_exception = Internal::CerealSerializer<
          AttributeNameAlreadyDeclaredException>::deserialize(res.serialized_response_data);
        throw client_exception;
      }
      case Types::ServerResponseType::AttributeNotDefinedException: {
        auto client_exception = Internal::CerealSerializer<
          AttributeNotDefinedException>::deserialize(res.serialized_response_data);
        throw client_exception;
      }
      case Types::ServerResponseType::EventNameAlreadyDeclaredException: {
        auto client_exception = Internal::CerealSerializer<
          EventNameAlreadyDeclaredException>::deserialize(res.serialized_response_data);
        throw client_exception;
      }
      case Types::ServerResponseType::EventNotDefinedException: {
        auto client_exception = Internal::CerealSerializer<
          EventNotDefinedException>::deserialize(res.serialized_response_data);
        throw client_exception;
      }
      case Types::ServerResponseType::EventNotInStreamException: {
        auto client_exception = Internal::CerealSerializer<
          EventNotInStreamException>::deserialize(res.serialized_response_data);
        throw client_exception;
      }
      case Types::ServerResponseType::ParsingSyntaxException: {
        auto client_exception = Internal::CerealSerializer<
          ParsingSyntaxException>::deserialize(res.serialized_response_data);
        throw client_exception;
      }
      case Types::ServerResponseType::StreamNameAlreadyDeclaredException: {
        auto client_exception = Internal::CerealSerializer<
          StreamNameAlreadyDeclaredException>::deserialize(res.serialized_response_data);
        throw client_exception;
      }
      case Types::ServerResponseType::StreamNotFoundException: {
        auto client_exception = Internal::CerealSerializer<
          StreamNotFoundException>::deserialize(res.serialized_response_data);
        throw client_exception;
      }
      case Types::ServerResponseType::Warning: {
        auto response_string = Internal::CerealSerializer<std::string>::deserialize(
          res.serialized_response_data);
        std::cout << response_string << std::flush;
        break;
      }
      case Types::ServerResponseType::Error: {
        auto response_string = Internal::CerealSerializer<std::string>::deserialize(
          res.serialized_response_data);
        std::cout << response_string << std::flush;
        break;
      }
      default:
        return res;
    }
    return res;
  }

  std::vector<Types::ComplexEvent> extract_complex_events(Types::Enumerator& enumerator) {
    std::vector<Types::ComplexEvent> out;
    for (auto val : enumerator) {
      out.push_back(std::move(val));
    }
    return out;
  }

  SubscriptionId create_subscribers_and_stop_conditions(Types::PortNumber port) {
    subscribers.push_back(std::make_unique<Internal::ZMQMessageSubscriber>(
      address + ":" + std::to_string(port)));
    stop_conditions.push_back(std::make_unique<std::atomic<bool>>(false));
    SubscriptionId subscription_id = subscribers.size() - 1;
    assert(stop_conditions.size() == subscription_id + 1);
    return subscription_id;
  }
};
}  // namespace CORE
