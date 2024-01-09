#include <thread>

#include "core_client/client.hpp"
#include "core_server/library/server.hpp"
#include "core_streamer/streamer.hpp"
#include "polkura_data.hpp"

using namespace CORE;

void do_declarations(Client& client) {
  std::vector<Types::EventTypeId> event_types{};
  for (std::string name : {"WindSpeed",
                           "WindDirection",
                           "GustSpeed",
                           "SoilTemperatureAt30CmDepth",
                           "ApparentDielectricPermittivityAt30CmDepth",
                           "AirTemperature",
                           "RelativeAirHumidity",
                           "BatteryVoltage",
                           "LatinosDiagnosticCode",
                           "ElectricConductivityOfWater",
                           "WaterTemperature",
                           "WaterLevel",
                           "MicroflowSignalStability",
                           "FlowSpeed",
                           "Discharge",
                           "MicroflowSignalStrength",
                           "Rainfall",
                           "Distance",
                           "DistanceNc",
                           "SnowdepthNc",
                           "SnowDepth",
                           "WindTemperature",
                           "SoilTemperatureAt60CmDepth",
                           "ApparentDielectricPermittivityAt60CmDepth",
                           "SoilTemperatureAt90CmDepth",
                           "ApparentDielectricPermittivityAt90CmDepth",
                           "AtmosphericPressure"}) {
    event_types.push_back(client.declare_event_type(
      name,
      {Types::AttributeInfo("id", Types::ValueTypes::INT64),
       Types::AttributeInfo("sitio", Types::ValueTypes::STRING_VIEW),
       Types::AttributeInfo("value", Types::ValueTypes::DOUBLE)}));
  }
  client.declare_stream_type("S", std::move(event_types));
}

Types::PortNumber create_queries(Client& client) {
  std::vector<std::string> queries;
  // clang-format off
  queries.push_back(
    "SELECT ApparentDielectricPermittivityAt30CmDepth\n"
    "FROM S\n"
    "WHERE ( BatteryVoltage ; ApparentDielectricPermittivityAt30CmDepth AS a2 )\n"
    "FILTER ( (BatteryVoltage[value > 5.5 or value < 4.5] OR a2[value > 4]))\n");
  // clang-format on

  Types::PortNumber final_port_number = 5002;
  for (auto& query : queries) {
    auto port_number = client.add_query(query);
    assert(port_number == final_port_number);
    final_port_number++;
  }

  std::cout << "Created queries" << std::endl;
  return final_port_number;
}

void subscribe_to_queries(Client& client,
                          Types::PortNumber initial_port,
                          Types::PortNumber final_port) {
  std::vector<std::unique_ptr<Printer>> handlers;
  for (size_t port = initial_port; port < final_port; port++) {
    std::cout << "Subscribing to port: " << port << std::endl;
    handlers.emplace_back(
      std::make_unique<Printer>());  // Store one enumerator.
    client.subscribe_to_complex_event<Printer>(handlers.back().get(), port);
  }
  std::cout << "Created handlers" << std::endl;
}

void send_a_stream(PolkuraData::Data data) {
  Streamer streamer("tcp://localhost", 5001);
  // clang-format off
  Types::Event event_to_send{
    data.event_type,
    {std::make_shared<Types::IntValue>(data.id),
     std::make_shared<Types::StringValue>(data.sitio),
     std::make_shared<Types::DoubleValue>(data.value)}
  };
  // clang-format on
  streamer.send_stream(0, event_to_send);
}

int main(int argc, char** argv) {
  try {
    Types::PortNumber starting_port{5000};
    Library::OnlineServer server{starting_port};
    Client client{"tcp://localhost", 5000};

    do_declarations(client);
    Types::PortNumber initial_port_number = 5002;
    Types::PortNumber final_port_number = create_queries(client);
    subscribe_to_queries(client, initial_port_number, final_port_number);

    for (int i = 0; i < PolkuraData::stream.size(); i++) {
      send_a_stream(PolkuraData::stream[i]);
    }

    client.stop_all_subscriptions();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::cout << "Joining threads" << std::endl;

    client.join_all_threads();

    return 0;
  } catch (std::exception& e) {
    std::cout << "Exception: " << e.what() << std::endl;
    return 1;
  }
}
