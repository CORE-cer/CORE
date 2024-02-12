
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
      "consumption_policy", "limit", "filter", "predicate", "string_literal", 
      "string_literal_or_regexp", "math_expr", "value_seq", "number_seq", 
      "string_seq", "time_window", "event_span", "time_span", "hour_span", 
      "minute_span", "second_span", "custom_span", "named_event", "s_event_name", 
      "event_name", "stream_name", "attribute_name", "integer", "double", 
      "number", "string", "any_name", "keyword", "regexp", "regexp_alternation", 
      "regexp_exp", "regexp_element", "regexp_group", "parenthesis", "quantifier", 
      "quantity", "quantExact", "quantRange", "quantMin", "quantMax", "atom", 
      "characterClass", "ccAtom", "ccRange", "ccSingle", "ccLiteral", "ccEscapes", 
      "ccOther", "literal", "escapes", "other", "sharedAtom", "regexp_number"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "'%'", 
      "", "", "", "'/'", "'<'", "'<='", "'>'", "'>='", "", "", "';'", "':'", 
      "", "", "", "", "", "", "", "", "':+'", "", "", "", "", "", "", "", 
      "", "", "'<<'", "'>>'", "'\\>'", "'|'", "'!'", "", "", "", "", "", 
      "'\\u003F'", "", "", "'^'", "", "", "", "'\\'", "", "'.'", "", "", 
      "'\\d'", "'\\D'", "'\\s'", "'\\S'", "'\\w'", "'\\W'"
    },
    std::vector<std::string>{
      "", "K_ALL", "K_AND", "K_ANY", "K_AS", "K_BY", "K_CONSUME", "K_LIMIT", 
      "K_DISTINCT", "K_EVENT", "K_EVENTS", "K_FILTER", "K_FROM", "K_HOURS", 
      "K_IN", "K_LAST", "K_LIKE", "K_MAX", "K_MINUTES", "K_NEXT", "K_NONE", 
      "K_NOT", "K_OR", "K_PARTITION", "K_RANGE", "K_SECONDS", "K_SELECT", 
      "K_STREAM", "K_STRICT", "K_UNLESS", "K_WHERE", "K_WITHIN", "PERCENT", 
      "PLUS", "MINUS", "STAR", "SLASH", "LE", "LEQ", "GE", "GEQ", "EQ", 
      "NEQ", "SEMICOLON", "COLON", "COMMA", "DOUBLE_DOT", "LEFT_PARENTHESIS", 
      "RIGHT_PARENTHESIS", "LEFT_SQUARE_BRACKET", "RIGHT_SQUARE_BRACKET", 
      "LEFT_CURLY_BRACKET", "RIGHT_CURLY_BRACKET", "COLON_PLUS", "IDENTIFIER", 
      "DOUBLE_LITERAL", "INTEGER_LITERAL", "NUMERICAL_EXPONENT", "STRING_LITERAL", 
      "SINGLE_LINE_COMMENT", "MULTILINE_COMMENT", "SPACES", "UNEXPECTED_CHAR", 
      "REGEX_START", "REGEX_END", "REGEX_END_ESCAPED", "REGEX_PIPE", "REGEX_EXCLAMAITON", 
      "REGEX_L_CURLY", "REGEX_R_CURLY", "REGEX_L_PAR", "REGEX_R_PAR", "REGEX_COMMA", 
      "REGEX_QUESTION", "REGEX_PLUS", "REGEX_STAR", "REGEX_HAT", "REGEX_HYPHEN", 
      "REGEX_L_BRACK", "REGEX_R_BRACK", "REGEX_BACKSLASH", "REGEX_ALPHA", 
      "REGEX_DOT", "REGEX_DOUBLED_DOT", "UNRECOGNIZED", "REGEX_DECIMAL_DIGIT", 
      "REGEX_NOT_DECIMAL_DIGIT", "REGEX_WHITESPACE", "REGEX_NOT_WHITESPACE", 
      "REGEX_ALPHANUMERIC", "REGEX_NOT_ALPHANUMERIC", "REGEX_DIGIT"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,91,567,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,7,
  	35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,2,40,7,40,2,41,7,41,2,42,7,
  	42,2,43,7,43,2,44,7,44,2,45,7,45,2,46,7,46,2,47,7,47,2,48,7,48,2,49,7,
  	49,2,50,7,50,2,51,7,51,2,52,7,52,2,53,7,53,2,54,7,54,2,55,7,55,2,56,7,
  	56,2,57,7,57,2,58,7,58,2,59,7,59,2,60,7,60,2,61,7,61,1,0,1,0,5,0,127,
  	8,0,10,0,12,0,130,9,0,1,0,1,0,1,1,1,1,1,1,1,2,1,2,3,2,139,8,2,1,2,1,2,
  	1,2,1,2,1,2,1,2,1,2,3,2,148,8,2,1,2,1,2,3,2,152,8,2,1,2,1,2,1,2,3,2,157,
  	8,2,1,2,1,2,3,2,161,8,2,1,3,1,3,1,3,1,3,1,3,1,3,3,3,169,8,3,1,4,1,4,1,
  	4,1,4,1,4,5,4,176,8,4,10,4,12,4,179,9,4,3,4,181,8,4,1,5,1,5,1,5,1,5,5,
  	5,187,8,5,10,5,12,5,190,9,5,3,5,192,8,5,1,6,1,6,1,6,1,6,1,6,1,6,3,6,200,
  	8,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,
  	6,1,6,1,6,5,6,221,8,6,10,6,12,6,224,9,6,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,
  	7,3,7,234,8,7,1,8,1,8,1,8,5,8,239,8,8,10,8,12,8,242,9,8,1,9,1,9,1,9,3,
  	9,247,8,9,1,10,1,10,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,
  	3,11,261,8,11,1,11,1,11,1,11,1,11,1,11,1,11,5,11,269,8,11,10,11,12,11,
  	272,9,11,1,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,
  	1,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,3,12,297,8,12,
  	1,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,3,12,310,8,12,
  	1,12,1,12,1,12,1,12,1,12,1,12,5,12,318,8,12,10,12,12,12,321,9,12,1,13,
  	1,13,3,13,325,8,13,1,14,1,14,1,14,3,14,330,8,14,1,15,1,15,1,15,1,15,1,
  	15,1,15,1,15,1,15,1,15,3,15,341,8,15,1,15,1,15,1,15,1,15,1,15,1,15,5,
  	15,349,8,15,10,15,12,15,352,9,15,1,16,1,16,1,16,1,16,1,16,1,16,1,16,1,
  	16,3,16,362,8,16,1,17,1,17,1,17,5,17,367,8,17,10,17,12,17,370,9,17,1,
  	17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,3,17,385,
  	8,17,1,18,1,18,1,18,5,18,390,8,18,10,18,12,18,393,9,18,1,19,1,19,1,19,
  	3,19,398,8,19,1,20,1,20,1,20,1,21,3,21,404,8,21,1,21,3,21,407,8,21,1,
  	21,3,21,410,8,21,1,22,1,22,1,22,1,23,1,23,1,23,1,24,1,24,1,24,1,25,1,
  	25,1,25,1,25,1,25,1,26,1,26,1,26,3,26,429,8,26,1,27,1,27,1,27,3,27,434,
  	8,27,1,27,1,27,1,28,1,28,1,29,1,29,1,30,1,30,1,31,1,31,1,32,1,32,1,33,
  	1,33,3,33,450,8,33,1,34,1,34,1,35,1,35,1,36,1,36,1,37,1,37,1,37,1,37,
  	1,38,1,38,1,38,5,38,465,8,38,10,38,12,38,468,9,38,1,39,4,39,471,8,39,
  	11,39,12,39,472,1,40,1,40,3,40,477,8,40,1,41,1,41,3,41,481,8,41,1,42,
  	1,42,1,42,1,42,1,43,1,43,1,43,1,43,1,43,1,43,1,43,3,43,494,8,43,1,44,
  	1,44,1,44,1,44,3,44,500,8,44,1,45,1,45,1,46,1,46,1,46,1,46,1,47,1,47,
  	1,47,1,48,1,48,1,48,1,49,1,49,1,49,3,49,517,8,49,1,50,1,50,3,50,521,8,
  	50,1,50,4,50,524,8,50,11,50,12,50,525,1,50,1,50,1,51,1,51,1,51,3,51,533,
  	8,51,1,52,1,52,1,52,1,52,1,53,1,53,1,54,1,54,3,54,543,8,54,1,55,1,55,
  	1,55,1,56,1,56,1,57,1,57,1,57,3,57,553,8,57,1,58,1,58,1,58,1,59,1,59,
  	1,60,1,60,1,61,4,61,563,8,61,11,61,12,61,564,1,61,0,4,12,22,24,30,62,
  	0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,
  	50,52,54,56,58,60,62,64,66,68,70,72,74,76,78,80,82,84,86,88,90,92,94,
  	96,98,100,102,104,106,108,110,112,114,116,118,120,122,0,9,1,0,37,42,1,
  	0,41,42,1,0,33,34,2,0,32,32,35,36,2,0,1,6,8,31,2,0,76,77,79,80,5,0,66,
  	66,68,71,73,75,78,80,82,82,4,0,66,66,68,71,73,75,78,80,1,0,85,90,588,
  	0,128,1,0,0,0,2,133,1,0,0,0,4,136,1,0,0,0,6,168,1,0,0,0,8,180,1,0,0,0,
  	10,191,1,0,0,0,12,199,1,0,0,0,14,225,1,0,0,0,16,235,1,0,0,0,18,246,1,
  	0,0,0,20,248,1,0,0,0,22,260,1,0,0,0,24,309,1,0,0,0,26,324,1,0,0,0,28,
  	329,1,0,0,0,30,340,1,0,0,0,32,361,1,0,0,0,34,384,1,0,0,0,36,386,1,0,0,
  	0,38,397,1,0,0,0,40,399,1,0,0,0,42,403,1,0,0,0,44,411,1,0,0,0,46,414,
  	1,0,0,0,48,417,1,0,0,0,50,420,1,0,0,0,52,425,1,0,0,0,54,433,1,0,0,0,56,
  	437,1,0,0,0,58,439,1,0,0,0,60,441,1,0,0,0,62,443,1,0,0,0,64,445,1,0,0,
  	0,66,449,1,0,0,0,68,451,1,0,0,0,70,453,1,0,0,0,72,455,1,0,0,0,74,457,
  	1,0,0,0,76,461,1,0,0,0,78,470,1,0,0,0,80,474,1,0,0,0,82,480,1,0,0,0,84,
  	482,1,0,0,0,86,493,1,0,0,0,88,499,1,0,0,0,90,501,1,0,0,0,92,503,1,0,0,
  	0,94,507,1,0,0,0,96,510,1,0,0,0,98,516,1,0,0,0,100,518,1,0,0,0,102,532,
  	1,0,0,0,104,534,1,0,0,0,106,538,1,0,0,0,108,542,1,0,0,0,110,544,1,0,0,
  	0,112,547,1,0,0,0,114,552,1,0,0,0,116,554,1,0,0,0,118,557,1,0,0,0,120,
  	559,1,0,0,0,122,562,1,0,0,0,124,127,3,4,2,0,125,127,3,2,1,0,126,124,1,
  	0,0,0,126,125,1,0,0,0,127,130,1,0,0,0,128,126,1,0,0,0,128,129,1,0,0,0,
  	129,131,1,0,0,0,130,128,1,0,0,0,131,132,5,0,0,1,132,1,1,0,0,0,133,134,
  	5,62,0,0,134,135,6,1,-1,0,135,3,1,0,0,0,136,138,5,26,0,0,137,139,3,6,
  	3,0,138,137,1,0,0,0,138,139,1,0,0,0,139,140,1,0,0,0,140,141,3,8,4,0,141,
  	142,3,10,5,0,142,143,5,30,0,0,143,147,3,12,6,0,144,145,5,23,0,0,145,146,
  	5,5,0,0,146,148,3,14,7,0,147,144,1,0,0,0,147,148,1,0,0,0,148,151,1,0,
  	0,0,149,150,5,31,0,0,150,152,3,38,19,0,151,149,1,0,0,0,151,152,1,0,0,
  	0,152,156,1,0,0,0,153,154,5,6,0,0,154,155,5,5,0,0,155,157,3,18,9,0,156,
  	153,1,0,0,0,156,157,1,0,0,0,157,160,1,0,0,0,158,159,5,7,0,0,159,161,3,
  	20,10,0,160,158,1,0,0,0,160,161,1,0,0,0,161,5,1,0,0,0,162,169,5,1,0,0,
  	163,169,5,3,0,0,164,169,5,15,0,0,165,169,5,17,0,0,166,169,5,19,0,0,167,
  	169,5,28,0,0,168,162,1,0,0,0,168,163,1,0,0,0,168,164,1,0,0,0,168,165,
  	1,0,0,0,168,166,1,0,0,0,168,167,1,0,0,0,169,7,1,0,0,0,170,181,5,35,0,
  	0,171,181,5,20,0,0,172,177,3,70,35,0,173,174,5,45,0,0,174,176,3,70,35,
  	0,175,173,1,0,0,0,176,179,1,0,0,0,177,175,1,0,0,0,177,178,1,0,0,0,178,
  	181,1,0,0,0,179,177,1,0,0,0,180,170,1,0,0,0,180,171,1,0,0,0,180,172,1,
  	0,0,0,181,9,1,0,0,0,182,183,5,12,0,0,183,188,3,58,29,0,184,185,5,45,0,
  	0,185,187,3,58,29,0,186,184,1,0,0,0,187,190,1,0,0,0,188,186,1,0,0,0,188,
  	189,1,0,0,0,189,192,1,0,0,0,190,188,1,0,0,0,191,182,1,0,0,0,191,192,1,
  	0,0,0,192,11,1,0,0,0,193,194,6,6,-1,0,194,195,5,47,0,0,195,196,3,12,6,
  	0,196,197,5,48,0,0,197,200,1,0,0,0,198,200,3,54,27,0,199,193,1,0,0,0,
  	199,198,1,0,0,0,200,222,1,0,0,0,201,202,10,4,0,0,202,203,5,43,0,0,203,
  	221,3,12,6,5,204,205,10,3,0,0,205,206,5,44,0,0,206,221,3,12,6,4,207,208,
  	10,2,0,0,208,209,5,22,0,0,209,221,3,12,6,3,210,211,10,7,0,0,211,212,5,
  	4,0,0,212,221,3,56,28,0,213,214,10,6,0,0,214,221,5,33,0,0,215,216,10,
  	5,0,0,216,221,5,53,0,0,217,218,10,1,0,0,218,219,5,11,0,0,219,221,3,22,
  	11,0,220,201,1,0,0,0,220,204,1,0,0,0,220,207,1,0,0,0,220,210,1,0,0,0,
  	220,213,1,0,0,0,220,215,1,0,0,0,220,217,1,0,0,0,221,224,1,0,0,0,222,220,
  	1,0,0,0,222,223,1,0,0,0,223,13,1,0,0,0,224,222,1,0,0,0,225,226,5,49,0,
  	0,226,227,3,16,8,0,227,233,5,50,0,0,228,229,5,45,0,0,229,230,5,49,0,0,
  	230,231,3,16,8,0,231,232,5,50,0,0,232,234,1,0,0,0,233,228,1,0,0,0,233,
  	234,1,0,0,0,234,15,1,0,0,0,235,240,3,60,30,0,236,237,5,45,0,0,237,239,
  	3,60,30,0,238,236,1,0,0,0,239,242,1,0,0,0,240,238,1,0,0,0,240,241,1,0,
  	0,0,241,17,1,0,0,0,242,240,1,0,0,0,243,247,5,3,0,0,244,247,5,23,0,0,245,
  	247,5,20,0,0,246,243,1,0,0,0,246,244,1,0,0,0,246,245,1,0,0,0,247,19,1,
  	0,0,0,248,249,3,62,31,0,249,21,1,0,0,0,250,251,6,11,-1,0,251,252,5,47,
  	0,0,252,253,3,22,11,0,253,254,5,48,0,0,254,261,1,0,0,0,255,256,3,56,28,
  	0,256,257,5,49,0,0,257,258,3,24,12,0,258,259,5,50,0,0,259,261,1,0,0,0,
  	260,250,1,0,0,0,260,255,1,0,0,0,261,270,1,0,0,0,262,263,10,2,0,0,263,
  	264,5,2,0,0,264,269,3,22,11,3,265,266,10,1,0,0,266,267,5,22,0,0,267,269,
  	3,22,11,2,268,262,1,0,0,0,268,265,1,0,0,0,269,272,1,0,0,0,270,268,1,0,
  	0,0,270,271,1,0,0,0,271,23,1,0,0,0,272,270,1,0,0,0,273,274,6,12,-1,0,
  	274,275,5,47,0,0,275,276,3,24,12,0,276,277,5,48,0,0,277,310,1,0,0,0,278,
  	279,5,21,0,0,279,310,3,24,12,8,280,281,3,30,15,0,281,282,7,0,0,0,282,
  	283,3,30,15,0,283,310,1,0,0,0,284,285,3,26,13,0,285,286,7,1,0,0,286,287,
  	3,28,14,0,287,310,1,0,0,0,288,289,3,60,30,0,289,290,5,16,0,0,290,291,
  	3,74,37,0,291,310,1,0,0,0,292,296,3,60,30,0,293,297,5,14,0,0,294,295,
  	5,21,0,0,295,297,5,14,0,0,296,293,1,0,0,0,296,294,1,0,0,0,297,298,1,0,
  	0,0,298,299,3,32,16,0,299,310,1,0,0,0,300,301,3,30,15,0,301,302,5,14,
  	0,0,302,303,5,24,0,0,303,304,5,47,0,0,304,305,3,30,15,0,305,306,5,45,
  	0,0,306,307,3,30,15,0,307,308,5,48,0,0,308,310,1,0,0,0,309,273,1,0,0,
  	0,309,278,1,0,0,0,309,280,1,0,0,0,309,284,1,0,0,0,309,288,1,0,0,0,309,
  	292,1,0,0,0,309,300,1,0,0,0,310,319,1,0,0,0,311,312,10,5,0,0,312,313,
  	5,2,0,0,313,318,3,24,12,6,314,315,10,4,0,0,315,316,5,22,0,0,316,318,3,
  	24,12,5,317,311,1,0,0,0,317,314,1,0,0,0,318,321,1,0,0,0,319,317,1,0,0,
  	0,319,320,1,0,0,0,320,25,1,0,0,0,321,319,1,0,0,0,322,325,3,68,34,0,323,
  	325,3,60,30,0,324,322,1,0,0,0,324,323,1,0,0,0,325,27,1,0,0,0,326,330,
  	3,68,34,0,327,330,3,60,30,0,328,330,3,74,37,0,329,326,1,0,0,0,329,327,
  	1,0,0,0,329,328,1,0,0,0,330,29,1,0,0,0,331,332,6,15,-1,0,332,333,5,47,
  	0,0,333,334,3,30,15,0,334,335,5,48,0,0,335,341,1,0,0,0,336,341,3,66,33,
  	0,337,341,3,60,30,0,338,339,7,2,0,0,339,341,3,30,15,3,340,331,1,0,0,0,
  	340,336,1,0,0,0,340,337,1,0,0,0,340,338,1,0,0,0,341,350,1,0,0,0,342,343,
  	10,2,0,0,343,344,7,3,0,0,344,349,3,30,15,3,345,346,10,1,0,0,346,347,7,
  	2,0,0,347,349,3,30,15,2,348,342,1,0,0,0,348,345,1,0,0,0,349,352,1,0,0,
  	0,350,348,1,0,0,0,350,351,1,0,0,0,351,31,1,0,0,0,352,350,1,0,0,0,353,
  	354,5,51,0,0,354,355,3,34,17,0,355,356,5,52,0,0,356,362,1,0,0,0,357,358,
  	5,51,0,0,358,359,3,36,18,0,359,360,5,52,0,0,360,362,1,0,0,0,361,353,1,
  	0,0,0,361,357,1,0,0,0,362,33,1,0,0,0,363,368,3,66,33,0,364,365,5,45,0,
  	0,365,367,3,66,33,0,366,364,1,0,0,0,367,370,1,0,0,0,368,366,1,0,0,0,368,
  	369,1,0,0,0,369,385,1,0,0,0,370,368,1,0,0,0,371,372,3,62,31,0,372,373,
  	5,46,0,0,373,374,3,62,31,0,374,385,1,0,0,0,375,376,3,64,32,0,376,377,
  	5,46,0,0,377,378,3,64,32,0,378,385,1,0,0,0,379,380,3,66,33,0,380,381,
  	5,46,0,0,381,385,1,0,0,0,382,383,5,46,0,0,383,385,3,66,33,0,384,363,1,
  	0,0,0,384,371,1,0,0,0,384,375,1,0,0,0,384,379,1,0,0,0,384,382,1,0,0,0,
  	385,35,1,0,0,0,386,391,3,68,34,0,387,388,5,45,0,0,388,390,3,68,34,0,389,
  	387,1,0,0,0,390,393,1,0,0,0,391,389,1,0,0,0,391,392,1,0,0,0,392,37,1,
  	0,0,0,393,391,1,0,0,0,394,398,3,40,20,0,395,398,3,42,21,0,396,398,3,50,
  	25,0,397,394,1,0,0,0,397,395,1,0,0,0,397,396,1,0,0,0,398,39,1,0,0,0,399,
  	400,3,62,31,0,400,401,5,10,0,0,401,41,1,0,0,0,402,404,3,44,22,0,403,402,
  	1,0,0,0,403,404,1,0,0,0,404,406,1,0,0,0,405,407,3,46,23,0,406,405,1,0,
  	0,0,406,407,1,0,0,0,407,409,1,0,0,0,408,410,3,48,24,0,409,408,1,0,0,0,
  	409,410,1,0,0,0,410,43,1,0,0,0,411,412,3,66,33,0,412,413,5,13,0,0,413,
  	45,1,0,0,0,414,415,3,66,33,0,415,416,5,18,0,0,416,47,1,0,0,0,417,418,
  	3,66,33,0,418,419,5,25,0,0,419,49,1,0,0,0,420,421,3,62,31,0,421,422,5,
  	49,0,0,422,423,3,70,35,0,423,424,5,50,0,0,424,51,1,0,0,0,425,428,3,54,
  	27,0,426,427,5,4,0,0,427,429,3,56,28,0,428,426,1,0,0,0,428,429,1,0,0,
  	0,429,53,1,0,0,0,430,431,3,58,29,0,431,432,5,39,0,0,432,434,1,0,0,0,433,
  	430,1,0,0,0,433,434,1,0,0,0,434,435,1,0,0,0,435,436,3,56,28,0,436,55,
  	1,0,0,0,437,438,3,70,35,0,438,57,1,0,0,0,439,440,3,70,35,0,440,59,1,0,
  	0,0,441,442,3,70,35,0,442,61,1,0,0,0,443,444,5,56,0,0,444,63,1,0,0,0,
  	445,446,5,55,0,0,446,65,1,0,0,0,447,450,3,62,31,0,448,450,3,64,32,0,449,
  	447,1,0,0,0,449,448,1,0,0,0,450,67,1,0,0,0,451,452,5,58,0,0,452,69,1,
  	0,0,0,453,454,5,54,0,0,454,71,1,0,0,0,455,456,7,4,0,0,456,73,1,0,0,0,
  	457,458,5,63,0,0,458,459,3,76,38,0,459,460,5,64,0,0,460,75,1,0,0,0,461,
  	466,3,78,39,0,462,463,5,66,0,0,463,465,3,78,39,0,464,462,1,0,0,0,465,
  	468,1,0,0,0,466,464,1,0,0,0,466,467,1,0,0,0,467,77,1,0,0,0,468,466,1,
  	0,0,0,469,471,3,80,40,0,470,469,1,0,0,0,471,472,1,0,0,0,472,470,1,0,0,
  	0,472,473,1,0,0,0,473,79,1,0,0,0,474,476,3,82,41,0,475,477,3,86,43,0,
  	476,475,1,0,0,0,476,477,1,0,0,0,477,81,1,0,0,0,478,481,3,84,42,0,479,
  	481,3,98,49,0,480,478,1,0,0,0,480,479,1,0,0,0,481,83,1,0,0,0,482,483,
  	5,70,0,0,483,484,3,76,38,0,484,485,5,71,0,0,485,85,1,0,0,0,486,494,5,
  	73,0,0,487,494,5,74,0,0,488,494,5,75,0,0,489,490,5,68,0,0,490,491,3,88,
  	44,0,491,492,5,69,0,0,492,494,1,0,0,0,493,486,1,0,0,0,493,487,1,0,0,0,
  	493,488,1,0,0,0,493,489,1,0,0,0,494,87,1,0,0,0,495,500,3,90,45,0,496,
  	500,3,92,46,0,497,500,3,94,47,0,498,500,3,96,48,0,499,495,1,0,0,0,499,
  	496,1,0,0,0,499,497,1,0,0,0,499,498,1,0,0,0,500,89,1,0,0,0,501,502,3,
  	122,61,0,502,91,1,0,0,0,503,504,3,122,61,0,504,505,5,72,0,0,505,506,3,
  	122,61,0,506,93,1,0,0,0,507,508,3,122,61,0,508,509,5,72,0,0,509,95,1,
  	0,0,0,510,511,5,72,0,0,511,512,3,122,61,0,512,97,1,0,0,0,513,517,3,100,
  	50,0,514,517,3,120,60,0,515,517,3,114,57,0,516,513,1,0,0,0,516,514,1,
  	0,0,0,516,515,1,0,0,0,517,99,1,0,0,0,518,520,5,78,0,0,519,521,5,76,0,
  	0,520,519,1,0,0,0,520,521,1,0,0,0,521,523,1,0,0,0,522,524,3,102,51,0,
  	523,522,1,0,0,0,524,525,1,0,0,0,525,523,1,0,0,0,525,526,1,0,0,0,526,527,
  	1,0,0,0,527,528,5,79,0,0,528,101,1,0,0,0,529,533,3,104,52,0,530,533,3,
  	120,60,0,531,533,3,106,53,0,532,529,1,0,0,0,532,530,1,0,0,0,532,531,1,
  	0,0,0,533,103,1,0,0,0,534,535,3,108,54,0,535,536,5,77,0,0,536,537,3,108,
  	54,0,537,105,1,0,0,0,538,539,3,108,54,0,539,107,1,0,0,0,540,543,3,110,
  	55,0,541,543,3,112,56,0,542,540,1,0,0,0,542,541,1,0,0,0,543,109,1,0,0,
  	0,544,545,5,80,0,0,545,546,7,5,0,0,546,111,1,0,0,0,547,548,8,5,0,0,548,
  	113,1,0,0,0,549,553,3,116,58,0,550,553,5,82,0,0,551,553,3,118,59,0,552,
  	549,1,0,0,0,552,550,1,0,0,0,552,551,1,0,0,0,553,115,1,0,0,0,554,555,5,
  	80,0,0,555,556,7,6,0,0,556,117,1,0,0,0,557,558,8,7,0,0,558,119,1,0,0,
  	0,559,560,7,8,0,0,560,121,1,0,0,0,561,563,5,91,0,0,562,561,1,0,0,0,563,
  	564,1,0,0,0,564,562,1,0,0,0,564,565,1,0,0,0,565,123,1,0,0,0,54,126,128,
  	138,147,151,156,160,168,177,180,188,191,199,220,222,233,240,246,260,268,
  	270,296,309,317,319,324,329,340,348,350,361,368,384,391,397,403,406,409,
  	428,433,449,466,472,476,480,493,499,516,520,525,532,542,552,564
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
    setState(128);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CEQLQueryParser::K_SELECT

    || _la == CEQLQueryParser::UNEXPECTED_CHAR) {
      setState(126);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case CEQLQueryParser::K_SELECT: {
          setState(124);
          core_query();
          break;
        }

        case CEQLQueryParser::UNEXPECTED_CHAR: {
          setState(125);
          error();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(130);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(131);
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
    setState(133);
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

tree::TerminalNode* CEQLQueryParser::Core_queryContext::K_LIMIT() {
  return getToken(CEQLQueryParser::K_LIMIT, 0);
}

CEQLQueryParser::LimitContext* CEQLQueryParser::Core_queryContext::limit() {
  return getRuleContext<CEQLQueryParser::LimitContext>(0);
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
    setState(136);
    match(CEQLQueryParser::K_SELECT);
    setState(138);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 269123594) != 0)) {
      setState(137);
      selection_strategy();
    }
    setState(140);
    list_of_variables();
    setState(141);
    from_clause();
    setState(142);
    match(CEQLQueryParser::K_WHERE);
    setState(143);
    cel_formula(0);
    setState(147);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQLQueryParser::K_PARTITION) {
      setState(144);
      match(CEQLQueryParser::K_PARTITION);
      setState(145);
      match(CEQLQueryParser::K_BY);
      setState(146);
      partition_list();
    }
    setState(151);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQLQueryParser::K_WITHIN) {
      setState(149);
      match(CEQLQueryParser::K_WITHIN);
      setState(150);
      time_window();
    }
    setState(156);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQLQueryParser::K_CONSUME) {
      setState(153);
      match(CEQLQueryParser::K_CONSUME);
      setState(154);
      match(CEQLQueryParser::K_BY);
      setState(155);
      consumption_policy();
    }
    setState(160);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQLQueryParser::K_LIMIT) {
      setState(158);
      match(CEQLQueryParser::K_LIMIT);
      setState(159);
      limit();
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
    setState(168);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::K_ALL: {
        _localctx = _tracker.createInstance<CEQLQueryParser::Ss_allContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(162);
        match(CEQLQueryParser::K_ALL);
        break;
      }

      case CEQLQueryParser::K_ANY: {
        _localctx = _tracker.createInstance<CEQLQueryParser::Ss_anyContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(163);
        match(CEQLQueryParser::K_ANY);
        break;
      }

      case CEQLQueryParser::K_LAST: {
        _localctx = _tracker.createInstance<CEQLQueryParser::Ss_lastContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(164);
        match(CEQLQueryParser::K_LAST);
        break;
      }

      case CEQLQueryParser::K_MAX: {
        _localctx = _tracker.createInstance<CEQLQueryParser::Ss_maxContext>(_localctx);
        enterOuterAlt(_localctx, 4);
        setState(165);
        match(CEQLQueryParser::K_MAX);
        break;
      }

      case CEQLQueryParser::K_NEXT: {
        _localctx = _tracker.createInstance<CEQLQueryParser::Ss_nextContext>(_localctx);
        enterOuterAlt(_localctx, 5);
        setState(166);
        match(CEQLQueryParser::K_NEXT);
        break;
      }

      case CEQLQueryParser::K_STRICT: {
        _localctx = _tracker.createInstance<CEQLQueryParser::Ss_strictContext>(_localctx);
        enterOuterAlt(_localctx, 6);
        setState(167);
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
//----------------- S_noneContext ------------------------------------------------------------------

tree::TerminalNode* CEQLQueryParser::S_noneContext::K_NONE() {
  return getToken(CEQLQueryParser::K_NONE, 0);
}

CEQLQueryParser::S_noneContext::S_noneContext(List_of_variablesContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::S_noneContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitS_none(this);
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
    setState(180);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::STAR: {
        _localctx = _tracker.createInstance<CEQLQueryParser::S_starContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(170);
        match(CEQLQueryParser::STAR);
        break;
      }

      case CEQLQueryParser::K_NONE: {
        _localctx = _tracker.createInstance<CEQLQueryParser::S_noneContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(171);
        match(CEQLQueryParser::K_NONE);
        break;
      }

      case CEQLQueryParser::IDENTIFIER: {
        _localctx = _tracker.createInstance<CEQLQueryParser::S_list_of_variablesContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(172);
        any_name();
        setState(177);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == CEQLQueryParser::COMMA) {
          setState(173);
          match(CEQLQueryParser::COMMA);
          setState(174);
          any_name();
          setState(179);
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
    setState(191);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQLQueryParser::K_FROM) {
      setState(182);
      match(CEQLQueryParser::K_FROM);
      setState(183);
      stream_name();
      setState(188);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == CEQLQueryParser::COMMA) {
        setState(184);
        match(CEQLQueryParser::COMMA);
        setState(185);
        stream_name();
        setState(190);
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
//----------------- Contiguous_iteration_cel_formulaContext ------------------------------------------------------------------

CEQLQueryParser::Cel_formulaContext* CEQLQueryParser::Contiguous_iteration_cel_formulaContext::cel_formula() {
  return getRuleContext<CEQLQueryParser::Cel_formulaContext>(0);
}

tree::TerminalNode* CEQLQueryParser::Contiguous_iteration_cel_formulaContext::COLON_PLUS() {
  return getToken(CEQLQueryParser::COLON_PLUS, 0);
}

CEQLQueryParser::Contiguous_iteration_cel_formulaContext::Contiguous_iteration_cel_formulaContext(Cel_formulaContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Contiguous_iteration_cel_formulaContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitContiguous_iteration_cel_formula(this);
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
//----------------- Non_contiguous_iteration_cel_formulaContext ------------------------------------------------------------------

CEQLQueryParser::Cel_formulaContext* CEQLQueryParser::Non_contiguous_iteration_cel_formulaContext::cel_formula() {
  return getRuleContext<CEQLQueryParser::Cel_formulaContext>(0);
}

tree::TerminalNode* CEQLQueryParser::Non_contiguous_iteration_cel_formulaContext::PLUS() {
  return getToken(CEQLQueryParser::PLUS, 0);
}

CEQLQueryParser::Non_contiguous_iteration_cel_formulaContext::Non_contiguous_iteration_cel_formulaContext(Cel_formulaContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Non_contiguous_iteration_cel_formulaContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitNon_contiguous_iteration_cel_formula(this);
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
    setState(199);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::LEFT_PARENTHESIS: {
        _localctx = _tracker.createInstance<Par_cel_formulaContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(194);
        match(CEQLQueryParser::LEFT_PARENTHESIS);
        setState(195);
        cel_formula(0);
        setState(196);
        match(CEQLQueryParser::RIGHT_PARENTHESIS);
        break;
      }

      case CEQLQueryParser::IDENTIFIER: {
        _localctx = _tracker.createInstance<Event_type_cel_formulaContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(198);
        s_event_name();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(222);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 14, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(220);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 13, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<Non_contiguous_sequencing_cel_formulaContext>(_tracker.createInstance<Cel_formulaContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleCel_formula);
          setState(201);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(202);
          match(CEQLQueryParser::SEMICOLON);
          setState(203);
          cel_formula(5);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<Contiguous_sequencing_cel_formulaContext>(_tracker.createInstance<Cel_formulaContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleCel_formula);
          setState(204);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(205);
          match(CEQLQueryParser::COLON);
          setState(206);
          cel_formula(4);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<Or_cel_formulaContext>(_tracker.createInstance<Cel_formulaContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleCel_formula);
          setState(207);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(208);
          match(CEQLQueryParser::K_OR);
          setState(209);
          cel_formula(3);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<As_cel_formulaContext>(_tracker.createInstance<Cel_formulaContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleCel_formula);
          setState(210);

          if (!(precpred(_ctx, 7))) throw FailedPredicateException(this, "precpred(_ctx, 7)");
          setState(211);
          match(CEQLQueryParser::K_AS);
          setState(212);
          event_name();
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<Non_contiguous_iteration_cel_formulaContext>(_tracker.createInstance<Cel_formulaContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleCel_formula);
          setState(213);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(214);
          match(CEQLQueryParser::PLUS);
          break;
        }

        case 6: {
          auto newContext = _tracker.createInstance<Contiguous_iteration_cel_formulaContext>(_tracker.createInstance<Cel_formulaContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleCel_formula);
          setState(215);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(216);
          match(CEQLQueryParser::COLON_PLUS);
          break;
        }

        case 7: {
          auto newContext = _tracker.createInstance<Filter_cel_formulaContext>(_tracker.createInstance<Cel_formulaContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleCel_formula);
          setState(217);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(218);
          match(CEQLQueryParser::K_FILTER);
          setState(219);
          filter(0);
          break;
        }

        default:
          break;
        } 
      }
      setState(224);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 14, _ctx);
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
    setState(225);
    match(CEQLQueryParser::LEFT_SQUARE_BRACKET);
    setState(226);
    attribute_list();
    setState(227);
    match(CEQLQueryParser::RIGHT_SQUARE_BRACKET);
    setState(233);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQLQueryParser::COMMA) {
      setState(228);
      match(CEQLQueryParser::COMMA);
      setState(229);
      match(CEQLQueryParser::LEFT_SQUARE_BRACKET);
      setState(230);
      attribute_list();
      setState(231);
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
    setState(235);
    attribute_name();
    setState(240);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CEQLQueryParser::COMMA) {
      setState(236);
      match(CEQLQueryParser::COMMA);
      setState(237);
      attribute_name();
      setState(242);
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
    setState(246);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::K_ANY: {
        _localctx = _tracker.createInstance<CEQLQueryParser::Cp_anyContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(243);
        match(CEQLQueryParser::K_ANY);
        break;
      }

      case CEQLQueryParser::K_PARTITION: {
        _localctx = _tracker.createInstance<CEQLQueryParser::Cp_partitionContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(244);
        match(CEQLQueryParser::K_PARTITION);
        break;
      }

      case CEQLQueryParser::K_NONE: {
        _localctx = _tracker.createInstance<CEQLQueryParser::Cp_noneContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(245);
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

//----------------- LimitContext ------------------------------------------------------------------

CEQLQueryParser::LimitContext::LimitContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CEQLQueryParser::LimitContext::getRuleIndex() const {
  return CEQLQueryParser::RuleLimit;
}

void CEQLQueryParser::LimitContext::copyFrom(LimitContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- L_integerContext ------------------------------------------------------------------

CEQLQueryParser::IntegerContext* CEQLQueryParser::L_integerContext::integer() {
  return getRuleContext<CEQLQueryParser::IntegerContext>(0);
}

CEQLQueryParser::L_integerContext::L_integerContext(LimitContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::L_integerContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitL_integer(this);
  else
    return visitor->visitChildren(this);
}
CEQLQueryParser::LimitContext* CEQLQueryParser::limit() {
  LimitContext *_localctx = _tracker.createInstance<LimitContext>(_ctx, getState());
  enterRule(_localctx, 20, CEQLQueryParser::RuleLimit);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    _localctx = _tracker.createInstance<CEQLQueryParser::L_integerContext>(_localctx);
    enterOuterAlt(_localctx, 1);
    setState(248);
    integer();
   
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
  size_t startState = 22;
  enterRecursionRule(_localctx, 22, CEQLQueryParser::RuleFilter, precedence);

    

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
    setState(260);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::LEFT_PARENTHESIS: {
        _localctx = _tracker.createInstance<Par_filterContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(251);
        match(CEQLQueryParser::LEFT_PARENTHESIS);
        setState(252);
        filter(0);
        setState(253);
        match(CEQLQueryParser::RIGHT_PARENTHESIS);
        break;
      }

      case CEQLQueryParser::IDENTIFIER: {
        _localctx = _tracker.createInstance<Atomic_filterContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(255);
        event_name();
        setState(256);
        match(CEQLQueryParser::LEFT_SQUARE_BRACKET);
        setState(257);
        predicate(0);
        setState(258);
        match(CEQLQueryParser::RIGHT_SQUARE_BRACKET);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(270);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 20, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(268);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 19, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<And_filterContext>(_tracker.createInstance<FilterContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleFilter);
          setState(262);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(263);
          match(CEQLQueryParser::K_AND);
          setState(264);
          filter(3);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<Or_filterContext>(_tracker.createInstance<FilterContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleFilter);
          setState(265);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(266);
          match(CEQLQueryParser::K_OR);
          setState(267);
          filter(2);
          break;
        }

        default:
          break;
        } 
      }
      setState(272);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 20, _ctx);
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
//----------------- In_range_predicateContext ------------------------------------------------------------------

std::vector<CEQLQueryParser::Math_exprContext *> CEQLQueryParser::In_range_predicateContext::math_expr() {
  return getRuleContexts<CEQLQueryParser::Math_exprContext>();
}

CEQLQueryParser::Math_exprContext* CEQLQueryParser::In_range_predicateContext::math_expr(size_t i) {
  return getRuleContext<CEQLQueryParser::Math_exprContext>(i);
}

tree::TerminalNode* CEQLQueryParser::In_range_predicateContext::K_IN() {
  return getToken(CEQLQueryParser::K_IN, 0);
}

tree::TerminalNode* CEQLQueryParser::In_range_predicateContext::K_RANGE() {
  return getToken(CEQLQueryParser::K_RANGE, 0);
}

tree::TerminalNode* CEQLQueryParser::In_range_predicateContext::LEFT_PARENTHESIS() {
  return getToken(CEQLQueryParser::LEFT_PARENTHESIS, 0);
}

tree::TerminalNode* CEQLQueryParser::In_range_predicateContext::COMMA() {
  return getToken(CEQLQueryParser::COMMA, 0);
}

tree::TerminalNode* CEQLQueryParser::In_range_predicateContext::RIGHT_PARENTHESIS() {
  return getToken(CEQLQueryParser::RIGHT_PARENTHESIS, 0);
}

CEQLQueryParser::In_range_predicateContext::In_range_predicateContext(PredicateContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::In_range_predicateContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitIn_range_predicate(this);
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
  size_t startState = 24;
  enterRecursionRule(_localctx, 24, CEQLQueryParser::RulePredicate, precedence);

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
    setState(309);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 22, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<Par_predicateContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(274);
      match(CEQLQueryParser::LEFT_PARENTHESIS);
      setState(275);
      predicate(0);
      setState(276);
      match(CEQLQueryParser::RIGHT_PARENTHESIS);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<Not_predicateContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(278);
      match(CEQLQueryParser::K_NOT);
      setState(279);
      predicate(8);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<Inequality_predicateContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(280);
      math_expr(0);
      setState(281);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 8658654068736) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(282);
      math_expr(0);
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<Equality_string_predicateContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(284);
      string_literal();
      setState(285);
      _la = _input->LA(1);
      if (!(_la == CEQLQueryParser::EQ

      || _la == CEQLQueryParser::NEQ)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(286);
      string_literal_or_regexp();
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<Regex_predicateContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(288);
      attribute_name();
      setState(289);
      match(CEQLQueryParser::K_LIKE);
      setState(290);
      regexp();
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<In_predicateContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(292);
      attribute_name();
      setState(296);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case CEQLQueryParser::K_IN: {
          setState(293);
          match(CEQLQueryParser::K_IN);
          break;
        }

        case CEQLQueryParser::K_NOT: {
          setState(294);
          match(CEQLQueryParser::K_NOT);
          setState(295);
          match(CEQLQueryParser::K_IN);
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(298);
      value_seq();
      break;
    }

    case 7: {
      _localctx = _tracker.createInstance<In_range_predicateContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(300);
      math_expr(0);
      setState(301);
      match(CEQLQueryParser::K_IN);
      setState(302);
      match(CEQLQueryParser::K_RANGE);
      setState(303);
      match(CEQLQueryParser::LEFT_PARENTHESIS);
      setState(304);
      math_expr(0);
      setState(305);
      match(CEQLQueryParser::COMMA);
      setState(306);
      math_expr(0);
      setState(307);
      match(CEQLQueryParser::RIGHT_PARENTHESIS);
      break;
    }

    default:
      break;
    }
    _ctx->stop = _input->LT(-1);
    setState(319);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 24, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(317);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 23, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<And_predicateContext>(_tracker.createInstance<PredicateContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RulePredicate);
          setState(311);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(312);
          match(CEQLQueryParser::K_AND);
          setState(313);
          predicate(6);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<Or_predicateContext>(_tracker.createInstance<PredicateContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RulePredicate);
          setState(314);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(315);
          match(CEQLQueryParser::K_OR);
          setState(316);
          predicate(5);
          break;
        }

        default:
          break;
        } 
      }
      setState(321);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 24, _ctx);
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
  enterRule(_localctx, 26, CEQLQueryParser::RuleString_literal);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(324);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::STRING_LITERAL: {
        enterOuterAlt(_localctx, 1);
        setState(322);
        string();
        break;
      }

      case CEQLQueryParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 2);
        setState(323);
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
  enterRule(_localctx, 28, CEQLQueryParser::RuleString_literal_or_regexp);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(329);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::STRING_LITERAL: {
        enterOuterAlt(_localctx, 1);
        setState(326);
        string();
        break;
      }

      case CEQLQueryParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 2);
        setState(327);
        attribute_name();
        break;
      }

      case CEQLQueryParser::REGEX_START: {
        enterOuterAlt(_localctx, 3);
        setState(328);
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
  size_t startState = 30;
  enterRecursionRule(_localctx, 30, CEQLQueryParser::RuleMath_expr, precedence);

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
    setState(340);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::LEFT_PARENTHESIS: {
        _localctx = _tracker.createInstance<Par_math_exprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(332);
        match(CEQLQueryParser::LEFT_PARENTHESIS);
        setState(333);
        math_expr(0);
        setState(334);
        match(CEQLQueryParser::RIGHT_PARENTHESIS);
        break;
      }

      case CEQLQueryParser::DOUBLE_LITERAL:
      case CEQLQueryParser::INTEGER_LITERAL: {
        _localctx = _tracker.createInstance<Number_math_exprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(336);
        number();
        break;
      }

      case CEQLQueryParser::IDENTIFIER: {
        _localctx = _tracker.createInstance<Attribute_math_exprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(337);
        attribute_name();
        break;
      }

      case CEQLQueryParser::PLUS:
      case CEQLQueryParser::MINUS: {
        _localctx = _tracker.createInstance<Unary_math_exprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(338);
        _la = _input->LA(1);
        if (!(_la == CEQLQueryParser::PLUS

        || _la == CEQLQueryParser::MINUS)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(339);
        math_expr(3);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(350);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 29, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(348);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 28, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<Mul_math_exprContext>(_tracker.createInstance<Math_exprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleMath_expr);
          setState(342);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(343);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & 107374182400) != 0))) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(344);
          math_expr(3);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<Sum_math_exprContext>(_tracker.createInstance<Math_exprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleMath_expr);
          setState(345);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(346);
          _la = _input->LA(1);
          if (!(_la == CEQLQueryParser::PLUS

          || _la == CEQLQueryParser::MINUS)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(347);
          math_expr(2);
          break;
        }

        default:
          break;
        } 
      }
      setState(352);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 29, _ctx);
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
  enterRule(_localctx, 32, CEQLQueryParser::RuleValue_seq);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(361);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 30, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(353);
      match(CEQLQueryParser::LEFT_CURLY_BRACKET);
      setState(354);
      number_seq();
      setState(355);
      match(CEQLQueryParser::RIGHT_CURLY_BRACKET);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(357);
      match(CEQLQueryParser::LEFT_CURLY_BRACKET);
      setState(358);
      string_seq();
      setState(359);
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
  enterRule(_localctx, 34, CEQLQueryParser::RuleNumber_seq);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(384);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 32, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<CEQLQueryParser::Number_listContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(363);
      number();
      setState(368);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == CEQLQueryParser::COMMA) {
        setState(364);
        match(CEQLQueryParser::COMMA);
        setState(365);
        number();
        setState(370);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<CEQLQueryParser::Integer_rangeContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(371);
      integer();
      setState(372);
      match(CEQLQueryParser::DOUBLE_DOT);
      setState(373);
      integer();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<CEQLQueryParser::Double_rangeContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(375);
      double_();
      setState(376);
      match(CEQLQueryParser::DOUBLE_DOT);
      setState(377);
      double_();
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<CEQLQueryParser::Number_range_lowerContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(379);
      number();
      setState(380);
      match(CEQLQueryParser::DOUBLE_DOT);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<CEQLQueryParser::Number_range_upperContext>(_localctx);
      enterOuterAlt(_localctx, 5);
      setState(382);
      match(CEQLQueryParser::DOUBLE_DOT);
      setState(383);
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
  enterRule(_localctx, 36, CEQLQueryParser::RuleString_seq);
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
    setState(386);
    string();
    setState(391);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CEQLQueryParser::COMMA) {
      setState(387);
      match(CEQLQueryParser::COMMA);
      setState(388);
      string();
      setState(393);
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
  enterRule(_localctx, 38, CEQLQueryParser::RuleTime_window);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(397);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 34, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(394);
      event_span();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(395);
      time_span();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(396);
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
  enterRule(_localctx, 40, CEQLQueryParser::RuleEvent_span);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(399);
    integer();
    setState(400);
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
  enterRule(_localctx, 42, CEQLQueryParser::RuleTime_span);
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
    setState(403);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 35, _ctx)) {
    case 1: {
      setState(402);
      hour_span();
      break;
    }

    default:
      break;
    }
    setState(406);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 36, _ctx)) {
    case 1: {
      setState(405);
      minute_span();
      break;
    }

    default:
      break;
    }
    setState(409);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQLQueryParser::DOUBLE_LITERAL

    || _la == CEQLQueryParser::INTEGER_LITERAL) {
      setState(408);
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

CEQLQueryParser::NumberContext* CEQLQueryParser::Hour_spanContext::number() {
  return getRuleContext<CEQLQueryParser::NumberContext>(0);
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
  enterRule(_localctx, 44, CEQLQueryParser::RuleHour_span);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(411);
    number();
    setState(412);
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

CEQLQueryParser::NumberContext* CEQLQueryParser::Minute_spanContext::number() {
  return getRuleContext<CEQLQueryParser::NumberContext>(0);
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
  enterRule(_localctx, 46, CEQLQueryParser::RuleMinute_span);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(414);
    number();
    setState(415);
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

CEQLQueryParser::NumberContext* CEQLQueryParser::Second_spanContext::number() {
  return getRuleContext<CEQLQueryParser::NumberContext>(0);
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
  enterRule(_localctx, 48, CEQLQueryParser::RuleSecond_span);

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
    number();
    setState(418);
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
  enterRule(_localctx, 50, CEQLQueryParser::RuleCustom_span);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(420);
    integer();
    setState(421);
    match(CEQLQueryParser::LEFT_SQUARE_BRACKET);
    setState(422);
    any_name();
    setState(423);
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
  enterRule(_localctx, 52, CEQLQueryParser::RuleNamed_event);
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
    setState(425);
    s_event_name();
    setState(428);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQLQueryParser::K_AS) {
      setState(426);
      match(CEQLQueryParser::K_AS);
      setState(427);
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
  enterRule(_localctx, 54, CEQLQueryParser::RuleS_event_name);

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
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 39, _ctx)) {
    case 1: {
      setState(430);
      stream_name();
      setState(431);
      match(CEQLQueryParser::GE);
      break;
    }

    default:
      break;
    }
    setState(435);
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
  enterRule(_localctx, 56, CEQLQueryParser::RuleEvent_name);

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
  enterRule(_localctx, 58, CEQLQueryParser::RuleStream_name);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(439);
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
  enterRule(_localctx, 60, CEQLQueryParser::RuleAttribute_name);

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
  enterRule(_localctx, 62, CEQLQueryParser::RuleInteger);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(443);
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
  enterRule(_localctx, 64, CEQLQueryParser::RuleDouble);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(445);
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
  enterRule(_localctx, 66, CEQLQueryParser::RuleNumber);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(449);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::INTEGER_LITERAL: {
        enterOuterAlt(_localctx, 1);
        setState(447);
        integer();
        break;
      }

      case CEQLQueryParser::DOUBLE_LITERAL: {
        enterOuterAlt(_localctx, 2);
        setState(448);
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
  enterRule(_localctx, 68, CEQLQueryParser::RuleString);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(451);
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
  enterRule(_localctx, 70, CEQLQueryParser::RuleAny_name);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(453);
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

tree::TerminalNode* CEQLQueryParser::KeywordContext::K_RANGE() {
  return getToken(CEQLQueryParser::K_RANGE, 0);
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
  enterRule(_localctx, 72, CEQLQueryParser::RuleKeyword);
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
    setState(455);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 4294967166) != 0))) {
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
  enterRule(_localctx, 74, CEQLQueryParser::RuleRegexp);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(457);
    match(CEQLQueryParser::REGEX_START);
    setState(458);
    regexp_alternation();
    setState(459);
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
  enterRule(_localctx, 76, CEQLQueryParser::RuleRegexp_alternation);
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
    setState(461);
    regexp_exp();
    setState(466);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CEQLQueryParser::REGEX_PIPE) {
      setState(462);
      match(CEQLQueryParser::REGEX_PIPE);
      setState(463);
      regexp_exp();
      setState(468);
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
  enterRule(_localctx, 78, CEQLQueryParser::RuleRegexp_exp);

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
    setState(470); 
    _errHandler->sync(this);
    alt = 1;
    do {
      switch (alt) {
        case 1: {
              setState(469);
              regexp_element();
              break;
            }

      default:
        throw NoViableAltException(this);
      }
      setState(472); 
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 42, _ctx);
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
  enterRule(_localctx, 80, CEQLQueryParser::RuleRegexp_element);
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
    setState(474);
    regexp_group();
    setState(476);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 68) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 68)) & 225) != 0)) {
      setState(475);
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
  enterRule(_localctx, 82, CEQLQueryParser::RuleRegexp_group);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(480);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::REGEX_L_PAR: {
        enterOuterAlt(_localctx, 1);
        setState(478);
        parenthesis();
        break;
      }

      case CEQLQueryParser::K_ALL:
      case CEQLQueryParser::K_AND:
      case CEQLQueryParser::K_ANY:
      case CEQLQueryParser::K_AS:
      case CEQLQueryParser::K_BY:
      case CEQLQueryParser::K_CONSUME:
      case CEQLQueryParser::K_LIMIT:
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
      case CEQLQueryParser::K_RANGE:
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
      case CEQLQueryParser::COLON_PLUS:
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
        setState(479);
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
  enterRule(_localctx, 84, CEQLQueryParser::RuleParenthesis);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(482);
    match(CEQLQueryParser::REGEX_L_PAR);
    setState(483);
    regexp_alternation();
    setState(484);
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
  enterRule(_localctx, 86, CEQLQueryParser::RuleQuantifier);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(493);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::REGEX_QUESTION: {
        enterOuterAlt(_localctx, 1);
        setState(486);
        match(CEQLQueryParser::REGEX_QUESTION);
        break;
      }

      case CEQLQueryParser::REGEX_PLUS: {
        enterOuterAlt(_localctx, 2);
        setState(487);
        match(CEQLQueryParser::REGEX_PLUS);
        break;
      }

      case CEQLQueryParser::REGEX_STAR: {
        enterOuterAlt(_localctx, 3);
        setState(488);
        match(CEQLQueryParser::REGEX_STAR);
        break;
      }

      case CEQLQueryParser::REGEX_L_CURLY: {
        enterOuterAlt(_localctx, 4);
        setState(489);
        match(CEQLQueryParser::REGEX_L_CURLY);
        setState(490);
        quantity();
        setState(491);
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
  enterRule(_localctx, 88, CEQLQueryParser::RuleQuantity);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(499);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 46, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(495);
      quantExact();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(496);
      quantRange();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(497);
      quantMin();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(498);
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
  enterRule(_localctx, 90, CEQLQueryParser::RuleQuantExact);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(501);
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
  enterRule(_localctx, 92, CEQLQueryParser::RuleQuantRange);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(503);
    regexp_number();
    setState(504);
    match(CEQLQueryParser::REGEX_COMMA);
    setState(505);
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
  enterRule(_localctx, 94, CEQLQueryParser::RuleQuantMin);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(507);
    regexp_number();
    setState(508);
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
  enterRule(_localctx, 96, CEQLQueryParser::RuleQuantMax);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(510);
    match(CEQLQueryParser::REGEX_COMMA);
    setState(511);
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
  enterRule(_localctx, 98, CEQLQueryParser::RuleAtom);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(516);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 47, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(513);
      characterClass();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(514);
      sharedAtom();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(515);
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
  enterRule(_localctx, 100, CEQLQueryParser::RuleCharacterClass);
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
    setState(518);
    match(CEQLQueryParser::REGEX_L_BRACK);
    setState(520);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQLQueryParser::REGEX_HAT) {
      setState(519);
      match(CEQLQueryParser::REGEX_HAT);
    }
    setState(523); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(522);
      ccAtom();
      setState(525); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -2) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & 268390399) != 0));
    setState(527);
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
  enterRule(_localctx, 102, CEQLQueryParser::RuleCcAtom);

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
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 50, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(529);
      ccRange();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(530);
      sharedAtom();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(531);
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
  enterRule(_localctx, 104, CEQLQueryParser::RuleCcRange);

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
    ccLiteral();
    setState(535);
    match(CEQLQueryParser::REGEX_HYPHEN);
    setState(536);
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
  enterRule(_localctx, 106, CEQLQueryParser::RuleCcSingle);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(538);
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
  enterRule(_localctx, 108, CEQLQueryParser::RuleCcLiteral);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(542);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::REGEX_BACKSLASH: {
        enterOuterAlt(_localctx, 1);
        setState(540);
        ccEscapes();
        break;
      }

      case CEQLQueryParser::K_ALL:
      case CEQLQueryParser::K_AND:
      case CEQLQueryParser::K_ANY:
      case CEQLQueryParser::K_AS:
      case CEQLQueryParser::K_BY:
      case CEQLQueryParser::K_CONSUME:
      case CEQLQueryParser::K_LIMIT:
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
      case CEQLQueryParser::K_RANGE:
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
      case CEQLQueryParser::COLON_PLUS:
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
        setState(541);
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
  enterRule(_localctx, 110, CEQLQueryParser::RuleCcEscapes);
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
    setState(544);
    match(CEQLQueryParser::REGEX_BACKSLASH);
    setState(545);
    _la = _input->LA(1);
    if (!(((((_la - 76) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 76)) & 27) != 0))) {
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
  enterRule(_localctx, 112, CEQLQueryParser::RuleCcOther);
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
    setState(547);
    _la = _input->LA(1);
    if (_la == 0 || _la == Token::EOF || (((((_la - 76) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 76)) & 27) != 0))) {
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
  enterRule(_localctx, 114, CEQLQueryParser::RuleLiteral);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(552);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 52, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(549);
      escapes();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(550);
      match(CEQLQueryParser::REGEX_DOT);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(551);
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
  enterRule(_localctx, 116, CEQLQueryParser::RuleEscapes);
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
    setState(554);
    match(CEQLQueryParser::REGEX_BACKSLASH);
    setState(555);
    _la = _input->LA(1);
    if (!(((((_la - 66) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 66)) & 95165) != 0))) {
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
  enterRule(_localctx, 118, CEQLQueryParser::RuleOther);
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
    setState(557);
    _la = _input->LA(1);
    if (_la == 0 || _la == Token::EOF || (((((_la - 66) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 66)) & 29629) != 0))) {
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
  enterRule(_localctx, 120, CEQLQueryParser::RuleSharedAtom);
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
    setState(559);
    _la = _input->LA(1);
    if (!(((((_la - 85) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 85)) & 63) != 0))) {
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
  enterRule(_localctx, 122, CEQLQueryParser::RuleRegexp_number);
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
    setState(562); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(561);
      match(CEQLQueryParser::REGEX_DIGIT);
      setState(564); 
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
    case 11: return filterSempred(antlrcpp::downCast<FilterContext *>(context), predicateIndex);
    case 12: return predicateSempred(antlrcpp::downCast<PredicateContext *>(context), predicateIndex);
    case 15: return math_exprSempred(antlrcpp::downCast<Math_exprContext *>(context), predicateIndex);

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
    case 3: return precpred(_ctx, 7);
    case 4: return precpred(_ctx, 6);
    case 5: return precpred(_ctx, 5);
    case 6: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

bool CEQLQueryParser::filterSempred(FilterContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 7: return precpred(_ctx, 2);
    case 8: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

bool CEQLQueryParser::predicateSempred(PredicateContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 9: return precpred(_ctx, 5);
    case 10: return precpred(_ctx, 4);

  default:
    break;
  }
  return true;
}

bool CEQLQueryParser::math_exprSempred(Math_exprContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 11: return precpred(_ctx, 2);
    case 12: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

void CEQLQueryParser::initialize() {
  ::antlr4::internal::call_once(ceqlqueryparserParserOnceFlag, ceqlqueryparserParserInitialize);
}
