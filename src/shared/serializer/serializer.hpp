#include <string>
#include <string_view>

namespace CORE::Internal {
template <typename StructName>
class Serializer {
 public:
  static std::string serialize(const StructName);
  static StructName deserialize(std::string_view message);
};
}  // namespace CORE::Internal
