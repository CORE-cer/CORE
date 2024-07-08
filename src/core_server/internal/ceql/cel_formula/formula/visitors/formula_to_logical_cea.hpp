#pragma once

#include <gmpxx.h>

#include <cassert>
#include <cstdint>
#include <iostream>
#include <map>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>

#include "core_server/internal/ceql/cel_formula/filters/visitors/apply_filters_to_logical_cea.hpp"
#include "core_server/internal/ceql/cel_formula/formula/formula_headers.hpp"
#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/constructions/contiguous_iteration.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/constructions/contiguous_sequencing.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/constructions/mark_variable.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/constructions/non_contiguous_iteration.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/constructions/non_contiguous_sequencing.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/constructions/project.hpp"
#include "core_server/internal/evaluation/logical_cea/transformations/constructions/union.hpp"
#include "formula_visitor.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/catalog/event_info.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"

namespace CORE::Internal::CEQL {

class FormulaToLogicalCEA : public FormulaVisitor {
  using VariablesToMark = mpz_class;
  using EndNodeId = int64_t;
  using StreamName = std::string;
  using EventName = std::string;

 private:
  const QueryCatalog& query_catalog;

  std::map<std::pair<StreamName, EventName>, uint64_t> stream_event_to_id;
  // Only stores variables declared on AS
  std::map<std::string, uint64_t> variables_to_id;
  uint64_t next_variable_id = query_catalog.number_of_events()
                              + query_catalog.number_of_unique_event_names_query();

 public:
  CEA::LogicalCEA current_cea{0};

  FormulaToLogicalCEA(const QueryCatalog& query_catalog) : query_catalog(query_catalog) {
    uint64_t next_id = 0;
    for (const Types::StreamInfo& stream : query_catalog.get_all_streams_info()) {
      for (const Types::EventInfo& event : stream.events_info) {
        if (stream_event_to_id.contains({stream.name, event.name})) {
          throw std::logic_error(
            "Stream name and Event name combination already processed");
        }
        stream_event_to_id.insert({{stream.name, event.name}, next_id++});
      }
    }
    assert(next_id + query_catalog.number_of_unique_event_names_query()
           == next_variable_id);
  }

  ~FormulaToLogicalCEA() override = default;

  void visit(EventTypeFormula& formula) override {
    if (query_catalog.get_unique_events_from_event_name(formula.event_name).size() == 0) {
      throw std::runtime_error("The event_name: " + formula.event_name +
                               " is not in the catalog, and base cases "
                               "that are variables are not allowed.");
    }
    if (formula.stream_name.has_value()) {
      current_cea = CEA::LogicalCEA::atomic_cea(query_catalog,
                                                stream_event_to_id,
                                                formula.stream_name.value(),
                                                formula.event_name);
    } else {
      current_cea = CEA::LogicalCEA::atomic_cea(query_catalog,
                                                stream_event_to_id,
                                                formula.event_name);
    }
  }

  void visit(FilterFormula& formula) override {
    formula.formula->accept_visitor(*this);
    ApplyFiltersToLogicalCEA visitor(current_cea,
                                     stream_event_to_id,
                                     variables_to_id,
                                     query_catalog);
    formula.filter->accept_visitor(visitor);
  }

  void visit(OrFormula& formula) override {
    formula.left->accept_visitor(*this);
    CEA::LogicalCEA left_cea = std::move(current_cea);
    formula.right->accept_visitor(*this);
    CEA::LogicalCEA right_cea = std::move(current_cea);
    current_cea = CEA::Union()(left_cea, right_cea);
  }

  void visit(UnlessFormula& formula) override {
    formula.left->accept_visitor(*this);

    // de esa manera no hay que añadir este if, solo negar y llamar a accept_visitor

    auto right = formula.right.get();
    EventTypeFormula* event_type_formula = dynamic_cast<EventTypeFormula*>(right);
    if (event_type_formula != nullptr) {
      // TODO(unless): create from event
      // CEA::UnlessEventTransform transform(query_catalog, *event_type_formula);
      // current_cea = transform(std::move(current_cea));

      // Obtener el índice del evento
      if (query_catalog.get_unique_events_from_event_name(event_type_formula->event_name)
            .size()
          == 0) {
        throw std::runtime_error("The event_name: " + event_type_formula->event_name +
                               " is not in the catalog, and base cases "
                               "that are variables are not allowed.");
      }
      // TODO: no se revisa que la combinación de Stream/Event sea correcta

      std::size_t number_of_streams = query_catalog.number_of_streams();
      Types::StreamTypeId query_stream_id = query_catalog
                                              .get_query_stream_id_from_stream_name(
                                                event_type_formula->stream_name.value());
      Types::EventNameTypeId
        query_event_name_id = query_catalog.get_query_event_name_id_from_event_name(
          event_type_formula->stream_name.value());

      mpz_class stream_predicate_position = mpz_class(1) << query_stream_id;
      mpz_class event_name_predicate_position = mpz_class(1) << (number_of_streams
                                                                 + query_event_name_id);

      mpz_class expected_eval = stream_predicate_position | event_name_predicate_position;

      // Modificar las transiciones para que no acepten el evento
      for (int i = 0; i < current_cea.amount_of_states; i++) {
        auto transition = current_cea.transitions[i].begin();
        while (transition != current_cea.transitions[i].end()) {
          CEA::PredicateSet transition_predicate_set = std::get<0>(*transition);
          mpz_class transition_expected_eval = transition_predicate_set.mask;
          mpz_class transition_predicate_mask = transition_predicate_set.predicates;
          if (expected_eval == transition_expected_eval) {
            // (X or Y) UNLESS X y estamos en la transición X del or
            transition = current_cea.transitions[i].erase(transition);

          } else {
            // Esperar que el evento sea 0
            auto eval = transition_expected_eval & ~expected_eval;
            // Considerar 1 en la posición del evento
            auto mask = transition_predicate_mask | expected_eval;
            // Modificar la transición (overload)
            *transition = std::make_tuple(CEA::PredicateSet(eval, mask),
                                          std::get<1>(*transition),
                                          std::get<2>(*transition));
            // Pasar al siguiente (overload)
            ++transition;
          }
        }
      }

      return;
    }

    FilterFormula* filterFormula = dynamic_cast<FilterFormula*>(right);
    if (filterFormula != nullptr) {
      // TODO(unless): create from filter
      // CEA::UnlessEventTransform transform(query_catalog, *event_type_formula);
      // current_cea = transform(std::move(current_cea));
      return;
    }

    throw std::runtime_error("UNLESS [" + formula.right->to_string()
                             + "] not implemented");
  }

  void visit(NonContiguousSequencingFormula& formula) override {
    formula.left->accept_visitor(*this);
    CEA::LogicalCEA left_cea = std::move(current_cea);
    formula.right->accept_visitor(*this);
    CEA::LogicalCEA right_cea = std::move(current_cea);
    current_cea = CEA::NonContiguousSequencing()(left_cea, right_cea);
  }

  void visit(ContiguousSequencingFormula& formula) override {
    formula.left->accept_visitor(*this);
    CEA::LogicalCEA left_cea = std::move(current_cea);
    formula.right->accept_visitor(*this);
    CEA::LogicalCEA right_cea = std::move(current_cea);
    current_cea = CEA::ContiguousSequencing()(left_cea, right_cea);
  }

  void visit(NonContiguousIterationFormula& formula) override {
    formula.formula->accept_visitor(*this);  // updates current_cea
    current_cea = CEA::NonContiguousIteration()(std::move(current_cea));
  }

  void visit(ContiguousIterationFormula& formula) override {
    formula.formula->accept_visitor(*this);
    current_cea = CEA::ContiguousIteration()(std::move(current_cea));
  }

  void visit(ProjectionFormula& formula) override {
    mpz_class variables_to_project = 0;
    for (const std::string& var_name : formula.variables) {
      if (variables_to_id.contains(var_name)) {
        // As variable
        variables_to_project |= mpz_class(1) << variables_to_id.find(var_name)->second;
      } else {
        try {
          Types::EventNameTypeId
            query_event_name_id = query_catalog.get_query_event_name_id_from_event_name(
              var_name);
          variables_to_project |= mpz_class(1)
                                  << (stream_event_to_id.size() + query_event_name_id);
        } catch (std::runtime_error e) {
          std::cout << "Projecting on unknown variable" << std::endl;
        }
      }
    }
    for (auto&& [stream_name, event_name] : formula.streams_events) {
      auto stream_event_id_iter = stream_event_to_id.find({stream_name, event_name});
      if (stream_event_id_iter == stream_event_to_id.end()) {
        std::cout << "Projecting on unknown variable" << std::endl;
        continue;
      }
      variables_to_project |= mpz_class(1) << (stream_event_id_iter->second);
    }
    current_cea = CEA::Project(variables_to_project)(std::move(current_cea));
  }

  void visit(AsFormula& formula) override {
    formula.formula->accept_visitor(*this);
    if (!variables_to_id.contains(formula.variable_name)) {
      variables_to_id[formula.variable_name] = next_variable_id++;
    }
    uint64_t variable_id = variables_to_id[formula.variable_name];
    current_cea = CEA::MarkVariable(variable_id)(std::move(current_cea));
  }
};
}  // namespace CORE::Internal::CEQL
