#pragma once
#include "core_server/library/components/result_handler/result_handler.hpp"

namespace CORE::Library::Components {

template <typename Derived, typename HandlerType>
class ResultHandlerFactory {
 public:
   ResultHandler<HandlerType> create_handler() {
     return static_cast<Derived*>(this)->create_handler_impl();
   }
};

class OfflineResultHandlerFactory : public ResultHandlerFactory<OfflineResultHandlerFactory, OfflineResultHandler> {
  public:
    OfflineResultHandler create_handler_impl(){
      return {};
    }
};

}  // namespace CORE::Library::Components
