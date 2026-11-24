#pragma once

#include <bit>
#include <concepts>
#include <cstdint>
#include <cstdio>
#include <initializer_list>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <typeinfo>

// Helpers for PhysicalPredicate::structural_key() and MathExpr::structural_key():
// an EXACT description of what a predicate computes, so that two separately
// built predicates that always give the same answer on the same event can be
// recognized as such (backlog item 33: the minterm tree gives structurally
// identical opaque atoms one shared Z3 boolean instead of one each).
//
// Exactness is the whole point. Two predicates with equal keys are treated as
// the same condition, so a key must capture EVERYTHING eval() depends on:
//   - the concrete class, including its template arguments (the comparison,
//     int64 vs double, the stored vs the computed type). typeid().name() is the
//     mangled name, which includes all of them;
//   - every field eval() reads, encoded without loss: a double by its bit
//     pattern (to_string() would print 6 decimals, so 1.0000001 and 1.0000002
//     would collide), a string with its length in front (so no separator can
//     be confused with its content), an integer in decimal;
//   - the children's keys, recursively.
// A key is built as `<class>(<part>,<part>,...)`. Mangled names contain no
// parentheses or commas, and every string part is length-prefixed, so two
// different trees can never produce the same key.
//
// A class that does not override structural_key() returns nullopt, meaning
// "never merge": being conservative only costs tree size, never correctness.
// And if any child has no key, neither has its parent.
namespace CORE::Internal::CEA::StructuralKey {

// One field, encoded without loss and tagged with its kind.
template <typename T>
std::string field(const T& value) {
  if constexpr (std::is_convertible_v<const T&, std::string_view>) {
    std::string_view text = value;
    return "s" + std::to_string(text.size()) + ":" + std::string(text);
  } else if constexpr (std::is_same_v<T, bool>) {
    return value ? "b1" : "b0";
  } else if constexpr (std::floating_point<T>) {
    // The bit pattern: exact, and keeps 0.0 and -0.0 apart (they compare equal
    // but are distinct values, so treating them as different is the safe side).
    static_assert(sizeof(T) == sizeof(uint64_t), "only double is expected here");
    char hex[17];
    std::snprintf(hex,
                  sizeof(hex),
                  "%016llx",
                  static_cast<unsigned long long>(std::bit_cast<uint64_t>(value)));
    return std::string("d") + hex;
  } else if constexpr (std::signed_integral<T>) {
    return "i" + std::to_string(static_cast<long long>(value));
  } else if constexpr (std::unsigned_integral<T>) {
    return "u" + std::to_string(static_cast<unsigned long long>(value));
  } else {
    static_assert(sizeof(T) == 0, "no structural encoding for this field type");
  }
}

// A node: its class and its parts (fields and children keys, in a fixed order).
// nullopt if any part is nullopt, i.e. some child cannot be described.
template <typename Self>
std::optional<std::string>
node(const Self& self, std::initializer_list<std::optional<std::string>> parts) {
  std::string out = typeid(self).name();
  out += '(';
  bool first = true;
  for (const std::optional<std::string>& part : parts) {
    if (!part.has_value()) return std::nullopt;
    if (!first) out += ',';
    out += *part;
    first = false;
  }
  out += ')';
  return out;
}

}  // namespace CORE::Internal::CEA::StructuralKey
