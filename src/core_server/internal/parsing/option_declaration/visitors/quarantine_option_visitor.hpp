#pragma once

#include <any>
#include <chrono>
#include <cstdint>
#include <optional>
#include <set>
#include <string>

#include "core_server/internal/interface/modules/quarantine/quarantine_policies/quarantine_policy_type.hpp"
#include "core_server/internal/parsing/option_declaration/autogenerated/OptionDeclarationParser.h"
#include "core_server/internal/parsing/option_declaration/autogenerated/OptionDeclarationParserBaseVisitor.h"

namespace CORE::Internal::Parsing::Option {
class QuarantineOptionVisitor : public OptionDeclarationParserBaseVisitor {
 public:
  Interface::Module::Quarantine::QuarantinePolicy::QuarantinePolicyType policy_type;
  std::optional<std::chrono::nanoseconds> time_window;
  std::set<std::string> streams;

  std::any
  visitFixed_time_policy(OptionDeclarationParser::Fixed_time_policyContext* ctx) override {
    policy_type = Interface::Module::Quarantine::QuarantinePolicy::QuarantinePolicyType::
      WaitFixedTimePolicy;
    visitChildren(ctx);
    return {};
  }

  std::any
  visitDirect_policy(OptionDeclarationParser::Direct_policyContext* ctx) override {
    policy_type = Interface::Module::Quarantine::QuarantinePolicy::QuarantinePolicyType::
      DirectPolicy;
    visitChildren(ctx);
    return {};
  }

  std::any visitHour_span(OptionDeclarationParser::Hour_spanContext* ctx) override {
    int64_t hours = std::stoll(ctx->integer()->getText());
    int64_t ns = static_cast<int64_t>(hours) * 60 * 60 * 1000 * 1000 * 1000;
    time_window = std::chrono::nanoseconds(ns);
    return {};
  }

  std::any visitMinute_span(OptionDeclarationParser::Minute_spanContext* ctx) override {
    int64_t minutes = std::stoll(ctx->integer()->getText());
    int64_t ns = static_cast<int64_t>(minutes) * 60 * 1000 * 1000 * 1000;
    time_window = std::chrono::nanoseconds(ns);
    return {};
  }

  std::any visitSecond_span(OptionDeclarationParser::Second_spanContext* ctx) override {
    int64_t seconds = std::stoll(ctx->integer()->getText());
    int64_t ns = static_cast<int64_t>(seconds) * 1000 * 1000 * 1000;
    time_window = std::chrono::nanoseconds(ns);
    return {};
  }

  std::any visitStream_name(OptionDeclarationParser::Stream_nameContext* ctx) override {
    streams.insert(ctx->getText());
    return {};
  }
};
}  // namespace CORE::Internal::Parsing::Option
