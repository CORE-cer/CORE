#pragma once

#include <atomic>
#include <utility>
#include <vector>

#include "base_policy.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/eventWrapper.hpp"

namespace CORE::Internal::Interface::Module::Quarantine {

template <typename ResultHandlerT>
class DirectPolicy : public BasePolicy<ResultHandlerT> {
  // TODO: Optimize
  std::vector<Types::EventWrapper> events;

 public:
  DirectPolicy(Catalog& catalog,
               std::atomic<Types::PortNumber>& next_available_inproc_port)
      : BasePolicy<ResultHandlerT>(catalog, next_available_inproc_port) {
    this->start();
  }

  ~DirectPolicy() { this->handle_destruction(); }

  void receive_event(Types::EventWrapper&& event) override {
    this->send_event_queue.enqueue(std::move(event));
  }

 protected:
  void try_add_tuples_to_send_queue() override { this->events.clear(); }

  void force_add_tuples_to_send_queue() override { try_add_tuples_to_send_queue(); }
};
}  // namespace CORE::Internal::Interface::Module::Quarantine
