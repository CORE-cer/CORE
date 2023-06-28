#include <string>

namespace InternalCORE {
class MessageSender {
  /**
   * An interface for sending messages to an address. Note that this
   * interface does not use virtual methods because it is intented to be
   * used in metetemplating to avoid dereferenciation as it will be used
   * repeatedly in the program.
   */
  void send(const std::string& message);
};
}  // namespace InternalCORE
