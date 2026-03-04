#pragma once

#include <boost/dynamic_bitset.hpp>
#include <cstddef>
#include <string>

namespace CORE {

class Bitset {
  boost::dynamic_bitset<> bits_;

 public:
  static constexpr auto npos = boost::dynamic_bitset<>::npos;

  // Construction
  Bitset() = default;
  explicit Bitset(std::size_t num_bits) : bits_(num_bits) {}

  // Static factory: single bit set at position `pos` in a bitset of `num_bits`
  static Bitset with_bit(std::size_t pos, std::size_t num_bits) {
    Bitset b(num_bits);
    b.set(pos);
    return b;
  }

  // Static factory: create a bitset from an unsigned long value (auto-sized)
  static Bitset from_ulong(unsigned long val) {
    if (val == 0) return {};
    unsigned long tmp = val;
    std::size_t num_bits = 0;
    while (tmp != 0) {
      ++num_bits;
      tmp >>= 1;
    }
    return from_ulong(val, num_bits);
  }

  // Static factory: create a bitset from an unsigned long value with explicit size
  static Bitset from_ulong(unsigned long val, std::size_t num_bits) {
    Bitset b(num_bits);
    for (std::size_t i = 0; val != 0; ++i, val >>= 1) {
      if (val & 1) {
        b.set(i);
      }
    }
    return b;
  }

  // Static factory: all bits set in a bitset of `num_bits`
  static Bitset all_set(std::size_t num_bits) {
    Bitset b(num_bits);
    b.bits_.set();
    return b;
  }

  // Size management
  std::size_t size() const { return bits_.size(); }

  void resize(std::size_t num_bits) { bits_.resize(num_bits); }

  // Bit access
  bool test(std::size_t pos) const { return bits_.test(pos); }

  Bitset& set(std::size_t pos, bool val = true) {
    bits_.set(pos, val);
    return *this;
  }

  Bitset& set() {
    bits_.set();
    return *this;
  }

  Bitset& reset(std::size_t pos) {
    bits_.reset(pos);
    return *this;
  }

  Bitset& reset() {
    bits_.reset();
    return *this;
  }

  // Queries
  bool none() const { return bits_.none(); }
  bool any() const { return bits_.any(); }

  // Iteration over set bits
  std::size_t find_first() const { return bits_.find_first(); }
  std::size_t find_next(std::size_t pos) const { return bits_.find_next(pos); }

  // Conversion
  unsigned long to_ulong() const { return bits_.to_ulong(); }

  std::string to_string() const {
    std::string s;
    boost::to_string(bits_, s);
    return s;
  }

  // Shift operators (left shift auto-extends to avoid losing bits)
  Bitset& operator<<=(std::size_t n) {
    bits_.resize(bits_.size() + n);
    bits_ <<= n;
    return *this;
  }

  Bitset& operator>>=(std::size_t n) {
    bits_ >>= n;
    return *this;
  }

  Bitset operator<<(std::size_t n) const {
    Bitset r(*this);
    r <<= n;
    return r;
  }

  Bitset operator>>(std::size_t n) const {
    Bitset r;
    r.bits_ = bits_ >> n;
    return r;
  }

  // Complement
  Bitset operator~() const {
    Bitset r;
    r.bits_ = ~bits_;
    return r;
  }

  // Compound assignment (auto-resize to match sizes)
  Bitset& operator&=(const Bitset& rhs) {
    if (bits_.size() == rhs.bits_.size()) {
      bits_ &= rhs.bits_;
    } else if (bits_.size() < rhs.bits_.size()) {
      bits_.resize(rhs.bits_.size());
      bits_ &= rhs.bits_;
    } else {
      boost::dynamic_bitset<> tmp(rhs.bits_);
      tmp.resize(bits_.size());
      bits_ &= tmp;
    }
    return *this;
  }

  Bitset& operator|=(const Bitset& rhs) {
    if (bits_.size() == rhs.bits_.size()) {
      bits_ |= rhs.bits_;
    } else if (bits_.size() < rhs.bits_.size()) {
      bits_.resize(rhs.bits_.size());
      bits_ |= rhs.bits_;
    } else {
      boost::dynamic_bitset<> tmp(rhs.bits_);
      tmp.resize(bits_.size());
      bits_ |= tmp;
    }
    return *this;
  }

  Bitset& operator^=(const Bitset& rhs) {
    if (bits_.size() == rhs.bits_.size()) {
      bits_ ^= rhs.bits_;
    } else if (bits_.size() < rhs.bits_.size()) {
      bits_.resize(rhs.bits_.size());
      bits_ ^= rhs.bits_;
    } else {
      boost::dynamic_bitset<> tmp(rhs.bits_);
      tmp.resize(bits_.size());
      bits_ ^= tmp;
    }
    return *this;
  }

  // Binary operators
  friend Bitset operator&(const Bitset& a, const Bitset& b) {
    Bitset result(a);
    result &= b;
    return result;
  }

  friend Bitset operator|(const Bitset& a, const Bitset& b) {
    Bitset result(a);
    result |= b;
    return result;
  }

  friend Bitset operator^(const Bitset& a, const Bitset& b) {
    Bitset result(a);
    result ^= b;
    return result;
  }

  // Comparison (auto-resize to match before comparing)
  friend bool operator==(const Bitset& a, const Bitset& b) {
    if (a.bits_.size() == b.bits_.size()) {
      return a.bits_ == b.bits_;
    }
    // Zero-extend comparison: equal if same set bits
    Bitset ac(a);
    Bitset bc(b);
    auto max_size = std::max(ac.bits_.size(), bc.bits_.size());
    ac.bits_.resize(max_size);
    bc.bits_.resize(max_size);
    return ac.bits_ == bc.bits_;
  }

  friend bool operator!=(const Bitset& a, const Bitset& b) { return !(a == b); }

  // Integer comparison (compares the numeric value represented by the bitset)
  friend bool operator==(const Bitset& a, unsigned long val) {
    if (val == 0) return a.none();
    if (a.bits_.empty()) return val == 0;
    return a.bits_ == boost::dynamic_bitset<>(a.bits_.size(), val);
  }

  friend bool operator==(unsigned long val, const Bitset& a) { return a == val; }
  friend bool operator!=(const Bitset& a, unsigned long val) { return !(a == val); }
  friend bool operator!=(unsigned long val, const Bitset& a) { return !(a == val); }

  friend bool operator<(const Bitset& a, const Bitset& b) {
    if (a.bits_.size() == b.bits_.size()) {
      return a.bits_ < b.bits_;
    }
    Bitset ac(a);
    Bitset bc(b);
    auto max_size = std::max(ac.bits_.size(), bc.bits_.size());
    ac.bits_.resize(max_size);
    bc.bits_.resize(max_size);
    return ac.bits_ < bc.bits_;
  }

  friend bool operator>(const Bitset& a, const Bitset& b) { return b < a; }
  friend bool operator<=(const Bitset& a, const Bitset& b) { return !(b < a); }
  friend bool operator>=(const Bitset& a, const Bitset& b) { return !(a < b); }

 private:
  void match_sizes(const Bitset& other) {
    if (bits_.size() < other.bits_.size()) {
      bits_.resize(other.bits_.size());
    }
  }
};

}  // namespace CORE
