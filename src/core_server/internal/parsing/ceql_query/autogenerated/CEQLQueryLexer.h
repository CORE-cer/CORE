
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
    SEMICOLON = 42, COLON = 43, COMMA = 44, DOUBLE_DOT = 45, LEFT_PARENTHESIS = 46, 
    RIGHT_PARENTHESIS = 47, LEFT_SQUARE_BRACKET = 48, RIGHT_SQUARE_BRACKET = 49, 
    LEFT_CURLY_BRACKET = 50, RIGHT_CURLY_BRACKET = 51, COLON_PLUS = 52, 
    IDENTIFIER = 53, DOUBLE_LITERAL = 54, INTEGER_LITERAL = 55, NUMERICAL_EXPONENT = 56, 
    STRING_LITERAL = 57, SINGLE_LINE_COMMENT = 58, MULTILINE_COMMENT = 59, 
    SPACES = 60, UNEXPECTED_CHAR = 61, REGEX_START = 62, REGEX_END = 63, 
    REGEX_END_ESCAPED = 64, REGEX_PIPE = 65, REGEX_EXCLAMAITON = 66, REGEX_L_CURLY = 67, 
    REGEX_R_CURLY = 68, REGEX_L_PAR = 69, REGEX_R_PAR = 70, REGEX_COMMA = 71, 
    REGEX_QUESTION = 72, REGEX_PLUS = 73, REGEX_STAR = 74, REGEX_HAT = 75, 
    REGEX_HYPHEN = 76, REGEX_L_BRACK = 77, REGEX_R_BRACK = 78, REGEX_BACKSLASH = 79, 
    REGEX_ALPHA = 80, REGEX_DOT = 81, REGEX_DOUBLED_DOT = 82, UNRECOGNIZED = 83, 
    REGEX_DECIMAL_DIGIT = 84, REGEX_NOT_DECIMAL_DIGIT = 85, REGEX_WHITESPACE = 86, 
    REGEX_NOT_WHITESPACE = 87, REGEX_ALPHANUMERIC = 88, REGEX_NOT_ALPHANUMERIC = 89, 
    REGEX_DIGIT = 90
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

