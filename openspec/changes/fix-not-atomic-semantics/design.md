# Design

## Context

See proposal.md for the motivation. A query formula is compiled to an automaton whose transitions carry a predicate set (which predicate bits must hold) and the variables they mark. `NOT(α)` was built by rewriting each transition of α's automaton on its own into "one of these bits is 0". That is correct for one transition and wrong for several: a filter `OR` produces a union of automata, and negating each branch separately gives ¬t1 ∨ ¬t2 where ¬(t1 ∨ t2) is needed. The rewrite also assumed every expected bit was 1.

## Goals / Non-Goals

**Goals:**

- Build `NOT(α)` so it matches the spec for every atomic formula, however α's automaton was assembled.
- Fail loudly, not silently, if a formula that is not atomic ever reaches the construction.
- Support the atomic operators of the paper: `AS`, `OR` and `AND`.

**Non-Goals:**

- NEXT and LAST. They build on `NOT` and come in a later change.
- A general complement of automata. Only single-event automata are negated.
- Formula-level projection and `AND` outside `NOT`.

## Decisions

### Negate the disjunction of all transitions, after normalizing

`NegateExpected` first normalizes α's automaton with the passes the CEA construction already uses (unique initial state, remove epsilon transitions, remove useless states, remove unreachable states). It then requires the result to be single-step: every transition leaves the initial state and reaches a final state that has no outgoing transitions. Under that shape α captures an event exactly when the disjunction P1 ∨ … ∨ Pn of the transition predicates holds, so `NOT(α)` is the negation of that disjunction, expanded to one unmarked transition per disjunct.

- *Why normalize first:* constructions such as union and intersection may leave epsilon transitions or dead states. Normalizing means the negation does not depend on how α was built.
- *Why check the shape:* the grammar only lets atomic formulas through, but the AST type does not enforce it. A `std::logic_error` turns a future mistake into a visible failure instead of a wrong automaton.
- *Alternative considered:* compile atomic formulas straight to a predicate without building an automaton. Cleaner in principle, but it duplicates the existing constructions for event types, filters and `AS`. Rejected for now.

### Use the expected value of each predicate bit

Each disjunct ¬Pi is the set of single-bit literals that contradict Pi, taking the bit's expected value into account. Contradiction and tautology predicate sets are handled explicitly (¬⊥ is neutral, ¬⊤ makes the result empty). Conjunctions that imply another one are dropped so the automaton does not carry redundant transitions.

### AND as a product that synchronizes on equal markings

`AND` is compiled with a new `Intersection` construction: a product automaton in which two transitions synchronize only when they mark the same variables, with their predicates conjoined. This is the paper's intersection of complex events, where both sides must bind the same events to the same variables.

- *Consequence:* `(SELL AS s) AND SELL` captures nothing, because only one side marks `s`. The spec states this.
- *Expected consequence:* `Stream>Event AND Event` should also capture nothing, because CORE binds a stream-qualified event type to an extra variable. This is read off the construction; no test or scenario covers it.

### Grammar: extend the atomic rule, do not reuse `cel_formula`

`atomic_cel_formula` gains parentheses, `AS`, `AND` and `OR`, with the precedence of `cel_formula` (`AS`, then `AND`, then `OR`, then `FILTER`). Keeping a separate rule means sequencing, iteration and nested `NOT` are rejected by the parser with a syntax error.

- *Nested `NOT` stays out.* The construction would handle it, since the automaton of `NOT(α)` is itself single-step, but the paper's atomic grammar does not include the not-operator and the decision is to follow it.
- *Projection stays out.* CEQL has no formula-level projection syntax, and projection does not change which positions α captures, so it would not change what `NOT` captures.

## Risks / Trade-offs

- [The negation expands to a product of literals, which can grow with many `OR` branches] → Implied conjunctions are pruned after each step. Atomic formulas are small in practice; revisit if a real query shows a blow-up.
- [`AND` has the paper's strict meaning, which can surprise users] → The spec has a scenario for sides that bind different variables.
- [Queries with `NOT` over an `OR` of filters change results] → This is the bug being fixed. No query in the e2e datasets uses `NOT`.

## Open Questions

- Whether nested `NOT` should be allowed once NEXT and LAST exist, for example `NEXT(NOT(SELL))`. It does not affect this change.
