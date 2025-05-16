#pragma once

#include <cstdint>
#include <string>

#include "shared/datatypes/custom_bitset.hpp"

namespace CORE::Internal::CEA {

struct PredicateSet {
  enum Type { Satisfiable, Contradiction, Tautology };

  CustomBitset mask;        // Specifies relevant bits
  CustomBitset predicates;  // expected evaluation
  Type type;

  PredicateSet(Type type = Contradiction) : mask(0), predicates(0), type(type) {}

  PredicateSet(CustomBitset mask, CustomBitset predicates)
      : mask(mask), predicates(predicates), type(Satisfiable) {}

  /**
   * This creates a new PredicateSet that is satisfied iff both this and
   * other are satisfied
   */
  PredicateSet operator&(const PredicateSet& other) const {
    if (type == Contradiction || other.type == Contradiction) {
      return PredicateSet(Contradiction);
    } else if (type == Tautology)
      return PredicateSet(other);
    else if (other.type == Tautology)
      return PredicateSet(*this);
    CustomBitset conflict = (mask & other.mask) & (predicates ^ other.predicates);
    if (conflict != 0) {
      return PredicateSet(Contradiction);
    }

    CustomBitset combined_mask = mask | other.mask;
    CustomBitset combined_predicates = predicates | other.predicates;
    PredicateSet combined(combined_mask, combined_predicates);
    return combined;
  }

  PredicateSet negate(int64_t amount_of_bits) const {
    switch (type) {
      case Contradiction:
        return PredicateSet(Tautology);
      case Tautology:
        return PredicateSet(Contradiction);
      default:
        CustomBitset unselective_mask = ~(CustomBitset(1) << (amount_of_bits + 1));
        CustomBitset new_mask = unselective_mask ^ mask;
        CustomBitset new_predicates = ~predicates;
        return PredicateSet(new_mask, new_predicates);
    }
  }

  bool is_satisfied_by(const CustomBitset& predicate_evaluation) const {
    switch (type) {
      case Contradiction:
        return false;
      case Tautology:
        return true;
      default:
        CustomBitset conflicts = (predicate_evaluation ^ predicates) & mask;
        return conflicts == 0;
    }
  }

  bool operator==(const PredicateSet other) const {
    if (type != other.type || mask != other.mask) return false;
    if (type == Contradiction || type == Tautology) return true;
    auto conflicts = static_cast<CustomBitset>((predicates ^ other.predicates)) & mask;
    return conflicts == 0;
  }

  // For set comparison
  bool operator<(const PredicateSet other) const {
    if (type < other.type) return true;
    if (type > other.type) return false;
    if (mask < other.mask) return true;
    if (mask > other.mask) return false;
    if (mask < other.predicates) return true;
    return false;
  }

  std::string to_string() const {
    if (type == Contradiction)
      return "⊥";
    else if (type == Tautology)
      return "⊤";
    std::string out = predicates.get_str(2);
    std::string mask_string = mask.get_str(2);

    uint64_t buffer_length = out.size() > mask_string.size()
                               ? out.size() - mask_string.size()
                               : mask_string.size() - out.size();
    std::string buffer = "";
    for (int i = 0; i < buffer_length; i++) {
      buffer += '0';
    }

    if (out.size() > mask_string.size())
      out = buffer + out;
    else
      mask_string = buffer + mask_string;
    for (int i = 0; i <= out.size(); i++) {
      if (mask_string[i] == '0') {
        out[i] = '?';
      }
    }
    return out;
  }
};
}  // namespace CORE::Internal::CEA
