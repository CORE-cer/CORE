#include <string>

namespace InternalCORE {
template <typename StructName>
class Serializer {
 public:
  static std::string serialize(const StructName);
  StructName deserialize(const std::string& message);
};
}  // namespace InternalCORE
