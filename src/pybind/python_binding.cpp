#include <nanobind/make_iterator.h>
#include <nanobind/nanobind.h>
#include <nanobind/stl/function.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/unique_ptr.h>
#include <nanobind/stl/vector.h>

#include <cstddef>
#include <cstdint>
#include <ctime>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "core_client/client.hpp"
#include "core_client/message_handler.hpp"
#include "core_server/library/components/result_handler/result_handler_types.hpp"
#include "core_streamer/streamer.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/aliases/stream_type_id.hpp"
#include "shared/datatypes/catalog/attribute_info.hpp"
#include "shared/datatypes/catalog/datatypes.hpp"
#include "shared/datatypes/catalog/event_info.hpp"
#include "shared/datatypes/catalog/query_info.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/complex_event.hpp"
#include "shared/datatypes/enumerator.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/parsing/event_info_parsed.hpp"
#include "shared/datatypes/parsing/stream_info_parsed.hpp"
#include "shared/datatypes/stream.hpp"
#include "shared/datatypes/value.hpp"
#include "shared/exceptions/parsing/attribute_name_already_declared_exception.hpp"
#include "shared/exceptions/parsing/attribute_not_defined_exception.hpp"
#include "shared/exceptions/parsing/client_exception.hpp"
#include "shared/exceptions/parsing/event_name_already_declared_exception.hpp"
#include "shared/exceptions/parsing/event_not_defined_exception.hpp"
#include "shared/exceptions/parsing/event_not_in_stream_exception.hpp"
#include "shared/exceptions/parsing/parsing_syntax_exception.hpp"
#include "shared/exceptions/parsing/stream_name_already_declared_exception.hpp"
#include "shared/exceptions/parsing/stream_not_found_exception.hpp"
#include "shared/exceptions/parsing/warning_exception.hpp"

namespace nb = nanobind;

namespace CORE {
using namespace nanobind::literals;

void hello_world() { std::cout << "Hello World" << std::endl; }

std::vector<std::unique_ptr<CallbackHandler>>
subscribe_to_queries(Client& client,
                     Types::PortNumber initial_port,
                     Types::PortNumber final_port) {
  std::vector<std::unique_ptr<CallbackHandler>> handlers;
  for (size_t port = initial_port; port < final_port; port++) {
    std::cout << "Subscribing to port: " << port << std::endl;
    handlers.emplace_back(std::make_unique<CallbackHandler>());
    client.subscribe_to_complex_event<CallbackHandler>(handlers.back().get(), port);
  }
  std::cout << "Created handlers" << std::endl;
  return handlers;
}

NB_MODULE(pycer, m) {
  // clang-format off
        m.doc() = "CORE";

        m.def("hello", &hello_world);

        m.def("subscribe_to_queries", &subscribe_to_queries);

        nb::enum_<Types::ValueTypes>(m, "PyValueTypes")
            .value("INT64", Types::ValueTypes::INT64)
            .value("DOUBLE", Types::ValueTypes::DOUBLE)
            .value("BOOL", Types::ValueTypes::BOOL)
            .value("STRING_VIEW", Types::ValueTypes::STRING_VIEW)
            .value("DATE", Types::ValueTypes::DATE)
            .value("PRIMARY_TIME", Types::ValueTypes::PRIMARY_TIME)
            .export_values();

        nb::class_<Types::Value>(m, "PyValue");

        nb::class_<Types::IntValue, Types::Value>(m, "PyIntValue")
            .def(nb::init<int64_t>());

        nb::class_<Types::StringValue, Types::Value>(m, "PyStringValue")
            .def(nb::init<std::string>());

        nb::class_<Types::DoubleValue, Types::Value>(m, "PyDoubleValue")
            .def(nb::init<double>());

        nb::class_<Types::BoolValue, Types::Value>(m, "PyBoolValue")
            .def(nb::init<bool>());

        nb::class_<Types::DateValue, Types::Value>(m, "PyDateValue")
            .def(nb::init<std::time_t>());

        nb::class_<Types::AttributeInfo>(m, "PyAttributeInfo")
            .def(nb::init<std::string, Types::ValueTypes>())
            .def_ro("name", &Types::AttributeInfo::name)
            .def_ro("value_type", &Types::AttributeInfo::value_type);

        nb::class_<Types::Event>(m, "PyEvent")
            .def(nb::init<uint64_t, std::vector<std::shared_ptr<Types::Value>>>())
            .def(nb::init<uint64_t, std::vector<std::shared_ptr<Types::Value>>, Types::IntValue>());

        nb::class_<Types::EventInfoParsed>(m, "PyEventInfoParsed")
            .def(nb::init<std::string, std::vector<Types::AttributeInfo>>())
            .def_ro("name", &Types::EventInfoParsed::name)
            .def_ro("attributes", &Types::EventInfoParsed::attributes_info);

        nb::class_<Types::EventInfo>(m, "PyEventInfo")
            .def(nb::init<uint64_t, std::string, std::vector<Types::AttributeInfo>>())
            .def_ro("id", &Types::EventInfo::id)
            .def_ro("name", &Types::EventInfo::name);

        nb::class_<Types::Stream>(m, "PyStream")
            .def(nb::init<uint64_t, std::vector<std::shared_ptr<Types::Event>>&&>())
            .def_ro("id", &Types::Stream::id)
            .def_ro("events", &Types::Stream::events);

        nb::class_<Types::StreamInfoParsed>(m, "PyStreamInfoParsed")
            .def(nb::init<std::string, std::vector<Types::EventInfoParsed>>())
            .def_ro("name", &Types::StreamInfoParsed::name)
            .def_ro("events_info", &Types::StreamInfoParsed::events_info);

        nb::class_<Types::StreamInfo>(m, "PyStreamInfo")
            .def(nb::init<uint64_t, std::string, std::vector<Types::EventInfo>&&>())
            .def_ro("events_info", &Types::StreamInfo::events_info)
            .def_ro("name", &Types::StreamInfo::name)
            .def_ro("id", &Types::StreamInfo::id);

        nb::class_<Streamer>(m, "PyStreamer")
            .def(nb::init<std::string, uint16_t>())
            .def("send_stream", nb::overload_cast<Types::Stream>(&Streamer::send_stream))
            .def("send_stream", [](Streamer& self, Types::StreamTypeId stream_id, std::shared_ptr<Types::Event> event) {
                self.send_stream(stream_id, std::move(event));
            }, nb::arg("stream_id"), nb::arg("event"),
            "Sends a single event to a stream.");

        nb::enum_<Library::Components::ResultHandlerType>(m, "PyResultHandlerType")
            .value("OFFLINE", Library::Components::ResultHandlerType::OFFLINE)
            .value("ONLINE", Library::Components::ResultHandlerType::ONLINE)
            .value("WEBSOCKET", Library::Components::ResultHandlerType::WEBSOCKET)
            .value("CUSTOM", Library::Components::ResultHandlerType::CUSTOM)
            .export_values();

        nb::class_<Types::QueryInfo>(m, "PyQueryInfo")
            .def_ro("result_handler_identifier", &Types::QueryInfo::result_handler_identifier)
            .def_ro("query_string", &Types::QueryInfo::query_string)
            .def_ro("query_name", &Types::QueryInfo::query_name)
            .def_ro("active", &Types::QueryInfo::active);

        nb::class_<Client>(m, "PyClient")
            .def(nb::init<std::string, uint16_t>())
            .def("declare_stream", nb::overload_cast<std::string>(&Client::declare_stream))
            .def("declare_option", nb::overload_cast<std::string>(&Client::declare_option))
            .def("add_query", nb::overload_cast<std::string>(&Client::add_query))
            .def("list_all_streams", &Client::list_all_streams)
            .def("list_all_queries", &Client::list_all_queries)
            .def("inactivate_query", &Client::inactivate_query);

        nb::class_<Types::ComplexEvent>(m, "PyComplexEvent")
            .def("to_string", &Types::ComplexEvent::to_string)
            .def_ro("start", &Types::ComplexEvent::start)
            .def_ro("end", &Types::ComplexEvent::end)
            .def_ro("events", &Types::ComplexEvent::events);

        nb::class_<CallbackHandler>(m, "PyCallbackHandler")
            .def_static("set_event_handler", [](std::function<void(const Types::Enumerator&)> handler) {
                CallbackHandler::event_handler = handler;
            });

        nb::class_<CORE::Types::Enumerator>(m, "PyEnumerator")
            .def_ro("complex_events", &CORE::Types::Enumerator::complex_events)
            .def("__iter__", [](CORE::Types::Enumerator &self) {
                return nb::make_iterator(nb::type<CORE::Types::Enumerator>(), "iterator",
                    self.complex_events.begin(), self.complex_events.end());
            }, nb::keep_alive<0, 1>());

        nb::exception<ClientException>(m, "PyClientException");
        nb::exception<AttributeNameAlreadyDeclaredException>(m, "PyAttributeNameAlreadyDeclared");
        nb::exception<AttributeNotDefinedException>(m, "PyAttributeNotDefinedException");
        nb::exception<EventNameAlreadyDeclaredException>(m, "PyEventNameAlreadyDeclared");
        nb::exception<EventNotDefinedException>(m, "PyEventNotDefinedException");
        nb::exception<EventNotInStreamException>(m, "EventNotInStreamException");
        nb::exception<ParsingSyntaxException>(m, "PyParsingSyntaxException");
        nb::exception<StreamNameAlreadyDeclaredException>(m, "PyStreamNameAlreadyDeclaredException");
        nb::exception<StreamNotFoundException>(m, "PyStreamNotFoundException");
        nb::exception<WarningException>(m, "PyWarningException");
  // clang-format on
}
}  // namespace CORE
