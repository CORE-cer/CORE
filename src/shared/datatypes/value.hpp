#pragma once

#include <cstdint>
#include <cstdio>
#include <ctime>
#include <limits>
#include <memory>
#include <string>
#include <string_view>

namespace CORE::Types {
struct Value {
  virtual ~Value() = default;
  virtual std::unique_ptr<Value> clone() const = 0;
  virtual std::string to_string() const = 0;
  virtual std::string to_json() const = 0;
  virtual std::string get_type() const = 0;

  virtual bool operator==(const Value& other) const = 0;
};

struct StringValue final : public Value {
  static std::size_t extra_space(const std::string_view s) noexcept {
    std::size_t result = 0;

    for (const auto& c : s) {
      switch (c) {
        case '"':
        case '\\':
        case '\b':
        case '\f':
        case '\n':
        case '\r':
        case '\t': {
          // from c (1 byte) to \x (2 bytes)
          result += 1;
          break;
        }

        default: {
          if (c >= 0x00 and c <= 0x1f) {
            // from c (1 byte) to \uxxxx (6 bytes)
            result += 5;
          }
          break;
        }
      }
    }

    return result;
  }

  static std::string escape_string(const std::string_view s) noexcept {
    const auto space = extra_space(s);
    if (space == 0) {
      return std::string(s);
    }

    // create a result string of necessary size
    std::string result(s.size() + space, '\\');
    std::size_t pos = 0;

    for (const auto& c : s) {
      switch (c) {
        // quotation mark (0x22)
        case '"': {
          result[pos + 1] = '"';
          pos += 2;
          break;
        }

        // reverse solidus (0x5c)
        case '\\': {
          // nothing to change
          pos += 2;
          break;
        }

        // backspace (0x08)
        case '\b': {
          result[pos + 1] = 'b';
          pos += 2;
          break;
        }

        // formfeed (0x0c)
        case '\f': {
          result[pos + 1] = 'f';
          pos += 2;
          break;
        }

        // newline (0x0a)
        case '\n': {
          result[pos + 1] = 'n';
          pos += 2;
          break;
        }

        // carriage return (0x0d)
        case '\r': {
          result[pos + 1] = 'r';
          pos += 2;
          break;
        }

        // horizontal tab (0x09)
        case '\t': {
          result[pos + 1] = 't';
          pos += 2;
          break;
        }

        default: {
          if (c >= 0x00 and c <= 0x1f) {
            // print character c as \uxxxx
            sprintf(&result[pos + 1], "u%04x", int(c));
            pos += 6;
            // overwrite trailing null character
            result[pos] = '\\';
          } else {
            // all other characters are added as-is
            result[pos++] = c;
          }
          break;
        }
      }
    }

    return result;
  }

  std::string val{""};

  StringValue() {}

  StringValue(std::string val) : val(val) {}

  ~StringValue() override = default;

  std::unique_ptr<Value> clone() const override {
    return std::make_unique<StringValue>(this->val);
  }

  std::string to_string() const override { return val; }

  std::string to_json() const override {
    return "\"" + escape_string(to_string()) + "\"";
  }

  std::string get_type() const override { return "StringValue"; }

  bool operator==(const Value& other) const override {
    const auto* other_ptr = dynamic_cast<const StringValue*>(&other);
    return other_ptr != nullptr && this->val == other_ptr->val;
  }

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

  bool operator==(const Value& other) const override {
    const auto* other_ptr = dynamic_cast<const IntValue*>(&other);
    return other_ptr != nullptr && this->val == other_ptr->val;
  }

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

  bool operator==(const Value& other) const override {
    const auto* other_ptr = dynamic_cast<const DoubleValue*>(&other);
    return other_ptr != nullptr && this->val == other_ptr->val;
  }

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

  bool operator==(const Value& other) const override {
    const auto* other_ptr = dynamic_cast<const BoolValue*>(&other);
    return other_ptr != nullptr && this->val == other_ptr->val;
  }

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

  bool operator==(const Value& other) const override {
    const auto* other_ptr = dynamic_cast<const DateValue*>(&other);
    return other_ptr != nullptr && this->val == other_ptr->val;
  }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(val);
  }
};
}  // namespace CORE::Types
