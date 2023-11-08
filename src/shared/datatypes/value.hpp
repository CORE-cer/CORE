#pragma once

#include <chrono>
#include <string>

namespace CORE::Types {
struct Value {
  virtual ~Value() = default;
  virtual std::string to_string() const = 0;
};

struct StringValue final : public Value {
  std::string val{""};

  StringValue() {}

  StringValue(std::string val) : val(val) {}

  ~StringValue() override = default;

  std::string to_string() const override { return val; }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(val);
  }
};

struct IntValue final : public Value {
  int64_t val{0};

  IntValue() {}

  IntValue(int64_t val) : val(val) {}

  ~IntValue() override = default;

  std::string to_string() const override { return std::to_string(val); }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(val);
  }
};

struct DoubleValue final : public Value {
  double val{0.0};

  DoubleValue() {}

  DoubleValue(double val) : val(val) {}

  ~DoubleValue() override = default;

  std::string to_string() const override { return std::to_string(val); }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(val);
  }
};

struct BoolValue final : public Value {
  bool val{false};

  BoolValue() {}

  BoolValue(bool val) : val(val) {}

  ~BoolValue() override = default;

  std::string to_string() const override { return std::to_string(val); }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(val);
  }
};

struct DateValue final : public Value {
  std::time_t val{0};

  DateValue() {}

  DateValue(std::time_t val) : val(val) {}

  ~DateValue() override = default;

  std::string to_string() const override { return std::to_string(val); }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(val);
  }
};
}  // namespace CORE::Types
