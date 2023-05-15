#include <memory>
#include <vector>

#include "shared/serializer/cereal_serializer.hpp"

namespace CerealThirdPartyTesting {
struct SimpleStruct {
  int x;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(x);
  }
};

struct VectorStruct {
  std::vector<int> values;

  VectorStruct() {}

  VectorStruct(std::initializer_list<int> vals) : values(vals) {}

  template <class Archive>
  void serialize(Archive& archive) {
    archive(values);
  }
};

struct AbstractClass {
  virtual ~AbstractClass() = default;

  template <class Archive>
  void serialize(Archive& archive) {}
};

struct ConcreteClass1 : AbstractClass {
  std::vector<int> values;

  ConcreteClass1() {}

  ConcreteClass1(std::initializer_list<int> vals) : values(vals) {}

  template <class Archive>
  void serialize(Archive& archive) {
    archive(values);
  }
};

struct ConcreteClass2 : AbstractClass {
  int value;

  ConcreteClass2() {}

  ConcreteClass2(int val) : value(val) {}

  template <class Archive>
  void serialize(Archive& archive) {
    archive(value);
  }
};

struct ClassWithAbstractClassSharedPtr {
  std::vector<std::shared_ptr<AbstractClass>> objects;

  ClassWithAbstractClassSharedPtr() {}

  ClassWithAbstractClassSharedPtr(
    std::initializer_list<std::shared_ptr<AbstractClass>> vals)
      : objects(vals) {}

  template <class Archive>
  void serialize(Archive& archive) {
    archive(objects);
  }
};
}  // namespace CerealThirdPartyTesting

// Need to add this in the header file of the CEREAL serializer
// for cereal to be able to serialize polimorphic types correctly

// This will be in the CerealSerializer class hpp:
CEREAL_REGISTER_TYPE(CerealThirdPartyTesting::ConcreteClass1);
CEREAL_REGISTER_TYPE(CerealThirdPartyTesting::ConcreteClass2);
CEREAL_REGISTER_POLYMORPHIC_RELATION(
  CerealThirdPartyTesting::AbstractClass,
  CerealThirdPartyTesting::ConcreteClass1);
CEREAL_REGISTER_POLYMORPHIC_RELATION(
  CerealThirdPartyTesting::AbstractClass,
  CerealThirdPartyTesting::ConcreteClass2);
