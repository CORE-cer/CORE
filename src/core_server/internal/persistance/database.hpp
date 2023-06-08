#include <iostream>  // Include the iostream header for std::cout and std::cerr
#include <pqxx/pqxx>
#include <string>
#include <vector>

#include "shared/datatypes/catalog/attribute_info.hpp"

using namespace CORETypes;

namespace DatabaseManager {
class Value;

class Database {
 private:
  std::unique_ptr<pqxx::connection> connection;

 public:
  Database() {
    // Connect to the database
    std::cout << "Probando" << std::endl;

    connect_database();
  }

  void add_event_type(std::string&& event_name,
                      std::vector<AttributeInfo>&& event_attributes,
                      uint64_t event_type_id);

  void add_stream_type(std::string stream_name,
                       std::vector<uint64_t>&& stream_event_types,
                       uint64_t stream_type_id);

  void connect_database() {
    try {
      std::unique_ptr<pqxx::connection> conn =
          std::make_unique<pqxx::connection>(
              "dbname=core user=postgres password=postgres host=localhost "
              "port=5432");
      if (conn->is_open()) {
        std::cout << "Connected to the database." << std::endl;
        connection = std::move(conn);

        // Call create_table() to create the necessary tables
        create_table();

        // Call fill_table() to insert data into tables if needed
        fill_table();
      }
    } catch (const std::exception& e) {
      std::cerr << "Error connecting to the database: " << e.what()
                << std::endl;
    }
  }

 private:
  void create_table() {
    try {
      pqxx::work txn(*connection);
      // Create the necessary SQL statements to create tables
      std::string eventTableSQL =
          "CREATE TABLE IF NOT EXISTS events("
          "event_id SERIAL PRIMARY KEY,"
          "event_name VARCHAR(255),"
          "event_attributes JSONB"
          ");";

      std::string streamTableSQL =
          "CREATE TABLE IF NOT EXISTS streams ("
          "stream_id SERIAL PRIMARY KEY,"
          "stream_name VARCHAR(255),"
          "stream_event_types JSONB"
          ");";

      // Execute the SQL statements to create tables
      txn.exec(eventTableSQL);
      txn.exec(streamTableSQL);

      txn.commit();
      std::cout << "Tables created successfully." << std::endl;
    } catch (const std::exception& e) {
      std::cerr << "Error creating tables: " << e.what() << std::endl;
    }
  }

  void fill_table() {
    // Implementation to insert data into tables if needed
  }
};

}  // namespace DatabaseManager
