#pragma once

namespace CORE::Internal::Optimizations {

// How the minterm-tree optimization treats `attr = "literal"` / `attr != "literal"`.
// Only meaningful when predicate_evaluation == MintermTree; inert otherwise.
//   Opaque           the original behaviour: a fresh, unconstrained boolean per
//                     atom, related to nothing.
//   InternedEquality every distinct string literal compared with `=`/`!=` in the
//                     query is interned to a distinct int64_t id (first-come,
//                     deduplicated), and the attribute is modeled as a Z3
//                     integer holding "which interned literal (if any) it
//                     currently equals". Two different literals always get
//                     different ids, so mutually exclusive string equalities on
//                     one attribute collapse the same way mutually exclusive
//                     int equalities already do; an attribute whose real value
//                     matches none of the query's literals is still correctly
//                     representable (its id is just some other, unconstrained
//                     integer). Exact, not approximate: unlike most of what this
//                     module models (NaN, int->double rounding), there is no
//                     precision loss here. LIKE/regex, string ORDERING
//                     comparisons, and attribute-vs-attribute string equality
//                     stay opaque either way (see
//                     guides/minterm_tree/08-the-files-one-by-one.md
//                     §8.4/§8.12).
// This header is deliberately free of any dependency (e.g. Z3), like
// predicate_evaluation_strategy.hpp, so the rest of the engine can refer to the
// strategy without knowing how it is implemented.
enum class StringEqualityStrategy {
  Opaque,
  InternedEquality,
};

}  // namespace CORE::Internal::Optimizations
