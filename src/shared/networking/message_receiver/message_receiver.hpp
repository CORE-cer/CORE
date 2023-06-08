#include <string>

namespace InternalCORE {
class MessageReceiver {
  /**
   * An interface for receiving messages from an address. Note that this
   * interface does not use virtual methods because it is intented to be
   * used in metetemplating to avoid dereferenciation as it will be used
   * repeatedly in the program.
   */
  std::string receive();
};
}  // namespace InternalCORE
