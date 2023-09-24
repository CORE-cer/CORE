
// Generated from CEQLQueryLexer.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"




class  CEQLQueryLexer : public antlr4::Lexer {
public:
  enum {
    K_ALL = 1, K_AND = 2, K_ANY = 3, K_AS = 4, K_BY = 5, K_CONSUME = 6, 
    K_DISTINCT = 7, K_EVENT = 8, K_EVENTS = 9, K_FILTER = 10, K_FROM = 11, 
    K_HOURS = 12, K_IN = 13, K_LAST = 14, K_LIKE = 15, K_MAX = 16, K_MINUTES = 17, 
    K_NEXT = 18, K_NONE = 19, K_NOT = 20, K_OR = 21, K_PARTITION = 22, K_SECONDS = 23, 
    K_SELECT = 24, K_STREAM = 25, K_STRICT = 26, K_UNLESS = 27, K_WHERE = 28, 
    K_WITHIN = 29, PERCENT = 30, PLUS = 31, MINUS = 32, STAR = 33, SLASH = 34, 
    LE = 35, LEQ = 36, GE = 37, GEQ = 38, EQ = 39, NEQ = 40, SEMICOLON = 41, 
    COLON = 42, COMMA = 43, DOUBLE_DOT = 44, LEFT_PARENTHESIS = 45, RIGHT_PARENTHESIS = 46, 
    LEFT_SQUARE_BRACKET = 47, RIGHT_SQUARE_BRACKET = 48, LEFT_CURLY_BRACKET = 49, 
    RIGHT_CURLY_BRACKET = 50, COLON_PLUS = 51, IDENTIFIER = 52, DOUBLE_LITERAL = 53, 
    INTEGER_LITERAL = 54, NUMERICAL_EXPONENT = 55, STRING_LITERAL = 56, 
    SINGLE_LINE_COMMENT = 57, MULTILINE_COMMENT = 58, SPACES = 59, UNEXPECTED_CHAR = 60, 
    REGEX_START = 61, REGEX_END = 62, REGEX_END_ESCAPED = 63, REGEX_PIPE = 64, 
    REGEX_EXCLAMAITON = 65, REGEX_L_CURLY = 66, REGEX_R_CURLY = 67, REGEX_L_PAR = 68, 
    REGEX_R_PAR = 69, REGEX_COMMA = 70, REGEX_QUESTION = 71, REGEX_PLUS = 72, 
    REGEX_STAR = 73, REGEX_HAT = 74, REGEX_HYPHEN = 75, REGEX_L_BRACK = 76, 
    REGEX_R_BRACK = 77, REGEX_BACKSLASH = 78, REGEX_ALPHA = 79, REGEX_DOT = 80, 
    REGEX_DOUBLED_DOT = 81, UNRECOGNIZED = 82, REGEX_DECIMAL_DIGIT = 83, 
    REGEX_NOT_DECIMAL_DIGIT = 84, REGEX_WHITESPACE = 85, REGEX_NOT_WHITESPACE = 86, 
    REGEX_ALPHANUMERIC = 87, REGEX_NOT_ALPHANUMERIC = 88, REGEX_DIGIT = 89
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

