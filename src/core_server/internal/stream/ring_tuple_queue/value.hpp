#ifndef VALUE_HPP
#define VALUE_HPP

#include <cstdint>
#include <type_traits>

namespace RingTupleQueue {
template <typename T> class Value {
private:
  uint64_t *const val_ptr;

public:
  explicit Value(uint64_t *val_ptr) : val_ptr(val_ptr) {}

  T get() const {
    if constexpr (std::is_trivially_copyable_v<T>) {
      return *reinterpret_cast<const T *>(val_ptr);
    } else {
      // Note: This assumes that T can be constructed from a uint64_t* and a
      // uint64_t*
      return T(reinterpret_cast<const char *>(*val_ptr),
               reinterpret_cast<const char *>(*(val_ptr + 1)));
    }
  }
};
} // namespace RingTupleQueue

#endif // VALUE_HPP
