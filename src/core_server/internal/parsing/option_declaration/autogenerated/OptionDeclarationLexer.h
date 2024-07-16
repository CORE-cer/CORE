
// Generated from OptionDeclarationLexer.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"




class  OptionDeclarationLexer : public antlr4::Lexer {
public:
  enum {
    K_DECLARE = 1, K_QUARANTINE = 2, K_FIXED_TIME = 3, K_DIRECT = 4, K_HOURS = 5, 
    K_MINUTES = 6, K_SECONDS = 7, LEFT_CURLY_BRACKET = 8, RIGHT_CURLY_BRACKET = 9, 
    COMMA = 10, DOUBLE_LITERAL = 11, INTEGER_LITERAL = 12, NUMERICAL_EXPONENT = 13, 
    IDENTIFIER = 14, UNEXPECTED_CHAR = 15
  };

  explicit OptionDeclarationLexer(antlr4::CharStream *input);

  ~OptionDeclarationLexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};

