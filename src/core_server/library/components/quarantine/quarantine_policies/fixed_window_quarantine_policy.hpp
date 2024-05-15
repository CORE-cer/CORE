#include <atomic>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <thread>
#include <vector>

#include "core_server/internal/interface/backend.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"

namespace CORE::Library::Components::Quarantine::QuarantinePolicy {
template <typename HandlerType>
class FixedWindowPolicy {
  using Backend = CORE::Internal::Interface::Backend<HandlerType>;

  Backend& backend;
  std::size_t applicable_query_idx;
  int64_t window_size;
  std::mutex quarantined_tuples_lock;
  std::vector<RingTupleQueue::Tuple> quarantined_tuples{};
  std::thread worker_thread;
  std::atomic<bool> stop_condition = false;

 public:
  FixedWindowPolicy(Backend& backend,
                    std::size_t applicable_query_idx,
                    int64_t window_size = 1000)
      : backend(backend),
        applicable_query_idx(applicable_query_idx),
        window_size(window_size) {}

  ~FixedWindowPolicy() { stop_condition = true; }

  // TODO: Refactor
  void receive_tuple(RingTupleQueue::Tuple tuple) {}

 private:
  void start() {
    worker_thread = std::thread([&]() {
      while (!stop_condition) {
        std::lock_guard<std::mutex> lock(quarantined_tuples_lock);
        if (!quarantined_tuples.empty()) {
          RingTupleQueue::Tuple first_tuple = quarantined_tuples.back();
          // TODO: Refactor
          // backend.send_tuple_to_query(first_tuple, applicable_query_idx);
        }
      }
    });
  }
};
}  // namespace CORE::Library::Components::Quarantine::QuarantinePolicy
