
// Generated from CEQLQueryLexer.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"




class  CEQLQueryLexer : public antlr4::Lexer {
public:
  enum {
    K_ALL = 1, K_AND = 2, K_ANY = 3, K_AS = 4, K_BY = 5, K_CONSUME = 6, 
    K_DISTINCT = 7, K_EVENT = 8, K_EVENTS = 9, K_FILTER = 10, K_FROM = 11, 
    K_HOURS = 12, K_IN = 13, K_LAST = 14, K_LIKE = 15, K_MAX = 16, K_MINUTES = 17, 
    K_NEXT = 18, K_NONE = 19, K_NOT = 20, K_OR = 21, K_PARTITION = 22, K_RANGE = 23, 
    K_SECONDS = 24, K_SELECT = 25, K_STREAM = 26, K_STRICT = 27, K_UNLESS = 28, 
    K_WHERE = 29, K_WITHIN = 30, PERCENT = 31, PLUS = 32, MINUS = 33, STAR = 34, 
    SLASH = 35, LE = 36, LEQ = 37, GE = 38, GEQ = 39, EQ = 40, NEQ = 41, 
    SEMICOLON = 42, COMMA = 43, DOUBLE_DOT = 44, LEFT_PARENTHESIS = 45, 
    RIGHT_PARENTHESIS = 46, LEFT_SQUARE_BRACKET = 47, RIGHT_SQUARE_BRACKET = 48, 
    LEFT_CURLY_BRACKET = 49, RIGHT_CURLY_BRACKET = 50, IDENTIFIER = 51, 
    DOUBLE_LITERAL = 52, INTEGER_LITERAL = 53, NUMERICAL_EXPONENT = 54, 
    STRING_LITERAL = 55, SINGLE_LINE_COMMENT = 56, MULTILINE_COMMENT = 57, 
    SPACES = 58, UNEXPECTED_CHAR = 59, REGEX_START = 60, REGEX_END = 61, 
    REGEX_END_ESCAPED = 62, REGEX_PIPE = 63, REGEX_EXCLAMAITON = 64, REGEX_L_CURLY = 65, 
    REGEX_R_CURLY = 66, REGEX_L_PAR = 67, REGEX_R_PAR = 68, REGEX_COMMA = 69, 
    REGEX_QUESTION = 70, REGEX_PLUS = 71, REGEX_STAR = 72, REGEX_HAT = 73, 
    REGEX_HYPHEN = 74, REGEX_L_BRACK = 75, REGEX_R_BRACK = 76, REGEX_BACKSLASH = 77, 
    REGEX_ALPHA = 78, REGEX_DOT = 79, REGEX_DOUBLED_DOT = 80, UNRECOGNIZED = 81, 
    REGEX_DECIMAL_DIGIT = 82, REGEX_NOT_DECIMAL_DIGIT = 83, REGEX_WHITESPACE = 84, 
    REGEX_NOT_WHITESPACE = 85, REGEX_ALPHANUMERIC = 86, REGEX_NOT_ALPHANUMERIC = 87, 
    REGEX_DIGIT = 88
  };

  enum {
    REGEX = 1
  };

  explicit CEQLQueryLexer(antlr4::CharStream *input);

  ~CEQLQueryLexer() override;


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

