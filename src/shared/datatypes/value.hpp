#pragma once

#include <cstdint>
#include <ctime>
#include <limits>
#include <memory>
#include <string>

namespace CORE::Types {
struct Value {
  virtual ~Value() = default;
  virtual std::unique_ptr<Value> clone() const = 0;
  virtual std::string to_string() const = 0;
  virtual std::string to_json() const = 0;
  virtual std::string get_type() const = 0;
};

struct StringValue final : public Value {
  std::string val{""};

  StringValue() {}

  StringValue(std::string val) : val(val) {}

  ~StringValue() override = default;

  std::unique_ptr<Value> clone() const override {
    return std::make_unique<StringValue>(this->val);
  }

  std::string to_string() const override { return val; }

  std::string to_json() const override { return "\"" + to_string() + "\""; }

  std::string get_type() const override { return "StringValue"; }

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

  static IntValue create_lower_bound() {
    return IntValue(std::numeric_limits<int64_t>::min());
  }

  std::unique_ptr<Value> clone() const override {
    return std::make_unique<IntValue>(this->val);
  }

  std::string to_string() const override { return std::to_string(val); }

  std::string to_json() const override { return std::to_string(val); }

  std::string get_type() const override { return "IntValue"; }

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

  std::unique_ptr<Value> clone() const override {
    return std::make_unique<DoubleValue>(this->val);
  }

  std::string to_string() const override { return std::to_string(val); }

  std::string to_json() const override { return std::to_string(val); }

  std::string get_type() const override { return "DoubleValue"; }

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

  std::unique_ptr<Value> clone() const override {
    return std::make_unique<BoolValue>(this->val);
  }

  std::string to_string() const override { return std::to_string(val); }

  std::string to_json() const override { return std::to_string(val); }

  std::string get_type() const override { return "BoolValue"; }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(val);
  }
};

struct DateValue final : public Value {
  time_t val{0};

  DateValue() {}

  DateValue(std::time_t val) : val(val) {}

  ~DateValue() override = default;

  std::unique_ptr<Value> clone() const override {
    return std::make_unique<DateValue>(this->val);
  }

  std::string to_string() const override { return std::to_string(val); }

  std::string to_json() const override { return std::to_string(val); }

  std::string get_type() const override { return "DateValue"; }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(val);
  }
};
}  // namespace CORE::Types
