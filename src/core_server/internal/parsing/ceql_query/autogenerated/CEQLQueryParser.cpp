
// Generated from CEQLQueryParser.g4 by ANTLR 4.12.0


#include "CEQLQueryParserVisitor.h"

#include "CEQLQueryParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct CEQLQueryParserStaticData final {
  CEQLQueryParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  CEQLQueryParserStaticData(const CEQLQueryParserStaticData&) = delete;
  CEQLQueryParserStaticData(CEQLQueryParserStaticData&&) = delete;
  CEQLQueryParserStaticData& operator=(const CEQLQueryParserStaticData&) = delete;
  CEQLQueryParserStaticData& operator=(CEQLQueryParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag ceqlqueryparserParserOnceFlag;
CEQLQueryParserStaticData *ceqlqueryparserParserStaticData = nullptr;

void ceqlqueryparserParserInitialize() {
  assert(ceqlqueryparserParserStaticData == nullptr);
  auto staticData = std::make_unique<CEQLQueryParserStaticData>(
    std::vector<std::string>{
      "parse", "error", "core_query", "selection_strategy", "list_of_variables", 
      "from_clause", "cel_formula", "partition_list", "attribute_list", 
      "consumption_policy", "filter", "predicate", "string_literal", "string_literal_or_regexp", 
      "math_expr", "value_seq", "number_seq", "string_seq", "time_window", 
      "event_span", "time_span", "hour_span", "minute_span", "second_span", 
      "custom_span", "named_event", "s_event_name", "event_name", "stream_name", 
      "attribute_name", "integer", "double", "number", "string", "any_name", 
      "keyword", "regexp", "regexp_alternation", "regexp_exp", "regexp_element", 
      "regexp_group", "parenthesis", "quantifier", "quantity", "quantExact", 
      "quantRange", "quantMin", "quantMax", "atom", "characterClass", "ccAtom", 
      "ccRange", "ccSingle", "ccLiteral", "ccEscapes", "ccOther", "literal", 
      "escapes", "other", "sharedAtom", "regexp_number"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "'%'", "", "", 
      "", "'/'", "'<'", "'<='", "'>'", "'>='", "", "", "';'", "':'", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "'<<'", 
      "'>>'", "'\\>'", "'|'", "'!'", "", "", "", "", "", "'\\u003F'", "", 
      "", "'^'", "", "", "", "'\\'", "", "'.'", "", "", "'\\d'", "'\\D'", 
      "'\\s'", "'\\S'", "'\\w'", "'\\W'"
    },
    std::vector<std::string>{
      "", "K_ALL", "K_AND", "K_ANY", "K_AS", "K_BY", "K_CONSUME", "K_DISTINCT", 
      "K_EVENT", "K_EVENTS", "K_FILTER", "K_FROM", "K_HOURS", "K_IN", "K_LAST", 
      "K_LIKE", "K_MAX", "K_MINUTES", "K_NEXT", "K_NONE", "K_NOT", "K_OR", 
      "K_PARTITION", "K_SECONDS", "K_SELECT", "K_STREAM", "K_STRICT", "K_UNLESS", 
      "K_WHERE", "K_WITHIN", "PERCENT", "PLUS", "MINUS", "STAR", "SLASH", 
      "LE", "LEQ", "GE", "GEQ", "EQ", "NEQ", "SEMICOLON", "COLON", "COMMA", 
      "DOUBLE_DOT", "LEFT_PARENTHESIS", "RIGHT_PARENTHESIS", "LEFT_SQUARE_BRACKET", 
      "RIGHT_SQUARE_BRACKET", "LEFT_CURLY_BRACKET", "RIGHT_CURLY_BRACKET", 
      "IDENTIFIER", "DOUBLE_LITERAL", "INTEGER_LITERAL", "NUMERICAL_EXPONENT", 
      "STRING_LITERAL", "SINGLE_LINE_COMMENT", "MULTILINE_COMMENT", "SPACES", 
      "UNEXPECTED_CHAR", "REGEX_START", "REGEX_END", "REGEX_END_ESCAPED", 
      "REGEX_PIPE", "REGEX_EXCLAMAITON", "REGEX_L_CURLY", "REGEX_R_CURLY", 
      "REGEX_L_PAR", "REGEX_R_PAR", "REGEX_COMMA", "REGEX_QUESTION", "REGEX_PLUS", 
      "REGEX_STAR", "REGEX_HAT", "REGEX_HYPHEN", "REGEX_L_BRACK", "REGEX_R_BRACK", 
      "REGEX_BACKSLASH", "REGEX_ALPHA", "REGEX_DOT", "REGEX_DOUBLED_DOT", 
      "UNRECOGNIZED", "REGEX_DECIMAL_DIGIT", "REGEX_NOT_DECIMAL_DIGIT", 
      "REGEX_WHITESPACE", "REGEX_NOT_WHITESPACE", "REGEX_ALPHANUMERIC", 
      "REGEX_NOT_ALPHANUMERIC", "REGEX_DIGIT"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,88,547,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,7,
  	35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,2,40,7,40,2,41,7,41,2,42,7,
  	42,2,43,7,43,2,44,7,44,2,45,7,45,2,46,7,46,2,47,7,47,2,48,7,48,2,49,7,
  	49,2,50,7,50,2,51,7,51,2,52,7,52,2,53,7,53,2,54,7,54,2,55,7,55,2,56,7,
  	56,2,57,7,57,2,58,7,58,2,59,7,59,2,60,7,60,1,0,1,0,5,0,125,8,0,10,0,12,
  	0,128,9,0,1,0,1,0,1,1,1,1,1,1,1,2,1,2,3,2,137,8,2,1,2,1,2,1,2,1,2,1,2,
  	1,2,1,2,3,2,146,8,2,1,2,1,2,3,2,150,8,2,1,2,1,2,1,2,3,2,155,8,2,1,3,1,
  	3,1,3,1,3,1,3,1,3,3,3,163,8,3,1,4,1,4,1,4,1,4,5,4,169,8,4,10,4,12,4,172,
  	9,4,3,4,174,8,4,1,5,1,5,1,5,1,5,5,5,180,8,5,10,5,12,5,183,9,5,3,5,185,
  	8,5,1,6,1,6,1,6,1,6,1,6,1,6,3,6,193,8,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,
  	6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,5,6,212,8,6,10,6,12,6,215,9,6,1,
  	7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,3,7,225,8,7,1,8,1,8,1,8,5,8,230,8,8,10,
  	8,12,8,233,9,8,1,9,1,9,1,9,3,9,238,8,9,1,10,1,10,1,10,1,10,1,10,1,10,
  	1,10,1,10,1,10,1,10,3,10,250,8,10,1,10,1,10,1,10,1,10,1,10,1,10,5,10,
  	258,8,10,10,10,12,10,261,9,10,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,
  	1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,
  	1,11,3,11,286,8,11,1,11,1,11,3,11,290,8,11,1,11,1,11,1,11,1,11,1,11,1,
  	11,5,11,298,8,11,10,11,12,11,301,9,11,1,12,1,12,3,12,305,8,12,1,13,1,
  	13,1,13,3,13,310,8,13,1,14,1,14,1,14,1,14,1,14,1,14,1,14,1,14,1,14,3,
  	14,321,8,14,1,14,1,14,1,14,1,14,1,14,1,14,5,14,329,8,14,10,14,12,14,332,
  	9,14,1,15,1,15,1,15,1,15,1,15,1,15,1,15,1,15,3,15,342,8,15,1,16,1,16,
  	1,16,5,16,347,8,16,10,16,12,16,350,9,16,1,16,1,16,1,16,1,16,1,16,1,16,
  	1,16,1,16,1,16,1,16,1,16,1,16,1,16,3,16,365,8,16,1,17,1,17,1,17,5,17,
  	370,8,17,10,17,12,17,373,9,17,1,18,1,18,1,18,3,18,378,8,18,1,19,1,19,
  	1,19,1,20,3,20,384,8,20,1,20,3,20,387,8,20,1,20,3,20,390,8,20,1,21,1,
  	21,1,21,1,22,1,22,1,22,1,23,1,23,1,23,1,24,1,24,1,24,1,24,1,24,1,25,1,
  	25,1,25,3,25,409,8,25,1,26,1,26,1,26,3,26,414,8,26,1,26,1,26,1,27,1,27,
  	1,28,1,28,1,29,1,29,1,30,1,30,1,31,1,31,1,32,1,32,3,32,430,8,32,1,33,
  	1,33,1,34,1,34,1,35,1,35,1,36,1,36,1,36,1,36,1,37,1,37,1,37,5,37,445,
  	8,37,10,37,12,37,448,9,37,1,38,4,38,451,8,38,11,38,12,38,452,1,39,1,39,
  	3,39,457,8,39,1,40,1,40,3,40,461,8,40,1,41,1,41,1,41,1,41,1,42,1,42,1,
  	42,1,42,1,42,1,42,1,42,3,42,474,8,42,1,43,1,43,1,43,1,43,3,43,480,8,43,
  	1,44,1,44,1,45,1,45,1,45,1,45,1,46,1,46,1,46,1,47,1,47,1,47,1,48,1,48,
  	1,48,3,48,497,8,48,1,49,1,49,3,49,501,8,49,1,49,4,49,504,8,49,11,49,12,
  	49,505,1,49,1,49,1,50,1,50,1,50,3,50,513,8,50,1,51,1,51,1,51,1,51,1,52,
  	1,52,1,53,1,53,3,53,523,8,53,1,54,1,54,1,54,1,55,1,55,1,56,1,56,1,56,
  	3,56,533,8,56,1,57,1,57,1,57,1,58,1,58,1,59,1,59,1,60,4,60,543,8,60,11,
  	60,12,60,544,1,60,0,4,12,20,22,28,61,0,2,4,6,8,10,12,14,16,18,20,22,24,
  	26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,
  	72,74,76,78,80,82,84,86,88,90,92,94,96,98,100,102,104,106,108,110,112,
  	114,116,118,120,0,9,1,0,35,40,1,0,39,40,1,0,31,32,2,0,30,30,33,34,1,0,
  	1,29,2,0,73,74,76,77,5,0,63,63,65,68,70,72,75,77,79,79,4,0,63,63,65,68,
  	70,72,75,77,1,0,82,87,565,0,126,1,0,0,0,2,131,1,0,0,0,4,134,1,0,0,0,6,
  	162,1,0,0,0,8,173,1,0,0,0,10,184,1,0,0,0,12,192,1,0,0,0,14,216,1,0,0,
  	0,16,226,1,0,0,0,18,237,1,0,0,0,20,249,1,0,0,0,22,289,1,0,0,0,24,304,
  	1,0,0,0,26,309,1,0,0,0,28,320,1,0,0,0,30,341,1,0,0,0,32,364,1,0,0,0,34,
  	366,1,0,0,0,36,377,1,0,0,0,38,379,1,0,0,0,40,383,1,0,0,0,42,391,1,0,0,
  	0,44,394,1,0,0,0,46,397,1,0,0,0,48,400,1,0,0,0,50,405,1,0,0,0,52,413,
  	1,0,0,0,54,417,1,0,0,0,56,419,1,0,0,0,58,421,1,0,0,0,60,423,1,0,0,0,62,
  	425,1,0,0,0,64,429,1,0,0,0,66,431,1,0,0,0,68,433,1,0,0,0,70,435,1,0,0,
  	0,72,437,1,0,0,0,74,441,1,0,0,0,76,450,1,0,0,0,78,454,1,0,0,0,80,460,
  	1,0,0,0,82,462,1,0,0,0,84,473,1,0,0,0,86,479,1,0,0,0,88,481,1,0,0,0,90,
  	483,1,0,0,0,92,487,1,0,0,0,94,490,1,0,0,0,96,496,1,0,0,0,98,498,1,0,0,
  	0,100,512,1,0,0,0,102,514,1,0,0,0,104,518,1,0,0,0,106,522,1,0,0,0,108,
  	524,1,0,0,0,110,527,1,0,0,0,112,532,1,0,0,0,114,534,1,0,0,0,116,537,1,
  	0,0,0,118,539,1,0,0,0,120,542,1,0,0,0,122,125,3,4,2,0,123,125,3,2,1,0,
  	124,122,1,0,0,0,124,123,1,0,0,0,125,128,1,0,0,0,126,124,1,0,0,0,126,127,
  	1,0,0,0,127,129,1,0,0,0,128,126,1,0,0,0,129,130,5,0,0,1,130,1,1,0,0,0,
  	131,132,5,59,0,0,132,133,6,1,-1,0,133,3,1,0,0,0,134,136,5,24,0,0,135,
  	137,3,6,3,0,136,135,1,0,0,0,136,137,1,0,0,0,137,138,1,0,0,0,138,139,3,
  	8,4,0,139,140,3,10,5,0,140,141,5,28,0,0,141,145,3,12,6,0,142,143,5,22,
  	0,0,143,144,5,5,0,0,144,146,3,14,7,0,145,142,1,0,0,0,145,146,1,0,0,0,
  	146,149,1,0,0,0,147,148,5,29,0,0,148,150,3,36,18,0,149,147,1,0,0,0,149,
  	150,1,0,0,0,150,154,1,0,0,0,151,152,5,6,0,0,152,153,5,5,0,0,153,155,3,
  	18,9,0,154,151,1,0,0,0,154,155,1,0,0,0,155,5,1,0,0,0,156,163,5,1,0,0,
  	157,163,5,3,0,0,158,163,5,14,0,0,159,163,5,16,0,0,160,163,5,18,0,0,161,
  	163,5,26,0,0,162,156,1,0,0,0,162,157,1,0,0,0,162,158,1,0,0,0,162,159,
  	1,0,0,0,162,160,1,0,0,0,162,161,1,0,0,0,163,7,1,0,0,0,164,174,5,33,0,
  	0,165,170,3,68,34,0,166,167,5,43,0,0,167,169,3,68,34,0,168,166,1,0,0,
  	0,169,172,1,0,0,0,170,168,1,0,0,0,170,171,1,0,0,0,171,174,1,0,0,0,172,
  	170,1,0,0,0,173,164,1,0,0,0,173,165,1,0,0,0,174,9,1,0,0,0,175,176,5,11,
  	0,0,176,181,3,56,28,0,177,178,5,43,0,0,178,180,3,56,28,0,179,177,1,0,
  	0,0,180,183,1,0,0,0,181,179,1,0,0,0,181,182,1,0,0,0,182,185,1,0,0,0,183,
  	181,1,0,0,0,184,175,1,0,0,0,184,185,1,0,0,0,185,11,1,0,0,0,186,187,6,
  	6,-1,0,187,188,5,45,0,0,188,189,3,12,6,0,189,190,5,46,0,0,190,193,1,0,
  	0,0,191,193,3,52,26,0,192,186,1,0,0,0,192,191,1,0,0,0,193,213,1,0,0,0,
  	194,195,10,4,0,0,195,196,5,41,0,0,196,212,3,12,6,5,197,198,10,3,0,0,198,
  	199,5,42,0,0,199,212,3,12,6,4,200,201,10,2,0,0,201,202,5,21,0,0,202,212,
  	3,12,6,3,203,204,10,6,0,0,204,205,5,4,0,0,205,212,3,54,27,0,206,207,10,
  	5,0,0,207,212,5,31,0,0,208,209,10,1,0,0,209,210,5,10,0,0,210,212,3,20,
  	10,0,211,194,1,0,0,0,211,197,1,0,0,0,211,200,1,0,0,0,211,203,1,0,0,0,
  	211,206,1,0,0,0,211,208,1,0,0,0,212,215,1,0,0,0,213,211,1,0,0,0,213,214,
  	1,0,0,0,214,13,1,0,0,0,215,213,1,0,0,0,216,217,5,47,0,0,217,218,3,16,
  	8,0,218,224,5,48,0,0,219,220,5,43,0,0,220,221,5,47,0,0,221,222,3,16,8,
  	0,222,223,5,48,0,0,223,225,1,0,0,0,224,219,1,0,0,0,224,225,1,0,0,0,225,
  	15,1,0,0,0,226,231,3,58,29,0,227,228,5,43,0,0,228,230,3,58,29,0,229,227,
  	1,0,0,0,230,233,1,0,0,0,231,229,1,0,0,0,231,232,1,0,0,0,232,17,1,0,0,
  	0,233,231,1,0,0,0,234,238,5,3,0,0,235,238,5,22,0,0,236,238,5,19,0,0,237,
  	234,1,0,0,0,237,235,1,0,0,0,237,236,1,0,0,0,238,19,1,0,0,0,239,240,6,
  	10,-1,0,240,241,5,45,0,0,241,242,3,20,10,0,242,243,5,46,0,0,243,250,1,
  	0,0,0,244,245,3,54,27,0,245,246,5,47,0,0,246,247,3,22,11,0,247,248,5,
  	48,0,0,248,250,1,0,0,0,249,239,1,0,0,0,249,244,1,0,0,0,250,259,1,0,0,
  	0,251,252,10,2,0,0,252,253,5,2,0,0,253,258,3,20,10,3,254,255,10,1,0,0,
  	255,256,5,21,0,0,256,258,3,20,10,2,257,251,1,0,0,0,257,254,1,0,0,0,258,
  	261,1,0,0,0,259,257,1,0,0,0,259,260,1,0,0,0,260,21,1,0,0,0,261,259,1,
  	0,0,0,262,263,6,11,-1,0,263,264,5,45,0,0,264,265,3,22,11,0,265,266,5,
  	46,0,0,266,290,1,0,0,0,267,268,5,20,0,0,268,290,3,22,11,7,269,270,3,28,
  	14,0,270,271,7,0,0,0,271,272,3,28,14,0,272,290,1,0,0,0,273,274,3,24,12,
  	0,274,275,7,1,0,0,275,276,3,26,13,0,276,290,1,0,0,0,277,278,3,58,29,0,
  	278,279,5,15,0,0,279,280,3,72,36,0,280,290,1,0,0,0,281,285,3,58,29,0,
  	282,286,5,13,0,0,283,284,5,20,0,0,284,286,5,13,0,0,285,282,1,0,0,0,285,
  	283,1,0,0,0,286,287,1,0,0,0,287,288,3,30,15,0,288,290,1,0,0,0,289,262,
  	1,0,0,0,289,267,1,0,0,0,289,269,1,0,0,0,289,273,1,0,0,0,289,277,1,0,0,
  	0,289,281,1,0,0,0,290,299,1,0,0,0,291,292,10,4,0,0,292,293,5,2,0,0,293,
  	298,3,22,11,5,294,295,10,3,0,0,295,296,5,21,0,0,296,298,3,22,11,4,297,
  	291,1,0,0,0,297,294,1,0,0,0,298,301,1,0,0,0,299,297,1,0,0,0,299,300,1,
  	0,0,0,300,23,1,0,0,0,301,299,1,0,0,0,302,305,3,66,33,0,303,305,3,58,29,
  	0,304,302,1,0,0,0,304,303,1,0,0,0,305,25,1,0,0,0,306,310,3,66,33,0,307,
  	310,3,58,29,0,308,310,3,72,36,0,309,306,1,0,0,0,309,307,1,0,0,0,309,308,
  	1,0,0,0,310,27,1,0,0,0,311,312,6,14,-1,0,312,313,5,45,0,0,313,314,3,28,
  	14,0,314,315,5,46,0,0,315,321,1,0,0,0,316,321,3,64,32,0,317,321,3,58,
  	29,0,318,319,7,2,0,0,319,321,3,28,14,3,320,311,1,0,0,0,320,316,1,0,0,
  	0,320,317,1,0,0,0,320,318,1,0,0,0,321,330,1,0,0,0,322,323,10,2,0,0,323,
  	324,7,3,0,0,324,329,3,28,14,3,325,326,10,1,0,0,326,327,7,2,0,0,327,329,
  	3,28,14,2,328,322,1,0,0,0,328,325,1,0,0,0,329,332,1,0,0,0,330,328,1,0,
  	0,0,330,331,1,0,0,0,331,29,1,0,0,0,332,330,1,0,0,0,333,334,5,49,0,0,334,
  	335,3,32,16,0,335,336,5,50,0,0,336,342,1,0,0,0,337,338,5,49,0,0,338,339,
  	3,34,17,0,339,340,5,50,0,0,340,342,1,0,0,0,341,333,1,0,0,0,341,337,1,
  	0,0,0,342,31,1,0,0,0,343,348,3,64,32,0,344,345,5,43,0,0,345,347,3,64,
  	32,0,346,344,1,0,0,0,347,350,1,0,0,0,348,346,1,0,0,0,348,349,1,0,0,0,
  	349,365,1,0,0,0,350,348,1,0,0,0,351,352,3,60,30,0,352,353,5,44,0,0,353,
  	354,3,60,30,0,354,365,1,0,0,0,355,356,3,62,31,0,356,357,5,44,0,0,357,
  	358,3,62,31,0,358,365,1,0,0,0,359,360,3,64,32,0,360,361,5,44,0,0,361,
  	365,1,0,0,0,362,363,5,44,0,0,363,365,3,64,32,0,364,343,1,0,0,0,364,351,
  	1,0,0,0,364,355,1,0,0,0,364,359,1,0,0,0,364,362,1,0,0,0,365,33,1,0,0,
  	0,366,371,3,66,33,0,367,368,5,43,0,0,368,370,3,66,33,0,369,367,1,0,0,
  	0,370,373,1,0,0,0,371,369,1,0,0,0,371,372,1,0,0,0,372,35,1,0,0,0,373,
  	371,1,0,0,0,374,378,3,38,19,0,375,378,3,40,20,0,376,378,3,48,24,0,377,
  	374,1,0,0,0,377,375,1,0,0,0,377,376,1,0,0,0,378,37,1,0,0,0,379,380,3,
  	60,30,0,380,381,5,9,0,0,381,39,1,0,0,0,382,384,3,42,21,0,383,382,1,0,
  	0,0,383,384,1,0,0,0,384,386,1,0,0,0,385,387,3,44,22,0,386,385,1,0,0,0,
  	386,387,1,0,0,0,387,389,1,0,0,0,388,390,3,46,23,0,389,388,1,0,0,0,389,
  	390,1,0,0,0,390,41,1,0,0,0,391,392,3,60,30,0,392,393,5,12,0,0,393,43,
  	1,0,0,0,394,395,3,60,30,0,395,396,5,17,0,0,396,45,1,0,0,0,397,398,3,60,
  	30,0,398,399,5,23,0,0,399,47,1,0,0,0,400,401,3,60,30,0,401,402,5,47,0,
  	0,402,403,3,68,34,0,403,404,5,48,0,0,404,49,1,0,0,0,405,408,3,52,26,0,
  	406,407,5,4,0,0,407,409,3,54,27,0,408,406,1,0,0,0,408,409,1,0,0,0,409,
  	51,1,0,0,0,410,411,3,56,28,0,411,412,5,37,0,0,412,414,1,0,0,0,413,410,
  	1,0,0,0,413,414,1,0,0,0,414,415,1,0,0,0,415,416,3,54,27,0,416,53,1,0,
  	0,0,417,418,3,68,34,0,418,55,1,0,0,0,419,420,3,68,34,0,420,57,1,0,0,0,
  	421,422,3,68,34,0,422,59,1,0,0,0,423,424,5,53,0,0,424,61,1,0,0,0,425,
  	426,5,52,0,0,426,63,1,0,0,0,427,430,3,60,30,0,428,430,3,62,31,0,429,427,
  	1,0,0,0,429,428,1,0,0,0,430,65,1,0,0,0,431,432,5,55,0,0,432,67,1,0,0,
  	0,433,434,5,51,0,0,434,69,1,0,0,0,435,436,7,4,0,0,436,71,1,0,0,0,437,
  	438,5,60,0,0,438,439,3,74,37,0,439,440,5,61,0,0,440,73,1,0,0,0,441,446,
  	3,76,38,0,442,443,5,63,0,0,443,445,3,76,38,0,444,442,1,0,0,0,445,448,
  	1,0,0,0,446,444,1,0,0,0,446,447,1,0,0,0,447,75,1,0,0,0,448,446,1,0,0,
  	0,449,451,3,78,39,0,450,449,1,0,0,0,451,452,1,0,0,0,452,450,1,0,0,0,452,
  	453,1,0,0,0,453,77,1,0,0,0,454,456,3,80,40,0,455,457,3,84,42,0,456,455,
  	1,0,0,0,456,457,1,0,0,0,457,79,1,0,0,0,458,461,3,82,41,0,459,461,3,96,
  	48,0,460,458,1,0,0,0,460,459,1,0,0,0,461,81,1,0,0,0,462,463,5,67,0,0,
  	463,464,3,74,37,0,464,465,5,68,0,0,465,83,1,0,0,0,466,474,5,70,0,0,467,
  	474,5,71,0,0,468,474,5,72,0,0,469,470,5,65,0,0,470,471,3,86,43,0,471,
  	472,5,66,0,0,472,474,1,0,0,0,473,466,1,0,0,0,473,467,1,0,0,0,473,468,
  	1,0,0,0,473,469,1,0,0,0,474,85,1,0,0,0,475,480,3,88,44,0,476,480,3,90,
  	45,0,477,480,3,92,46,0,478,480,3,94,47,0,479,475,1,0,0,0,479,476,1,0,
  	0,0,479,477,1,0,0,0,479,478,1,0,0,0,480,87,1,0,0,0,481,482,3,120,60,0,
  	482,89,1,0,0,0,483,484,3,120,60,0,484,485,5,69,0,0,485,486,3,120,60,0,
  	486,91,1,0,0,0,487,488,3,120,60,0,488,489,5,69,0,0,489,93,1,0,0,0,490,
  	491,5,69,0,0,491,492,3,120,60,0,492,95,1,0,0,0,493,497,3,98,49,0,494,
  	497,3,118,59,0,495,497,3,112,56,0,496,493,1,0,0,0,496,494,1,0,0,0,496,
  	495,1,0,0,0,497,97,1,0,0,0,498,500,5,75,0,0,499,501,5,73,0,0,500,499,
  	1,0,0,0,500,501,1,0,0,0,501,503,1,0,0,0,502,504,3,100,50,0,503,502,1,
  	0,0,0,504,505,1,0,0,0,505,503,1,0,0,0,505,506,1,0,0,0,506,507,1,0,0,0,
  	507,508,5,76,0,0,508,99,1,0,0,0,509,513,3,102,51,0,510,513,3,118,59,0,
  	511,513,3,104,52,0,512,509,1,0,0,0,512,510,1,0,0,0,512,511,1,0,0,0,513,
  	101,1,0,0,0,514,515,3,106,53,0,515,516,5,74,0,0,516,517,3,106,53,0,517,
  	103,1,0,0,0,518,519,3,106,53,0,519,105,1,0,0,0,520,523,3,108,54,0,521,
  	523,3,110,55,0,522,520,1,0,0,0,522,521,1,0,0,0,523,107,1,0,0,0,524,525,
  	5,77,0,0,525,526,7,5,0,0,526,109,1,0,0,0,527,528,8,5,0,0,528,111,1,0,
  	0,0,529,533,3,114,57,0,530,533,5,79,0,0,531,533,3,116,58,0,532,529,1,
  	0,0,0,532,530,1,0,0,0,532,531,1,0,0,0,533,113,1,0,0,0,534,535,5,77,0,
  	0,535,536,7,6,0,0,536,115,1,0,0,0,537,538,8,7,0,0,538,117,1,0,0,0,539,
  	540,7,8,0,0,540,119,1,0,0,0,541,543,5,88,0,0,542,541,1,0,0,0,543,544,
  	1,0,0,0,544,542,1,0,0,0,544,545,1,0,0,0,545,121,1,0,0,0,53,124,126,136,
  	145,149,154,162,170,173,181,184,192,211,213,224,231,237,249,257,259,285,
  	289,297,299,304,309,320,328,330,341,348,364,371,377,383,386,389,408,413,
  	429,446,452,456,460,473,479,496,500,505,512,522,532,544
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  ceqlqueryparserParserStaticData = staticData.release();
}

}

CEQLQueryParser::CEQLQueryParser(TokenStream *input) : CEQLQueryParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

CEQLQueryParser::CEQLQueryParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  CEQLQueryParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *ceqlqueryparserParserStaticData->atn, ceqlqueryparserParserStaticData->decisionToDFA, ceqlqueryparserParserStaticData->sharedContextCache, options);
}

CEQLQueryParser::~CEQLQueryParser() {
  delete _interpreter;
}

const atn::ATN& CEQLQueryParser::getATN() const {
  return *ceqlqueryparserParserStaticData->atn;
}

std::string CEQLQueryParser::getGrammarFileName() const {
  return "CEQLQueryParser.g4";
}

const std::vector<std::string>& CEQLQueryParser::getRuleNames() const {
  return ceqlqueryparserParserStaticData->ruleNames;
}

const dfa::Vocabulary& CEQLQueryParser::getVocabulary() const {
  return ceqlqueryparserParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView CEQLQueryParser::getSerializedATN() const {
  return ceqlqueryparserParserStaticData->serializedATN;
}


//----------------- ParseContext ------------------------------------------------------------------

CEQLQueryParser::ParseContext::ParseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CEQLQueryParser::ParseContext::EOF() {
  return getToken(CEQLQueryParser::EOF, 0);
}

std::vector<CEQLQueryParser::Core_queryContext *> CEQLQueryParser::ParseContext::core_query() {
  return getRuleContexts<CEQLQueryParser::Core_queryContext>();
}

CEQLQueryParser::Core_queryContext* CEQLQueryParser::ParseContext::core_query(size_t i) {
  return getRuleContext<CEQLQueryParser::Core_queryContext>(i);
}

std::vector<CEQLQueryParser::ErrorContext *> CEQLQueryParser::ParseContext::error() {
  return getRuleContexts<CEQLQueryParser::ErrorContext>();
}

CEQLQueryParser::ErrorContext* CEQLQueryParser::ParseContext::error(size_t i) {
  return getRuleContext<CEQLQueryParser::ErrorContext>(i);
}


size_t CEQLQueryParser::ParseContext::getRuleIndex() const {
  return CEQLQueryParser::RuleParse;
}


std::any CEQLQueryParser::ParseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitParse(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::ParseContext* CEQLQueryParser::parse() {
  ParseContext *_localctx = _tracker.createInstance<ParseContext>(_ctx, getState());
  enterRule(_localctx, 0, CEQLQueryParser::RuleParse);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(126);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CEQLQueryParser::K_SELECT

    || _la == CEQLQueryParser::UNEXPECTED_CHAR) {
      setState(124);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case CEQLQueryParser::K_SELECT: {
          setState(122);
          core_query();
          break;
        }

        case CEQLQueryParser::UNEXPECTED_CHAR: {
          setState(123);
          error();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(128);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(129);
    match(CEQLQueryParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ErrorContext ------------------------------------------------------------------

CEQLQueryParser::ErrorContext::ErrorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CEQLQueryParser::ErrorContext::UNEXPECTED_CHAR() {
  return getToken(CEQLQueryParser::UNEXPECTED_CHAR, 0);
}


size_t CEQLQueryParser::ErrorContext::getRuleIndex() const {
  return CEQLQueryParser::RuleError;
}


std::any CEQLQueryParser::ErrorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitError(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::ErrorContext* CEQLQueryParser::error() {
  ErrorContext *_localctx = _tracker.createInstance<ErrorContext>(_ctx, getState());
  enterRule(_localctx, 2, CEQLQueryParser::RuleError);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(131);
    antlrcpp::downCast<ErrorContext *>(_localctx)->unexpected_charToken = match(CEQLQueryParser::UNEXPECTED_CHAR);

         throw new RuntimeException("UNEXPECTED_CHAR=" + (antlrcpp::downCast<ErrorContext *>(_localctx)->unexpected_charToken != nullptr ? antlrcpp::downCast<ErrorContext *>(_localctx)->unexpected_charToken->getText() : ""));
       
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Core_queryContext ------------------------------------------------------------------

CEQLQueryParser::Core_queryContext::Core_queryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CEQLQueryParser::Core_queryContext::K_SELECT() {
  return getToken(CEQLQueryParser::K_SELECT, 0);
}

CEQLQueryParser::List_of_variablesContext* CEQLQueryParser::Core_queryContext::list_of_variables() {
  return getRuleContext<CEQLQueryParser::List_of_variablesContext>(0);
}

CEQLQueryParser::From_clauseContext* CEQLQueryParser::Core_queryContext::from_clause() {
  return getRuleContext<CEQLQueryParser::From_clauseContext>(0);
}

tree::TerminalNode* CEQLQueryParser::Core_queryContext::K_WHERE() {
  return getToken(CEQLQueryParser::K_WHERE, 0);
}

CEQLQueryParser::Cel_formulaContext* CEQLQueryParser::Core_queryContext::cel_formula() {
  return getRuleContext<CEQLQueryParser::Cel_formulaContext>(0);
}

CEQLQueryParser::Selection_strategyContext* CEQLQueryParser::Core_queryContext::selection_strategy() {
  return getRuleContext<CEQLQueryParser::Selection_strategyContext>(0);
}

tree::TerminalNode* CEQLQueryParser::Core_queryContext::K_PARTITION() {
  return getToken(CEQLQueryParser::K_PARTITION, 0);
}

std::vector<tree::TerminalNode *> CEQLQueryParser::Core_queryContext::K_BY() {
  return getTokens(CEQLQueryParser::K_BY);
}

tree::TerminalNode* CEQLQueryParser::Core_queryContext::K_BY(size_t i) {
  return getToken(CEQLQueryParser::K_BY, i);
}

CEQLQueryParser::Partition_listContext* CEQLQueryParser::Core_queryContext::partition_list() {
  return getRuleContext<CEQLQueryParser::Partition_listContext>(0);
}

tree::TerminalNode* CEQLQueryParser::Core_queryContext::K_WITHIN() {
  return getToken(CEQLQueryParser::K_WITHIN, 0);
}

CEQLQueryParser::Time_windowContext* CEQLQueryParser::Core_queryContext::time_window() {
  return getRuleContext<CEQLQueryParser::Time_windowContext>(0);
}

tree::TerminalNode* CEQLQueryParser::Core_queryContext::K_CONSUME() {
  return getToken(CEQLQueryParser::K_CONSUME, 0);
}

CEQLQueryParser::Consumption_policyContext* CEQLQueryParser::Core_queryContext::consumption_policy() {
  return getRuleContext<CEQLQueryParser::Consumption_policyContext>(0);
}


size_t CEQLQueryParser::Core_queryContext::getRuleIndex() const {
  return CEQLQueryParser::RuleCore_query;
}


std::any CEQLQueryParser::Core_queryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitCore_query(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::Core_queryContext* CEQLQueryParser::core_query() {
  Core_queryContext *_localctx = _tracker.createInstance<Core_queryContext>(_ctx, getState());
  enterRule(_localctx, 4, CEQLQueryParser::RuleCore_query);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(134);
    match(CEQLQueryParser::K_SELECT);
    setState(136);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 67452938) != 0)) {
      setState(135);
      selection_strategy();
    }
    setState(138);
    list_of_variables();
    setState(139);
    from_clause();
    setState(140);
    match(CEQLQueryParser::K_WHERE);
    setState(141);
    cel_formula(0);
    setState(145);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQLQueryParser::K_PARTITION) {
      setState(142);
      match(CEQLQueryParser::K_PARTITION);
      setState(143);
      match(CEQLQueryParser::K_BY);
      setState(144);
      partition_list();
    }
    setState(149);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQLQueryParser::K_WITHIN) {
      setState(147);
      match(CEQLQueryParser::K_WITHIN);
      setState(148);
      time_window();
    }
    setState(154);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQLQueryParser::K_CONSUME) {
      setState(151);
      match(CEQLQueryParser::K_CONSUME);
      setState(152);
      match(CEQLQueryParser::K_BY);
      setState(153);
      consumption_policy();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Selection_strategyContext ------------------------------------------------------------------

CEQLQueryParser::Selection_strategyContext::Selection_strategyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CEQLQueryParser::Selection_strategyContext::getRuleIndex() const {
  return CEQLQueryParser::RuleSelection_strategy;
}

void CEQLQueryParser::Selection_strategyContext::copyFrom(Selection_strategyContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Ss_lastContext ------------------------------------------------------------------

tree::TerminalNode* CEQLQueryParser::Ss_lastContext::K_LAST() {
  return getToken(CEQLQueryParser::K_LAST, 0);
}

CEQLQueryParser::Ss_lastContext::Ss_lastContext(Selection_strategyContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Ss_lastContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitSs_last(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Ss_strictContext ------------------------------------------------------------------

tree::TerminalNode* CEQLQueryParser::Ss_strictContext::K_STRICT() {
  return getToken(CEQLQueryParser::K_STRICT, 0);
}

CEQLQueryParser::Ss_strictContext::Ss_strictContext(Selection_strategyContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Ss_strictContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitSs_strict(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Ss_anyContext ------------------------------------------------------------------

tree::TerminalNode* CEQLQueryParser::Ss_anyContext::K_ANY() {
  return getToken(CEQLQueryParser::K_ANY, 0);
}

CEQLQueryParser::Ss_anyContext::Ss_anyContext(Selection_strategyContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Ss_anyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitSs_any(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Ss_nextContext ------------------------------------------------------------------

tree::TerminalNode* CEQLQueryParser::Ss_nextContext::K_NEXT() {
  return getToken(CEQLQueryParser::K_NEXT, 0);
}

CEQLQueryParser::Ss_nextContext::Ss_nextContext(Selection_strategyContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Ss_nextContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitSs_next(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Ss_maxContext ------------------------------------------------------------------

tree::TerminalNode* CEQLQueryParser::Ss_maxContext::K_MAX() {
  return getToken(CEQLQueryParser::K_MAX, 0);
}

CEQLQueryParser::Ss_maxContext::Ss_maxContext(Selection_strategyContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Ss_maxContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitSs_max(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Ss_allContext ------------------------------------------------------------------

tree::TerminalNode* CEQLQueryParser::Ss_allContext::K_ALL() {
  return getToken(CEQLQueryParser::K_ALL, 0);
}

CEQLQueryParser::Ss_allContext::Ss_allContext(Selection_strategyContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Ss_allContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitSs_all(this);
  else
    return visitor->visitChildren(this);
}
CEQLQueryParser::Selection_strategyContext* CEQLQueryParser::selection_strategy() {
  Selection_strategyContext *_localctx = _tracker.createInstance<Selection_strategyContext>(_ctx, getState());
  enterRule(_localctx, 6, CEQLQueryParser::RuleSelection_strategy);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(162);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::K_ALL: {
        _localctx = _tracker.createInstance<CEQLQueryParser::Ss_allContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(156);
        match(CEQLQueryParser::K_ALL);
        break;
      }

      case CEQLQueryParser::K_ANY: {
        _localctx = _tracker.createInstance<CEQLQueryParser::Ss_anyContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(157);
        match(CEQLQueryParser::K_ANY);
        break;
      }

      case CEQLQueryParser::K_LAST: {
        _localctx = _tracker.createInstance<CEQLQueryParser::Ss_lastContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(158);
        match(CEQLQueryParser::K_LAST);
        break;
      }

      case CEQLQueryParser::K_MAX: {
        _localctx = _tracker.createInstance<CEQLQueryParser::Ss_maxContext>(_localctx);
        enterOuterAlt(_localctx, 4);
        setState(159);
        match(CEQLQueryParser::K_MAX);
        break;
      }

      case CEQLQueryParser::K_NEXT: {
        _localctx = _tracker.createInstance<CEQLQueryParser::Ss_nextContext>(_localctx);
        enterOuterAlt(_localctx, 5);
        setState(160);
        match(CEQLQueryParser::K_NEXT);
        break;
      }

      case CEQLQueryParser::K_STRICT: {
        _localctx = _tracker.createInstance<CEQLQueryParser::Ss_strictContext>(_localctx);
        enterOuterAlt(_localctx, 6);
        setState(161);
        match(CEQLQueryParser::K_STRICT);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- List_of_variablesContext ------------------------------------------------------------------

CEQLQueryParser::List_of_variablesContext::List_of_variablesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CEQLQueryParser::List_of_variablesContext::getRuleIndex() const {
  return CEQLQueryParser::RuleList_of_variables;
}

void CEQLQueryParser::List_of_variablesContext::copyFrom(List_of_variablesContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- S_starContext ------------------------------------------------------------------

tree::TerminalNode* CEQLQueryParser::S_starContext::STAR() {
  return getToken(CEQLQueryParser::STAR, 0);
}

CEQLQueryParser::S_starContext::S_starContext(List_of_variablesContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::S_starContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitS_star(this);
  else
    return visitor->visitChildren(this);
}
//----------------- S_list_of_variablesContext ------------------------------------------------------------------

std::vector<CEQLQueryParser::Any_nameContext *> CEQLQueryParser::S_list_of_variablesContext::any_name() {
  return getRuleContexts<CEQLQueryParser::Any_nameContext>();
}

CEQLQueryParser::Any_nameContext* CEQLQueryParser::S_list_of_variablesContext::any_name(size_t i) {
  return getRuleContext<CEQLQueryParser::Any_nameContext>(i);
}

std::vector<tree::TerminalNode *> CEQLQueryParser::S_list_of_variablesContext::COMMA() {
  return getTokens(CEQLQueryParser::COMMA);
}

tree::TerminalNode* CEQLQueryParser::S_list_of_variablesContext::COMMA(size_t i) {
  return getToken(CEQLQueryParser::COMMA, i);
}

CEQLQueryParser::S_list_of_variablesContext::S_list_of_variablesContext(List_of_variablesContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::S_list_of_variablesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitS_list_of_variables(this);
  else
    return visitor->visitChildren(this);
}
CEQLQueryParser::List_of_variablesContext* CEQLQueryParser::list_of_variables() {
  List_of_variablesContext *_localctx = _tracker.createInstance<List_of_variablesContext>(_ctx, getState());
  enterRule(_localctx, 8, CEQLQueryParser::RuleList_of_variables);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(173);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::STAR: {
        _localctx = _tracker.createInstance<CEQLQueryParser::S_starContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(164);
        match(CEQLQueryParser::STAR);
        break;
      }

      case CEQLQueryParser::IDENTIFIER: {
        _localctx = _tracker.createInstance<CEQLQueryParser::S_list_of_variablesContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(165);
        any_name();
        setState(170);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == CEQLQueryParser::COMMA) {
          setState(166);
          match(CEQLQueryParser::COMMA);
          setState(167);
          any_name();
          setState(172);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- From_clauseContext ------------------------------------------------------------------

CEQLQueryParser::From_clauseContext::From_clauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CEQLQueryParser::From_clauseContext::K_FROM() {
  return getToken(CEQLQueryParser::K_FROM, 0);
}

std::vector<CEQLQueryParser::Stream_nameContext *> CEQLQueryParser::From_clauseContext::stream_name() {
  return getRuleContexts<CEQLQueryParser::Stream_nameContext>();
}

CEQLQueryParser::Stream_nameContext* CEQLQueryParser::From_clauseContext::stream_name(size_t i) {
  return getRuleContext<CEQLQueryParser::Stream_nameContext>(i);
}

std::vector<tree::TerminalNode *> CEQLQueryParser::From_clauseContext::COMMA() {
  return getTokens(CEQLQueryParser::COMMA);
}

tree::TerminalNode* CEQLQueryParser::From_clauseContext::COMMA(size_t i) {
  return getToken(CEQLQueryParser::COMMA, i);
}


size_t CEQLQueryParser::From_clauseContext::getRuleIndex() const {
  return CEQLQueryParser::RuleFrom_clause;
}


std::any CEQLQueryParser::From_clauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitFrom_clause(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::From_clauseContext* CEQLQueryParser::from_clause() {
  From_clauseContext *_localctx = _tracker.createInstance<From_clauseContext>(_ctx, getState());
  enterRule(_localctx, 10, CEQLQueryParser::RuleFrom_clause);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(184);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQLQueryParser::K_FROM) {
      setState(175);
      match(CEQLQueryParser::K_FROM);
      setState(176);
      stream_name();
      setState(181);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == CEQLQueryParser::COMMA) {
        setState(177);
        match(CEQLQueryParser::COMMA);
        setState(178);
        stream_name();
        setState(183);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Cel_formulaContext ------------------------------------------------------------------

CEQLQueryParser::Cel_formulaContext::Cel_formulaContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CEQLQueryParser::Cel_formulaContext::getRuleIndex() const {
  return CEQLQueryParser::RuleCel_formula;
}

void CEQLQueryParser::Cel_formulaContext::copyFrom(Cel_formulaContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Contiguous_sequencing_cel_formulaContext ------------------------------------------------------------------

std::vector<CEQLQueryParser::Cel_formulaContext *> CEQLQueryParser::Contiguous_sequencing_cel_formulaContext::cel_formula() {
  return getRuleContexts<CEQLQueryParser::Cel_formulaContext>();
}

CEQLQueryParser::Cel_formulaContext* CEQLQueryParser::Contiguous_sequencing_cel_formulaContext::cel_formula(size_t i) {
  return getRuleContext<CEQLQueryParser::Cel_formulaContext>(i);
}

tree::TerminalNode* CEQLQueryParser::Contiguous_sequencing_cel_formulaContext::COLON() {
  return getToken(CEQLQueryParser::COLON, 0);
}

CEQLQueryParser::Contiguous_sequencing_cel_formulaContext::Contiguous_sequencing_cel_formulaContext(Cel_formulaContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Contiguous_sequencing_cel_formulaContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitContiguous_sequencing_cel_formula(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Event_type_cel_formulaContext ------------------------------------------------------------------

CEQLQueryParser::S_event_nameContext* CEQLQueryParser::Event_type_cel_formulaContext::s_event_name() {
  return getRuleContext<CEQLQueryParser::S_event_nameContext>(0);
}

CEQLQueryParser::Event_type_cel_formulaContext::Event_type_cel_formulaContext(Cel_formulaContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Event_type_cel_formulaContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitEvent_type_cel_formula(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Par_cel_formulaContext ------------------------------------------------------------------

tree::TerminalNode* CEQLQueryParser::Par_cel_formulaContext::LEFT_PARENTHESIS() {
  return getToken(CEQLQueryParser::LEFT_PARENTHESIS, 0);
}

CEQLQueryParser::Cel_formulaContext* CEQLQueryParser::Par_cel_formulaContext::cel_formula() {
  return getRuleContext<CEQLQueryParser::Cel_formulaContext>(0);
}

tree::TerminalNode* CEQLQueryParser::Par_cel_formulaContext::RIGHT_PARENTHESIS() {
  return getToken(CEQLQueryParser::RIGHT_PARENTHESIS, 0);
}

CEQLQueryParser::Par_cel_formulaContext::Par_cel_formulaContext(Cel_formulaContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Par_cel_formulaContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitPar_cel_formula(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Kleene_cel_formulaContext ------------------------------------------------------------------

CEQLQueryParser::Cel_formulaContext* CEQLQueryParser::Kleene_cel_formulaContext::cel_formula() {
  return getRuleContext<CEQLQueryParser::Cel_formulaContext>(0);
}

tree::TerminalNode* CEQLQueryParser::Kleene_cel_formulaContext::PLUS() {
  return getToken(CEQLQueryParser::PLUS, 0);
}

CEQLQueryParser::Kleene_cel_formulaContext::Kleene_cel_formulaContext(Cel_formulaContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Kleene_cel_formulaContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitKleene_cel_formula(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Filter_cel_formulaContext ------------------------------------------------------------------

CEQLQueryParser::Cel_formulaContext* CEQLQueryParser::Filter_cel_formulaContext::cel_formula() {
  return getRuleContext<CEQLQueryParser::Cel_formulaContext>(0);
}

tree::TerminalNode* CEQLQueryParser::Filter_cel_formulaContext::K_FILTER() {
  return getToken(CEQLQueryParser::K_FILTER, 0);
}

CEQLQueryParser::FilterContext* CEQLQueryParser::Filter_cel_formulaContext::filter() {
  return getRuleContext<CEQLQueryParser::FilterContext>(0);
}

CEQLQueryParser::Filter_cel_formulaContext::Filter_cel_formulaContext(Cel_formulaContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Filter_cel_formulaContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitFilter_cel_formula(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Or_cel_formulaContext ------------------------------------------------------------------

std::vector<CEQLQueryParser::Cel_formulaContext *> CEQLQueryParser::Or_cel_formulaContext::cel_formula() {
  return getRuleContexts<CEQLQueryParser::Cel_formulaContext>();
}

CEQLQueryParser::Cel_formulaContext* CEQLQueryParser::Or_cel_formulaContext::cel_formula(size_t i) {
  return getRuleContext<CEQLQueryParser::Cel_formulaContext>(i);
}

tree::TerminalNode* CEQLQueryParser::Or_cel_formulaContext::K_OR() {
  return getToken(CEQLQueryParser::K_OR, 0);
}

CEQLQueryParser::Or_cel_formulaContext::Or_cel_formulaContext(Cel_formulaContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Or_cel_formulaContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitOr_cel_formula(this);
  else
    return visitor->visitChildren(this);
}
//----------------- As_cel_formulaContext ------------------------------------------------------------------

CEQLQueryParser::Cel_formulaContext* CEQLQueryParser::As_cel_formulaContext::cel_formula() {
  return getRuleContext<CEQLQueryParser::Cel_formulaContext>(0);
}

tree::TerminalNode* CEQLQueryParser::As_cel_formulaContext::K_AS() {
  return getToken(CEQLQueryParser::K_AS, 0);
}

CEQLQueryParser::Event_nameContext* CEQLQueryParser::As_cel_formulaContext::event_name() {
  return getRuleContext<CEQLQueryParser::Event_nameContext>(0);
}

CEQLQueryParser::As_cel_formulaContext::As_cel_formulaContext(Cel_formulaContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::As_cel_formulaContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitAs_cel_formula(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Non_contiguous_sequencing_cel_formulaContext ------------------------------------------------------------------

std::vector<CEQLQueryParser::Cel_formulaContext *> CEQLQueryParser::Non_contiguous_sequencing_cel_formulaContext::cel_formula() {
  return getRuleContexts<CEQLQueryParser::Cel_formulaContext>();
}

CEQLQueryParser::Cel_formulaContext* CEQLQueryParser::Non_contiguous_sequencing_cel_formulaContext::cel_formula(size_t i) {
  return getRuleContext<CEQLQueryParser::Cel_formulaContext>(i);
}

tree::TerminalNode* CEQLQueryParser::Non_contiguous_sequencing_cel_formulaContext::SEMICOLON() {
  return getToken(CEQLQueryParser::SEMICOLON, 0);
}

CEQLQueryParser::Non_contiguous_sequencing_cel_formulaContext::Non_contiguous_sequencing_cel_formulaContext(Cel_formulaContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Non_contiguous_sequencing_cel_formulaContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitNon_contiguous_sequencing_cel_formula(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::Cel_formulaContext* CEQLQueryParser::cel_formula() {
   return cel_formula(0);
}

CEQLQueryParser::Cel_formulaContext* CEQLQueryParser::cel_formula(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  CEQLQueryParser::Cel_formulaContext *_localctx = _tracker.createInstance<Cel_formulaContext>(_ctx, parentState);
  CEQLQueryParser::Cel_formulaContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 12;
  enterRecursionRule(_localctx, 12, CEQLQueryParser::RuleCel_formula, precedence);

    

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(192);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::LEFT_PARENTHESIS: {
        _localctx = _tracker.createInstance<Par_cel_formulaContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(187);
        match(CEQLQueryParser::LEFT_PARENTHESIS);
        setState(188);
        cel_formula(0);
        setState(189);
        match(CEQLQueryParser::RIGHT_PARENTHESIS);
        break;
      }

      case CEQLQueryParser::IDENTIFIER: {
        _localctx = _tracker.createInstance<Event_type_cel_formulaContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(191);
        s_event_name();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(213);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 13, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(211);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 12, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<Non_contiguous_sequencing_cel_formulaContext>(_tracker.createInstance<Cel_formulaContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleCel_formula);
          setState(194);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(195);
          match(CEQLQueryParser::SEMICOLON);
          setState(196);
          cel_formula(5);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<Contiguous_sequencing_cel_formulaContext>(_tracker.createInstance<Cel_formulaContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleCel_formula);
          setState(197);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(198);
          match(CEQLQueryParser::COLON);
          setState(199);
          cel_formula(4);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<Or_cel_formulaContext>(_tracker.createInstance<Cel_formulaContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleCel_formula);
          setState(200);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(201);
          match(CEQLQueryParser::K_OR);
          setState(202);
          cel_formula(3);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<As_cel_formulaContext>(_tracker.createInstance<Cel_formulaContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleCel_formula);
          setState(203);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(204);
          match(CEQLQueryParser::K_AS);
          setState(205);
          event_name();
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<Kleene_cel_formulaContext>(_tracker.createInstance<Cel_formulaContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleCel_formula);
          setState(206);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(207);
          match(CEQLQueryParser::PLUS);
          break;
        }

        case 6: {
          auto newContext = _tracker.createInstance<Filter_cel_formulaContext>(_tracker.createInstance<Cel_formulaContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleCel_formula);
          setState(208);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(209);
          match(CEQLQueryParser::K_FILTER);
          setState(210);
          filter(0);
          break;
        }

        default:
          break;
        } 
      }
      setState(215);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 13, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- Partition_listContext ------------------------------------------------------------------

CEQLQueryParser::Partition_listContext::Partition_listContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> CEQLQueryParser::Partition_listContext::LEFT_SQUARE_BRACKET() {
  return getTokens(CEQLQueryParser::LEFT_SQUARE_BRACKET);
}

tree::TerminalNode* CEQLQueryParser::Partition_listContext::LEFT_SQUARE_BRACKET(size_t i) {
  return getToken(CEQLQueryParser::LEFT_SQUARE_BRACKET, i);
}

std::vector<CEQLQueryParser::Attribute_listContext *> CEQLQueryParser::Partition_listContext::attribute_list() {
  return getRuleContexts<CEQLQueryParser::Attribute_listContext>();
}

CEQLQueryParser::Attribute_listContext* CEQLQueryParser::Partition_listContext::attribute_list(size_t i) {
  return getRuleContext<CEQLQueryParser::Attribute_listContext>(i);
}

std::vector<tree::TerminalNode *> CEQLQueryParser::Partition_listContext::RIGHT_SQUARE_BRACKET() {
  return getTokens(CEQLQueryParser::RIGHT_SQUARE_BRACKET);
}

tree::TerminalNode* CEQLQueryParser::Partition_listContext::RIGHT_SQUARE_BRACKET(size_t i) {
  return getToken(CEQLQueryParser::RIGHT_SQUARE_BRACKET, i);
}

tree::TerminalNode* CEQLQueryParser::Partition_listContext::COMMA() {
  return getToken(CEQLQueryParser::COMMA, 0);
}


size_t CEQLQueryParser::Partition_listContext::getRuleIndex() const {
  return CEQLQueryParser::RulePartition_list;
}


std::any CEQLQueryParser::Partition_listContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitPartition_list(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::Partition_listContext* CEQLQueryParser::partition_list() {
  Partition_listContext *_localctx = _tracker.createInstance<Partition_listContext>(_ctx, getState());
  enterRule(_localctx, 14, CEQLQueryParser::RulePartition_list);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(216);
    match(CEQLQueryParser::LEFT_SQUARE_BRACKET);
    setState(217);
    attribute_list();
    setState(218);
    match(CEQLQueryParser::RIGHT_SQUARE_BRACKET);
    setState(224);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQLQueryParser::COMMA) {
      setState(219);
      match(CEQLQueryParser::COMMA);
      setState(220);
      match(CEQLQueryParser::LEFT_SQUARE_BRACKET);
      setState(221);
      attribute_list();
      setState(222);
      match(CEQLQueryParser::RIGHT_SQUARE_BRACKET);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Attribute_listContext ------------------------------------------------------------------

CEQLQueryParser::Attribute_listContext::Attribute_listContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<CEQLQueryParser::Attribute_nameContext *> CEQLQueryParser::Attribute_listContext::attribute_name() {
  return getRuleContexts<CEQLQueryParser::Attribute_nameContext>();
}

CEQLQueryParser::Attribute_nameContext* CEQLQueryParser::Attribute_listContext::attribute_name(size_t i) {
  return getRuleContext<CEQLQueryParser::Attribute_nameContext>(i);
}

std::vector<tree::TerminalNode *> CEQLQueryParser::Attribute_listContext::COMMA() {
  return getTokens(CEQLQueryParser::COMMA);
}

tree::TerminalNode* CEQLQueryParser::Attribute_listContext::COMMA(size_t i) {
  return getToken(CEQLQueryParser::COMMA, i);
}


size_t CEQLQueryParser::Attribute_listContext::getRuleIndex() const {
  return CEQLQueryParser::RuleAttribute_list;
}


std::any CEQLQueryParser::Attribute_listContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitAttribute_list(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::Attribute_listContext* CEQLQueryParser::attribute_list() {
  Attribute_listContext *_localctx = _tracker.createInstance<Attribute_listContext>(_ctx, getState());
  enterRule(_localctx, 16, CEQLQueryParser::RuleAttribute_list);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(226);
    attribute_name();
    setState(231);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CEQLQueryParser::COMMA) {
      setState(227);
      match(CEQLQueryParser::COMMA);
      setState(228);
      attribute_name();
      setState(233);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Consumption_policyContext ------------------------------------------------------------------

CEQLQueryParser::Consumption_policyContext::Consumption_policyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CEQLQueryParser::Consumption_policyContext::getRuleIndex() const {
  return CEQLQueryParser::RuleConsumption_policy;
}

void CEQLQueryParser::Consumption_policyContext::copyFrom(Consumption_policyContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Cp_partitionContext ------------------------------------------------------------------

tree::TerminalNode* CEQLQueryParser::Cp_partitionContext::K_PARTITION() {
  return getToken(CEQLQueryParser::K_PARTITION, 0);
}

CEQLQueryParser::Cp_partitionContext::Cp_partitionContext(Consumption_policyContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Cp_partitionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitCp_partition(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Cp_anyContext ------------------------------------------------------------------

tree::TerminalNode* CEQLQueryParser::Cp_anyContext::K_ANY() {
  return getToken(CEQLQueryParser::K_ANY, 0);
}

CEQLQueryParser::Cp_anyContext::Cp_anyContext(Consumption_policyContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Cp_anyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitCp_any(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Cp_noneContext ------------------------------------------------------------------

tree::TerminalNode* CEQLQueryParser::Cp_noneContext::K_NONE() {
  return getToken(CEQLQueryParser::K_NONE, 0);
}

CEQLQueryParser::Cp_noneContext::Cp_noneContext(Consumption_policyContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Cp_noneContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitCp_none(this);
  else
    return visitor->visitChildren(this);
}
CEQLQueryParser::Consumption_policyContext* CEQLQueryParser::consumption_policy() {
  Consumption_policyContext *_localctx = _tracker.createInstance<Consumption_policyContext>(_ctx, getState());
  enterRule(_localctx, 18, CEQLQueryParser::RuleConsumption_policy);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(237);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::K_ANY: {
        _localctx = _tracker.createInstance<CEQLQueryParser::Cp_anyContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(234);
        match(CEQLQueryParser::K_ANY);
        break;
      }

      case CEQLQueryParser::K_PARTITION: {
        _localctx = _tracker.createInstance<CEQLQueryParser::Cp_partitionContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(235);
        match(CEQLQueryParser::K_PARTITION);
        break;
      }

      case CEQLQueryParser::K_NONE: {
        _localctx = _tracker.createInstance<CEQLQueryParser::Cp_noneContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(236);
        match(CEQLQueryParser::K_NONE);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FilterContext ------------------------------------------------------------------

CEQLQueryParser::FilterContext::FilterContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CEQLQueryParser::FilterContext::getRuleIndex() const {
  return CEQLQueryParser::RuleFilter;
}

void CEQLQueryParser::FilterContext::copyFrom(FilterContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Par_filterContext ------------------------------------------------------------------

tree::TerminalNode* CEQLQueryParser::Par_filterContext::LEFT_PARENTHESIS() {
  return getToken(CEQLQueryParser::LEFT_PARENTHESIS, 0);
}

CEQLQueryParser::FilterContext* CEQLQueryParser::Par_filterContext::filter() {
  return getRuleContext<CEQLQueryParser::FilterContext>(0);
}

tree::TerminalNode* CEQLQueryParser::Par_filterContext::RIGHT_PARENTHESIS() {
  return getToken(CEQLQueryParser::RIGHT_PARENTHESIS, 0);
}

CEQLQueryParser::Par_filterContext::Par_filterContext(FilterContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Par_filterContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitPar_filter(this);
  else
    return visitor->visitChildren(this);
}
//----------------- And_filterContext ------------------------------------------------------------------

std::vector<CEQLQueryParser::FilterContext *> CEQLQueryParser::And_filterContext::filter() {
  return getRuleContexts<CEQLQueryParser::FilterContext>();
}

CEQLQueryParser::FilterContext* CEQLQueryParser::And_filterContext::filter(size_t i) {
  return getRuleContext<CEQLQueryParser::FilterContext>(i);
}

tree::TerminalNode* CEQLQueryParser::And_filterContext::K_AND() {
  return getToken(CEQLQueryParser::K_AND, 0);
}

CEQLQueryParser::And_filterContext::And_filterContext(FilterContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::And_filterContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitAnd_filter(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Atomic_filterContext ------------------------------------------------------------------

CEQLQueryParser::Event_nameContext* CEQLQueryParser::Atomic_filterContext::event_name() {
  return getRuleContext<CEQLQueryParser::Event_nameContext>(0);
}

tree::TerminalNode* CEQLQueryParser::Atomic_filterContext::LEFT_SQUARE_BRACKET() {
  return getToken(CEQLQueryParser::LEFT_SQUARE_BRACKET, 0);
}

CEQLQueryParser::PredicateContext* CEQLQueryParser::Atomic_filterContext::predicate() {
  return getRuleContext<CEQLQueryParser::PredicateContext>(0);
}

tree::TerminalNode* CEQLQueryParser::Atomic_filterContext::RIGHT_SQUARE_BRACKET() {
  return getToken(CEQLQueryParser::RIGHT_SQUARE_BRACKET, 0);
}

CEQLQueryParser::Atomic_filterContext::Atomic_filterContext(FilterContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Atomic_filterContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitAtomic_filter(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Or_filterContext ------------------------------------------------------------------

std::vector<CEQLQueryParser::FilterContext *> CEQLQueryParser::Or_filterContext::filter() {
  return getRuleContexts<CEQLQueryParser::FilterContext>();
}

CEQLQueryParser::FilterContext* CEQLQueryParser::Or_filterContext::filter(size_t i) {
  return getRuleContext<CEQLQueryParser::FilterContext>(i);
}

tree::TerminalNode* CEQLQueryParser::Or_filterContext::K_OR() {
  return getToken(CEQLQueryParser::K_OR, 0);
}

CEQLQueryParser::Or_filterContext::Or_filterContext(FilterContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Or_filterContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitOr_filter(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::FilterContext* CEQLQueryParser::filter() {
   return filter(0);
}

CEQLQueryParser::FilterContext* CEQLQueryParser::filter(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  CEQLQueryParser::FilterContext *_localctx = _tracker.createInstance<FilterContext>(_ctx, parentState);
  CEQLQueryParser::FilterContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 20;
  enterRecursionRule(_localctx, 20, CEQLQueryParser::RuleFilter, precedence);

    

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(249);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::LEFT_PARENTHESIS: {
        _localctx = _tracker.createInstance<Par_filterContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(240);
        match(CEQLQueryParser::LEFT_PARENTHESIS);
        setState(241);
        filter(0);
        setState(242);
        match(CEQLQueryParser::RIGHT_PARENTHESIS);
        break;
      }

      case CEQLQueryParser::IDENTIFIER: {
        _localctx = _tracker.createInstance<Atomic_filterContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(244);
        event_name();
        setState(245);
        match(CEQLQueryParser::LEFT_SQUARE_BRACKET);
        setState(246);
        predicate(0);
        setState(247);
        match(CEQLQueryParser::RIGHT_SQUARE_BRACKET);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(259);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 19, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(257);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 18, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<And_filterContext>(_tracker.createInstance<FilterContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleFilter);
          setState(251);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(252);
          match(CEQLQueryParser::K_AND);
          setState(253);
          filter(3);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<Or_filterContext>(_tracker.createInstance<FilterContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleFilter);
          setState(254);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(255);
          match(CEQLQueryParser::K_OR);
          setState(256);
          filter(2);
          break;
        }

        default:
          break;
        } 
      }
      setState(261);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 19, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- PredicateContext ------------------------------------------------------------------

CEQLQueryParser::PredicateContext::PredicateContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CEQLQueryParser::PredicateContext::getRuleIndex() const {
  return CEQLQueryParser::RulePredicate;
}

void CEQLQueryParser::PredicateContext::copyFrom(PredicateContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Regex_predicateContext ------------------------------------------------------------------

CEQLQueryParser::Attribute_nameContext* CEQLQueryParser::Regex_predicateContext::attribute_name() {
  return getRuleContext<CEQLQueryParser::Attribute_nameContext>(0);
}

tree::TerminalNode* CEQLQueryParser::Regex_predicateContext::K_LIKE() {
  return getToken(CEQLQueryParser::K_LIKE, 0);
}

CEQLQueryParser::RegexpContext* CEQLQueryParser::Regex_predicateContext::regexp() {
  return getRuleContext<CEQLQueryParser::RegexpContext>(0);
}

CEQLQueryParser::Regex_predicateContext::Regex_predicateContext(PredicateContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Regex_predicateContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitRegex_predicate(this);
  else
    return visitor->visitChildren(this);
}
//----------------- In_predicateContext ------------------------------------------------------------------

CEQLQueryParser::Attribute_nameContext* CEQLQueryParser::In_predicateContext::attribute_name() {
  return getRuleContext<CEQLQueryParser::Attribute_nameContext>(0);
}

CEQLQueryParser::Value_seqContext* CEQLQueryParser::In_predicateContext::value_seq() {
  return getRuleContext<CEQLQueryParser::Value_seqContext>(0);
}

tree::TerminalNode* CEQLQueryParser::In_predicateContext::K_IN() {
  return getToken(CEQLQueryParser::K_IN, 0);
}

tree::TerminalNode* CEQLQueryParser::In_predicateContext::K_NOT() {
  return getToken(CEQLQueryParser::K_NOT, 0);
}

CEQLQueryParser::In_predicateContext::In_predicateContext(PredicateContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::In_predicateContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitIn_predicate(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Not_predicateContext ------------------------------------------------------------------

tree::TerminalNode* CEQLQueryParser::Not_predicateContext::K_NOT() {
  return getToken(CEQLQueryParser::K_NOT, 0);
}

CEQLQueryParser::PredicateContext* CEQLQueryParser::Not_predicateContext::predicate() {
  return getRuleContext<CEQLQueryParser::PredicateContext>(0);
}

CEQLQueryParser::Not_predicateContext::Not_predicateContext(PredicateContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Not_predicateContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitNot_predicate(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Par_predicateContext ------------------------------------------------------------------

tree::TerminalNode* CEQLQueryParser::Par_predicateContext::LEFT_PARENTHESIS() {
  return getToken(CEQLQueryParser::LEFT_PARENTHESIS, 0);
}

CEQLQueryParser::PredicateContext* CEQLQueryParser::Par_predicateContext::predicate() {
  return getRuleContext<CEQLQueryParser::PredicateContext>(0);
}

tree::TerminalNode* CEQLQueryParser::Par_predicateContext::RIGHT_PARENTHESIS() {
  return getToken(CEQLQueryParser::RIGHT_PARENTHESIS, 0);
}

CEQLQueryParser::Par_predicateContext::Par_predicateContext(PredicateContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Par_predicateContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitPar_predicate(this);
  else
    return visitor->visitChildren(this);
}
//----------------- And_predicateContext ------------------------------------------------------------------

std::vector<CEQLQueryParser::PredicateContext *> CEQLQueryParser::And_predicateContext::predicate() {
  return getRuleContexts<CEQLQueryParser::PredicateContext>();
}

CEQLQueryParser::PredicateContext* CEQLQueryParser::And_predicateContext::predicate(size_t i) {
  return getRuleContext<CEQLQueryParser::PredicateContext>(i);
}

tree::TerminalNode* CEQLQueryParser::And_predicateContext::K_AND() {
  return getToken(CEQLQueryParser::K_AND, 0);
}

CEQLQueryParser::And_predicateContext::And_predicateContext(PredicateContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::And_predicateContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitAnd_predicate(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Equality_string_predicateContext ------------------------------------------------------------------

CEQLQueryParser::String_literalContext* CEQLQueryParser::Equality_string_predicateContext::string_literal() {
  return getRuleContext<CEQLQueryParser::String_literalContext>(0);
}

CEQLQueryParser::String_literal_or_regexpContext* CEQLQueryParser::Equality_string_predicateContext::string_literal_or_regexp() {
  return getRuleContext<CEQLQueryParser::String_literal_or_regexpContext>(0);
}

tree::TerminalNode* CEQLQueryParser::Equality_string_predicateContext::EQ() {
  return getToken(CEQLQueryParser::EQ, 0);
}

tree::TerminalNode* CEQLQueryParser::Equality_string_predicateContext::NEQ() {
  return getToken(CEQLQueryParser::NEQ, 0);
}

CEQLQueryParser::Equality_string_predicateContext::Equality_string_predicateContext(PredicateContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Equality_string_predicateContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitEquality_string_predicate(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Inequality_predicateContext ------------------------------------------------------------------

std::vector<CEQLQueryParser::Math_exprContext *> CEQLQueryParser::Inequality_predicateContext::math_expr() {
  return getRuleContexts<CEQLQueryParser::Math_exprContext>();
}

CEQLQueryParser::Math_exprContext* CEQLQueryParser::Inequality_predicateContext::math_expr(size_t i) {
  return getRuleContext<CEQLQueryParser::Math_exprContext>(i);
}

tree::TerminalNode* CEQLQueryParser::Inequality_predicateContext::LE() {
  return getToken(CEQLQueryParser::LE, 0);
}

tree::TerminalNode* CEQLQueryParser::Inequality_predicateContext::LEQ() {
  return getToken(CEQLQueryParser::LEQ, 0);
}

tree::TerminalNode* CEQLQueryParser::Inequality_predicateContext::GE() {
  return getToken(CEQLQueryParser::GE, 0);
}

tree::TerminalNode* CEQLQueryParser::Inequality_predicateContext::GEQ() {
  return getToken(CEQLQueryParser::GEQ, 0);
}

tree::TerminalNode* CEQLQueryParser::Inequality_predicateContext::EQ() {
  return getToken(CEQLQueryParser::EQ, 0);
}

tree::TerminalNode* CEQLQueryParser::Inequality_predicateContext::NEQ() {
  return getToken(CEQLQueryParser::NEQ, 0);
}

CEQLQueryParser::Inequality_predicateContext::Inequality_predicateContext(PredicateContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Inequality_predicateContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitInequality_predicate(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Or_predicateContext ------------------------------------------------------------------

std::vector<CEQLQueryParser::PredicateContext *> CEQLQueryParser::Or_predicateContext::predicate() {
  return getRuleContexts<CEQLQueryParser::PredicateContext>();
}

CEQLQueryParser::PredicateContext* CEQLQueryParser::Or_predicateContext::predicate(size_t i) {
  return getRuleContext<CEQLQueryParser::PredicateContext>(i);
}

tree::TerminalNode* CEQLQueryParser::Or_predicateContext::K_OR() {
  return getToken(CEQLQueryParser::K_OR, 0);
}

CEQLQueryParser::Or_predicateContext::Or_predicateContext(PredicateContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Or_predicateContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitOr_predicate(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::PredicateContext* CEQLQueryParser::predicate() {
   return predicate(0);
}

CEQLQueryParser::PredicateContext* CEQLQueryParser::predicate(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  CEQLQueryParser::PredicateContext *_localctx = _tracker.createInstance<PredicateContext>(_ctx, parentState);
  CEQLQueryParser::PredicateContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 22;
  enterRecursionRule(_localctx, 22, CEQLQueryParser::RulePredicate, precedence);

    size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(289);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 21, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<Par_predicateContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(263);
      match(CEQLQueryParser::LEFT_PARENTHESIS);
      setState(264);
      predicate(0);
      setState(265);
      match(CEQLQueryParser::RIGHT_PARENTHESIS);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<Not_predicateContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(267);
      match(CEQLQueryParser::K_NOT);
      setState(268);
      predicate(7);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<Inequality_predicateContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(269);
      math_expr(0);
      setState(270);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 2164663517184) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(271);
      math_expr(0);
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<Equality_string_predicateContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(273);
      string_literal();
      setState(274);
      _la = _input->LA(1);
      if (!(_la == CEQLQueryParser::EQ

      || _la == CEQLQueryParser::NEQ)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(275);
      string_literal_or_regexp();
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<Regex_predicateContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(277);
      attribute_name();
      setState(278);
      match(CEQLQueryParser::K_LIKE);
      setState(279);
      regexp();
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<In_predicateContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(281);
      attribute_name();
      setState(285);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case CEQLQueryParser::K_IN: {
          setState(282);
          match(CEQLQueryParser::K_IN);
          break;
        }

        case CEQLQueryParser::K_NOT: {
          setState(283);
          match(CEQLQueryParser::K_NOT);
          setState(284);
          match(CEQLQueryParser::K_IN);
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(287);
      value_seq();
      break;
    }

    default:
      break;
    }
    _ctx->stop = _input->LT(-1);
    setState(299);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 23, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(297);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 22, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<And_predicateContext>(_tracker.createInstance<PredicateContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RulePredicate);
          setState(291);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(292);
          match(CEQLQueryParser::K_AND);
          setState(293);
          predicate(5);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<Or_predicateContext>(_tracker.createInstance<PredicateContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RulePredicate);
          setState(294);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(295);
          match(CEQLQueryParser::K_OR);
          setState(296);
          predicate(4);
          break;
        }

        default:
          break;
        } 
      }
      setState(301);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 23, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- String_literalContext ------------------------------------------------------------------

CEQLQueryParser::String_literalContext::String_literalContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQLQueryParser::StringContext* CEQLQueryParser::String_literalContext::string() {
  return getRuleContext<CEQLQueryParser::StringContext>(0);
}

CEQLQueryParser::Attribute_nameContext* CEQLQueryParser::String_literalContext::attribute_name() {
  return getRuleContext<CEQLQueryParser::Attribute_nameContext>(0);
}


size_t CEQLQueryParser::String_literalContext::getRuleIndex() const {
  return CEQLQueryParser::RuleString_literal;
}


std::any CEQLQueryParser::String_literalContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitString_literal(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::String_literalContext* CEQLQueryParser::string_literal() {
  String_literalContext *_localctx = _tracker.createInstance<String_literalContext>(_ctx, getState());
  enterRule(_localctx, 24, CEQLQueryParser::RuleString_literal);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(304);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::STRING_LITERAL: {
        enterOuterAlt(_localctx, 1);
        setState(302);
        string();
        break;
      }

      case CEQLQueryParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 2);
        setState(303);
        attribute_name();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- String_literal_or_regexpContext ------------------------------------------------------------------

CEQLQueryParser::String_literal_or_regexpContext::String_literal_or_regexpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQLQueryParser::StringContext* CEQLQueryParser::String_literal_or_regexpContext::string() {
  return getRuleContext<CEQLQueryParser::StringContext>(0);
}

CEQLQueryParser::Attribute_nameContext* CEQLQueryParser::String_literal_or_regexpContext::attribute_name() {
  return getRuleContext<CEQLQueryParser::Attribute_nameContext>(0);
}

CEQLQueryParser::RegexpContext* CEQLQueryParser::String_literal_or_regexpContext::regexp() {
  return getRuleContext<CEQLQueryParser::RegexpContext>(0);
}


size_t CEQLQueryParser::String_literal_or_regexpContext::getRuleIndex() const {
  return CEQLQueryParser::RuleString_literal_or_regexp;
}


std::any CEQLQueryParser::String_literal_or_regexpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitString_literal_or_regexp(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::String_literal_or_regexpContext* CEQLQueryParser::string_literal_or_regexp() {
  String_literal_or_regexpContext *_localctx = _tracker.createInstance<String_literal_or_regexpContext>(_ctx, getState());
  enterRule(_localctx, 26, CEQLQueryParser::RuleString_literal_or_regexp);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(309);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::STRING_LITERAL: {
        enterOuterAlt(_localctx, 1);
        setState(306);
        string();
        break;
      }

      case CEQLQueryParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 2);
        setState(307);
        attribute_name();
        break;
      }

      case CEQLQueryParser::REGEX_START: {
        enterOuterAlt(_localctx, 3);
        setState(308);
        regexp();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Math_exprContext ------------------------------------------------------------------

CEQLQueryParser::Math_exprContext::Math_exprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CEQLQueryParser::Math_exprContext::getRuleIndex() const {
  return CEQLQueryParser::RuleMath_expr;
}

void CEQLQueryParser::Math_exprContext::copyFrom(Math_exprContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Mul_math_exprContext ------------------------------------------------------------------

std::vector<CEQLQueryParser::Math_exprContext *> CEQLQueryParser::Mul_math_exprContext::math_expr() {
  return getRuleContexts<CEQLQueryParser::Math_exprContext>();
}

CEQLQueryParser::Math_exprContext* CEQLQueryParser::Mul_math_exprContext::math_expr(size_t i) {
  return getRuleContext<CEQLQueryParser::Math_exprContext>(i);
}

tree::TerminalNode* CEQLQueryParser::Mul_math_exprContext::STAR() {
  return getToken(CEQLQueryParser::STAR, 0);
}

tree::TerminalNode* CEQLQueryParser::Mul_math_exprContext::SLASH() {
  return getToken(CEQLQueryParser::SLASH, 0);
}

tree::TerminalNode* CEQLQueryParser::Mul_math_exprContext::PERCENT() {
  return getToken(CEQLQueryParser::PERCENT, 0);
}

CEQLQueryParser::Mul_math_exprContext::Mul_math_exprContext(Math_exprContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Mul_math_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitMul_math_expr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Sum_math_exprContext ------------------------------------------------------------------

std::vector<CEQLQueryParser::Math_exprContext *> CEQLQueryParser::Sum_math_exprContext::math_expr() {
  return getRuleContexts<CEQLQueryParser::Math_exprContext>();
}

CEQLQueryParser::Math_exprContext* CEQLQueryParser::Sum_math_exprContext::math_expr(size_t i) {
  return getRuleContext<CEQLQueryParser::Math_exprContext>(i);
}

tree::TerminalNode* CEQLQueryParser::Sum_math_exprContext::PLUS() {
  return getToken(CEQLQueryParser::PLUS, 0);
}

tree::TerminalNode* CEQLQueryParser::Sum_math_exprContext::MINUS() {
  return getToken(CEQLQueryParser::MINUS, 0);
}

CEQLQueryParser::Sum_math_exprContext::Sum_math_exprContext(Math_exprContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Sum_math_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitSum_math_expr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Number_math_exprContext ------------------------------------------------------------------

CEQLQueryParser::NumberContext* CEQLQueryParser::Number_math_exprContext::number() {
  return getRuleContext<CEQLQueryParser::NumberContext>(0);
}

CEQLQueryParser::Number_math_exprContext::Number_math_exprContext(Math_exprContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Number_math_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitNumber_math_expr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Unary_math_exprContext ------------------------------------------------------------------

CEQLQueryParser::Math_exprContext* CEQLQueryParser::Unary_math_exprContext::math_expr() {
  return getRuleContext<CEQLQueryParser::Math_exprContext>(0);
}

tree::TerminalNode* CEQLQueryParser::Unary_math_exprContext::PLUS() {
  return getToken(CEQLQueryParser::PLUS, 0);
}

tree::TerminalNode* CEQLQueryParser::Unary_math_exprContext::MINUS() {
  return getToken(CEQLQueryParser::MINUS, 0);
}

CEQLQueryParser::Unary_math_exprContext::Unary_math_exprContext(Math_exprContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Unary_math_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitUnary_math_expr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Attribute_math_exprContext ------------------------------------------------------------------

CEQLQueryParser::Attribute_nameContext* CEQLQueryParser::Attribute_math_exprContext::attribute_name() {
  return getRuleContext<CEQLQueryParser::Attribute_nameContext>(0);
}

CEQLQueryParser::Attribute_math_exprContext::Attribute_math_exprContext(Math_exprContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Attribute_math_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitAttribute_math_expr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Par_math_exprContext ------------------------------------------------------------------

tree::TerminalNode* CEQLQueryParser::Par_math_exprContext::LEFT_PARENTHESIS() {
  return getToken(CEQLQueryParser::LEFT_PARENTHESIS, 0);
}

CEQLQueryParser::Math_exprContext* CEQLQueryParser::Par_math_exprContext::math_expr() {
  return getRuleContext<CEQLQueryParser::Math_exprContext>(0);
}

tree::TerminalNode* CEQLQueryParser::Par_math_exprContext::RIGHT_PARENTHESIS() {
  return getToken(CEQLQueryParser::RIGHT_PARENTHESIS, 0);
}

CEQLQueryParser::Par_math_exprContext::Par_math_exprContext(Math_exprContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Par_math_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitPar_math_expr(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::Math_exprContext* CEQLQueryParser::math_expr() {
   return math_expr(0);
}

CEQLQueryParser::Math_exprContext* CEQLQueryParser::math_expr(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  CEQLQueryParser::Math_exprContext *_localctx = _tracker.createInstance<Math_exprContext>(_ctx, parentState);
  CEQLQueryParser::Math_exprContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 28;
  enterRecursionRule(_localctx, 28, CEQLQueryParser::RuleMath_expr, precedence);

    size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(320);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::LEFT_PARENTHESIS: {
        _localctx = _tracker.createInstance<Par_math_exprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(312);
        match(CEQLQueryParser::LEFT_PARENTHESIS);
        setState(313);
        math_expr(0);
        setState(314);
        match(CEQLQueryParser::RIGHT_PARENTHESIS);
        break;
      }

      case CEQLQueryParser::DOUBLE_LITERAL:
      case CEQLQueryParser::INTEGER_LITERAL: {
        _localctx = _tracker.createInstance<Number_math_exprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(316);
        number();
        break;
      }

      case CEQLQueryParser::IDENTIFIER: {
        _localctx = _tracker.createInstance<Attribute_math_exprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(317);
        attribute_name();
        break;
      }

      case CEQLQueryParser::PLUS:
      case CEQLQueryParser::MINUS: {
        _localctx = _tracker.createInstance<Unary_math_exprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(318);
        _la = _input->LA(1);
        if (!(_la == CEQLQueryParser::PLUS

        || _la == CEQLQueryParser::MINUS)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(319);
        math_expr(3);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(330);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 28, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(328);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 27, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<Mul_math_exprContext>(_tracker.createInstance<Math_exprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleMath_expr);
          setState(322);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(323);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & 26843545600) != 0))) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(324);
          math_expr(3);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<Sum_math_exprContext>(_tracker.createInstance<Math_exprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleMath_expr);
          setState(325);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(326);
          _la = _input->LA(1);
          if (!(_la == CEQLQueryParser::PLUS

          || _la == CEQLQueryParser::MINUS)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(327);
          math_expr(2);
          break;
        }

        default:
          break;
        } 
      }
      setState(332);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 28, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- Value_seqContext ------------------------------------------------------------------

CEQLQueryParser::Value_seqContext::Value_seqContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CEQLQueryParser::Value_seqContext::LEFT_CURLY_BRACKET() {
  return getToken(CEQLQueryParser::LEFT_CURLY_BRACKET, 0);
}

CEQLQueryParser::Number_seqContext* CEQLQueryParser::Value_seqContext::number_seq() {
  return getRuleContext<CEQLQueryParser::Number_seqContext>(0);
}

tree::TerminalNode* CEQLQueryParser::Value_seqContext::RIGHT_CURLY_BRACKET() {
  return getToken(CEQLQueryParser::RIGHT_CURLY_BRACKET, 0);
}

CEQLQueryParser::String_seqContext* CEQLQueryParser::Value_seqContext::string_seq() {
  return getRuleContext<CEQLQueryParser::String_seqContext>(0);
}


size_t CEQLQueryParser::Value_seqContext::getRuleIndex() const {
  return CEQLQueryParser::RuleValue_seq;
}


std::any CEQLQueryParser::Value_seqContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitValue_seq(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::Value_seqContext* CEQLQueryParser::value_seq() {
  Value_seqContext *_localctx = _tracker.createInstance<Value_seqContext>(_ctx, getState());
  enterRule(_localctx, 30, CEQLQueryParser::RuleValue_seq);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(341);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 29, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(333);
      match(CEQLQueryParser::LEFT_CURLY_BRACKET);
      setState(334);
      number_seq();
      setState(335);
      match(CEQLQueryParser::RIGHT_CURLY_BRACKET);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(337);
      match(CEQLQueryParser::LEFT_CURLY_BRACKET);
      setState(338);
      string_seq();
      setState(339);
      match(CEQLQueryParser::RIGHT_CURLY_BRACKET);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Number_seqContext ------------------------------------------------------------------

CEQLQueryParser::Number_seqContext::Number_seqContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CEQLQueryParser::Number_seqContext::getRuleIndex() const {
  return CEQLQueryParser::RuleNumber_seq;
}

void CEQLQueryParser::Number_seqContext::copyFrom(Number_seqContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Number_listContext ------------------------------------------------------------------

std::vector<CEQLQueryParser::NumberContext *> CEQLQueryParser::Number_listContext::number() {
  return getRuleContexts<CEQLQueryParser::NumberContext>();
}

CEQLQueryParser::NumberContext* CEQLQueryParser::Number_listContext::number(size_t i) {
  return getRuleContext<CEQLQueryParser::NumberContext>(i);
}

std::vector<tree::TerminalNode *> CEQLQueryParser::Number_listContext::COMMA() {
  return getTokens(CEQLQueryParser::COMMA);
}

tree::TerminalNode* CEQLQueryParser::Number_listContext::COMMA(size_t i) {
  return getToken(CEQLQueryParser::COMMA, i);
}

CEQLQueryParser::Number_listContext::Number_listContext(Number_seqContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Number_listContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitNumber_list(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Double_rangeContext ------------------------------------------------------------------

std::vector<CEQLQueryParser::DoubleContext *> CEQLQueryParser::Double_rangeContext::double_() {
  return getRuleContexts<CEQLQueryParser::DoubleContext>();
}

CEQLQueryParser::DoubleContext* CEQLQueryParser::Double_rangeContext::double_(size_t i) {
  return getRuleContext<CEQLQueryParser::DoubleContext>(i);
}

tree::TerminalNode* CEQLQueryParser::Double_rangeContext::DOUBLE_DOT() {
  return getToken(CEQLQueryParser::DOUBLE_DOT, 0);
}

CEQLQueryParser::Double_rangeContext::Double_rangeContext(Number_seqContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Double_rangeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitDouble_range(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Number_range_lowerContext ------------------------------------------------------------------

CEQLQueryParser::NumberContext* CEQLQueryParser::Number_range_lowerContext::number() {
  return getRuleContext<CEQLQueryParser::NumberContext>(0);
}

tree::TerminalNode* CEQLQueryParser::Number_range_lowerContext::DOUBLE_DOT() {
  return getToken(CEQLQueryParser::DOUBLE_DOT, 0);
}

CEQLQueryParser::Number_range_lowerContext::Number_range_lowerContext(Number_seqContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Number_range_lowerContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitNumber_range_lower(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Number_range_upperContext ------------------------------------------------------------------

tree::TerminalNode* CEQLQueryParser::Number_range_upperContext::DOUBLE_DOT() {
  return getToken(CEQLQueryParser::DOUBLE_DOT, 0);
}

CEQLQueryParser::NumberContext* CEQLQueryParser::Number_range_upperContext::number() {
  return getRuleContext<CEQLQueryParser::NumberContext>(0);
}

CEQLQueryParser::Number_range_upperContext::Number_range_upperContext(Number_seqContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Number_range_upperContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitNumber_range_upper(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Integer_rangeContext ------------------------------------------------------------------

std::vector<CEQLQueryParser::IntegerContext *> CEQLQueryParser::Integer_rangeContext::integer() {
  return getRuleContexts<CEQLQueryParser::IntegerContext>();
}

CEQLQueryParser::IntegerContext* CEQLQueryParser::Integer_rangeContext::integer(size_t i) {
  return getRuleContext<CEQLQueryParser::IntegerContext>(i);
}

tree::TerminalNode* CEQLQueryParser::Integer_rangeContext::DOUBLE_DOT() {
  return getToken(CEQLQueryParser::DOUBLE_DOT, 0);
}

CEQLQueryParser::Integer_rangeContext::Integer_rangeContext(Number_seqContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Integer_rangeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitInteger_range(this);
  else
    return visitor->visitChildren(this);
}
CEQLQueryParser::Number_seqContext* CEQLQueryParser::number_seq() {
  Number_seqContext *_localctx = _tracker.createInstance<Number_seqContext>(_ctx, getState());
  enterRule(_localctx, 32, CEQLQueryParser::RuleNumber_seq);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(364);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 31, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<CEQLQueryParser::Number_listContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(343);
      number();
      setState(348);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == CEQLQueryParser::COMMA) {
        setState(344);
        match(CEQLQueryParser::COMMA);
        setState(345);
        number();
        setState(350);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<CEQLQueryParser::Integer_rangeContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(351);
      integer();
      setState(352);
      match(CEQLQueryParser::DOUBLE_DOT);
      setState(353);
      integer();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<CEQLQueryParser::Double_rangeContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(355);
      double_();
      setState(356);
      match(CEQLQueryParser::DOUBLE_DOT);
      setState(357);
      double_();
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<CEQLQueryParser::Number_range_lowerContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(359);
      number();
      setState(360);
      match(CEQLQueryParser::DOUBLE_DOT);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<CEQLQueryParser::Number_range_upperContext>(_localctx);
      enterOuterAlt(_localctx, 5);
      setState(362);
      match(CEQLQueryParser::DOUBLE_DOT);
      setState(363);
      number();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- String_seqContext ------------------------------------------------------------------

CEQLQueryParser::String_seqContext::String_seqContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<CEQLQueryParser::StringContext *> CEQLQueryParser::String_seqContext::string() {
  return getRuleContexts<CEQLQueryParser::StringContext>();
}

CEQLQueryParser::StringContext* CEQLQueryParser::String_seqContext::string(size_t i) {
  return getRuleContext<CEQLQueryParser::StringContext>(i);
}

std::vector<tree::TerminalNode *> CEQLQueryParser::String_seqContext::COMMA() {
  return getTokens(CEQLQueryParser::COMMA);
}

tree::TerminalNode* CEQLQueryParser::String_seqContext::COMMA(size_t i) {
  return getToken(CEQLQueryParser::COMMA, i);
}


size_t CEQLQueryParser::String_seqContext::getRuleIndex() const {
  return CEQLQueryParser::RuleString_seq;
}


std::any CEQLQueryParser::String_seqContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitString_seq(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::String_seqContext* CEQLQueryParser::string_seq() {
  String_seqContext *_localctx = _tracker.createInstance<String_seqContext>(_ctx, getState());
  enterRule(_localctx, 34, CEQLQueryParser::RuleString_seq);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(366);
    string();
    setState(371);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CEQLQueryParser::COMMA) {
      setState(367);
      match(CEQLQueryParser::COMMA);
      setState(368);
      string();
      setState(373);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Time_windowContext ------------------------------------------------------------------

CEQLQueryParser::Time_windowContext::Time_windowContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQLQueryParser::Event_spanContext* CEQLQueryParser::Time_windowContext::event_span() {
  return getRuleContext<CEQLQueryParser::Event_spanContext>(0);
}

CEQLQueryParser::Time_spanContext* CEQLQueryParser::Time_windowContext::time_span() {
  return getRuleContext<CEQLQueryParser::Time_spanContext>(0);
}

CEQLQueryParser::Custom_spanContext* CEQLQueryParser::Time_windowContext::custom_span() {
  return getRuleContext<CEQLQueryParser::Custom_spanContext>(0);
}


size_t CEQLQueryParser::Time_windowContext::getRuleIndex() const {
  return CEQLQueryParser::RuleTime_window;
}


std::any CEQLQueryParser::Time_windowContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitTime_window(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::Time_windowContext* CEQLQueryParser::time_window() {
  Time_windowContext *_localctx = _tracker.createInstance<Time_windowContext>(_ctx, getState());
  enterRule(_localctx, 36, CEQLQueryParser::RuleTime_window);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(377);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 33, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(374);
      event_span();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(375);
      time_span();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(376);
      custom_span();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Event_spanContext ------------------------------------------------------------------

CEQLQueryParser::Event_spanContext::Event_spanContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQLQueryParser::IntegerContext* CEQLQueryParser::Event_spanContext::integer() {
  return getRuleContext<CEQLQueryParser::IntegerContext>(0);
}

tree::TerminalNode* CEQLQueryParser::Event_spanContext::K_EVENTS() {
  return getToken(CEQLQueryParser::K_EVENTS, 0);
}


size_t CEQLQueryParser::Event_spanContext::getRuleIndex() const {
  return CEQLQueryParser::RuleEvent_span;
}


std::any CEQLQueryParser::Event_spanContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitEvent_span(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::Event_spanContext* CEQLQueryParser::event_span() {
  Event_spanContext *_localctx = _tracker.createInstance<Event_spanContext>(_ctx, getState());
  enterRule(_localctx, 38, CEQLQueryParser::RuleEvent_span);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(379);
    integer();
    setState(380);
    match(CEQLQueryParser::K_EVENTS);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Time_spanContext ------------------------------------------------------------------

CEQLQueryParser::Time_spanContext::Time_spanContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQLQueryParser::Hour_spanContext* CEQLQueryParser::Time_spanContext::hour_span() {
  return getRuleContext<CEQLQueryParser::Hour_spanContext>(0);
}

CEQLQueryParser::Minute_spanContext* CEQLQueryParser::Time_spanContext::minute_span() {
  return getRuleContext<CEQLQueryParser::Minute_spanContext>(0);
}

CEQLQueryParser::Second_spanContext* CEQLQueryParser::Time_spanContext::second_span() {
  return getRuleContext<CEQLQueryParser::Second_spanContext>(0);
}


size_t CEQLQueryParser::Time_spanContext::getRuleIndex() const {
  return CEQLQueryParser::RuleTime_span;
}


std::any CEQLQueryParser::Time_spanContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitTime_span(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::Time_spanContext* CEQLQueryParser::time_span() {
  Time_spanContext *_localctx = _tracker.createInstance<Time_spanContext>(_ctx, getState());
  enterRule(_localctx, 40, CEQLQueryParser::RuleTime_span);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(383);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 34, _ctx)) {
    case 1: {
      setState(382);
      hour_span();
      break;
    }

    default:
      break;
    }
    setState(386);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 35, _ctx)) {
    case 1: {
      setState(385);
      minute_span();
      break;
    }

    default:
      break;
    }
    setState(389);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQLQueryParser::INTEGER_LITERAL) {
      setState(388);
      second_span();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Hour_spanContext ------------------------------------------------------------------

CEQLQueryParser::Hour_spanContext::Hour_spanContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQLQueryParser::IntegerContext* CEQLQueryParser::Hour_spanContext::integer() {
  return getRuleContext<CEQLQueryParser::IntegerContext>(0);
}

tree::TerminalNode* CEQLQueryParser::Hour_spanContext::K_HOURS() {
  return getToken(CEQLQueryParser::K_HOURS, 0);
}


size_t CEQLQueryParser::Hour_spanContext::getRuleIndex() const {
  return CEQLQueryParser::RuleHour_span;
}


std::any CEQLQueryParser::Hour_spanContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitHour_span(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::Hour_spanContext* CEQLQueryParser::hour_span() {
  Hour_spanContext *_localctx = _tracker.createInstance<Hour_spanContext>(_ctx, getState());
  enterRule(_localctx, 42, CEQLQueryParser::RuleHour_span);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(391);
    integer();
    setState(392);
    match(CEQLQueryParser::K_HOURS);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Minute_spanContext ------------------------------------------------------------------

CEQLQueryParser::Minute_spanContext::Minute_spanContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQLQueryParser::IntegerContext* CEQLQueryParser::Minute_spanContext::integer() {
  return getRuleContext<CEQLQueryParser::IntegerContext>(0);
}

tree::TerminalNode* CEQLQueryParser::Minute_spanContext::K_MINUTES() {
  return getToken(CEQLQueryParser::K_MINUTES, 0);
}


size_t CEQLQueryParser::Minute_spanContext::getRuleIndex() const {
  return CEQLQueryParser::RuleMinute_span;
}


std::any CEQLQueryParser::Minute_spanContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitMinute_span(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::Minute_spanContext* CEQLQueryParser::minute_span() {
  Minute_spanContext *_localctx = _tracker.createInstance<Minute_spanContext>(_ctx, getState());
  enterRule(_localctx, 44, CEQLQueryParser::RuleMinute_span);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(394);
    integer();
    setState(395);
    match(CEQLQueryParser::K_MINUTES);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Second_spanContext ------------------------------------------------------------------

CEQLQueryParser::Second_spanContext::Second_spanContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQLQueryParser::IntegerContext* CEQLQueryParser::Second_spanContext::integer() {
  return getRuleContext<CEQLQueryParser::IntegerContext>(0);
}

tree::TerminalNode* CEQLQueryParser::Second_spanContext::K_SECONDS() {
  return getToken(CEQLQueryParser::K_SECONDS, 0);
}


size_t CEQLQueryParser::Second_spanContext::getRuleIndex() const {
  return CEQLQueryParser::RuleSecond_span;
}


std::any CEQLQueryParser::Second_spanContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitSecond_span(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::Second_spanContext* CEQLQueryParser::second_span() {
  Second_spanContext *_localctx = _tracker.createInstance<Second_spanContext>(_ctx, getState());
  enterRule(_localctx, 46, CEQLQueryParser::RuleSecond_span);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(397);
    integer();
    setState(398);
    match(CEQLQueryParser::K_SECONDS);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Custom_spanContext ------------------------------------------------------------------

CEQLQueryParser::Custom_spanContext::Custom_spanContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQLQueryParser::IntegerContext* CEQLQueryParser::Custom_spanContext::integer() {
  return getRuleContext<CEQLQueryParser::IntegerContext>(0);
}

tree::TerminalNode* CEQLQueryParser::Custom_spanContext::LEFT_SQUARE_BRACKET() {
  return getToken(CEQLQueryParser::LEFT_SQUARE_BRACKET, 0);
}

CEQLQueryParser::Any_nameContext* CEQLQueryParser::Custom_spanContext::any_name() {
  return getRuleContext<CEQLQueryParser::Any_nameContext>(0);
}

tree::TerminalNode* CEQLQueryParser::Custom_spanContext::RIGHT_SQUARE_BRACKET() {
  return getToken(CEQLQueryParser::RIGHT_SQUARE_BRACKET, 0);
}


size_t CEQLQueryParser::Custom_spanContext::getRuleIndex() const {
  return CEQLQueryParser::RuleCustom_span;
}


std::any CEQLQueryParser::Custom_spanContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitCustom_span(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::Custom_spanContext* CEQLQueryParser::custom_span() {
  Custom_spanContext *_localctx = _tracker.createInstance<Custom_spanContext>(_ctx, getState());
  enterRule(_localctx, 48, CEQLQueryParser::RuleCustom_span);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(400);
    integer();
    setState(401);
    match(CEQLQueryParser::LEFT_SQUARE_BRACKET);
    setState(402);
    any_name();
    setState(403);
    match(CEQLQueryParser::RIGHT_SQUARE_BRACKET);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Named_eventContext ------------------------------------------------------------------

CEQLQueryParser::Named_eventContext::Named_eventContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQLQueryParser::S_event_nameContext* CEQLQueryParser::Named_eventContext::s_event_name() {
  return getRuleContext<CEQLQueryParser::S_event_nameContext>(0);
}

tree::TerminalNode* CEQLQueryParser::Named_eventContext::K_AS() {
  return getToken(CEQLQueryParser::K_AS, 0);
}

CEQLQueryParser::Event_nameContext* CEQLQueryParser::Named_eventContext::event_name() {
  return getRuleContext<CEQLQueryParser::Event_nameContext>(0);
}


size_t CEQLQueryParser::Named_eventContext::getRuleIndex() const {
  return CEQLQueryParser::RuleNamed_event;
}


std::any CEQLQueryParser::Named_eventContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitNamed_event(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::Named_eventContext* CEQLQueryParser::named_event() {
  Named_eventContext *_localctx = _tracker.createInstance<Named_eventContext>(_ctx, getState());
  enterRule(_localctx, 50, CEQLQueryParser::RuleNamed_event);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(405);
    s_event_name();
    setState(408);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQLQueryParser::K_AS) {
      setState(406);
      match(CEQLQueryParser::K_AS);
      setState(407);
      event_name();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- S_event_nameContext ------------------------------------------------------------------

CEQLQueryParser::S_event_nameContext::S_event_nameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQLQueryParser::Event_nameContext* CEQLQueryParser::S_event_nameContext::event_name() {
  return getRuleContext<CEQLQueryParser::Event_nameContext>(0);
}

CEQLQueryParser::Stream_nameContext* CEQLQueryParser::S_event_nameContext::stream_name() {
  return getRuleContext<CEQLQueryParser::Stream_nameContext>(0);
}

tree::TerminalNode* CEQLQueryParser::S_event_nameContext::GE() {
  return getToken(CEQLQueryParser::GE, 0);
}


size_t CEQLQueryParser::S_event_nameContext::getRuleIndex() const {
  return CEQLQueryParser::RuleS_event_name;
}


std::any CEQLQueryParser::S_event_nameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitS_event_name(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::S_event_nameContext* CEQLQueryParser::s_event_name() {
  S_event_nameContext *_localctx = _tracker.createInstance<S_event_nameContext>(_ctx, getState());
  enterRule(_localctx, 52, CEQLQueryParser::RuleS_event_name);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(413);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 38, _ctx)) {
    case 1: {
      setState(410);
      stream_name();
      setState(411);
      match(CEQLQueryParser::GE);
      break;
    }

    default:
      break;
    }
    setState(415);
    event_name();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Event_nameContext ------------------------------------------------------------------

CEQLQueryParser::Event_nameContext::Event_nameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQLQueryParser::Any_nameContext* CEQLQueryParser::Event_nameContext::any_name() {
  return getRuleContext<CEQLQueryParser::Any_nameContext>(0);
}


size_t CEQLQueryParser::Event_nameContext::getRuleIndex() const {
  return CEQLQueryParser::RuleEvent_name;
}


std::any CEQLQueryParser::Event_nameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitEvent_name(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::Event_nameContext* CEQLQueryParser::event_name() {
  Event_nameContext *_localctx = _tracker.createInstance<Event_nameContext>(_ctx, getState());
  enterRule(_localctx, 54, CEQLQueryParser::RuleEvent_name);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(417);
    any_name();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Stream_nameContext ------------------------------------------------------------------

CEQLQueryParser::Stream_nameContext::Stream_nameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQLQueryParser::Any_nameContext* CEQLQueryParser::Stream_nameContext::any_name() {
  return getRuleContext<CEQLQueryParser::Any_nameContext>(0);
}


size_t CEQLQueryParser::Stream_nameContext::getRuleIndex() const {
  return CEQLQueryParser::RuleStream_name;
}


std::any CEQLQueryParser::Stream_nameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitStream_name(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::Stream_nameContext* CEQLQueryParser::stream_name() {
  Stream_nameContext *_localctx = _tracker.createInstance<Stream_nameContext>(_ctx, getState());
  enterRule(_localctx, 56, CEQLQueryParser::RuleStream_name);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(419);
    any_name();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Attribute_nameContext ------------------------------------------------------------------

CEQLQueryParser::Attribute_nameContext::Attribute_nameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQLQueryParser::Any_nameContext* CEQLQueryParser::Attribute_nameContext::any_name() {
  return getRuleContext<CEQLQueryParser::Any_nameContext>(0);
}


size_t CEQLQueryParser::Attribute_nameContext::getRuleIndex() const {
  return CEQLQueryParser::RuleAttribute_name;
}


std::any CEQLQueryParser::Attribute_nameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitAttribute_name(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::Attribute_nameContext* CEQLQueryParser::attribute_name() {
  Attribute_nameContext *_localctx = _tracker.createInstance<Attribute_nameContext>(_ctx, getState());
  enterRule(_localctx, 58, CEQLQueryParser::RuleAttribute_name);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(421);
    any_name();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IntegerContext ------------------------------------------------------------------

CEQLQueryParser::IntegerContext::IntegerContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CEQLQueryParser::IntegerContext::INTEGER_LITERAL() {
  return getToken(CEQLQueryParser::INTEGER_LITERAL, 0);
}


size_t CEQLQueryParser::IntegerContext::getRuleIndex() const {
  return CEQLQueryParser::RuleInteger;
}


std::any CEQLQueryParser::IntegerContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitInteger(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::IntegerContext* CEQLQueryParser::integer() {
  IntegerContext *_localctx = _tracker.createInstance<IntegerContext>(_ctx, getState());
  enterRule(_localctx, 60, CEQLQueryParser::RuleInteger);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(423);
    match(CEQLQueryParser::INTEGER_LITERAL);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DoubleContext ------------------------------------------------------------------

CEQLQueryParser::DoubleContext::DoubleContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CEQLQueryParser::DoubleContext::DOUBLE_LITERAL() {
  return getToken(CEQLQueryParser::DOUBLE_LITERAL, 0);
}


size_t CEQLQueryParser::DoubleContext::getRuleIndex() const {
  return CEQLQueryParser::RuleDouble;
}


std::any CEQLQueryParser::DoubleContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitDouble(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::DoubleContext* CEQLQueryParser::double_() {
  DoubleContext *_localctx = _tracker.createInstance<DoubleContext>(_ctx, getState());
  enterRule(_localctx, 62, CEQLQueryParser::RuleDouble);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(425);
    match(CEQLQueryParser::DOUBLE_LITERAL);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NumberContext ------------------------------------------------------------------

CEQLQueryParser::NumberContext::NumberContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQLQueryParser::IntegerContext* CEQLQueryParser::NumberContext::integer() {
  return getRuleContext<CEQLQueryParser::IntegerContext>(0);
}

CEQLQueryParser::DoubleContext* CEQLQueryParser::NumberContext::double_() {
  return getRuleContext<CEQLQueryParser::DoubleContext>(0);
}


size_t CEQLQueryParser::NumberContext::getRuleIndex() const {
  return CEQLQueryParser::RuleNumber;
}


std::any CEQLQueryParser::NumberContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitNumber(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::NumberContext* CEQLQueryParser::number() {
  NumberContext *_localctx = _tracker.createInstance<NumberContext>(_ctx, getState());
  enterRule(_localctx, 64, CEQLQueryParser::RuleNumber);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(429);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::INTEGER_LITERAL: {
        enterOuterAlt(_localctx, 1);
        setState(427);
        integer();
        break;
      }

      case CEQLQueryParser::DOUBLE_LITERAL: {
        enterOuterAlt(_localctx, 2);
        setState(428);
        double_();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StringContext ------------------------------------------------------------------

CEQLQueryParser::StringContext::StringContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CEQLQueryParser::StringContext::STRING_LITERAL() {
  return getToken(CEQLQueryParser::STRING_LITERAL, 0);
}


size_t CEQLQueryParser::StringContext::getRuleIndex() const {
  return CEQLQueryParser::RuleString;
}


std::any CEQLQueryParser::StringContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitString(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::StringContext* CEQLQueryParser::string() {
  StringContext *_localctx = _tracker.createInstance<StringContext>(_ctx, getState());
  enterRule(_localctx, 66, CEQLQueryParser::RuleString);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(431);
    match(CEQLQueryParser::STRING_LITERAL);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Any_nameContext ------------------------------------------------------------------

CEQLQueryParser::Any_nameContext::Any_nameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CEQLQueryParser::Any_nameContext::IDENTIFIER() {
  return getToken(CEQLQueryParser::IDENTIFIER, 0);
}


size_t CEQLQueryParser::Any_nameContext::getRuleIndex() const {
  return CEQLQueryParser::RuleAny_name;
}


std::any CEQLQueryParser::Any_nameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitAny_name(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::Any_nameContext* CEQLQueryParser::any_name() {
  Any_nameContext *_localctx = _tracker.createInstance<Any_nameContext>(_ctx, getState());
  enterRule(_localctx, 68, CEQLQueryParser::RuleAny_name);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(433);
    match(CEQLQueryParser::IDENTIFIER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- KeywordContext ------------------------------------------------------------------

CEQLQueryParser::KeywordContext::KeywordContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CEQLQueryParser::KeywordContext::K_ALL() {
  return getToken(CEQLQueryParser::K_ALL, 0);
}

tree::TerminalNode* CEQLQueryParser::KeywordContext::K_AND() {
  return getToken(CEQLQueryParser::K_AND, 0);
}

tree::TerminalNode* CEQLQueryParser::KeywordContext::K_ANY() {
  return getToken(CEQLQueryParser::K_ANY, 0);
}

tree::TerminalNode* CEQLQueryParser::KeywordContext::K_AS() {
  return getToken(CEQLQueryParser::K_AS, 0);
}

tree::TerminalNode* CEQLQueryParser::KeywordContext::K_BY() {
  return getToken(CEQLQueryParser::K_BY, 0);
}

tree::TerminalNode* CEQLQueryParser::KeywordContext::K_CONSUME() {
  return getToken(CEQLQueryParser::K_CONSUME, 0);
}

tree::TerminalNode* CEQLQueryParser::KeywordContext::K_DISTINCT() {
  return getToken(CEQLQueryParser::K_DISTINCT, 0);
}

tree::TerminalNode* CEQLQueryParser::KeywordContext::K_EVENT() {
  return getToken(CEQLQueryParser::K_EVENT, 0);
}

tree::TerminalNode* CEQLQueryParser::KeywordContext::K_EVENTS() {
  return getToken(CEQLQueryParser::K_EVENTS, 0);
}

tree::TerminalNode* CEQLQueryParser::KeywordContext::K_FILTER() {
  return getToken(CEQLQueryParser::K_FILTER, 0);
}

tree::TerminalNode* CEQLQueryParser::KeywordContext::K_FROM() {
  return getToken(CEQLQueryParser::K_FROM, 0);
}

tree::TerminalNode* CEQLQueryParser::KeywordContext::K_HOURS() {
  return getToken(CEQLQueryParser::K_HOURS, 0);
}

tree::TerminalNode* CEQLQueryParser::KeywordContext::K_IN() {
  return getToken(CEQLQueryParser::K_IN, 0);
}

tree::TerminalNode* CEQLQueryParser::KeywordContext::K_LAST() {
  return getToken(CEQLQueryParser::K_LAST, 0);
}

tree::TerminalNode* CEQLQueryParser::KeywordContext::K_LIKE() {
  return getToken(CEQLQueryParser::K_LIKE, 0);
}

tree::TerminalNode* CEQLQueryParser::KeywordContext::K_MAX() {
  return getToken(CEQLQueryParser::K_MAX, 0);
}

tree::TerminalNode* CEQLQueryParser::KeywordContext::K_MINUTES() {
  return getToken(CEQLQueryParser::K_MINUTES, 0);
}

tree::TerminalNode* CEQLQueryParser::KeywordContext::K_NEXT() {
  return getToken(CEQLQueryParser::K_NEXT, 0);
}

tree::TerminalNode* CEQLQueryParser::KeywordContext::K_NONE() {
  return getToken(CEQLQueryParser::K_NONE, 0);
}

tree::TerminalNode* CEQLQueryParser::KeywordContext::K_NOT() {
  return getToken(CEQLQueryParser::K_NOT, 0);
}

tree::TerminalNode* CEQLQueryParser::KeywordContext::K_OR() {
  return getToken(CEQLQueryParser::K_OR, 0);
}

tree::TerminalNode* CEQLQueryParser::KeywordContext::K_PARTITION() {
  return getToken(CEQLQueryParser::K_PARTITION, 0);
}

tree::TerminalNode* CEQLQueryParser::KeywordContext::K_SECONDS() {
  return getToken(CEQLQueryParser::K_SECONDS, 0);
}

tree::TerminalNode* CEQLQueryParser::KeywordContext::K_SELECT() {
  return getToken(CEQLQueryParser::K_SELECT, 0);
}

tree::TerminalNode* CEQLQueryParser::KeywordContext::K_STREAM() {
  return getToken(CEQLQueryParser::K_STREAM, 0);
}

tree::TerminalNode* CEQLQueryParser::KeywordContext::K_STRICT() {
  return getToken(CEQLQueryParser::K_STRICT, 0);
}

tree::TerminalNode* CEQLQueryParser::KeywordContext::K_UNLESS() {
  return getToken(CEQLQueryParser::K_UNLESS, 0);
}

tree::TerminalNode* CEQLQueryParser::KeywordContext::K_WHERE() {
  return getToken(CEQLQueryParser::K_WHERE, 0);
}

tree::TerminalNode* CEQLQueryParser::KeywordContext::K_WITHIN() {
  return getToken(CEQLQueryParser::K_WITHIN, 0);
}


size_t CEQLQueryParser::KeywordContext::getRuleIndex() const {
  return CEQLQueryParser::RuleKeyword;
}


std::any CEQLQueryParser::KeywordContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitKeyword(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::KeywordContext* CEQLQueryParser::keyword() {
  KeywordContext *_localctx = _tracker.createInstance<KeywordContext>(_ctx, getState());
  enterRule(_localctx, 70, CEQLQueryParser::RuleKeyword);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(435);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 1073741822) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RegexpContext ------------------------------------------------------------------

CEQLQueryParser::RegexpContext::RegexpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CEQLQueryParser::RegexpContext::REGEX_START() {
  return getToken(CEQLQueryParser::REGEX_START, 0);
}

CEQLQueryParser::Regexp_alternationContext* CEQLQueryParser::RegexpContext::regexp_alternation() {
  return getRuleContext<CEQLQueryParser::Regexp_alternationContext>(0);
}

tree::TerminalNode* CEQLQueryParser::RegexpContext::REGEX_END() {
  return getToken(CEQLQueryParser::REGEX_END, 0);
}


size_t CEQLQueryParser::RegexpContext::getRuleIndex() const {
  return CEQLQueryParser::RuleRegexp;
}


std::any CEQLQueryParser::RegexpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitRegexp(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::RegexpContext* CEQLQueryParser::regexp() {
  RegexpContext *_localctx = _tracker.createInstance<RegexpContext>(_ctx, getState());
  enterRule(_localctx, 72, CEQLQueryParser::RuleRegexp);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(437);
    match(CEQLQueryParser::REGEX_START);
    setState(438);
    regexp_alternation();
    setState(439);
    match(CEQLQueryParser::REGEX_END);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Regexp_alternationContext ------------------------------------------------------------------

CEQLQueryParser::Regexp_alternationContext::Regexp_alternationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<CEQLQueryParser::Regexp_expContext *> CEQLQueryParser::Regexp_alternationContext::regexp_exp() {
  return getRuleContexts<CEQLQueryParser::Regexp_expContext>();
}

CEQLQueryParser::Regexp_expContext* CEQLQueryParser::Regexp_alternationContext::regexp_exp(size_t i) {
  return getRuleContext<CEQLQueryParser::Regexp_expContext>(i);
}

std::vector<tree::TerminalNode *> CEQLQueryParser::Regexp_alternationContext::REGEX_PIPE() {
  return getTokens(CEQLQueryParser::REGEX_PIPE);
}

tree::TerminalNode* CEQLQueryParser::Regexp_alternationContext::REGEX_PIPE(size_t i) {
  return getToken(CEQLQueryParser::REGEX_PIPE, i);
}


size_t CEQLQueryParser::Regexp_alternationContext::getRuleIndex() const {
  return CEQLQueryParser::RuleRegexp_alternation;
}


std::any CEQLQueryParser::Regexp_alternationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitRegexp_alternation(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::Regexp_alternationContext* CEQLQueryParser::regexp_alternation() {
  Regexp_alternationContext *_localctx = _tracker.createInstance<Regexp_alternationContext>(_ctx, getState());
  enterRule(_localctx, 74, CEQLQueryParser::RuleRegexp_alternation);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(441);
    regexp_exp();
    setState(446);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CEQLQueryParser::REGEX_PIPE) {
      setState(442);
      match(CEQLQueryParser::REGEX_PIPE);
      setState(443);
      regexp_exp();
      setState(448);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Regexp_expContext ------------------------------------------------------------------

CEQLQueryParser::Regexp_expContext::Regexp_expContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<CEQLQueryParser::Regexp_elementContext *> CEQLQueryParser::Regexp_expContext::regexp_element() {
  return getRuleContexts<CEQLQueryParser::Regexp_elementContext>();
}

CEQLQueryParser::Regexp_elementContext* CEQLQueryParser::Regexp_expContext::regexp_element(size_t i) {
  return getRuleContext<CEQLQueryParser::Regexp_elementContext>(i);
}


size_t CEQLQueryParser::Regexp_expContext::getRuleIndex() const {
  return CEQLQueryParser::RuleRegexp_exp;
}


std::any CEQLQueryParser::Regexp_expContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitRegexp_exp(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::Regexp_expContext* CEQLQueryParser::regexp_exp() {
  Regexp_expContext *_localctx = _tracker.createInstance<Regexp_expContext>(_ctx, getState());
  enterRule(_localctx, 76, CEQLQueryParser::RuleRegexp_exp);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(450); 
    _errHandler->sync(this);
    alt = 1;
    do {
      switch (alt) {
        case 1: {
              setState(449);
              regexp_element();
              break;
            }

      default:
        throw NoViableAltException(this);
      }
      setState(452); 
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 41, _ctx);
    } while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Regexp_elementContext ------------------------------------------------------------------

CEQLQueryParser::Regexp_elementContext::Regexp_elementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQLQueryParser::Regexp_groupContext* CEQLQueryParser::Regexp_elementContext::regexp_group() {
  return getRuleContext<CEQLQueryParser::Regexp_groupContext>(0);
}

CEQLQueryParser::QuantifierContext* CEQLQueryParser::Regexp_elementContext::quantifier() {
  return getRuleContext<CEQLQueryParser::QuantifierContext>(0);
}


size_t CEQLQueryParser::Regexp_elementContext::getRuleIndex() const {
  return CEQLQueryParser::RuleRegexp_element;
}


std::any CEQLQueryParser::Regexp_elementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitRegexp_element(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::Regexp_elementContext* CEQLQueryParser::regexp_element() {
  Regexp_elementContext *_localctx = _tracker.createInstance<Regexp_elementContext>(_ctx, getState());
  enterRule(_localctx, 78, CEQLQueryParser::RuleRegexp_element);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(454);
    regexp_group();
    setState(456);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 65) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 65)) & 225) != 0)) {
      setState(455);
      quantifier();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Regexp_groupContext ------------------------------------------------------------------

CEQLQueryParser::Regexp_groupContext::Regexp_groupContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQLQueryParser::ParenthesisContext* CEQLQueryParser::Regexp_groupContext::parenthesis() {
  return getRuleContext<CEQLQueryParser::ParenthesisContext>(0);
}

CEQLQueryParser::AtomContext* CEQLQueryParser::Regexp_groupContext::atom() {
  return getRuleContext<CEQLQueryParser::AtomContext>(0);
}


size_t CEQLQueryParser::Regexp_groupContext::getRuleIndex() const {
  return CEQLQueryParser::RuleRegexp_group;
}


std::any CEQLQueryParser::Regexp_groupContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitRegexp_group(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::Regexp_groupContext* CEQLQueryParser::regexp_group() {
  Regexp_groupContext *_localctx = _tracker.createInstance<Regexp_groupContext>(_ctx, getState());
  enterRule(_localctx, 80, CEQLQueryParser::RuleRegexp_group);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(460);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::REGEX_L_PAR: {
        enterOuterAlt(_localctx, 1);
        setState(458);
        parenthesis();
        break;
      }

      case CEQLQueryParser::K_ALL:
      case CEQLQueryParser::K_AND:
      case CEQLQueryParser::K_ANY:
      case CEQLQueryParser::K_AS:
      case CEQLQueryParser::K_BY:
      case CEQLQueryParser::K_CONSUME:
      case CEQLQueryParser::K_DISTINCT:
      case CEQLQueryParser::K_EVENT:
      case CEQLQueryParser::K_EVENTS:
      case CEQLQueryParser::K_FILTER:
      case CEQLQueryParser::K_FROM:
      case CEQLQueryParser::K_HOURS:
      case CEQLQueryParser::K_IN:
      case CEQLQueryParser::K_LAST:
      case CEQLQueryParser::K_LIKE:
      case CEQLQueryParser::K_MAX:
      case CEQLQueryParser::K_MINUTES:
      case CEQLQueryParser::K_NEXT:
      case CEQLQueryParser::K_NONE:
      case CEQLQueryParser::K_NOT:
      case CEQLQueryParser::K_OR:
      case CEQLQueryParser::K_PARTITION:
      case CEQLQueryParser::K_SECONDS:
      case CEQLQueryParser::K_SELECT:
      case CEQLQueryParser::K_STREAM:
      case CEQLQueryParser::K_STRICT:
      case CEQLQueryParser::K_UNLESS:
      case CEQLQueryParser::K_WHERE:
      case CEQLQueryParser::K_WITHIN:
      case CEQLQueryParser::PERCENT:
      case CEQLQueryParser::PLUS:
      case CEQLQueryParser::MINUS:
      case CEQLQueryParser::STAR:
      case CEQLQueryParser::SLASH:
      case CEQLQueryParser::LE:
      case CEQLQueryParser::LEQ:
      case CEQLQueryParser::GE:
      case CEQLQueryParser::GEQ:
      case CEQLQueryParser::EQ:
      case CEQLQueryParser::NEQ:
      case CEQLQueryParser::SEMICOLON:
      case CEQLQueryParser::COLON:
      case CEQLQueryParser::COMMA:
      case CEQLQueryParser::DOUBLE_DOT:
      case CEQLQueryParser::LEFT_PARENTHESIS:
      case CEQLQueryParser::RIGHT_PARENTHESIS:
      case CEQLQueryParser::LEFT_SQUARE_BRACKET:
      case CEQLQueryParser::RIGHT_SQUARE_BRACKET:
      case CEQLQueryParser::LEFT_CURLY_BRACKET:
      case CEQLQueryParser::RIGHT_CURLY_BRACKET:
      case CEQLQueryParser::IDENTIFIER:
      case CEQLQueryParser::DOUBLE_LITERAL:
      case CEQLQueryParser::INTEGER_LITERAL:
      case CEQLQueryParser::NUMERICAL_EXPONENT:
      case CEQLQueryParser::STRING_LITERAL:
      case CEQLQueryParser::SINGLE_LINE_COMMENT:
      case CEQLQueryParser::MULTILINE_COMMENT:
      case CEQLQueryParser::SPACES:
      case CEQLQueryParser::UNEXPECTED_CHAR:
      case CEQLQueryParser::REGEX_START:
      case CEQLQueryParser::REGEX_END:
      case CEQLQueryParser::REGEX_END_ESCAPED:
      case CEQLQueryParser::REGEX_EXCLAMAITON:
      case CEQLQueryParser::REGEX_COMMA:
      case CEQLQueryParser::REGEX_HAT:
      case CEQLQueryParser::REGEX_HYPHEN:
      case CEQLQueryParser::REGEX_L_BRACK:
      case CEQLQueryParser::REGEX_BACKSLASH:
      case CEQLQueryParser::REGEX_ALPHA:
      case CEQLQueryParser::REGEX_DOT:
      case CEQLQueryParser::REGEX_DOUBLED_DOT:
      case CEQLQueryParser::UNRECOGNIZED:
      case CEQLQueryParser::REGEX_DECIMAL_DIGIT:
      case CEQLQueryParser::REGEX_NOT_DECIMAL_DIGIT:
      case CEQLQueryParser::REGEX_WHITESPACE:
      case CEQLQueryParser::REGEX_NOT_WHITESPACE:
      case CEQLQueryParser::REGEX_ALPHANUMERIC:
      case CEQLQueryParser::REGEX_NOT_ALPHANUMERIC:
      case CEQLQueryParser::REGEX_DIGIT: {
        enterOuterAlt(_localctx, 2);
        setState(459);
        atom();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParenthesisContext ------------------------------------------------------------------

CEQLQueryParser::ParenthesisContext::ParenthesisContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CEQLQueryParser::ParenthesisContext::REGEX_L_PAR() {
  return getToken(CEQLQueryParser::REGEX_L_PAR, 0);
}

CEQLQueryParser::Regexp_alternationContext* CEQLQueryParser::ParenthesisContext::regexp_alternation() {
  return getRuleContext<CEQLQueryParser::Regexp_alternationContext>(0);
}

tree::TerminalNode* CEQLQueryParser::ParenthesisContext::REGEX_R_PAR() {
  return getToken(CEQLQueryParser::REGEX_R_PAR, 0);
}


size_t CEQLQueryParser::ParenthesisContext::getRuleIndex() const {
  return CEQLQueryParser::RuleParenthesis;
}


std::any CEQLQueryParser::ParenthesisContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitParenthesis(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::ParenthesisContext* CEQLQueryParser::parenthesis() {
  ParenthesisContext *_localctx = _tracker.createInstance<ParenthesisContext>(_ctx, getState());
  enterRule(_localctx, 82, CEQLQueryParser::RuleParenthesis);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(462);
    match(CEQLQueryParser::REGEX_L_PAR);
    setState(463);
    regexp_alternation();
    setState(464);
    match(CEQLQueryParser::REGEX_R_PAR);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- QuantifierContext ------------------------------------------------------------------

CEQLQueryParser::QuantifierContext::QuantifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CEQLQueryParser::QuantifierContext::REGEX_QUESTION() {
  return getToken(CEQLQueryParser::REGEX_QUESTION, 0);
}

tree::TerminalNode* CEQLQueryParser::QuantifierContext::REGEX_PLUS() {
  return getToken(CEQLQueryParser::REGEX_PLUS, 0);
}

tree::TerminalNode* CEQLQueryParser::QuantifierContext::REGEX_STAR() {
  return getToken(CEQLQueryParser::REGEX_STAR, 0);
}

tree::TerminalNode* CEQLQueryParser::QuantifierContext::REGEX_L_CURLY() {
  return getToken(CEQLQueryParser::REGEX_L_CURLY, 0);
}

CEQLQueryParser::QuantityContext* CEQLQueryParser::QuantifierContext::quantity() {
  return getRuleContext<CEQLQueryParser::QuantityContext>(0);
}

tree::TerminalNode* CEQLQueryParser::QuantifierContext::REGEX_R_CURLY() {
  return getToken(CEQLQueryParser::REGEX_R_CURLY, 0);
}


size_t CEQLQueryParser::QuantifierContext::getRuleIndex() const {
  return CEQLQueryParser::RuleQuantifier;
}


std::any CEQLQueryParser::QuantifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitQuantifier(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::QuantifierContext* CEQLQueryParser::quantifier() {
  QuantifierContext *_localctx = _tracker.createInstance<QuantifierContext>(_ctx, getState());
  enterRule(_localctx, 84, CEQLQueryParser::RuleQuantifier);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(473);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::REGEX_QUESTION: {
        enterOuterAlt(_localctx, 1);
        setState(466);
        match(CEQLQueryParser::REGEX_QUESTION);
        break;
      }

      case CEQLQueryParser::REGEX_PLUS: {
        enterOuterAlt(_localctx, 2);
        setState(467);
        match(CEQLQueryParser::REGEX_PLUS);
        break;
      }

      case CEQLQueryParser::REGEX_STAR: {
        enterOuterAlt(_localctx, 3);
        setState(468);
        match(CEQLQueryParser::REGEX_STAR);
        break;
      }

      case CEQLQueryParser::REGEX_L_CURLY: {
        enterOuterAlt(_localctx, 4);
        setState(469);
        match(CEQLQueryParser::REGEX_L_CURLY);
        setState(470);
        quantity();
        setState(471);
        match(CEQLQueryParser::REGEX_R_CURLY);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- QuantityContext ------------------------------------------------------------------

CEQLQueryParser::QuantityContext::QuantityContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQLQueryParser::QuantExactContext* CEQLQueryParser::QuantityContext::quantExact() {
  return getRuleContext<CEQLQueryParser::QuantExactContext>(0);
}

CEQLQueryParser::QuantRangeContext* CEQLQueryParser::QuantityContext::quantRange() {
  return getRuleContext<CEQLQueryParser::QuantRangeContext>(0);
}

CEQLQueryParser::QuantMinContext* CEQLQueryParser::QuantityContext::quantMin() {
  return getRuleContext<CEQLQueryParser::QuantMinContext>(0);
}

CEQLQueryParser::QuantMaxContext* CEQLQueryParser::QuantityContext::quantMax() {
  return getRuleContext<CEQLQueryParser::QuantMaxContext>(0);
}


size_t CEQLQueryParser::QuantityContext::getRuleIndex() const {
  return CEQLQueryParser::RuleQuantity;
}


std::any CEQLQueryParser::QuantityContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitQuantity(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::QuantityContext* CEQLQueryParser::quantity() {
  QuantityContext *_localctx = _tracker.createInstance<QuantityContext>(_ctx, getState());
  enterRule(_localctx, 86, CEQLQueryParser::RuleQuantity);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(479);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 45, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(475);
      quantExact();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(476);
      quantRange();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(477);
      quantMin();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(478);
      quantMax();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- QuantExactContext ------------------------------------------------------------------

CEQLQueryParser::QuantExactContext::QuantExactContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQLQueryParser::Regexp_numberContext* CEQLQueryParser::QuantExactContext::regexp_number() {
  return getRuleContext<CEQLQueryParser::Regexp_numberContext>(0);
}


size_t CEQLQueryParser::QuantExactContext::getRuleIndex() const {
  return CEQLQueryParser::RuleQuantExact;
}


std::any CEQLQueryParser::QuantExactContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitQuantExact(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::QuantExactContext* CEQLQueryParser::quantExact() {
  QuantExactContext *_localctx = _tracker.createInstance<QuantExactContext>(_ctx, getState());
  enterRule(_localctx, 88, CEQLQueryParser::RuleQuantExact);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(481);
    regexp_number();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- QuantRangeContext ------------------------------------------------------------------

CEQLQueryParser::QuantRangeContext::QuantRangeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<CEQLQueryParser::Regexp_numberContext *> CEQLQueryParser::QuantRangeContext::regexp_number() {
  return getRuleContexts<CEQLQueryParser::Regexp_numberContext>();
}

CEQLQueryParser::Regexp_numberContext* CEQLQueryParser::QuantRangeContext::regexp_number(size_t i) {
  return getRuleContext<CEQLQueryParser::Regexp_numberContext>(i);
}

tree::TerminalNode* CEQLQueryParser::QuantRangeContext::REGEX_COMMA() {
  return getToken(CEQLQueryParser::REGEX_COMMA, 0);
}


size_t CEQLQueryParser::QuantRangeContext::getRuleIndex() const {
  return CEQLQueryParser::RuleQuantRange;
}


std::any CEQLQueryParser::QuantRangeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitQuantRange(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::QuantRangeContext* CEQLQueryParser::quantRange() {
  QuantRangeContext *_localctx = _tracker.createInstance<QuantRangeContext>(_ctx, getState());
  enterRule(_localctx, 90, CEQLQueryParser::RuleQuantRange);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(483);
    regexp_number();
    setState(484);
    match(CEQLQueryParser::REGEX_COMMA);
    setState(485);
    regexp_number();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- QuantMinContext ------------------------------------------------------------------

CEQLQueryParser::QuantMinContext::QuantMinContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQLQueryParser::Regexp_numberContext* CEQLQueryParser::QuantMinContext::regexp_number() {
  return getRuleContext<CEQLQueryParser::Regexp_numberContext>(0);
}

tree::TerminalNode* CEQLQueryParser::QuantMinContext::REGEX_COMMA() {
  return getToken(CEQLQueryParser::REGEX_COMMA, 0);
}


size_t CEQLQueryParser::QuantMinContext::getRuleIndex() const {
  return CEQLQueryParser::RuleQuantMin;
}


std::any CEQLQueryParser::QuantMinContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitQuantMin(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::QuantMinContext* CEQLQueryParser::quantMin() {
  QuantMinContext *_localctx = _tracker.createInstance<QuantMinContext>(_ctx, getState());
  enterRule(_localctx, 92, CEQLQueryParser::RuleQuantMin);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(487);
    regexp_number();
    setState(488);
    match(CEQLQueryParser::REGEX_COMMA);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- QuantMaxContext ------------------------------------------------------------------

CEQLQueryParser::QuantMaxContext::QuantMaxContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CEQLQueryParser::QuantMaxContext::REGEX_COMMA() {
  return getToken(CEQLQueryParser::REGEX_COMMA, 0);
}

CEQLQueryParser::Regexp_numberContext* CEQLQueryParser::QuantMaxContext::regexp_number() {
  return getRuleContext<CEQLQueryParser::Regexp_numberContext>(0);
}


size_t CEQLQueryParser::QuantMaxContext::getRuleIndex() const {
  return CEQLQueryParser::RuleQuantMax;
}


std::any CEQLQueryParser::QuantMaxContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitQuantMax(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::QuantMaxContext* CEQLQueryParser::quantMax() {
  QuantMaxContext *_localctx = _tracker.createInstance<QuantMaxContext>(_ctx, getState());
  enterRule(_localctx, 94, CEQLQueryParser::RuleQuantMax);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(490);
    match(CEQLQueryParser::REGEX_COMMA);
    setState(491);
    regexp_number();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AtomContext ------------------------------------------------------------------

CEQLQueryParser::AtomContext::AtomContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQLQueryParser::CharacterClassContext* CEQLQueryParser::AtomContext::characterClass() {
  return getRuleContext<CEQLQueryParser::CharacterClassContext>(0);
}

CEQLQueryParser::SharedAtomContext* CEQLQueryParser::AtomContext::sharedAtom() {
  return getRuleContext<CEQLQueryParser::SharedAtomContext>(0);
}

CEQLQueryParser::LiteralContext* CEQLQueryParser::AtomContext::literal() {
  return getRuleContext<CEQLQueryParser::LiteralContext>(0);
}


size_t CEQLQueryParser::AtomContext::getRuleIndex() const {
  return CEQLQueryParser::RuleAtom;
}


std::any CEQLQueryParser::AtomContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitAtom(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::AtomContext* CEQLQueryParser::atom() {
  AtomContext *_localctx = _tracker.createInstance<AtomContext>(_ctx, getState());
  enterRule(_localctx, 96, CEQLQueryParser::RuleAtom);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(496);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 46, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(493);
      characterClass();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(494);
      sharedAtom();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(495);
      literal();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CharacterClassContext ------------------------------------------------------------------

CEQLQueryParser::CharacterClassContext::CharacterClassContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CEQLQueryParser::CharacterClassContext::REGEX_L_BRACK() {
  return getToken(CEQLQueryParser::REGEX_L_BRACK, 0);
}

tree::TerminalNode* CEQLQueryParser::CharacterClassContext::REGEX_R_BRACK() {
  return getToken(CEQLQueryParser::REGEX_R_BRACK, 0);
}

tree::TerminalNode* CEQLQueryParser::CharacterClassContext::REGEX_HAT() {
  return getToken(CEQLQueryParser::REGEX_HAT, 0);
}

std::vector<CEQLQueryParser::CcAtomContext *> CEQLQueryParser::CharacterClassContext::ccAtom() {
  return getRuleContexts<CEQLQueryParser::CcAtomContext>();
}

CEQLQueryParser::CcAtomContext* CEQLQueryParser::CharacterClassContext::ccAtom(size_t i) {
  return getRuleContext<CEQLQueryParser::CcAtomContext>(i);
}


size_t CEQLQueryParser::CharacterClassContext::getRuleIndex() const {
  return CEQLQueryParser::RuleCharacterClass;
}


std::any CEQLQueryParser::CharacterClassContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitCharacterClass(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::CharacterClassContext* CEQLQueryParser::characterClass() {
  CharacterClassContext *_localctx = _tracker.createInstance<CharacterClassContext>(_ctx, getState());
  enterRule(_localctx, 98, CEQLQueryParser::RuleCharacterClass);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(498);
    match(CEQLQueryParser::REGEX_L_BRACK);
    setState(500);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQLQueryParser::REGEX_HAT) {
      setState(499);
      match(CEQLQueryParser::REGEX_HAT);
    }
    setState(503); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(502);
      ccAtom();
      setState(505); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -2) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & 33548799) != 0));
    setState(507);
    match(CEQLQueryParser::REGEX_R_BRACK);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CcAtomContext ------------------------------------------------------------------

CEQLQueryParser::CcAtomContext::CcAtomContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQLQueryParser::CcRangeContext* CEQLQueryParser::CcAtomContext::ccRange() {
  return getRuleContext<CEQLQueryParser::CcRangeContext>(0);
}

CEQLQueryParser::SharedAtomContext* CEQLQueryParser::CcAtomContext::sharedAtom() {
  return getRuleContext<CEQLQueryParser::SharedAtomContext>(0);
}

CEQLQueryParser::CcSingleContext* CEQLQueryParser::CcAtomContext::ccSingle() {
  return getRuleContext<CEQLQueryParser::CcSingleContext>(0);
}


size_t CEQLQueryParser::CcAtomContext::getRuleIndex() const {
  return CEQLQueryParser::RuleCcAtom;
}


std::any CEQLQueryParser::CcAtomContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitCcAtom(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::CcAtomContext* CEQLQueryParser::ccAtom() {
  CcAtomContext *_localctx = _tracker.createInstance<CcAtomContext>(_ctx, getState());
  enterRule(_localctx, 100, CEQLQueryParser::RuleCcAtom);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(512);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 49, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(509);
      ccRange();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(510);
      sharedAtom();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(511);
      ccSingle();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CcRangeContext ------------------------------------------------------------------

CEQLQueryParser::CcRangeContext::CcRangeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<CEQLQueryParser::CcLiteralContext *> CEQLQueryParser::CcRangeContext::ccLiteral() {
  return getRuleContexts<CEQLQueryParser::CcLiteralContext>();
}

CEQLQueryParser::CcLiteralContext* CEQLQueryParser::CcRangeContext::ccLiteral(size_t i) {
  return getRuleContext<CEQLQueryParser::CcLiteralContext>(i);
}

tree::TerminalNode* CEQLQueryParser::CcRangeContext::REGEX_HYPHEN() {
  return getToken(CEQLQueryParser::REGEX_HYPHEN, 0);
}


size_t CEQLQueryParser::CcRangeContext::getRuleIndex() const {
  return CEQLQueryParser::RuleCcRange;
}


std::any CEQLQueryParser::CcRangeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitCcRange(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::CcRangeContext* CEQLQueryParser::ccRange() {
  CcRangeContext *_localctx = _tracker.createInstance<CcRangeContext>(_ctx, getState());
  enterRule(_localctx, 102, CEQLQueryParser::RuleCcRange);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(514);
    ccLiteral();
    setState(515);
    match(CEQLQueryParser::REGEX_HYPHEN);
    setState(516);
    ccLiteral();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CcSingleContext ------------------------------------------------------------------

CEQLQueryParser::CcSingleContext::CcSingleContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQLQueryParser::CcLiteralContext* CEQLQueryParser::CcSingleContext::ccLiteral() {
  return getRuleContext<CEQLQueryParser::CcLiteralContext>(0);
}


size_t CEQLQueryParser::CcSingleContext::getRuleIndex() const {
  return CEQLQueryParser::RuleCcSingle;
}


std::any CEQLQueryParser::CcSingleContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitCcSingle(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::CcSingleContext* CEQLQueryParser::ccSingle() {
  CcSingleContext *_localctx = _tracker.createInstance<CcSingleContext>(_ctx, getState());
  enterRule(_localctx, 104, CEQLQueryParser::RuleCcSingle);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(518);
    ccLiteral();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CcLiteralContext ------------------------------------------------------------------

CEQLQueryParser::CcLiteralContext::CcLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQLQueryParser::CcEscapesContext* CEQLQueryParser::CcLiteralContext::ccEscapes() {
  return getRuleContext<CEQLQueryParser::CcEscapesContext>(0);
}

CEQLQueryParser::CcOtherContext* CEQLQueryParser::CcLiteralContext::ccOther() {
  return getRuleContext<CEQLQueryParser::CcOtherContext>(0);
}


size_t CEQLQueryParser::CcLiteralContext::getRuleIndex() const {
  return CEQLQueryParser::RuleCcLiteral;
}


std::any CEQLQueryParser::CcLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitCcLiteral(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::CcLiteralContext* CEQLQueryParser::ccLiteral() {
  CcLiteralContext *_localctx = _tracker.createInstance<CcLiteralContext>(_ctx, getState());
  enterRule(_localctx, 106, CEQLQueryParser::RuleCcLiteral);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(522);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::REGEX_BACKSLASH: {
        enterOuterAlt(_localctx, 1);
        setState(520);
        ccEscapes();
        break;
      }

      case CEQLQueryParser::K_ALL:
      case CEQLQueryParser::K_AND:
      case CEQLQueryParser::K_ANY:
      case CEQLQueryParser::K_AS:
      case CEQLQueryParser::K_BY:
      case CEQLQueryParser::K_CONSUME:
      case CEQLQueryParser::K_DISTINCT:
      case CEQLQueryParser::K_EVENT:
      case CEQLQueryParser::K_EVENTS:
      case CEQLQueryParser::K_FILTER:
      case CEQLQueryParser::K_FROM:
      case CEQLQueryParser::K_HOURS:
      case CEQLQueryParser::K_IN:
      case CEQLQueryParser::K_LAST:
      case CEQLQueryParser::K_LIKE:
      case CEQLQueryParser::K_MAX:
      case CEQLQueryParser::K_MINUTES:
      case CEQLQueryParser::K_NEXT:
      case CEQLQueryParser::K_NONE:
      case CEQLQueryParser::K_NOT:
      case CEQLQueryParser::K_OR:
      case CEQLQueryParser::K_PARTITION:
      case CEQLQueryParser::K_SECONDS:
      case CEQLQueryParser::K_SELECT:
      case CEQLQueryParser::K_STREAM:
      case CEQLQueryParser::K_STRICT:
      case CEQLQueryParser::K_UNLESS:
      case CEQLQueryParser::K_WHERE:
      case CEQLQueryParser::K_WITHIN:
      case CEQLQueryParser::PERCENT:
      case CEQLQueryParser::PLUS:
      case CEQLQueryParser::MINUS:
      case CEQLQueryParser::STAR:
      case CEQLQueryParser::SLASH:
      case CEQLQueryParser::LE:
      case CEQLQueryParser::LEQ:
      case CEQLQueryParser::GE:
      case CEQLQueryParser::GEQ:
      case CEQLQueryParser::EQ:
      case CEQLQueryParser::NEQ:
      case CEQLQueryParser::SEMICOLON:
      case CEQLQueryParser::COLON:
      case CEQLQueryParser::COMMA:
      case CEQLQueryParser::DOUBLE_DOT:
      case CEQLQueryParser::LEFT_PARENTHESIS:
      case CEQLQueryParser::RIGHT_PARENTHESIS:
      case CEQLQueryParser::LEFT_SQUARE_BRACKET:
      case CEQLQueryParser::RIGHT_SQUARE_BRACKET:
      case CEQLQueryParser::LEFT_CURLY_BRACKET:
      case CEQLQueryParser::RIGHT_CURLY_BRACKET:
      case CEQLQueryParser::IDENTIFIER:
      case CEQLQueryParser::DOUBLE_LITERAL:
      case CEQLQueryParser::INTEGER_LITERAL:
      case CEQLQueryParser::NUMERICAL_EXPONENT:
      case CEQLQueryParser::STRING_LITERAL:
      case CEQLQueryParser::SINGLE_LINE_COMMENT:
      case CEQLQueryParser::MULTILINE_COMMENT:
      case CEQLQueryParser::SPACES:
      case CEQLQueryParser::UNEXPECTED_CHAR:
      case CEQLQueryParser::REGEX_START:
      case CEQLQueryParser::REGEX_END:
      case CEQLQueryParser::REGEX_END_ESCAPED:
      case CEQLQueryParser::REGEX_PIPE:
      case CEQLQueryParser::REGEX_EXCLAMAITON:
      case CEQLQueryParser::REGEX_L_CURLY:
      case CEQLQueryParser::REGEX_R_CURLY:
      case CEQLQueryParser::REGEX_L_PAR:
      case CEQLQueryParser::REGEX_R_PAR:
      case CEQLQueryParser::REGEX_COMMA:
      case CEQLQueryParser::REGEX_QUESTION:
      case CEQLQueryParser::REGEX_PLUS:
      case CEQLQueryParser::REGEX_STAR:
      case CEQLQueryParser::REGEX_L_BRACK:
      case CEQLQueryParser::REGEX_ALPHA:
      case CEQLQueryParser::REGEX_DOT:
      case CEQLQueryParser::REGEX_DOUBLED_DOT:
      case CEQLQueryParser::UNRECOGNIZED:
      case CEQLQueryParser::REGEX_DECIMAL_DIGIT:
      case CEQLQueryParser::REGEX_NOT_DECIMAL_DIGIT:
      case CEQLQueryParser::REGEX_WHITESPACE:
      case CEQLQueryParser::REGEX_NOT_WHITESPACE:
      case CEQLQueryParser::REGEX_ALPHANUMERIC:
      case CEQLQueryParser::REGEX_NOT_ALPHANUMERIC:
      case CEQLQueryParser::REGEX_DIGIT: {
        enterOuterAlt(_localctx, 2);
        setState(521);
        ccOther();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CcEscapesContext ------------------------------------------------------------------

CEQLQueryParser::CcEscapesContext::CcEscapesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> CEQLQueryParser::CcEscapesContext::REGEX_BACKSLASH() {
  return getTokens(CEQLQueryParser::REGEX_BACKSLASH);
}

tree::TerminalNode* CEQLQueryParser::CcEscapesContext::REGEX_BACKSLASH(size_t i) {
  return getToken(CEQLQueryParser::REGEX_BACKSLASH, i);
}

tree::TerminalNode* CEQLQueryParser::CcEscapesContext::REGEX_HAT() {
  return getToken(CEQLQueryParser::REGEX_HAT, 0);
}

tree::TerminalNode* CEQLQueryParser::CcEscapesContext::REGEX_HYPHEN() {
  return getToken(CEQLQueryParser::REGEX_HYPHEN, 0);
}

tree::TerminalNode* CEQLQueryParser::CcEscapesContext::REGEX_R_BRACK() {
  return getToken(CEQLQueryParser::REGEX_R_BRACK, 0);
}


size_t CEQLQueryParser::CcEscapesContext::getRuleIndex() const {
  return CEQLQueryParser::RuleCcEscapes;
}


std::any CEQLQueryParser::CcEscapesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitCcEscapes(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::CcEscapesContext* CEQLQueryParser::ccEscapes() {
  CcEscapesContext *_localctx = _tracker.createInstance<CcEscapesContext>(_ctx, getState());
  enterRule(_localctx, 108, CEQLQueryParser::RuleCcEscapes);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(524);
    match(CEQLQueryParser::REGEX_BACKSLASH);
    setState(525);
    _la = _input->LA(1);
    if (!(((((_la - 73) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 73)) & 27) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CcOtherContext ------------------------------------------------------------------

CEQLQueryParser::CcOtherContext::CcOtherContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CEQLQueryParser::CcOtherContext::REGEX_HAT() {
  return getToken(CEQLQueryParser::REGEX_HAT, 0);
}

tree::TerminalNode* CEQLQueryParser::CcOtherContext::REGEX_HYPHEN() {
  return getToken(CEQLQueryParser::REGEX_HYPHEN, 0);
}

tree::TerminalNode* CEQLQueryParser::CcOtherContext::REGEX_R_BRACK() {
  return getToken(CEQLQueryParser::REGEX_R_BRACK, 0);
}

tree::TerminalNode* CEQLQueryParser::CcOtherContext::REGEX_BACKSLASH() {
  return getToken(CEQLQueryParser::REGEX_BACKSLASH, 0);
}


size_t CEQLQueryParser::CcOtherContext::getRuleIndex() const {
  return CEQLQueryParser::RuleCcOther;
}


std::any CEQLQueryParser::CcOtherContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitCcOther(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::CcOtherContext* CEQLQueryParser::ccOther() {
  CcOtherContext *_localctx = _tracker.createInstance<CcOtherContext>(_ctx, getState());
  enterRule(_localctx, 110, CEQLQueryParser::RuleCcOther);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(527);
    _la = _input->LA(1);
    if (_la == 0 || _la == Token::EOF || (((((_la - 73) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 73)) & 27) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LiteralContext ------------------------------------------------------------------

CEQLQueryParser::LiteralContext::LiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQLQueryParser::EscapesContext* CEQLQueryParser::LiteralContext::escapes() {
  return getRuleContext<CEQLQueryParser::EscapesContext>(0);
}

tree::TerminalNode* CEQLQueryParser::LiteralContext::REGEX_DOT() {
  return getToken(CEQLQueryParser::REGEX_DOT, 0);
}

CEQLQueryParser::OtherContext* CEQLQueryParser::LiteralContext::other() {
  return getRuleContext<CEQLQueryParser::OtherContext>(0);
}


size_t CEQLQueryParser::LiteralContext::getRuleIndex() const {
  return CEQLQueryParser::RuleLiteral;
}


std::any CEQLQueryParser::LiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitLiteral(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::LiteralContext* CEQLQueryParser::literal() {
  LiteralContext *_localctx = _tracker.createInstance<LiteralContext>(_ctx, getState());
  enterRule(_localctx, 112, CEQLQueryParser::RuleLiteral);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(532);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 51, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(529);
      escapes();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(530);
      match(CEQLQueryParser::REGEX_DOT);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(531);
      other();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EscapesContext ------------------------------------------------------------------

CEQLQueryParser::EscapesContext::EscapesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> CEQLQueryParser::EscapesContext::REGEX_BACKSLASH() {
  return getTokens(CEQLQueryParser::REGEX_BACKSLASH);
}

tree::TerminalNode* CEQLQueryParser::EscapesContext::REGEX_BACKSLASH(size_t i) {
  return getToken(CEQLQueryParser::REGEX_BACKSLASH, i);
}

tree::TerminalNode* CEQLQueryParser::EscapesContext::REGEX_L_BRACK() {
  return getToken(CEQLQueryParser::REGEX_L_BRACK, 0);
}

tree::TerminalNode* CEQLQueryParser::EscapesContext::REGEX_R_BRACK() {
  return getToken(CEQLQueryParser::REGEX_R_BRACK, 0);
}

tree::TerminalNode* CEQLQueryParser::EscapesContext::REGEX_L_PAR() {
  return getToken(CEQLQueryParser::REGEX_L_PAR, 0);
}

tree::TerminalNode* CEQLQueryParser::EscapesContext::REGEX_R_PAR() {
  return getToken(CEQLQueryParser::REGEX_R_PAR, 0);
}

tree::TerminalNode* CEQLQueryParser::EscapesContext::REGEX_L_CURLY() {
  return getToken(CEQLQueryParser::REGEX_L_CURLY, 0);
}

tree::TerminalNode* CEQLQueryParser::EscapesContext::REGEX_R_CURLY() {
  return getToken(CEQLQueryParser::REGEX_R_CURLY, 0);
}

tree::TerminalNode* CEQLQueryParser::EscapesContext::REGEX_STAR() {
  return getToken(CEQLQueryParser::REGEX_STAR, 0);
}

tree::TerminalNode* CEQLQueryParser::EscapesContext::REGEX_PLUS() {
  return getToken(CEQLQueryParser::REGEX_PLUS, 0);
}

tree::TerminalNode* CEQLQueryParser::EscapesContext::REGEX_QUESTION() {
  return getToken(CEQLQueryParser::REGEX_QUESTION, 0);
}

tree::TerminalNode* CEQLQueryParser::EscapesContext::REGEX_PIPE() {
  return getToken(CEQLQueryParser::REGEX_PIPE, 0);
}

tree::TerminalNode* CEQLQueryParser::EscapesContext::REGEX_DOT() {
  return getToken(CEQLQueryParser::REGEX_DOT, 0);
}


size_t CEQLQueryParser::EscapesContext::getRuleIndex() const {
  return CEQLQueryParser::RuleEscapes;
}


std::any CEQLQueryParser::EscapesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitEscapes(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::EscapesContext* CEQLQueryParser::escapes() {
  EscapesContext *_localctx = _tracker.createInstance<EscapesContext>(_ctx, getState());
  enterRule(_localctx, 114, CEQLQueryParser::RuleEscapes);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(534);
    match(CEQLQueryParser::REGEX_BACKSLASH);
    setState(535);
    _la = _input->LA(1);
    if (!(((((_la - 63) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 63)) & 95165) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OtherContext ------------------------------------------------------------------

CEQLQueryParser::OtherContext::OtherContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CEQLQueryParser::OtherContext::REGEX_L_BRACK() {
  return getToken(CEQLQueryParser::REGEX_L_BRACK, 0);
}

tree::TerminalNode* CEQLQueryParser::OtherContext::REGEX_R_BRACK() {
  return getToken(CEQLQueryParser::REGEX_R_BRACK, 0);
}

tree::TerminalNode* CEQLQueryParser::OtherContext::REGEX_L_PAR() {
  return getToken(CEQLQueryParser::REGEX_L_PAR, 0);
}

tree::TerminalNode* CEQLQueryParser::OtherContext::REGEX_R_PAR() {
  return getToken(CEQLQueryParser::REGEX_R_PAR, 0);
}

tree::TerminalNode* CEQLQueryParser::OtherContext::REGEX_L_CURLY() {
  return getToken(CEQLQueryParser::REGEX_L_CURLY, 0);
}

tree::TerminalNode* CEQLQueryParser::OtherContext::REGEX_R_CURLY() {
  return getToken(CEQLQueryParser::REGEX_R_CURLY, 0);
}

tree::TerminalNode* CEQLQueryParser::OtherContext::REGEX_STAR() {
  return getToken(CEQLQueryParser::REGEX_STAR, 0);
}

tree::TerminalNode* CEQLQueryParser::OtherContext::REGEX_PLUS() {
  return getToken(CEQLQueryParser::REGEX_PLUS, 0);
}

tree::TerminalNode* CEQLQueryParser::OtherContext::REGEX_QUESTION() {
  return getToken(CEQLQueryParser::REGEX_QUESTION, 0);
}

tree::TerminalNode* CEQLQueryParser::OtherContext::REGEX_PIPE() {
  return getToken(CEQLQueryParser::REGEX_PIPE, 0);
}

tree::TerminalNode* CEQLQueryParser::OtherContext::REGEX_BACKSLASH() {
  return getToken(CEQLQueryParser::REGEX_BACKSLASH, 0);
}


size_t CEQLQueryParser::OtherContext::getRuleIndex() const {
  return CEQLQueryParser::RuleOther;
}


std::any CEQLQueryParser::OtherContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitOther(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::OtherContext* CEQLQueryParser::other() {
  OtherContext *_localctx = _tracker.createInstance<OtherContext>(_ctx, getState());
  enterRule(_localctx, 116, CEQLQueryParser::RuleOther);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(537);
    _la = _input->LA(1);
    if (_la == 0 || _la == Token::EOF || (((((_la - 63) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 63)) & 29629) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SharedAtomContext ------------------------------------------------------------------

CEQLQueryParser::SharedAtomContext::SharedAtomContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CEQLQueryParser::SharedAtomContext::REGEX_DECIMAL_DIGIT() {
  return getToken(CEQLQueryParser::REGEX_DECIMAL_DIGIT, 0);
}

tree::TerminalNode* CEQLQueryParser::SharedAtomContext::REGEX_NOT_DECIMAL_DIGIT() {
  return getToken(CEQLQueryParser::REGEX_NOT_DECIMAL_DIGIT, 0);
}

tree::TerminalNode* CEQLQueryParser::SharedAtomContext::REGEX_WHITESPACE() {
  return getToken(CEQLQueryParser::REGEX_WHITESPACE, 0);
}

tree::TerminalNode* CEQLQueryParser::SharedAtomContext::REGEX_NOT_WHITESPACE() {
  return getToken(CEQLQueryParser::REGEX_NOT_WHITESPACE, 0);
}

tree::TerminalNode* CEQLQueryParser::SharedAtomContext::REGEX_ALPHANUMERIC() {
  return getToken(CEQLQueryParser::REGEX_ALPHANUMERIC, 0);
}

tree::TerminalNode* CEQLQueryParser::SharedAtomContext::REGEX_NOT_ALPHANUMERIC() {
  return getToken(CEQLQueryParser::REGEX_NOT_ALPHANUMERIC, 0);
}


size_t CEQLQueryParser::SharedAtomContext::getRuleIndex() const {
  return CEQLQueryParser::RuleSharedAtom;
}


std::any CEQLQueryParser::SharedAtomContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitSharedAtom(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::SharedAtomContext* CEQLQueryParser::sharedAtom() {
  SharedAtomContext *_localctx = _tracker.createInstance<SharedAtomContext>(_ctx, getState());
  enterRule(_localctx, 118, CEQLQueryParser::RuleSharedAtom);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(539);
    _la = _input->LA(1);
    if (!(((((_la - 82) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 82)) & 63) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Regexp_numberContext ------------------------------------------------------------------

CEQLQueryParser::Regexp_numberContext::Regexp_numberContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> CEQLQueryParser::Regexp_numberContext::REGEX_DIGIT() {
  return getTokens(CEQLQueryParser::REGEX_DIGIT);
}

tree::TerminalNode* CEQLQueryParser::Regexp_numberContext::REGEX_DIGIT(size_t i) {
  return getToken(CEQLQueryParser::REGEX_DIGIT, i);
}


size_t CEQLQueryParser::Regexp_numberContext::getRuleIndex() const {
  return CEQLQueryParser::RuleRegexp_number;
}


std::any CEQLQueryParser::Regexp_numberContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitRegexp_number(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::Regexp_numberContext* CEQLQueryParser::regexp_number() {
  Regexp_numberContext *_localctx = _tracker.createInstance<Regexp_numberContext>(_ctx, getState());
  enterRule(_localctx, 120, CEQLQueryParser::RuleRegexp_number);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(542); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(541);
      match(CEQLQueryParser::REGEX_DIGIT);
      setState(544); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == CEQLQueryParser::REGEX_DIGIT);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool CEQLQueryParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 6: return cel_formulaSempred(antlrcpp::downCast<Cel_formulaContext *>(context), predicateIndex);
    case 10: return filterSempred(antlrcpp::downCast<FilterContext *>(context), predicateIndex);
    case 11: return predicateSempred(antlrcpp::downCast<PredicateContext *>(context), predicateIndex);
    case 14: return math_exprSempred(antlrcpp::downCast<Math_exprContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool CEQLQueryParser::cel_formulaSempred(Cel_formulaContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 4);
    case 1: return precpred(_ctx, 3);
    case 2: return precpred(_ctx, 2);
    case 3: return precpred(_ctx, 6);
    case 4: return precpred(_ctx, 5);
    case 5: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

bool CEQLQueryParser::filterSempred(FilterContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 6: return precpred(_ctx, 2);
    case 7: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

bool CEQLQueryParser::predicateSempred(PredicateContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 8: return precpred(_ctx, 4);
    case 9: return precpred(_ctx, 3);

  default:
    break;
  }
  return true;
}

bool CEQLQueryParser::math_exprSempred(Math_exprContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 10: return precpred(_ctx, 2);
    case 11: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

void CEQLQueryParser::initialize() {
  ::antlr4::internal::call_once(ceqlqueryparserParserOnceFlag, ceqlqueryparserParserInitialize);
}
