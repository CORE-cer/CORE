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
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/enumerator.hpp"
#include "shared/datatypes/event.hpp"

namespace CORE::Internal::Evaluation::UnitTests {
class DirectOutputTestResultHandler
    : public Library::Components::ResultHandler<DirectOutputTestResultHandler> {
  bool ready = false;

  std::condition_variable cv;
  std::mutex output_mutex;
  Types::Enumerator output;

 public:
  DirectOutputTestResultHandler(const QueryCatalog& query_catalog)
      : CORE::Library::Components::ResultHandler<DirectOutputTestResultHandler>(
          query_catalog) {}

  void
  handle_complex_event(std::optional<Internal::tECS::Enumerator>&& internal_enumerator) {
    Types::Enumerator enumerator;
    if (internal_enumerator.has_value()) {
      enumerator = query_catalog.convert_enumerator(
        std::move(internal_enumerator.value()));
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

  void start_impl() {}
};

class IndirectOutputTestResultHandler
    : public Library::Components::ResultHandler<DirectOutputTestResultHandler> {
  bool ready = false;

  std::condition_variable cv;
  std::mutex output_mutex;
  Types::Enumerator output;

 public:
  IndirectOutputTestResultHandler(const QueryCatalog& query_catalog)
      : CORE::Library::Components::ResultHandler<DirectOutputTestResultHandler>(
          query_catalog) {}

  void
  handle_complex_event(std::optional<Internal::tECS::Enumerator>&& internal_enumerator) {
    Types::Enumerator enumerator;
    if (internal_enumerator.has_value()) {
      enumerator = query_catalog.convert_enumerator(
        std::move(internal_enumerator.value()));
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

  void start_impl() {}
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

Types::StreamInfo
basic_stock_declaration(Interface::Backend<DirectOutputTestResultHandler>& backend);
}  // namespace CORE::Internal::Evaluation::UnitTests
