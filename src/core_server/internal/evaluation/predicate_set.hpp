#pragma once
#include <gmpxx.h>

#include <cstdint>
#include <string>

namespace CORE::Internal::CEA {

struct PredicateSet {
  enum Type { Satisfiable, Contradiction, Tautology };

  mpz_class mask;        // Specifies relevant bits
  mpz_class predicates;  // expected evaluation
  Type type;

  PredicateSet(Type type = Contradiction) : mask(0), predicates(0), type(type) {}

  PredicateSet(mpz_class mask, mpz_class predicates)
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
    mpz_class conflict = (mask & other.mask) & (predicates ^ other.predicates);
    if (conflict != 0) {
      return PredicateSet(Contradiction);
    }

    mpz_class combined_mask = mask | other.mask;
    mpz_class combined_predicates = predicates | other.predicates;
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
        mpz_class unselective_mask = (mpz_class(1) << (amount_of_bits + 1)) - 1;
        mpz_class new_mask = unselective_mask ^ mask;
        mpz_class new_predicates = ~predicates;
        return PredicateSet(new_mask, new_predicates);
    }
  }

  bool is_satisfied_by(const mpz_class& predicate_evaluation) const {
    switch (type) {
      case Contradiction:
        return false;
      case Tautology:
        return true;
      default:
        mpz_class conflicts = (predicate_evaluation ^ predicates) & mask;
        return conflicts == 0;
    }
  }

  bool operator==(const PredicateSet other) const {
    if (type != other.type || mask != other.mask) return false;
    if (type == Contradiction || type == Tautology) return true;
    auto conflicts = static_cast<mpz_class>((predicates ^ other.predicates)) & mask;
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
