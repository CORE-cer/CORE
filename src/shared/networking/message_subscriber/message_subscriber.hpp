#include <string>

namespace CORE::Internal {
class MessageSubscriber {
  /**
   * An interface for receiving all messaged broadcasted at the address provided.
   * interface does not use virtual methods because it is intented to be
   * used in metetemplating to avoid dereferenciation as it will be used
   * repeatedly in the program.
   * IMPORTANT: No warning will be given if it is not correctly implemented,
   * this file is only for documentation purposes so that you implement
   * the receive argument
   */
  std::string receive();
};
}  // namespace CORE::Internal
