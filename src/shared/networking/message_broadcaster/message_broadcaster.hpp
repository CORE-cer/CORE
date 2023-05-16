#include <string>

namespace InternalCORE {
class MessageBroadcaster {
  /**
   * An interface for sending messages to all connected subscribers.
   * interface does not use virtual methods because it is intented to be
   * used in metetemplating to avoid dereferenciation as it will be used
   * repeatedly in the program.
   */
  std::string broadcast();
};
}  // namespace InternalCORE
