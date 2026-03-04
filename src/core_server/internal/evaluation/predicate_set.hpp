#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>

#include "shared/datatypes/bitset.hpp"

namespace CORE::Internal::CEA {

struct PredicateSet {
  enum Type { Satisfiable, Contradiction, Tautology };

  Bitset mask;        // Specifies relevant bits
  Bitset predicates;  // expected evaluation
  Type type;

  PredicateSet(Type type = Contradiction) : type(type) {}

  PredicateSet(Bitset mask, Bitset predicates)
      : mask(std::move(mask)), predicates(std::move(predicates)), type(Satisfiable) {}

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
    Bitset conflict = (mask & other.mask) & (predicates ^ other.predicates);
    if (conflict.any()) {
      return PredicateSet(Contradiction);
    }

    Bitset combined_mask = mask | other.mask;
    Bitset combined_predicates = predicates | other.predicates;
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
        Bitset unselective_mask = Bitset::all_set(
          static_cast<std::size_t>(amount_of_bits + 1));
        Bitset new_mask = unselective_mask ^ mask;
        Bitset new_predicates = ~predicates;
        return PredicateSet(new_mask, new_predicates);
    }
  }

  bool is_satisfied_by(const Bitset& predicate_evaluation) const {
    switch (type) {
      case Contradiction:
        return false;
      case Tautology:
        return true;
      default:
        Bitset conflicts = (predicate_evaluation ^ predicates) & mask;
        return conflicts.none();
    }
  }

  bool operator==(const PredicateSet other) const {
    if (type != other.type || mask != other.mask) return false;
    if (type == Contradiction || type == Tautology) return true;
    Bitset conflicts = (predicates ^ other.predicates) & mask;
    return conflicts.none();
  }

  // For set comparison
  bool operator<(const PredicateSet other) const {
    if (type < other.type) return true;
    if (type > other.type) return false;
    if (mask < other.mask) return true;
    if (mask > other.mask) return false;
    if (predicates < other.predicates) return true;
    return false;
  }

  std::string to_string() const {
    if (type == Contradiction)
      return "\u22A5";
    else if (type == Tautology)
      return "\u22A4";
    std::string out = predicates.to_string();
    std::string mask_string = mask.to_string();

    // Pad shorter string with leading zeros
    if (out.size() < mask_string.size()) {
      out.insert(0, mask_string.size() - out.size(), '0');
    } else if (mask_string.size() < out.size()) {
      mask_string.insert(0, out.size() - mask_string.size(), '0');
    }
    for (std::size_t i = 0; i < out.size(); i++) {
      if (mask_string[i] == '0') {
        out[i] = '?';
      }
    }
    return out;
  }
};
}  // namespace CORE::Internal::CEA
