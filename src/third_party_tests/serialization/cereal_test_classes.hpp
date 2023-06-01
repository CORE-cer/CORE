#include <cereal/archives/json.hpp>
#include <memory>
#include <vector>

#include "shared/serializer/cereal_serializer.hpp"

namespace CerealThirdPartyTesting {
struct SimpleStruct {
  int x;

  template <class Archive>
  void serialize(Archive& archive) {
    serialize(archive, std::is_same<Archive, cereal::JSONOutputArchive>{});
  }

 private:
  template <class Archive>
  void serialize(Archive& archive, std::false_type) {
    archive(x);
  }

  template <class Archive>
  void serialize(Archive& archive, std::true_type) {
    archive(cereal::make_nvp("SimpleStruct", *this));
  }
};

struct VectorStruct {
  std::vector<int> values;

  VectorStruct() {}

  VectorStruct(std::initializer_list<int> vals) : values(vals) {}

  template <class Archive>
  void serialize(Archive& archive) {
    serialize(archive, std::is_same<Archive, cereal::JSONOutputArchive>{});
  }

 private:
  template <class Archive>
  void serialize(Archive& archive, std::false_type) {
    archive(values);
  }

  template <class Archive>
  void serialize(Archive& archive, std::true_type) {
    archive(cereal::make_nvp("values", values));
  }
};

struct AbstractClass {
  virtual ~AbstractClass() = default;

  template <class Archive>
  void serialize(Archive& archive) {
    serialize(archive, std::is_same<Archive, cereal::JSONOutputArchive>{});
  }

 private:
  template <class Archive>
  void serialize(Archive& archive, std::false_type) {}

  template <class Archive>
  void serialize(Archive& archive, std::true_type) {}
};

struct ConcreteClass1 : AbstractClass {
  std::vector<int> values;

  ConcreteClass1() {}

  ConcreteClass1(std::initializer_list<int> vals) : values(vals) {}

  template <class Archive>
  void serialize(Archive& archive) {
    serialize(archive, std::is_same<Archive, cereal::JSONOutputArchive>{});
  }

 private:
  template <class Archive>
  void serialize(Archive& archive, std::false_type) {
    archive(values);
  }

  template <class Archive>
  void serialize(Archive& archive, std::true_type) {
    archive(cereal::make_nvp("values", values));
  }
};

struct ConcreteClass2 : AbstractClass {
  int value;

  ConcreteClass2() {}

  ConcreteClass2(int val) : value(val) {}

  template <class Archive>
  void serialize(Archive& archive) {
    serialize(archive, std::is_same<Archive, cereal::JSONOutputArchive>{});
  }

 private:
  template <class Archive>
  void serialize(Archive& archive, std::false_type) {
    archive(value);
  }

  template <class Archive>
  void serialize(Archive& archive, std::true_type) {
    archive(cereal::make_nvp("value", value));
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
    serialize(archive, std::is_same<Archive, cereal::JSONOutputArchive>{});
  }

 private:
  template <class Archive>
  void serialize(Archive& archive, std::false_type) {
    archive(objects);
  }

  template <class Archive>
  void serialize(Archive& archive, std::true_type) {
    archive(cereal::make_nvp("objects", objects));
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
