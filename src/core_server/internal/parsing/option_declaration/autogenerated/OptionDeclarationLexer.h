
// Generated from OptionDeclarationLexer.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"




class  OptionDeclarationLexer : public antlr4::Lexer {
public:
  enum {
    K_DECLARE = 1, K_QUARANTINE = 2, K_FIXED_TIME = 3, K_HOURS = 4, K_MINUTES = 5, 
    K_SECONDS = 6, LEFT_CURLY_BRACKET = 7, RIGHT_CURLY_BRACKET = 8, DOUBLE_LITERAL = 9, 
    INTEGER_LITERAL = 10, NUMERICAL_EXPONENT = 11, IDENTIFIER = 12, UNEXPECTED_CHAR = 13
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

