#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <vector>

#include "shared/datatypes/catalog/attribute_info.hpp"
#include "shared/datatypes/value.hpp"

using namespace CORETypes;

namespace DatabaseManager {

class Database {
 private:
  std::unique_ptr<pqxx::connection> connection;

 public:
  Database(const std::string& db_connection_query) {
    connect_database(db_connection_query);
    create_streams_names_table(db_connection_query);
    create_event_names_table(db_connection_query);
  }

  void add_event_type(std::string&& event_name,
                      std::vector<AttributeInfo>&& event_attributes,
                      uint64_t event_type_id,
                      const std::string& db_connection_query) {
    std::cout << "Event Name: " << event_name << std::endl;
    std::cout << "Event Attributes: \n";
    for (const AttributeInfo& attribute : event_attributes) {
      std::cout << attribute.name << "\n";
    }
    std::cout << std::endl;
    std::cout << "Event Type ID: " << event_type_id << std::endl;

    std::string query = "CREATE TABLE event_" + event_name + " (";
    query += "event_uid INT PRIMARY KEY, ";
    query += "event_type_id INT, ";
    query += "stream_uid INT, ";
    query += "timestamp DATE, ";
    for (const AttributeInfo& attribute : event_attributes) {
      query += attribute.name + " VARCHAR(255), ";
    }

    query = query.substr(0, query.size() - 2);
    query += ");";

    execute_query(query, db_connection_query);
  }

  void add_event(uint64_t event_type_id,
                 uint64_t event_uid,
                 uint64_t stream_uid,
                 std::chrono::system_clock::time_point timestamp,
                 std::vector<Value> attributes) {
    std::string eventTableName = get_event_table_name(event_type_id);
    if (eventTableName.empty()) {
      std::cout << "Event table not found for event type ID: "
                << event_type_id << std::endl;
      return;
    }

    std::string
      query = "INSERT INTO " + eventTableName
              + " (event_uid, event_type_id, stream_uid, timestamp";
    std::string values = " VALUES ($1, $2, $3, $4";
  // std::vector<pqxx::parameter> preparedParams;
  // int paramIndex = 5;
  // for (const Value& attribute : attributes) {
  //   query += ", " + attribute.name;
  //   values += ", $" + std::to_string(paramIndex);
  //   preparedParams.emplace_back(attribute.value);
  //   paramIndex++;
  // }
  query += ")" + values + ");";

    try {
      pqxx::work transaction(*connection);
      //pqxx::prepare::invocation invoc = transaction.prepared(query);

      //for (const auto& param : preparedParams) {
      //invoc(param);
      //}

      //invoc.exec();
      //transaction.commit();

      //std::cout << "Event added successfully." << std::endl;
    } catch (const std::exception& e) {
      std::cout << "Failed to add event." << std::endl;
      std::cout << "Error message: " << e.what() << std::endl;
    }
  }




  void add_stream_type(std::string stream_name,
                       std::vector<uint64_t>&& stream_event_types,
                       uint64_t stream_type_id,
                       const std::string& db_connection_query,
                       const std::vector<std::string>& event_tables) {
    std::string query = "CREATE TABLE stream_" + stream_name + " (";
    query += "id SERIAL PRIMARY KEY, ";
    query += "event_uid INT, ";
    query += "event_type_id INT, ";
    query += "timestamp DATE";
    query +=
      ", FOREIGN KEY (event_type_id) REFERENCES "
      "eventnames(event_type_id)";  //FK
    query += ");";

    execute_query(query, db_connection_query);
  }

  void create_streams_table(const std::string& db_connection_query,
                            const std::vector<std::string>& event_tables) {
    std::string query =
      "CREATE TABLE streams (stream_uid INT, event_type_id INT, PRIMARY "
      "KEY (stream_uid, event_type_id)";

    query +=
      ", FOREIGN KEY (stream_uid) REFERENCES "
      "streamnames(stream_uid)";
    query += ");";

    execute_query(query, db_connection_query);
  }

  void create_event_names_table(const std::string& db_connection_query) {
    std::string query =
      "CREATE TABLE eventnames (event_type_id INT PRIMARY KEY, "
      "event_table_name VARCHAR(255));";
    execute_query(query, db_connection_query);
  }

  void create_streams_names_table(const std::string& db_connection_query) {
    std::string query =
      "CREATE TABLE streamnames (stream_uid INT PRIMARY KEY, "
      "stream_table_name VARCHAR(255));";
    execute_query(query, db_connection_query);
  }

  std::string get_event_table_name(uint64_t event_type_id) {
    std::string eventTableName;
    std::string query =
      "SELECT event_table_name FROM eventnames WHERE event_type_id = "
      + std::to_string(event_type_id) + ";";

    try {
      pqxx::work transaction(*connection);
      pqxx::result result = transaction.exec(query);
      if (!result.empty()) {
        eventTableName = result[0][0].as<std::string>();
      }
      transaction.commit();
    } catch (const std::exception& e) {
      std::cout << "Failed to get event table name." << std::endl;
      std::cout << "Error message: " << e.what() << std::endl;
    }

    return eventTableName;
  }




  void connect_database(const std::string& db_connection_query) {
    try {
      std::unique_ptr<pqxx::connection>
        conn = std::make_unique<pqxx::connection>(db_connection_query);
      if (conn->is_open()) {
        std::cout << "Connected to the database." << std::endl;
        connection = std::move(conn);
      }
    } catch (const std::exception& e) {
      std::cerr << "Error connecting to the database: " << e.what()
                << std::endl;
    }
  }

  void execute_query(const std::string& query,
                     const std::string& connection_query) {
    try {
      pqxx::connection connection(connection_query);

      pqxx::work transaction(connection);

      transaction.exec(query);

      transaction.commit();

      std::cout << "Query executed successfully." << std::endl;

    } catch (const std::exception& e) {
      std::cout << "Failed to execute the query." << std::endl;
      std::cout << "Error message: " << e.what() << std::endl;
    }
  }

 private:
  void fill_table() {}
};

}  // namespace DatabaseManager
