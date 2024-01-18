#pragma once

#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <condition_variable>

#include "core_server/library/components/result_handler/result_handler.hpp"

namespace CORE::Internal::Evaluation::UnitTests {
class TestResultHandler : public Library::Components::ResultHandler<TestResultHandler> {
  bool ready = false;

  std::condition_variable cv;
  std::mutex output_mutex;
  Types::Enumerator output;

 public:
  TestResultHandler(const Catalog& catalog)
      : CORE::Library::Components::ResultHandler<TestResultHandler>(catalog) {}

  void
  handle_complex_event(std::optional<Internal::tECS::Enumerator>&& internal_enumerator) {
    Types::Enumerator enumerator;
    if (internal_enumerator.has_value()) {
      enumerator = catalog.convert_enumerator(std::move(internal_enumerator.value()));
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

    cv.wait(lk, [this] { return ready; });

    Types::Enumerator enumerator = output;
    ready = false;

    lk.unlock();
    cv.notify_one();

    return enumerator;
  }

  void start_impl() {}
};

bool is_the_same_as(Types::Event event,
                    uint64_t event_type_id,
                    std::string name,
                    int64_t value);

bool is_the_same_as(Types::Event event,
                    uint64_t event_type_id,
                    std::string name,
                    int64_t value1,
                    int64_t value2);
}  // namespace CORE::Internal::Evaluation::UnitTests
