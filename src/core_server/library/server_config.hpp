#pragma once

#include <algorithm>
#include <argparse/argparse.hpp>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

#include "shared/datatypes/aliases/port_number.hpp"

namespace CORE::Library {

class ServerConfig {
 public:
  struct FixedPorts {
    Types::PortNumber webserver;
    Types::PortNumber router;
    Types::PortNumber stream_listener;

    static FixedPorts from_args(argparse::ArgumentParser& parsed_program) {
      int webserver = parsed_program.get<int>("webserver");
      int router = parsed_program.get<int>("router");
      int stream_listener = parsed_program.get<int>("stream_listener");

      return FixedPorts{static_cast<Types::PortNumber>(webserver),
                        static_cast<Types::PortNumber>(router),
                        static_cast<Types::PortNumber>(stream_listener)};
    }

    static void add_to_args(argparse::ArgumentParser& program) {
      webserver_from_args(program);
      router_from_args(program);
      stream_listener_from_args(program);
    }

   private:
    static void webserver_from_args(argparse::ArgumentParser& program) {
      program.add_argument("-w", "--webserver")
        .help("Port for the webserver")
        .default_value(443)
        .scan<'i', int>();
    }

    static void router_from_args(argparse::ArgumentParser& program) {
      program.add_argument("-r", "--router")
        .help("Port for the router")
        .default_value(5000)
        .scan<'i', int>();
    }

    static void stream_listener_from_args(argparse::ArgumentParser& program) {
      program.add_argument("-s", "--stream_listener")
        .help("Port for the stream listener")
        .default_value(5001)
        .scan<'i', int>();
    }
  };

 private:
  std::mutex server_config_mutex;
  FixedPorts fixed_ports;
  // Corresponds to the next open port available
  Types::PortNumber next_open_port;
  std::vector<Types::PortNumber> used_ports;

  std::string query_path;
  std::string declaration_path;
  std::string options_path;
  std::string csv_data_path;

 public:
  ServerConfig(FixedPorts fixed_ports,
               Types::PortNumber next_open_port,
               std::string query_path,
               std::string declaration_path,
               std::string options_path,
               std::string csv_data_path)
      : fixed_ports(fixed_ports),
        next_open_port(next_open_port),
        used_ports{
          fixed_ports.webserver,
          fixed_ports.router,
          fixed_ports.stream_listener,
        },
        query_path(query_path),
        declaration_path(declaration_path),
        options_path(options_path),
        csv_data_path(csv_data_path) {}

  ServerConfig& operator=(const ServerConfig& other) = delete;
  ServerConfig(const ServerConfig& other) = delete;
  ServerConfig& operator=(ServerConfig&& other) = delete;

  ServerConfig(ServerConfig&& other) {
    std::lock_guard<std::mutex> lock(server_config_mutex);
    std::lock_guard<std::mutex> lock_other(other.server_config_mutex);

    fixed_ports = std::move(other.fixed_ports);
    next_open_port = other.next_open_port;
    used_ports = std::move(other.used_ports);
    query_path = std::move(other.query_path);
    declaration_path = std::move(other.declaration_path);
    csv_data_path = std::move(other.csv_data_path);
  }

  static ServerConfig from_args(int argc, char** argv) {
    argparse::ArgumentParser program("core");

    FixedPorts::add_to_args(program);

    starting_port_from_args(program);
    query_from_args(program);
    declaration_from_args(program);
    options_from_args(program);
    csv_data_from_args(program);

    try {
      program.parse_args(argc, argv);
    } catch (const std::exception& err) {
      std::cerr << err.what() << std::endl;
      std::cerr << program;
      std::exit(1);
    }

    FixedPorts fixed_ports = FixedPorts::from_args(program);

    Types::PortNumber starting_port = program.get<int>("startingport");
    std::string query_path = program.get<std::string>("query");
    std::string declaration_path = program.get<std::string>("declaration");
    std::string options_path = program.get<std::string>("options");
    std::string csv_data_path = program.get<std::string>("csv");

    return ServerConfig{fixed_ports,
                        starting_port,
                        query_path,
                        declaration_path,
                        options_path,
                        csv_data_path};
  }

  [[nodiscard]] FixedPorts& get_fixed_ports() {
    std::lock_guard lock(server_config_mutex);
    return fixed_ports;
  }

  [[nodiscard]] Types::PortNumber get_next_open_port() {
    std::lock_guard lock(server_config_mutex);
    Types::PortNumber proposed_port = next_open_port++;

    while (std::find(used_ports.begin(), used_ports.end(), proposed_port)
           != used_ports.end()) {
      proposed_port = next_open_port++;
    }

    used_ports.push_back(proposed_port);

    return proposed_port;
  }

  [[nodiscard]] std::string get_query_path() {
    std::lock_guard lock(server_config_mutex);
    return query_path;
  }

  [[nodiscard]] std::string get_declaration_path() {
    std::lock_guard lock(server_config_mutex);
    return declaration_path;
  }

  [[nodiscard]] std::string get_options_path() {
    std::lock_guard lock(server_config_mutex);
    return options_path;
  }

  [[nodiscard]] std::string get_csv_data_path() {
    std::lock_guard lock(server_config_mutex);
    return csv_data_path;
  }

 private:
  static void starting_port_from_args(argparse::ArgumentParser& program) {
    program.add_argument("-sp", "--startingport")
      .help("Starting port for the server")
      .default_value(5002)
      .scan<'i', int>();
  }

  static void query_from_args(argparse::ArgumentParser& program) {
    program.add_argument("-q", "--query").help("Path to the query file").required();
  }

  static void declaration_from_args(argparse::ArgumentParser& program) {
    program.add_argument("-d", "--declaration")
      .help("Path to the declaration file")
      .required();
  }

  static void options_from_args(argparse::ArgumentParser& program) {
    program.add_argument("-o", "--options")
      .help("Path to the options file")
      .default_value("");
  }

  static void csv_data_from_args(argparse::ArgumentParser& program) {
    program.add_argument("-c", "--csv").help("Path to the csv data file").required();
  }
};

}  // namespace CORE::Library
