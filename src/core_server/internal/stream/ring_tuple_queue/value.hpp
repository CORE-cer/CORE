#ifndef VALUE_HPP
#define VALUE_HPP

#include <cstdint>
#include <iostream>
#include <type_traits>

namespace RingTupleQueue {
template <typename T>
class Value {
 private:
  uint64_t* const val_ptr;

 public:
  explicit Value(uint64_t* val_ptr) : val_ptr(val_ptr) {}

  T get() const {
    // Currently only supports string_view and other trivially_copiable classes
    if constexpr (!std::is_same_v<std::string_view, T>) {
      return *reinterpret_cast<const T*>(val_ptr);
    } else {
      auto first_pos = reinterpret_cast<char*>(val_ptr[0]);
      auto second_pos = reinterpret_cast<char*>(val_ptr[1]);
      return T(first_pos, second_pos - first_pos);
    }
  }
};
}  // namespace RingTupleQueue

#endif  // VALUE_HPP
