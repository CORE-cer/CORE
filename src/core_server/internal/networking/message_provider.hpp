#include <string>

namespace InternalCORE {
class MessagingProvider {
 public:
  /**
   * Note that this is an interface intended to specify what
   * MessagingProviders should have, these are used in meta-templating
   * because if not, there would be an overhead when dereferencing. This
   * overhead would occur in two places: dereferencing of virtual methods,
   * and dereferencing of the pointer that has a MessagingProvider if
   * using a Strategy design pattern.
   */
};
}  // namespace InternalCORE
