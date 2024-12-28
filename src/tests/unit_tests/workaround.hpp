#include <catch2/internal/catch_decomposer.hpp>
#include <cstddef>
#include <tuple>

// Workaround for https://github.com/llvm/llvm-project/issues/113087
namespace std {

template <>
struct tuple_size<Catch::Decomposer> {
  static constexpr size_t value = 1;
};

}  // namespace std
