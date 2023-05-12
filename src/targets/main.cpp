#include <getopt.h>

#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <thread>

#include "antlr4-runtime.h"

// #include "engine/engine.hpp"
// #include "engine/executors/executor_manager.hpp"
// #include "parser/plan/logical_plan.hpp"
// #include "streams/readers/csv_stream_reader.hpp"
// #include "streams/stream_queue.hpp"
// #include "user/dummy_user.hpp"

int main(int argc, char** argv) {
  // para guardar el opt
  int opt;

  // si están presentes las opciones obligatorias
  bool q_found = false;
  bool s_found = false;
  bool d_found = false;

  std::map<std::string, std::string> h;
  std::string s1 = "a";
  std::string s2 = "b";
  h.insert(make_pair(s1, s2));

  // el string de queryfile, el string de streamsfile y el string de
  // streamdescription
  std::string query_file_path;
  std::string stream_file_path;
  std::string stream_description_path;
  while ((opt = getopt(argc, argv, "q:s:d:vflo")) != -1) {
    switch (opt) {
      case 'q':
        query_file_path = optarg;
        q_found = true;
        break;
      case 's':
        stream_file_path = optarg;
        s_found = true;
        break;
      case 'd':
        stream_description_path = optarg;
        d_found = true;
        break;
      case 'v':
        break;
      case 'f':
        break;
      case 'l':
        break;
      case 'o':
        break;
      default: /* '?' */
        fprintf(stderr,
                "Usage: %s -q <path/to/queryfile> -s <path/to/streamfile> "
                "[-v] [-f] [-l] [-o]\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }
  }
  if (!q_found || !s_found || !d_found) {
    fprintf(stderr,
            "Usage: %s -q path/to/queryfile -s path/to/streamfile -d "
            "path/to/streamsdescription [-v] [-f] [-l] [-o]\n",
            argv[0]);
    exit(EXIT_FAILURE);
  }

  // Create instances
  // std::shared_ptr<InputSchemaManager> schema_manager =
  // InputSchemaManager::FromCoreFile(stream_description_path);

  //// Queue + Reader
  // std::shared_ptr<StreamQueue> stream_queue =
  // std::make_shared<StreamQueue>(schema_manager);
  // std::shared_ptr<CsvStreamReader> stream_reader =
  // CsvStreamReader::FromFile(schema_manager, stream_queue,
  // stream_file_path);
  // stream_queue->AddReader(stream_reader);

  //// Engine
  // const uint kEngineTimeout = 1;
  // std::shared_ptr<ExecutorManager> executor_manager =
  // ExecutorManager::FromCoreFile(schema_manager, query_file_path);
  // std::shared_ptr<Engine> engine = std::make_shared<Engine>(
  // stream_queue, executor_manager, kEngineTimeout);

  //// User
  // const uint kUserTimeout = 1;
  // std::shared_ptr<DummyUser> user =
  // std::make_shared<DummyUser>(kUserTimeout);
  // engine->AddCallback(user->GetCallback());

  //// Plan tests
  // std::cout << "MAIN: PLAN FILTERS: "
  //<< executor_manager->plans.back().GetFilters() << std::endl;

  //// Start simulation
  // std::cout << "MAIN: START READER" << std::endl;
  // std::thread reader_thread(&CsvStreamReader::Start, stream_reader);

  // std::cout << "MAIN: START USER" << std::endl;
  // std::thread user_thread(&DummyUser::Start, user);

  // std::cout << "MAIN: START ENGINE" << std::endl;
  // std::thread engine_thread(&Engine::Start, engine);

  // std::cout << "MAIN: JOINS" << std::endl;
  // reader_thread.join();
  // engine_thread.join();
  // user_thread.join();

  // std::cout << "MAIN: GG" << std::endl;
  return 0;
}
