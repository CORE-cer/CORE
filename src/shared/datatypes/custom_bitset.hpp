#pragma once

#include <gmpxx.h>

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>

class CustomBitset {
  // boost::dynamic_bitset<uint8_t> bitset;
  mpz_class bitset;

  CustomBitset(mpz_class bitset) : bitset(bitset) {}

 public:
  CustomBitset() : bitset() {}

  CustomBitset(uint64_t size) {
    // switch (size) {
    //   case 0:
    //     bitset = mpz_class(0);
    //     break;
    //   case 1:
    //     bitset = mpz_class(1);
    //     break;
    //   default:
    //     throw std::invalid_argument(
    //       "You are only allowed to initialize empty bitsets or 1");
    // }
    bitset = size;
  }

  CustomBitset(const CustomBitset& other) : bitset(other.bitset) {}

  CustomBitset(CustomBitset&& other) noexcept : bitset(std::move(other.bitset)) {}

  std::string get_str(int base) const {
    if (base != 2) {
      throw std::invalid_argument("You are only allowed to convert to binary string");
    }
    return bitset.get_str(base);
  }

  CustomBitset& operator|=(const CustomBitset& other) {
    bitset |= other.bitset;
    return *this;
  }

  CustomBitset& operator&=(const CustomBitset& other) {
    bitset &= other.bitset;
    return *this;
  }

  CustomBitset& operator^=(const CustomBitset& other) {
    bitset ^= other.bitset;
    return *this;
  }

  CustomBitset& operator<<=(size_t shift) {
    bitset <<= shift;
    return *this;
  }

  CustomBitset& operator>>=(size_t shift) {
    bitset >>= shift;
    return *this;
  }

  bool operator==(const int other) { return bitset == other; }

  bool operator==(const CustomBitset& other) const { return bitset == other.bitset; }

  bool operator>(const CustomBitset& other) const { return bitset > other.bitset; }

  bool operator<(const CustomBitset& other) const { return bitset < other.bitset; }

  CustomBitset operator|(const CustomBitset& other) const {
    return {bitset | other.bitset};
  }

  CustomBitset operator&(const CustomBitset& other) const {
    return {bitset & other.bitset};
  }

  CustomBitset operator^(const CustomBitset& other) const {
    return {bitset ^ other.bitset};
  }

  CustomBitset operator<<(size_t shift) { return {bitset << shift}; }

  CustomBitset operator>>(size_t shift) { return {bitset >> shift}; }

  CustomBitset operator~() const { return {~this->bitset}; }

  CustomBitset operator=(const CustomBitset& other) {
    bitset = other.bitset;
    return *this;
  }
};
