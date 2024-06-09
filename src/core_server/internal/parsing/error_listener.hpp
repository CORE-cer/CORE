#pragma once

#include "antlr4-runtime.h"
#include "shared/exceptions/parsing/parsing_syntax_exception.hpp"

namespace antlr4 {
class ParseErrorListener : public BaseErrorListener {
 public:
  void syntaxError(Recognizer* recognizer,
                   Token* offendingSymbol,
                   size_t line,
                   size_t charPositionInLine,
                   const std::string& msg,
                   std::exception_ptr e) override {
    std::string error_message = "Syntax error: ";
    throw CORE::ParsingSyntaxException(error_message, offendingSymbol->getText(), line);
  }
};
}  // namespace antlr4
