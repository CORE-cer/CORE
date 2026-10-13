# Proposal

## Why

`NOT(...)` gave wrong results when its formula combined filters with `OR`: it matched events that satisfied one of the filters, although the formula captured them. It also accepted only an event type with filters, so it could not express the atomic formulas that the not-operator is defined over in `docs/papers/Operators_report.pdf`, Section 3. NEXT and LAST are defined on top of the not-operator, so it has to be right first.

## What Changes

This change alters what some queries mean.

- **BREAKING**: `NOT` over an `OR` of filters now captures only the events that satisfy none of the filters. Queries that relied on the old result return fewer complex events.
- `NOT` now accepts the atomic formulas of the paper: an event type, `AS`, `FILTER`, `OR` and `AND`, with parentheses for grouping.
- `AND` is new syntax and is only accepted inside `NOT`.
- `NOT` keeps rejecting formulas that capture more than one event (sequencing and iteration) and a `NOT` nested inside another `NOT`, as in the paper.
- No change for queries that use `NOT` over an event type with at most `AND`-combined filters.

Out of scope: the NEXT and LAST operators, projection inside `NOT`, and `AND` outside `NOT`.

## Capabilities

### New Capabilities

- `ceql/not-operator`: what `NOT(...)` captures, which formulas it accepts, and how it combines with the rest of a query.

### Modified Capabilities

None. This is the first spec in the repository.

## Impact

- Query language: the grammar of the formula inside `NOT` grows; the generated parser changes with it.
- Query compilation: how the automaton of `NOT` is built from the automaton of its formula.
- Existing queries: only those with `NOT` over an `OR` of filters change results. No query in the e2e datasets uses `NOT`, and their expected outputs are unchanged.
- No API, client, server or Python binding changes.
