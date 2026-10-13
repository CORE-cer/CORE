# Spec Delta

## Purpose

Defines what the CEQL `NOT(...)` operator captures, which formulas it accepts, and how it combines with the rest of a query. It follows the not-operator `!α` of `docs/papers/Operators_report.pdf`, Section 3.

## ADDED Requirements

### Requirement: NOT captures the events its formula does not capture

`NOT(α)` SHALL capture, for each position `i` of the stream, the complex event `(i, i, ∅)` exactly when `α` captures no complex event at position `i`. This is the not-operator `!α` of `docs/papers/Operators_report.pdf`, Section 3.

#### Scenario: NOT of an event type

- **WHEN** `NOT(SELL)` runs over a stream of SELL and BUY events
- **THEN** it captures one complex event for each event that is not a SELL

```scenario
declare: CREATE STREAM Stock { EVENT SELL { name:string, price:int }, EVENT BUY { name:string, price:int } }
query: |
  SELECT * FROM Stock
  WHERE NOT(SELL)
  CONSUME BY NONE
stream:
  - SELL MSFT 101
  - SELL MSFT 102
  - SELL INTL 80
  - BUY INTL 80
  - SELL AMZN 100
  - BUY AMZN 2000
expect:
  - [3, 3, {}]
  - [5, 5, {}]
```

#### Scenario: An OR of filters is negated as a whole

- **WHEN** the formula inside `NOT` combines two filters with `OR`
- **THEN** `NOT` captures only the events that satisfy neither filter, plus the events of other types

```scenario
declare: CREATE STREAM Stock { EVENT SELL { name:string, price:int }, EVENT BUY { name:string, price:int } }
query: |
  SELECT * FROM Stock
  WHERE NOT(SELL FILTER SELL[name='MSFT'] OR SELL[price > 100])
  CONSUME BY NONE
stream:
  - SELL MSFT 50
  - SELL INTL 500
  - SELL INTL 50
  - BUY MSFT 500
expect:
  - [2, 2, {}]
  - [3, 3, {}]
```

### Requirement: NOT binds no variables

The event read by `NOT(α)` SHALL NOT be bound to any variable, including the event types and the `AS` variables that appear in `α`, so it never appears in the output. It SHALL still count toward the interval of the complex event. This is the empty valuation `µ∅` of `docs/papers/Operators_report.pdf`, Section 3.

#### Scenario: NOT in a contiguous sequence extends the interval without adding an event

- **WHEN** `NOT(SELL)` is followed contiguously by a BUY and a SELL
- **THEN** the complex event starts at the event read by `NOT` and binds only the BUY and the SELL

```scenario
declare: CREATE STREAM Stock { EVENT SELL { name:string, price:int }, EVENT BUY { name:string, price:int } }
query: |
  SELECT * FROM Stock
  WHERE NOT(SELL) : BUY : SELL
  CONSUME BY NONE
stream:
  - BUY MSFT 101
  - SELL INTL 1000
  - BUY AMZN 200
  - SELL AMZN 1000
  - BUY INTL 300
  - BUY MSFT 1000
  - SELL AMZN 30
expect:
  - [4, 6, {BUY: [5], SELL: [6]}]
```

#### Scenario: An AS variable inside NOT stays empty

- **WHEN** the formula inside `NOT` names its event with `AS` and filters on that name
- **THEN** the filter decides which events `NOT` captures, and the variable binds nothing in the output

```scenario
declare: CREATE STREAM Stock { EVENT SELL { name:string, price:int }, EVENT BUY { name:string, price:int } }
query: |
  SELECT * FROM Stock
  WHERE BUY : NOT(SELL AS s FILTER s[name='MSFT'])
  CONSUME BY NONE
stream:
  - BUY MSFT 100
  - SELL MSFT 100
  - BUY INTL 100
  - SELL INTL 100
expect:
  - [2, 3, {BUY: [2]}]
```

### Requirement: NOT accepts atomic formulas

The formula inside `NOT` SHALL be an atomic formula: an event type, optionally written `Stream>Event`, combined with `AS`, `FILTER`, `OR`, `AND` and parentheses. `AS` binds tightest, then `AND`, then `OR`, then `FILTER`. This is the atomic grammar of `docs/papers/Operators_report.pdf`, Section 3, with two differences: CORE adds stream-qualified event types, and it does not accept projection inside `NOT`.

#### Scenario: NOT of an OR of event types

- **WHEN** the formula inside `NOT` is an `OR` of two event types, one of them filtered
- **THEN** `NOT` captures only the events that neither side captures

```scenario
declare: CREATE STREAM Stock { EVENT SELL { name:string, price:int }, EVENT BUY { name:string, price:int } }
query: |
  SELECT * FROM Stock
  WHERE NOT(BUY OR SELL FILTER SELL[price > 100])
  CONSUME BY NONE
stream:
  - BUY MSFT 50
  - SELL MSFT 500
  - SELL MSFT 50
expect:
  - [2, 2, {}]
```

### Requirement: AND inside NOT intersects the complex events of both sides

Inside `NOT`, `α1 AND α2` SHALL capture the complex events captured by both sides: the same event with the same variable bindings. Sides that bind different variables therefore capture nothing in common. This is the `AND` of `docs/papers/Operators_report.pdf`, Section 2, Figure 1.

#### Scenario: AND of two filtered formulas

- **WHEN** both sides of `AND` are the same event type with different filters
- **THEN** `NOT` captures every event that fails at least one of the filters, plus the events of other types

```scenario
declare: CREATE STREAM Stock { EVENT SELL { name:string, price:int }, EVENT BUY { name:string, price:int } }
query: |
  SELECT * FROM Stock
  WHERE NOT((SELL FILTER SELL[price > 100]) AND (SELL FILTER SELL[name='MSFT']))
  CONSUME BY NONE
stream:
  - SELL MSFT 500
  - SELL MSFT 50
  - SELL INTL 500
  - BUY MSFT 500
expect:
  - [1, 1, {}]
  - [2, 2, {}]
  - [3, 3, {}]
```

#### Scenario: AND of sides that bind different variables captures nothing

- **WHEN** only one side of `AND` binds an `AS` variable
- **THEN** the two sides share no complex event, so `NOT` captures every event

```scenario
declare: CREATE STREAM Stock { EVENT SELL { name:string, price:int }, EVENT BUY { name:string, price:int } }
query: |
  SELECT * FROM Stock
  WHERE NOT((SELL AS s) AND SELL)
  CONSUME BY NONE
stream:
  - SELL MSFT 500
  - BUY MSFT 500
expect:
  - [0, 0, {}]
  - [1, 1, {}]
```

### Requirement: NOT rejects formulas that are not atomic

A query SHALL be rejected with a syntax error when the formula inside `NOT` uses sequencing (`;` or `:`), iteration (`+` or `:+`) or another `NOT`. A nested `NOT` is excluded on purpose: the atomic grammar of `docs/papers/Operators_report.pdf`, Section 3, does not contain the not-operator.

#### Scenario: Sequencing inside NOT is rejected

- **WHEN** the formula inside `NOT` is a sequence of two event types
- **THEN** the query is rejected with a syntax error

```scenario
declare: CREATE STREAM Stock { EVENT SELL { name:string, price:int }, EVENT BUY { name:string, price:int } }
query: |
  SELECT * FROM Stock
  WHERE NOT(BUY : SELL)
  CONSUME BY NONE
stream: []
expect: error
```

#### Scenario: Iteration inside NOT is rejected

- **WHEN** the formula inside `NOT` is an iteration
- **THEN** the query is rejected with a syntax error

```scenario
declare: CREATE STREAM Stock { EVENT SELL { name:string, price:int }, EVENT BUY { name:string, price:int } }
query: |
  SELECT * FROM Stock
  WHERE NOT(BUY+)
  CONSUME BY NONE
stream: []
expect: error
```

#### Scenario: NOT nested inside NOT is rejected

- **WHEN** the formula inside `NOT` is another `NOT`
- **THEN** the query is rejected with a syntax error

```scenario
declare: CREATE STREAM Stock { EVENT SELL { name:string, price:int }, EVENT BUY { name:string, price:int } }
query: |
  SELECT * FROM Stock
  WHERE NOT(NOT(SELL))
  CONSUME BY NONE
stream: []
expect: error
```
