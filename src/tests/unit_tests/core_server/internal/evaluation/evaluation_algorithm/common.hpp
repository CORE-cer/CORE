#pragma once

#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <optional>
#include <string>
#include <utility>

#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/evaluation/enumeration/tecs/enumerator.hpp"
#include "core_server/internal/interface/backend.hpp"
#include "core_server/library/components/result_handler/result_handler.hpp"
#include "core_server/library/components/result_handler/result_handler_types.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/enumerator.hpp"
#include "shared/datatypes/event.hpp"

namespace CORE::Internal::Evaluation::UnitTests {
class DirectOutputTestResultHandler : public Library::Components::ResultHandler {
  bool ready = false;

  std::condition_variable cv;
  std::mutex output_mutex;
  Types::Enumerator output;
  bool check_result = false;  // Add bool storage
  bool is_check_mode = false;

 public:
  DirectOutputTestResultHandler(const QueryCatalog& query_catalog)
      : CORE::Library::Components::ResultHandler(
          Library::Components::ResultHandlerType::CUSTOM) {
            this->set_query_catalog(query_catalog);
          }

  void handle_check_result(bool matched) override {
    std::unique_lock lk(output_mutex);
    cv.wait(lk, [this] { return !ready; });
    check_result = matched;
    is_check_mode = true;
    ready = true;
    lk.unlock();
    cv.notify_one();
  }

  bool get_bool() {
    std::unique_lock lk(output_mutex);
    cv.wait_for(lk, std::chrono::milliseconds(500), [this] { return ready; });
    bool result = check_result;
    ready = false;
    lk.unlock();
    cv.notify_one();
    return result;
  }

  void handle_complex_event(
    Library::Components::QueryResult&& result) override {
    Types::Enumerator enumerator;
    if (result.kind == Library::Components::QueryResult::Kind::Exists) {
      // For tests, a bool exists result can be represented as an empty or non-empty enumerator
      if (result.matched) {
        enumerator = Types::Enumerator{};
      }
    } else if (result.kind == Library::Components::QueryResult::Kind::Enumerate) {
      if (result.enumerator.has_value()) {
        enumerator = get_query_catalog().convert_enumerator(
          std::move(result.enumerator.value()));
      }
    }
    std::unique_lock lk(output_mutex);

    cv.wait(lk, [this] { return !ready; });

    output = enumerator;
    ready = true;

    lk.unlock();
    cv.notify_one();
  }

  Types::Enumerator get_enumerator() {
    std::unique_lock lk(output_mutex);

    Types::Enumerator enumerator;
    cv.wait_for(lk, std::chrono::milliseconds(500), [this] { return ready; });

    if (ready) {
      enumerator = output;
    }
    ready = false;

    lk.unlock();
    cv.notify_one();

    return enumerator;
  }

  void start() override {}

  std::string get_identifier() const override { return "DirectOutputTestResultHandler"; }
};

class IndirectOutputTestResultHandler : public Library::Components::ResultHandler {
  bool ready = false;

  std::condition_variable cv;
  std::mutex output_mutex;
  Types::Enumerator output;
  bool check_result = false;  // Add bool storage
  bool is_check_mode = false;

 public:
  IndirectOutputTestResultHandler(const QueryCatalog& query_catalog)
      : CORE::Library::Components::ResultHandler(
          Library::Components::ResultHandlerType::CUSTOM) {
            this->set_query_catalog(query_catalog);
          }

  void handle_check_result(bool matched) override {
    std::unique_lock lk(output_mutex);
    cv.wait(lk, [this] { return !ready; });
    check_result = matched;
    is_check_mode = true;
    ready = true;
    lk.unlock();
    cv.notify_one();
  }

  bool get_bool() {
    std::unique_lock lk(output_mutex);
    cv.wait_for(lk, std::chrono::milliseconds(500), [this] { return ready; });
    bool result = check_result;
    ready = false;
    lk.unlock();
    cv.notify_one();
    return result;
  }
  
  void handle_complex_event(
    Library::Components::QueryResult&& result) override {
    Types::Enumerator enumerator;
    if (result.kind == Library::Components::QueryResult::Kind::Exists) {
      if (result.matched) {
        enumerator = Types::Enumerator{};
      }
    } else if (result.kind == Library::Components::QueryResult::Kind::Enumerate) {
      if (result.enumerator.has_value()) {
        enumerator = get_query_catalog().convert_enumerator(
          std::move(result.enumerator.value()));
      }
    }
    std::unique_lock lk(output_mutex);

    cv.wait(lk, [this] { return !ready; });

    output = enumerator;
    ready = true;

    lk.unlock();
    cv.notify_one();
  }

  Types::Enumerator get_enumerator() {
    std::unique_lock lk(output_mutex);

    Types::Enumerator enumerator;
    cv.wait_for(lk, std::chrono::milliseconds(500), [this] { return ready; });

    if (ready) {
      enumerator = output;
    }
    ready = false;

    lk.unlock();
    cv.notify_one();

    return enumerator;
  }

  void start() override {}

  std::string get_identifier() const override {
    return "IndirectOutputTestResultHandler";
  }
};

bool is_the_same_as(Types::Event event, uint64_t event_type_id, std::string name);

bool is_the_same_as(Types::Event event,
                    uint64_t event_type_id,
                    std::string name,
                    int64_t value);

bool is_the_same_as(Types::Event event,
                    uint64_t event_type_id,
                    std::string name,
                    int64_t value1,
                    int64_t value2);

Types::StreamInfo basic_stock_declaration(Interface::Backend<>& backend);

Types::StreamInfo primary_time_stock_declaration(Interface::Backend<>& backend);
}  // namespace CORE::Internal::Evaluation::UnitTests
