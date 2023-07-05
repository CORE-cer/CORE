#include <string>

namespace CORE::Internal {
class MessageDealer {
  /**
   * An interface for requesting a reply messages. Note that this
   * interface does not use virtual methods because it is intented to be
   * used in metetemplating to avoid dereferenciation as it will be used
   * repeatedly in the program.
   */

  /// requests something using message.
  std::string send_and_receive(const std::string& message);
};
}  // namespace CORE::Internal
