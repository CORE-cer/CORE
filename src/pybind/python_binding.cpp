#include <pybind11/attr.h>
#include <pybind11/cast.h>
#include <pybind11/detail/common.h>
#include <pybind11/functional.h>  // NOLINT
#include <pybind11/pybind11.h>    // NOLINT
#include <pybind11/stl.h>         // NOLINT

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
#include "core_streamer/streamer.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/aliases/stream_type_id.hpp"
#include "shared/datatypes/catalog/attribute_info.hpp"
#include "shared/datatypes/catalog/datatypes.hpp"
#include "shared/datatypes/catalog/event_info.hpp"
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

namespace py = pybind11;

namespace CORE {
using namespace pybind11::literals;

void hello_world() { std::cout << "Hello World" << std::endl; }

std::vector<std::unique_ptr<CallbackHandler>>
subscribe_to_queries(Client& client,
                     Types::PortNumber initial_port,
                     Types::PortNumber final_port) {
  std::vector<std::unique_ptr<CallbackHandler>> handlers;
  for (size_t port = initial_port; port < final_port; port++) {
    std::cout << "Subscribing to port: " << port << std::endl;
    handlers.emplace_back(std::make_unique<CallbackHandler>());  // Store one enumerator.
    client.subscribe_to_complex_event<CallbackHandler>(handlers.back().get(), port);
  }
  std::cout << "Created handlers" << std::endl;
  return handlers;
}

PYBIND11_MODULE(_pycore, m) {
  // clang-format off
        m.doc() = "CORE";

        m.def("hello", &hello_world);

        m.def("subscribe_to_queries", &subscribe_to_queries);

        py::enum_<Types::ValueTypes>(m, "PyValueTypes")
            .value("INT64", Types::ValueTypes::INT64)
            .value("DOUBLE", Types::ValueTypes::DOUBLE)
            .value("BOOL", Types::ValueTypes::BOOL)
            .value("STRING_VIEW", Types::ValueTypes::STRING_VIEW)
            .value("DATE", Types::ValueTypes::DATE)
            .value("PRIMARY_TIME", Types::ValueTypes::PRIMARY_TIME)
            .export_values();

        py::class_<Types::Value, std::shared_ptr<Types::Value>>(m, "PyValue"); // NOLINT

        py::class_<Types::IntValue, Types::Value, std::shared_ptr<Types::IntValue>>(m, "PyIntValue")
            .def(py::init<int64_t>());

        py::class_<Types::StringValue, Types::Value, std::shared_ptr<Types::StringValue>>(m, "PyStringValue")
            .def(py::init<std::string>());

        py::class_<Types::DoubleValue, Types::Value, std::shared_ptr<Types::DoubleValue>>(m, "PyDoubleValue")
            .def(py::init<double>());

        py::class_<Types::BoolValue, Types::Value, std::shared_ptr<Types::BoolValue>>(m, "PyBoolValue")
            .def(py::init<bool>());

        py::class_<Types::DateValue, Types::Value, std::shared_ptr<Types::DateValue>>(m, "PyDateValue")
            .def(py::init<std::time_t>());

        py::class_<Types::AttributeInfo>(m, "PyAttributeInfo")
            .def(py::init<std::string, Types::ValueTypes>())
            .def_readonly("name", &Types::AttributeInfo::name)
            .def_readonly("value_type", &Types::AttributeInfo::value_type);

        py::class_<Types::Event, std::shared_ptr<Types::Event>>(m, "PyEvent")
            .def(py::init<uint64_t, std::vector<std::shared_ptr<Types::Value>>>())
            .def(py::init<uint64_t, std::vector<std::shared_ptr<Types::Value>>, Types::IntValue>());
 
        py::class_<Types::EventInfoParsed>(m, "PyEventInfoParsed")
            .def(py::init<std::string, std::vector<Types::AttributeInfo>>())
            .def_readonly("name", &Types::EventInfoParsed::name)
            .def_readonly("attributes", &Types::EventInfoParsed::attributes_info);

        py::class_<Types::EventInfo>(m, "PyEventInfo")
            .def(py::init<uint64_t, std::string, std::vector<Types::AttributeInfo>>())
            .def_readonly("id", &Types::EventInfo::id)
            .def_readonly("name", &Types::EventInfo::name);

        py::class_<Types::Stream>(m, "PyStream")
            .def(py::init<uint64_t, std::vector<std::shared_ptr<Types::Event>>&&>())
            .def_readonly("id", &Types::Stream::id)
            .def_readonly("events", &Types::Stream::events);

        py::class_<Types::StreamInfoParsed>(m, "PyStreamInfoParsed")
            .def(py::init<std::string, std::vector<Types::EventInfoParsed>>())
            .def_readonly("name", &Types::StreamInfoParsed::name)
            .def_readonly("events_info", &Types::StreamInfoParsed::events_info);

        py::class_<Types::StreamInfo>(m, "PyStreamInfo")
            .def(py::init<uint64_t, std::string, std::vector<Types::EventInfo>&&>())
            .def_readonly("events_info", &Types::StreamInfo::events_info)
            .def_readonly("name", &Types::StreamInfo::name)
            .def_readonly("id", &Types::StreamInfo::id);
            
        py::class_<Streamer>(m, "PyStreamer")
            .def(py::init<std::string, uint16_t>())
            .def("send_stream", py::overload_cast<Types::Stream>(&Streamer::send_stream))
            .def("send_stream", [](Streamer& self, Types::StreamTypeId stream_id, std::shared_ptr<Types::Event> event) {
                self.send_stream(stream_id, std::move(event));
            }, py::arg("stream_id"), py::arg("event"),
            "Envía un evento único a un stream.");

        py::class_<Client>(m, "PyClient")
            .def(py::init<std::string, uint16_t>())
            .def("declare_stream", py::overload_cast<std::string>(&Client::declare_stream))
            .def("declare_option", py::overload_cast<std::string>(&Client::declare_option))
            .def("add_query", py::overload_cast<std::string>(&Client::add_query));

        py::class_<Types::ComplexEvent>(m, "PyComplexEvent")
            .def("to_string", &Types::ComplexEvent::to_string)
            .def_readonly("start", &Types::ComplexEvent::start)
            .def_readonly("end", &Types::ComplexEvent::end)
            .def_readonly("start", &Types::ComplexEvent::start)
            .def_readonly("events", &Types::ComplexEvent::events);

        py::class_<CallbackHandler, std::unique_ptr<CallbackHandler>>(m, "PyCallbckHandler")
            .def_static("set_event_handler", [](std::function<void(const Types::Enumerator&)> handler) {
                CallbackHandler::event_handler = handler;
            });

        py::class_<CORE::Types::Enumerator>(m, "PyEnumerator")
            .def_readonly("complex_events", &CORE::Types::Enumerator::complex_events)
            .def("__iter__", [](CORE::Types::Enumerator &self) {
                return py::make_iterator(self.complex_events.begin(), self.complex_events.end());
            }, py::keep_alive<0, 1>());

        py::register_exception<ClientException>(m, "PyClientException");
        py::register_exception<AttributeNameAlreadyDeclaredException>(m, "PyAttributeNameAlreadyDeclared");
        py::register_exception<AttributeNotDefinedException>(m, "PyAttributeNotDefinedException");
        py::register_exception<EventNameAlreadyDeclaredException>(m, "PyEventNameAlreadyDeclared");
        py::register_exception<EventNotDefinedException>(m, "PyEventNotDefinedException");
        py::register_exception<EventNotInStreamException>(m, "EventNotInStreamException");
        py::register_exception<ParsingSyntaxException>(m, "PyParsingSyntaxException");
        py::register_exception<StreamNameAlreadyDeclaredException>(m, "PyStreamNameAlreadyDeclaredException");
        py::register_exception<StreamNotFoundException>(m, "PyStreamNotFoundException");
        py::register_exception<WarningException>(m, "PyWarningException");
  // clang-format on
}
}  // namespace CORE
