#include <cassert>
#include <chrono>
#include <exception>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "core_client/client.hpp"
#include "core_server/library/server.hpp"
#include "core_streamer/streamer.hpp"
#include "polkura_data.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/catalog/attribute_info.hpp"
#include "shared/datatypes/catalog/datatypes.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/parsing/event_info_parsed.hpp"
#include "shared/datatypes/value.hpp"

using namespace CORE;

void do_declarations(Client& client) {
  std::vector<Types::EventInfoParsed> event_types{};
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
    event_types.emplace_back(
      name,
      std::vector<Types::AttributeInfo>{{"id", Types::ValueTypes::INT64},
                                        {"sitio", Types::ValueTypes::STRING_VIEW},
                                        {"value", Types::ValueTypes::DOUBLE}});
  }
  client.declare_stream({"S", std::move(event_types)});
}

void create_queries(Client& client) {
  std::vector<std::string> queries;
  // clang-format off
  queries.push_back(
    "SELECT ApparentDielectricPermittivityAt30CmDepth\n"
    "FROM S\n"
    "WHERE ( BatteryVoltage ; ApparentDielectricPermittivityAt30CmDepth AS a2 )\n"
    "FILTER ( (BatteryVoltage[value > 5.5 or value < 4.5] OR a2[value > 4]))\n");
  // clang-format on

  for (auto& query : queries) {
    auto port_number = client.add_query(query);
    assert(port_number == 0);
  }

  std::cout << "Created queries" << std::endl;
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
    Library::OfflineServer server{starting_port};
    Client client{"tcp://localhost", 5000};

    do_declarations(client);
    create_queries(client);

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
