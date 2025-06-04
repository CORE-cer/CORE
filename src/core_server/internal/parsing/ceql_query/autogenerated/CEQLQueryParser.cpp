
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
      "minute_span", "second_span", "custom_span", "named_event", "s_event_name_with_projection", 
      "s_event_name", "event_name", "atomic_cel_formula", "stream_name", 
      "list_of_attribute_names", "attribute_name", "integer", "double", 
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
  	4,1,91,604,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,7,
  	35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,2,40,7,40,2,41,7,41,2,42,7,
  	42,2,43,7,43,2,44,7,44,2,45,7,45,2,46,7,46,2,47,7,47,2,48,7,48,2,49,7,
  	49,2,50,7,50,2,51,7,51,2,52,7,52,2,53,7,53,2,54,7,54,2,55,7,55,2,56,7,
  	56,2,57,7,57,2,58,7,58,2,59,7,59,2,60,7,60,2,61,7,61,2,62,7,62,2,63,7,
  	63,2,64,7,64,1,0,1,0,5,0,133,8,0,10,0,12,0,136,9,0,1,0,1,0,1,1,1,1,1,
  	1,1,2,1,2,3,2,145,8,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,3,2,154,8,2,1,2,1,2,
  	3,2,158,8,2,1,2,1,2,1,2,3,2,163,8,2,1,2,1,2,3,2,167,8,2,1,3,1,3,1,3,1,
  	3,1,3,1,3,3,3,175,8,3,1,4,1,4,1,4,1,4,1,4,5,4,182,8,4,10,4,12,4,185,9,
  	4,3,4,187,8,4,1,5,1,5,1,5,1,5,5,5,193,8,5,10,5,12,5,196,9,5,3,5,198,8,
  	5,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,3,6,211,8,6,1,6,1,6,1,6,
  	1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,5,6,232,
  	8,6,10,6,12,6,235,9,6,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,3,7,245,8,7,1,8,
  	1,8,1,8,5,8,250,8,8,10,8,12,8,253,9,8,1,9,1,9,1,9,3,9,258,8,9,1,10,1,
  	10,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,3,11,272,8,11,1,
  	11,1,11,1,11,1,11,1,11,1,11,5,11,280,8,11,10,11,12,11,283,9,11,1,12,1,
  	12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,1,
  	12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,3,12,308,8,12,1,12,1,12,1,12,1,
  	12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,3,12,321,8,12,1,12,1,12,1,12,1,
  	12,1,12,1,12,5,12,329,8,12,10,12,12,12,332,9,12,1,13,1,13,3,13,336,8,
  	13,1,14,1,14,1,14,3,14,341,8,14,1,15,1,15,1,15,1,15,1,15,1,15,1,15,1,
  	15,1,15,3,15,352,8,15,1,15,1,15,1,15,1,15,1,15,1,15,5,15,360,8,15,10,
  	15,12,15,363,9,15,1,16,1,16,1,16,1,16,1,16,1,16,1,16,1,16,3,16,373,8,
  	16,1,17,1,17,1,17,5,17,378,8,17,10,17,12,17,381,9,17,1,17,1,17,1,17,1,
  	17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,3,17,396,8,17,1,18,1,
  	18,1,18,5,18,401,8,18,10,18,12,18,404,9,18,1,19,1,19,1,19,3,19,409,8,
  	19,1,20,1,20,1,20,1,21,3,21,415,8,21,1,21,3,21,418,8,21,1,21,3,21,421,
  	8,21,1,22,1,22,1,22,1,23,1,23,1,23,1,24,1,24,1,24,1,25,1,25,1,25,1,25,
  	1,25,1,26,1,26,1,26,3,26,440,8,26,1,27,1,27,1,27,1,27,1,27,3,27,447,8,
  	27,1,28,1,28,1,28,3,28,452,8,28,1,28,1,28,1,29,1,29,1,30,1,30,1,30,1,
  	30,1,30,1,30,5,30,464,8,30,10,30,12,30,467,9,30,1,31,1,31,1,32,1,32,1,
  	32,5,32,474,8,32,10,32,12,32,477,9,32,1,33,1,33,1,34,1,34,1,35,1,35,1,
  	36,1,36,3,36,487,8,36,1,37,1,37,1,38,1,38,1,39,1,39,1,40,1,40,1,40,1,
  	40,1,41,1,41,1,41,5,41,502,8,41,10,41,12,41,505,9,41,1,42,4,42,508,8,
  	42,11,42,12,42,509,1,43,1,43,3,43,514,8,43,1,44,1,44,3,44,518,8,44,1,
  	45,1,45,1,45,1,45,1,46,1,46,1,46,1,46,1,46,1,46,1,46,3,46,531,8,46,1,
  	47,1,47,1,47,1,47,3,47,537,8,47,1,48,1,48,1,49,1,49,1,49,1,49,1,50,1,
  	50,1,50,1,51,1,51,1,51,1,52,1,52,1,52,3,52,554,8,52,1,53,1,53,3,53,558,
  	8,53,1,53,4,53,561,8,53,11,53,12,53,562,1,53,1,53,1,54,1,54,1,54,3,54,
  	570,8,54,1,55,1,55,1,55,1,55,1,56,1,56,1,57,1,57,3,57,580,8,57,1,58,1,
  	58,1,58,1,59,1,59,1,60,1,60,1,60,3,60,590,8,60,1,61,1,61,1,61,1,62,1,
  	62,1,63,1,63,1,64,4,64,600,8,64,11,64,12,64,601,1,64,0,5,12,22,24,30,
  	60,65,0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,
  	46,48,50,52,54,56,58,60,62,64,66,68,70,72,74,76,78,80,82,84,86,88,90,
  	92,94,96,98,100,102,104,106,108,110,112,114,116,118,120,122,124,126,128,
  	0,10,2,0,21,21,67,67,1,0,37,42,1,0,41,42,1,0,33,34,2,0,32,32,35,36,2,
  	0,1,6,8,31,2,0,76,77,79,80,5,0,66,66,68,71,73,75,78,80,82,82,4,0,66,66,
  	68,71,73,75,78,80,1,0,85,90,626,0,134,1,0,0,0,2,139,1,0,0,0,4,142,1,0,
  	0,0,6,174,1,0,0,0,8,186,1,0,0,0,10,197,1,0,0,0,12,210,1,0,0,0,14,236,
  	1,0,0,0,16,246,1,0,0,0,18,257,1,0,0,0,20,259,1,0,0,0,22,271,1,0,0,0,24,
  	320,1,0,0,0,26,335,1,0,0,0,28,340,1,0,0,0,30,351,1,0,0,0,32,372,1,0,0,
  	0,34,395,1,0,0,0,36,397,1,0,0,0,38,408,1,0,0,0,40,410,1,0,0,0,42,414,
  	1,0,0,0,44,422,1,0,0,0,46,425,1,0,0,0,48,428,1,0,0,0,50,431,1,0,0,0,52,
  	436,1,0,0,0,54,441,1,0,0,0,56,451,1,0,0,0,58,455,1,0,0,0,60,457,1,0,0,
  	0,62,468,1,0,0,0,64,470,1,0,0,0,66,478,1,0,0,0,68,480,1,0,0,0,70,482,
  	1,0,0,0,72,486,1,0,0,0,74,488,1,0,0,0,76,490,1,0,0,0,78,492,1,0,0,0,80,
  	494,1,0,0,0,82,498,1,0,0,0,84,507,1,0,0,0,86,511,1,0,0,0,88,517,1,0,0,
  	0,90,519,1,0,0,0,92,530,1,0,0,0,94,536,1,0,0,0,96,538,1,0,0,0,98,540,
  	1,0,0,0,100,544,1,0,0,0,102,547,1,0,0,0,104,553,1,0,0,0,106,555,1,0,0,
  	0,108,569,1,0,0,0,110,571,1,0,0,0,112,575,1,0,0,0,114,579,1,0,0,0,116,
  	581,1,0,0,0,118,584,1,0,0,0,120,589,1,0,0,0,122,591,1,0,0,0,124,594,1,
  	0,0,0,126,596,1,0,0,0,128,599,1,0,0,0,130,133,3,4,2,0,131,133,3,2,1,0,
  	132,130,1,0,0,0,132,131,1,0,0,0,133,136,1,0,0,0,134,132,1,0,0,0,134,135,
  	1,0,0,0,135,137,1,0,0,0,136,134,1,0,0,0,137,138,5,0,0,1,138,1,1,0,0,0,
  	139,140,5,62,0,0,140,141,6,1,-1,0,141,3,1,0,0,0,142,144,5,26,0,0,143,
  	145,3,6,3,0,144,143,1,0,0,0,144,145,1,0,0,0,145,146,1,0,0,0,146,147,3,
  	8,4,0,147,148,3,10,5,0,148,149,5,30,0,0,149,153,3,12,6,0,150,151,5,23,
  	0,0,151,152,5,5,0,0,152,154,3,14,7,0,153,150,1,0,0,0,153,154,1,0,0,0,
  	154,157,1,0,0,0,155,156,5,31,0,0,156,158,3,38,19,0,157,155,1,0,0,0,157,
  	158,1,0,0,0,158,162,1,0,0,0,159,160,5,6,0,0,160,161,5,5,0,0,161,163,3,
  	18,9,0,162,159,1,0,0,0,162,163,1,0,0,0,163,166,1,0,0,0,164,165,5,7,0,
  	0,165,167,3,20,10,0,166,164,1,0,0,0,166,167,1,0,0,0,167,5,1,0,0,0,168,
  	175,5,1,0,0,169,175,5,3,0,0,170,175,5,15,0,0,171,175,5,17,0,0,172,175,
  	5,19,0,0,173,175,5,28,0,0,174,168,1,0,0,0,174,169,1,0,0,0,174,170,1,0,
  	0,0,174,171,1,0,0,0,174,172,1,0,0,0,174,173,1,0,0,0,175,7,1,0,0,0,176,
  	187,5,35,0,0,177,187,5,20,0,0,178,183,3,54,27,0,179,180,5,45,0,0,180,
  	182,3,54,27,0,181,179,1,0,0,0,182,185,1,0,0,0,183,181,1,0,0,0,183,184,
  	1,0,0,0,184,187,1,0,0,0,185,183,1,0,0,0,186,176,1,0,0,0,186,177,1,0,0,
  	0,186,178,1,0,0,0,187,9,1,0,0,0,188,189,5,12,0,0,189,194,3,62,31,0,190,
  	191,5,45,0,0,191,193,3,62,31,0,192,190,1,0,0,0,193,196,1,0,0,0,194,192,
  	1,0,0,0,194,195,1,0,0,0,195,198,1,0,0,0,196,194,1,0,0,0,197,188,1,0,0,
  	0,197,198,1,0,0,0,198,11,1,0,0,0,199,200,6,6,-1,0,200,201,5,47,0,0,201,
  	202,3,12,6,0,202,203,5,48,0,0,203,211,1,0,0,0,204,211,3,56,28,0,205,206,
  	7,0,0,0,206,207,5,47,0,0,207,208,3,60,30,0,208,209,5,48,0,0,209,211,1,
  	0,0,0,210,199,1,0,0,0,210,204,1,0,0,0,210,205,1,0,0,0,211,233,1,0,0,0,
  	212,213,10,4,0,0,213,214,5,43,0,0,214,232,3,12,6,5,215,216,10,3,0,0,216,
  	217,5,44,0,0,217,232,3,12,6,4,218,219,10,2,0,0,219,220,5,22,0,0,220,232,
  	3,12,6,3,221,222,10,7,0,0,222,223,5,4,0,0,223,232,3,58,29,0,224,225,10,
  	6,0,0,225,232,5,33,0,0,226,227,10,5,0,0,227,232,5,53,0,0,228,229,10,1,
  	0,0,229,230,5,11,0,0,230,232,3,22,11,0,231,212,1,0,0,0,231,215,1,0,0,
  	0,231,218,1,0,0,0,231,221,1,0,0,0,231,224,1,0,0,0,231,226,1,0,0,0,231,
  	228,1,0,0,0,232,235,1,0,0,0,233,231,1,0,0,0,233,234,1,0,0,0,234,13,1,
  	0,0,0,235,233,1,0,0,0,236,237,5,49,0,0,237,238,3,16,8,0,238,244,5,50,
  	0,0,239,240,5,45,0,0,240,241,5,49,0,0,241,242,3,16,8,0,242,243,5,50,0,
  	0,243,245,1,0,0,0,244,239,1,0,0,0,244,245,1,0,0,0,245,15,1,0,0,0,246,
  	251,3,66,33,0,247,248,5,45,0,0,248,250,3,66,33,0,249,247,1,0,0,0,250,
  	253,1,0,0,0,251,249,1,0,0,0,251,252,1,0,0,0,252,17,1,0,0,0,253,251,1,
  	0,0,0,254,258,5,3,0,0,255,258,5,23,0,0,256,258,5,20,0,0,257,254,1,0,0,
  	0,257,255,1,0,0,0,257,256,1,0,0,0,258,19,1,0,0,0,259,260,3,68,34,0,260,
  	21,1,0,0,0,261,262,6,11,-1,0,262,263,5,47,0,0,263,264,3,22,11,0,264,265,
  	5,48,0,0,265,272,1,0,0,0,266,267,3,56,28,0,267,268,5,49,0,0,268,269,3,
  	24,12,0,269,270,5,50,0,0,270,272,1,0,0,0,271,261,1,0,0,0,271,266,1,0,
  	0,0,272,281,1,0,0,0,273,274,10,2,0,0,274,275,5,2,0,0,275,280,3,22,11,
  	3,276,277,10,1,0,0,277,278,5,22,0,0,278,280,3,22,11,2,279,273,1,0,0,0,
  	279,276,1,0,0,0,280,283,1,0,0,0,281,279,1,0,0,0,281,282,1,0,0,0,282,23,
  	1,0,0,0,283,281,1,0,0,0,284,285,6,12,-1,0,285,286,5,47,0,0,286,287,3,
  	24,12,0,287,288,5,48,0,0,288,321,1,0,0,0,289,290,5,21,0,0,290,321,3,24,
  	12,8,291,292,3,30,15,0,292,293,7,1,0,0,293,294,3,30,15,0,294,321,1,0,
  	0,0,295,296,3,26,13,0,296,297,7,2,0,0,297,298,3,28,14,0,298,321,1,0,0,
  	0,299,300,3,66,33,0,300,301,5,16,0,0,301,302,3,80,40,0,302,321,1,0,0,
  	0,303,307,3,66,33,0,304,308,5,14,0,0,305,306,5,21,0,0,306,308,5,14,0,
  	0,307,304,1,0,0,0,307,305,1,0,0,0,308,309,1,0,0,0,309,310,3,32,16,0,310,
  	321,1,0,0,0,311,312,3,30,15,0,312,313,5,14,0,0,313,314,5,24,0,0,314,315,
  	5,47,0,0,315,316,3,30,15,0,316,317,5,45,0,0,317,318,3,30,15,0,318,319,
  	5,48,0,0,319,321,1,0,0,0,320,284,1,0,0,0,320,289,1,0,0,0,320,291,1,0,
  	0,0,320,295,1,0,0,0,320,299,1,0,0,0,320,303,1,0,0,0,320,311,1,0,0,0,321,
  	330,1,0,0,0,322,323,10,5,0,0,323,324,5,2,0,0,324,329,3,24,12,6,325,326,
  	10,4,0,0,326,327,5,22,0,0,327,329,3,24,12,5,328,322,1,0,0,0,328,325,1,
  	0,0,0,329,332,1,0,0,0,330,328,1,0,0,0,330,331,1,0,0,0,331,25,1,0,0,0,
  	332,330,1,0,0,0,333,336,3,74,37,0,334,336,3,66,33,0,335,333,1,0,0,0,335,
  	334,1,0,0,0,336,27,1,0,0,0,337,341,3,74,37,0,338,341,3,66,33,0,339,341,
  	3,80,40,0,340,337,1,0,0,0,340,338,1,0,0,0,340,339,1,0,0,0,341,29,1,0,
  	0,0,342,343,6,15,-1,0,343,344,5,47,0,0,344,345,3,30,15,0,345,346,5,48,
  	0,0,346,352,1,0,0,0,347,352,3,72,36,0,348,352,3,66,33,0,349,350,7,3,0,
  	0,350,352,3,30,15,3,351,342,1,0,0,0,351,347,1,0,0,0,351,348,1,0,0,0,351,
  	349,1,0,0,0,352,361,1,0,0,0,353,354,10,2,0,0,354,355,7,4,0,0,355,360,
  	3,30,15,3,356,357,10,1,0,0,357,358,7,3,0,0,358,360,3,30,15,2,359,353,
  	1,0,0,0,359,356,1,0,0,0,360,363,1,0,0,0,361,359,1,0,0,0,361,362,1,0,0,
  	0,362,31,1,0,0,0,363,361,1,0,0,0,364,365,5,51,0,0,365,366,3,34,17,0,366,
  	367,5,52,0,0,367,373,1,0,0,0,368,369,5,51,0,0,369,370,3,36,18,0,370,371,
  	5,52,0,0,371,373,1,0,0,0,372,364,1,0,0,0,372,368,1,0,0,0,373,33,1,0,0,
  	0,374,379,3,72,36,0,375,376,5,45,0,0,376,378,3,72,36,0,377,375,1,0,0,
  	0,378,381,1,0,0,0,379,377,1,0,0,0,379,380,1,0,0,0,380,396,1,0,0,0,381,
  	379,1,0,0,0,382,383,3,68,34,0,383,384,5,46,0,0,384,385,3,68,34,0,385,
  	396,1,0,0,0,386,387,3,70,35,0,387,388,5,46,0,0,388,389,3,70,35,0,389,
  	396,1,0,0,0,390,391,3,72,36,0,391,392,5,46,0,0,392,396,1,0,0,0,393,394,
  	5,46,0,0,394,396,3,72,36,0,395,374,1,0,0,0,395,382,1,0,0,0,395,386,1,
  	0,0,0,395,390,1,0,0,0,395,393,1,0,0,0,396,35,1,0,0,0,397,402,3,74,37,
  	0,398,399,5,45,0,0,399,401,3,74,37,0,400,398,1,0,0,0,401,404,1,0,0,0,
  	402,400,1,0,0,0,402,403,1,0,0,0,403,37,1,0,0,0,404,402,1,0,0,0,405,409,
  	3,40,20,0,406,409,3,42,21,0,407,409,3,50,25,0,408,405,1,0,0,0,408,406,
  	1,0,0,0,408,407,1,0,0,0,409,39,1,0,0,0,410,411,3,68,34,0,411,412,5,10,
  	0,0,412,41,1,0,0,0,413,415,3,44,22,0,414,413,1,0,0,0,414,415,1,0,0,0,
  	415,417,1,0,0,0,416,418,3,46,23,0,417,416,1,0,0,0,417,418,1,0,0,0,418,
  	420,1,0,0,0,419,421,3,48,24,0,420,419,1,0,0,0,420,421,1,0,0,0,421,43,
  	1,0,0,0,422,423,3,72,36,0,423,424,5,13,0,0,424,45,1,0,0,0,425,426,3,72,
  	36,0,426,427,5,18,0,0,427,47,1,0,0,0,428,429,3,72,36,0,429,430,5,25,0,
  	0,430,49,1,0,0,0,431,432,3,68,34,0,432,433,5,49,0,0,433,434,3,76,38,0,
  	434,435,5,50,0,0,435,51,1,0,0,0,436,439,3,56,28,0,437,438,5,4,0,0,438,
  	440,3,58,29,0,439,437,1,0,0,0,439,440,1,0,0,0,440,53,1,0,0,0,441,446,
  	3,56,28,0,442,443,5,49,0,0,443,444,3,64,32,0,444,445,5,50,0,0,445,447,
  	1,0,0,0,446,442,1,0,0,0,446,447,1,0,0,0,447,55,1,0,0,0,448,449,3,62,31,
  	0,449,450,5,39,0,0,450,452,1,0,0,0,451,448,1,0,0,0,451,452,1,0,0,0,452,
  	453,1,0,0,0,453,454,3,58,29,0,454,57,1,0,0,0,455,456,3,76,38,0,456,59,
  	1,0,0,0,457,458,6,30,-1,0,458,459,3,56,28,0,459,465,1,0,0,0,460,461,10,
  	1,0,0,461,462,5,11,0,0,462,464,3,22,11,0,463,460,1,0,0,0,464,467,1,0,
  	0,0,465,463,1,0,0,0,465,466,1,0,0,0,466,61,1,0,0,0,467,465,1,0,0,0,468,
  	469,3,76,38,0,469,63,1,0,0,0,470,475,3,66,33,0,471,472,5,45,0,0,472,474,
  	3,66,33,0,473,471,1,0,0,0,474,477,1,0,0,0,475,473,1,0,0,0,475,476,1,0,
  	0,0,476,65,1,0,0,0,477,475,1,0,0,0,478,479,3,76,38,0,479,67,1,0,0,0,480,
  	481,5,56,0,0,481,69,1,0,0,0,482,483,5,55,0,0,483,71,1,0,0,0,484,487,3,
  	68,34,0,485,487,3,70,35,0,486,484,1,0,0,0,486,485,1,0,0,0,487,73,1,0,
  	0,0,488,489,5,58,0,0,489,75,1,0,0,0,490,491,5,54,0,0,491,77,1,0,0,0,492,
  	493,7,5,0,0,493,79,1,0,0,0,494,495,5,63,0,0,495,496,3,82,41,0,496,497,
  	5,64,0,0,497,81,1,0,0,0,498,503,3,84,42,0,499,500,5,66,0,0,500,502,3,
  	84,42,0,501,499,1,0,0,0,502,505,1,0,0,0,503,501,1,0,0,0,503,504,1,0,0,
  	0,504,83,1,0,0,0,505,503,1,0,0,0,506,508,3,86,43,0,507,506,1,0,0,0,508,
  	509,1,0,0,0,509,507,1,0,0,0,509,510,1,0,0,0,510,85,1,0,0,0,511,513,3,
  	88,44,0,512,514,3,92,46,0,513,512,1,0,0,0,513,514,1,0,0,0,514,87,1,0,
  	0,0,515,518,3,90,45,0,516,518,3,104,52,0,517,515,1,0,0,0,517,516,1,0,
  	0,0,518,89,1,0,0,0,519,520,5,70,0,0,520,521,3,82,41,0,521,522,5,71,0,
  	0,522,91,1,0,0,0,523,531,5,73,0,0,524,531,5,74,0,0,525,531,5,75,0,0,526,
  	527,5,68,0,0,527,528,3,94,47,0,528,529,5,69,0,0,529,531,1,0,0,0,530,523,
  	1,0,0,0,530,524,1,0,0,0,530,525,1,0,0,0,530,526,1,0,0,0,531,93,1,0,0,
  	0,532,537,3,96,48,0,533,537,3,98,49,0,534,537,3,100,50,0,535,537,3,102,
  	51,0,536,532,1,0,0,0,536,533,1,0,0,0,536,534,1,0,0,0,536,535,1,0,0,0,
  	537,95,1,0,0,0,538,539,3,128,64,0,539,97,1,0,0,0,540,541,3,128,64,0,541,
  	542,5,72,0,0,542,543,3,128,64,0,543,99,1,0,0,0,544,545,3,128,64,0,545,
  	546,5,72,0,0,546,101,1,0,0,0,547,548,5,72,0,0,548,549,3,128,64,0,549,
  	103,1,0,0,0,550,554,3,106,53,0,551,554,3,126,63,0,552,554,3,120,60,0,
  	553,550,1,0,0,0,553,551,1,0,0,0,553,552,1,0,0,0,554,105,1,0,0,0,555,557,
  	5,78,0,0,556,558,5,76,0,0,557,556,1,0,0,0,557,558,1,0,0,0,558,560,1,0,
  	0,0,559,561,3,108,54,0,560,559,1,0,0,0,561,562,1,0,0,0,562,560,1,0,0,
  	0,562,563,1,0,0,0,563,564,1,0,0,0,564,565,5,79,0,0,565,107,1,0,0,0,566,
  	570,3,110,55,0,567,570,3,126,63,0,568,570,3,112,56,0,569,566,1,0,0,0,
  	569,567,1,0,0,0,569,568,1,0,0,0,570,109,1,0,0,0,571,572,3,114,57,0,572,
  	573,5,77,0,0,573,574,3,114,57,0,574,111,1,0,0,0,575,576,3,114,57,0,576,
  	113,1,0,0,0,577,580,3,116,58,0,578,580,3,118,59,0,579,577,1,0,0,0,579,
  	578,1,0,0,0,580,115,1,0,0,0,581,582,5,80,0,0,582,583,7,6,0,0,583,117,
  	1,0,0,0,584,585,8,6,0,0,585,119,1,0,0,0,586,590,3,122,61,0,587,590,5,
  	82,0,0,588,590,3,124,62,0,589,586,1,0,0,0,589,587,1,0,0,0,589,588,1,0,
  	0,0,590,121,1,0,0,0,591,592,5,80,0,0,592,593,7,7,0,0,593,123,1,0,0,0,
  	594,595,8,8,0,0,595,125,1,0,0,0,596,597,7,9,0,0,597,127,1,0,0,0,598,600,
  	5,91,0,0,599,598,1,0,0,0,600,601,1,0,0,0,601,599,1,0,0,0,601,602,1,0,
  	0,0,602,129,1,0,0,0,57,132,134,144,153,157,162,166,174,183,186,194,197,
  	210,231,233,244,251,257,271,279,281,307,320,328,330,335,340,351,359,361,
  	372,379,395,402,408,414,417,420,439,446,451,465,475,486,503,509,513,517,
  	530,536,553,557,562,569,579,589,601
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
    setState(134);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CEQLQueryParser::K_SELECT

    || _la == CEQLQueryParser::UNEXPECTED_CHAR) {
      setState(132);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case CEQLQueryParser::K_SELECT: {
          setState(130);
          core_query();
          break;
        }

        case CEQLQueryParser::UNEXPECTED_CHAR: {
          setState(131);
          error();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(136);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(137);
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
    setState(139);
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
    setState(142);
    match(CEQLQueryParser::K_SELECT);
    setState(144);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 269123594) != 0)) {
      setState(143);
      selection_strategy();
    }
    setState(146);
    list_of_variables();
    setState(147);
    from_clause();
    setState(148);
    match(CEQLQueryParser::K_WHERE);
    setState(149);
    cel_formula(0);
    setState(153);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQLQueryParser::K_PARTITION) {
      setState(150);
      match(CEQLQueryParser::K_PARTITION);
      setState(151);
      match(CEQLQueryParser::K_BY);
      setState(152);
      partition_list();
    }
    setState(157);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQLQueryParser::K_WITHIN) {
      setState(155);
      match(CEQLQueryParser::K_WITHIN);
      setState(156);
      time_window();
    }
    setState(162);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQLQueryParser::K_CONSUME) {
      setState(159);
      match(CEQLQueryParser::K_CONSUME);
      setState(160);
      match(CEQLQueryParser::K_BY);
      setState(161);
      consumption_policy();
    }
    setState(166);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQLQueryParser::K_LIMIT) {
      setState(164);
      match(CEQLQueryParser::K_LIMIT);
      setState(165);
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
    setState(174);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::K_ALL: {
        _localctx = _tracker.createInstance<CEQLQueryParser::Ss_allContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(168);
        match(CEQLQueryParser::K_ALL);
        break;
      }

      case CEQLQueryParser::K_ANY: {
        _localctx = _tracker.createInstance<CEQLQueryParser::Ss_anyContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(169);
        match(CEQLQueryParser::K_ANY);
        break;
      }

      case CEQLQueryParser::K_LAST: {
        _localctx = _tracker.createInstance<CEQLQueryParser::Ss_lastContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(170);
        match(CEQLQueryParser::K_LAST);
        break;
      }

      case CEQLQueryParser::K_MAX: {
        _localctx = _tracker.createInstance<CEQLQueryParser::Ss_maxContext>(_localctx);
        enterOuterAlt(_localctx, 4);
        setState(171);
        match(CEQLQueryParser::K_MAX);
        break;
      }

      case CEQLQueryParser::K_NEXT: {
        _localctx = _tracker.createInstance<CEQLQueryParser::Ss_nextContext>(_localctx);
        enterOuterAlt(_localctx, 5);
        setState(172);
        match(CEQLQueryParser::K_NEXT);
        break;
      }

      case CEQLQueryParser::K_STRICT: {
        _localctx = _tracker.createInstance<CEQLQueryParser::Ss_strictContext>(_localctx);
        enterOuterAlt(_localctx, 6);
        setState(173);
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
//----------------- S_event_name_with_projection_listContext ------------------------------------------------------------------

std::vector<CEQLQueryParser::S_event_name_with_projectionContext *> CEQLQueryParser::S_event_name_with_projection_listContext::s_event_name_with_projection() {
  return getRuleContexts<CEQLQueryParser::S_event_name_with_projectionContext>();
}

CEQLQueryParser::S_event_name_with_projectionContext* CEQLQueryParser::S_event_name_with_projection_listContext::s_event_name_with_projection(size_t i) {
  return getRuleContext<CEQLQueryParser::S_event_name_with_projectionContext>(i);
}

std::vector<tree::TerminalNode *> CEQLQueryParser::S_event_name_with_projection_listContext::COMMA() {
  return getTokens(CEQLQueryParser::COMMA);
}

tree::TerminalNode* CEQLQueryParser::S_event_name_with_projection_listContext::COMMA(size_t i) {
  return getToken(CEQLQueryParser::COMMA, i);
}

CEQLQueryParser::S_event_name_with_projection_listContext::S_event_name_with_projection_listContext(List_of_variablesContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::S_event_name_with_projection_listContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitS_event_name_with_projection_list(this);
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
    setState(186);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::STAR: {
        _localctx = _tracker.createInstance<CEQLQueryParser::S_starContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(176);
        match(CEQLQueryParser::STAR);
        break;
      }

      case CEQLQueryParser::K_NONE: {
        _localctx = _tracker.createInstance<CEQLQueryParser::S_noneContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(177);
        match(CEQLQueryParser::K_NONE);
        break;
      }

      case CEQLQueryParser::IDENTIFIER: {
        _localctx = _tracker.createInstance<CEQLQueryParser::S_event_name_with_projection_listContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(178);
        s_event_name_with_projection();
        setState(183);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == CEQLQueryParser::COMMA) {
          setState(179);
          match(CEQLQueryParser::COMMA);
          setState(180);
          s_event_name_with_projection();
          setState(185);
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
    setState(197);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQLQueryParser::K_FROM) {
      setState(188);
      match(CEQLQueryParser::K_FROM);
      setState(189);
      stream_name();
      setState(194);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == CEQLQueryParser::COMMA) {
        setState(190);
        match(CEQLQueryParser::COMMA);
        setState(191);
        stream_name();
        setState(196);
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
//----------------- Not_event_type_atomic_cel_formulaContext ------------------------------------------------------------------

tree::TerminalNode* CEQLQueryParser::Not_event_type_atomic_cel_formulaContext::LEFT_PARENTHESIS() {
  return getToken(CEQLQueryParser::LEFT_PARENTHESIS, 0);
}

CEQLQueryParser::Atomic_cel_formulaContext* CEQLQueryParser::Not_event_type_atomic_cel_formulaContext::atomic_cel_formula() {
  return getRuleContext<CEQLQueryParser::Atomic_cel_formulaContext>(0);
}

tree::TerminalNode* CEQLQueryParser::Not_event_type_atomic_cel_formulaContext::RIGHT_PARENTHESIS() {
  return getToken(CEQLQueryParser::RIGHT_PARENTHESIS, 0);
}

tree::TerminalNode* CEQLQueryParser::Not_event_type_atomic_cel_formulaContext::K_NOT() {
  return getToken(CEQLQueryParser::K_NOT, 0);
}

tree::TerminalNode* CEQLQueryParser::Not_event_type_atomic_cel_formulaContext::REGEX_EXCLAMAITON() {
  return getToken(CEQLQueryParser::REGEX_EXCLAMAITON, 0);
}

CEQLQueryParser::Not_event_type_atomic_cel_formulaContext::Not_event_type_atomic_cel_formulaContext(Cel_formulaContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Not_event_type_atomic_cel_formulaContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitNot_event_type_atomic_cel_formula(this);
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
    setState(210);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::LEFT_PARENTHESIS: {
        _localctx = _tracker.createInstance<Par_cel_formulaContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(200);
        match(CEQLQueryParser::LEFT_PARENTHESIS);
        setState(201);
        cel_formula(0);
        setState(202);
        match(CEQLQueryParser::RIGHT_PARENTHESIS);
        break;
      }

      case CEQLQueryParser::IDENTIFIER: {
        _localctx = _tracker.createInstance<Event_type_cel_formulaContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(204);
        s_event_name();
        break;
      }

      case CEQLQueryParser::K_NOT:
      case CEQLQueryParser::REGEX_EXCLAMAITON: {
        _localctx = _tracker.createInstance<Not_event_type_atomic_cel_formulaContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(205);
        _la = _input->LA(1);
        if (!(_la == CEQLQueryParser::K_NOT

        || _la == CEQLQueryParser::REGEX_EXCLAMAITON)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(206);
        match(CEQLQueryParser::LEFT_PARENTHESIS);
        setState(207);
        atomic_cel_formula(0);
        setState(208);
        match(CEQLQueryParser::RIGHT_PARENTHESIS);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(233);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 14, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(231);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 13, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<Non_contiguous_sequencing_cel_formulaContext>(_tracker.createInstance<Cel_formulaContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleCel_formula);
          setState(212);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(213);
          match(CEQLQueryParser::SEMICOLON);
          setState(214);
          cel_formula(5);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<Contiguous_sequencing_cel_formulaContext>(_tracker.createInstance<Cel_formulaContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleCel_formula);
          setState(215);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(216);
          match(CEQLQueryParser::COLON);
          setState(217);
          cel_formula(4);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<Or_cel_formulaContext>(_tracker.createInstance<Cel_formulaContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleCel_formula);
          setState(218);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(219);
          match(CEQLQueryParser::K_OR);
          setState(220);
          cel_formula(3);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<As_cel_formulaContext>(_tracker.createInstance<Cel_formulaContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleCel_formula);
          setState(221);

          if (!(precpred(_ctx, 7))) throw FailedPredicateException(this, "precpred(_ctx, 7)");
          setState(222);
          match(CEQLQueryParser::K_AS);
          setState(223);
          event_name();
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<Non_contiguous_iteration_cel_formulaContext>(_tracker.createInstance<Cel_formulaContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleCel_formula);
          setState(224);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(225);
          match(CEQLQueryParser::PLUS);
          break;
        }

        case 6: {
          auto newContext = _tracker.createInstance<Contiguous_iteration_cel_formulaContext>(_tracker.createInstance<Cel_formulaContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleCel_formula);
          setState(226);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(227);
          match(CEQLQueryParser::COLON_PLUS);
          break;
        }

        case 7: {
          auto newContext = _tracker.createInstance<Filter_cel_formulaContext>(_tracker.createInstance<Cel_formulaContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleCel_formula);
          setState(228);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(229);
          match(CEQLQueryParser::K_FILTER);
          setState(230);
          filter(0);
          break;
        }

        default:
          break;
        } 
      }
      setState(235);
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
    setState(236);
    match(CEQLQueryParser::LEFT_SQUARE_BRACKET);
    setState(237);
    attribute_list();
    setState(238);
    match(CEQLQueryParser::RIGHT_SQUARE_BRACKET);
    setState(244);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQLQueryParser::COMMA) {
      setState(239);
      match(CEQLQueryParser::COMMA);
      setState(240);
      match(CEQLQueryParser::LEFT_SQUARE_BRACKET);
      setState(241);
      attribute_list();
      setState(242);
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
    setState(246);
    attribute_name();
    setState(251);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CEQLQueryParser::COMMA) {
      setState(247);
      match(CEQLQueryParser::COMMA);
      setState(248);
      attribute_name();
      setState(253);
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
    setState(257);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::K_ANY: {
        _localctx = _tracker.createInstance<CEQLQueryParser::Cp_anyContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(254);
        match(CEQLQueryParser::K_ANY);
        break;
      }

      case CEQLQueryParser::K_PARTITION: {
        _localctx = _tracker.createInstance<CEQLQueryParser::Cp_partitionContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(255);
        match(CEQLQueryParser::K_PARTITION);
        break;
      }

      case CEQLQueryParser::K_NONE: {
        _localctx = _tracker.createInstance<CEQLQueryParser::Cp_noneContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(256);
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
    setState(259);
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

CEQLQueryParser::S_event_nameContext* CEQLQueryParser::Atomic_filterContext::s_event_name() {
  return getRuleContext<CEQLQueryParser::S_event_nameContext>(0);
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
    setState(271);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::LEFT_PARENTHESIS: {
        _localctx = _tracker.createInstance<Par_filterContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(262);
        match(CEQLQueryParser::LEFT_PARENTHESIS);
        setState(263);
        filter(0);
        setState(264);
        match(CEQLQueryParser::RIGHT_PARENTHESIS);
        break;
      }

      case CEQLQueryParser::IDENTIFIER: {
        _localctx = _tracker.createInstance<Atomic_filterContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(266);
        s_event_name();
        setState(267);
        match(CEQLQueryParser::LEFT_SQUARE_BRACKET);
        setState(268);
        predicate(0);
        setState(269);
        match(CEQLQueryParser::RIGHT_SQUARE_BRACKET);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(281);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 20, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(279);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 19, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<And_filterContext>(_tracker.createInstance<FilterContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleFilter);
          setState(273);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(274);
          match(CEQLQueryParser::K_AND);
          setState(275);
          filter(3);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<Or_filterContext>(_tracker.createInstance<FilterContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleFilter);
          setState(276);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(277);
          match(CEQLQueryParser::K_OR);
          setState(278);
          filter(2);
          break;
        }

        default:
          break;
        } 
      }
      setState(283);
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
    setState(320);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 22, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<Par_predicateContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(285);
      match(CEQLQueryParser::LEFT_PARENTHESIS);
      setState(286);
      predicate(0);
      setState(287);
      match(CEQLQueryParser::RIGHT_PARENTHESIS);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<Not_predicateContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(289);
      match(CEQLQueryParser::K_NOT);
      setState(290);
      predicate(8);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<Inequality_predicateContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(291);
      math_expr(0);
      setState(292);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 8658654068736) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(293);
      math_expr(0);
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<Equality_string_predicateContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(295);
      string_literal();
      setState(296);
      _la = _input->LA(1);
      if (!(_la == CEQLQueryParser::EQ

      || _la == CEQLQueryParser::NEQ)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(297);
      string_literal_or_regexp();
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<Regex_predicateContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(299);
      attribute_name();
      setState(300);
      match(CEQLQueryParser::K_LIKE);
      setState(301);
      regexp();
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<In_predicateContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(303);
      attribute_name();
      setState(307);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case CEQLQueryParser::K_IN: {
          setState(304);
          match(CEQLQueryParser::K_IN);
          break;
        }

        case CEQLQueryParser::K_NOT: {
          setState(305);
          match(CEQLQueryParser::K_NOT);
          setState(306);
          match(CEQLQueryParser::K_IN);
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(309);
      value_seq();
      break;
    }

    case 7: {
      _localctx = _tracker.createInstance<In_range_predicateContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(311);
      math_expr(0);
      setState(312);
      match(CEQLQueryParser::K_IN);
      setState(313);
      match(CEQLQueryParser::K_RANGE);
      setState(314);
      match(CEQLQueryParser::LEFT_PARENTHESIS);
      setState(315);
      math_expr(0);
      setState(316);
      match(CEQLQueryParser::COMMA);
      setState(317);
      math_expr(0);
      setState(318);
      match(CEQLQueryParser::RIGHT_PARENTHESIS);
      break;
    }

    default:
      break;
    }
    _ctx->stop = _input->LT(-1);
    setState(330);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 24, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(328);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 23, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<And_predicateContext>(_tracker.createInstance<PredicateContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RulePredicate);
          setState(322);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(323);
          match(CEQLQueryParser::K_AND);
          setState(324);
          predicate(6);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<Or_predicateContext>(_tracker.createInstance<PredicateContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RulePredicate);
          setState(325);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(326);
          match(CEQLQueryParser::K_OR);
          setState(327);
          predicate(5);
          break;
        }

        default:
          break;
        } 
      }
      setState(332);
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
    setState(335);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::STRING_LITERAL: {
        enterOuterAlt(_localctx, 1);
        setState(333);
        string();
        break;
      }

      case CEQLQueryParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 2);
        setState(334);
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
    setState(340);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::STRING_LITERAL: {
        enterOuterAlt(_localctx, 1);
        setState(337);
        string();
        break;
      }

      case CEQLQueryParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 2);
        setState(338);
        attribute_name();
        break;
      }

      case CEQLQueryParser::REGEX_START: {
        enterOuterAlt(_localctx, 3);
        setState(339);
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
    setState(351);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::LEFT_PARENTHESIS: {
        _localctx = _tracker.createInstance<Par_math_exprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(343);
        match(CEQLQueryParser::LEFT_PARENTHESIS);
        setState(344);
        math_expr(0);
        setState(345);
        match(CEQLQueryParser::RIGHT_PARENTHESIS);
        break;
      }

      case CEQLQueryParser::DOUBLE_LITERAL:
      case CEQLQueryParser::INTEGER_LITERAL: {
        _localctx = _tracker.createInstance<Number_math_exprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(347);
        number();
        break;
      }

      case CEQLQueryParser::IDENTIFIER: {
        _localctx = _tracker.createInstance<Attribute_math_exprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(348);
        attribute_name();
        break;
      }

      case CEQLQueryParser::PLUS:
      case CEQLQueryParser::MINUS: {
        _localctx = _tracker.createInstance<Unary_math_exprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(349);
        _la = _input->LA(1);
        if (!(_la == CEQLQueryParser::PLUS

        || _la == CEQLQueryParser::MINUS)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(350);
        math_expr(3);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(361);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 29, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(359);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 28, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<Mul_math_exprContext>(_tracker.createInstance<Math_exprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleMath_expr);
          setState(353);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(354);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & 107374182400) != 0))) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(355);
          math_expr(3);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<Sum_math_exprContext>(_tracker.createInstance<Math_exprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleMath_expr);
          setState(356);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(357);
          _la = _input->LA(1);
          if (!(_la == CEQLQueryParser::PLUS

          || _la == CEQLQueryParser::MINUS)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(358);
          math_expr(2);
          break;
        }

        default:
          break;
        } 
      }
      setState(363);
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
    setState(372);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 30, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(364);
      match(CEQLQueryParser::LEFT_CURLY_BRACKET);
      setState(365);
      number_seq();
      setState(366);
      match(CEQLQueryParser::RIGHT_CURLY_BRACKET);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(368);
      match(CEQLQueryParser::LEFT_CURLY_BRACKET);
      setState(369);
      string_seq();
      setState(370);
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
    setState(395);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 32, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<CEQLQueryParser::Number_listContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(374);
      number();
      setState(379);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == CEQLQueryParser::COMMA) {
        setState(375);
        match(CEQLQueryParser::COMMA);
        setState(376);
        number();
        setState(381);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<CEQLQueryParser::Integer_rangeContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(382);
      integer();
      setState(383);
      match(CEQLQueryParser::DOUBLE_DOT);
      setState(384);
      integer();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<CEQLQueryParser::Double_rangeContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(386);
      double_();
      setState(387);
      match(CEQLQueryParser::DOUBLE_DOT);
      setState(388);
      double_();
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<CEQLQueryParser::Number_range_lowerContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(390);
      number();
      setState(391);
      match(CEQLQueryParser::DOUBLE_DOT);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<CEQLQueryParser::Number_range_upperContext>(_localctx);
      enterOuterAlt(_localctx, 5);
      setState(393);
      match(CEQLQueryParser::DOUBLE_DOT);
      setState(394);
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
    setState(397);
    string();
    setState(402);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CEQLQueryParser::COMMA) {
      setState(398);
      match(CEQLQueryParser::COMMA);
      setState(399);
      string();
      setState(404);
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
    setState(408);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 34, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(405);
      event_span();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(406);
      time_span();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(407);
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
    setState(410);
    integer();
    setState(411);
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
    setState(414);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 35, _ctx)) {
    case 1: {
      setState(413);
      hour_span();
      break;
    }

    default:
      break;
    }
    setState(417);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 36, _ctx)) {
    case 1: {
      setState(416);
      minute_span();
      break;
    }

    default:
      break;
    }
    setState(420);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQLQueryParser::DOUBLE_LITERAL

    || _la == CEQLQueryParser::INTEGER_LITERAL) {
      setState(419);
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
    setState(422);
    number();
    setState(423);
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
    setState(425);
    number();
    setState(426);
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
    setState(428);
    number();
    setState(429);
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
    setState(431);
    integer();
    setState(432);
    match(CEQLQueryParser::LEFT_SQUARE_BRACKET);
    setState(433);
    any_name();
    setState(434);
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
    setState(436);
    s_event_name();
    setState(439);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQLQueryParser::K_AS) {
      setState(437);
      match(CEQLQueryParser::K_AS);
      setState(438);
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

//----------------- S_event_name_with_projectionContext ------------------------------------------------------------------

CEQLQueryParser::S_event_name_with_projectionContext::S_event_name_with_projectionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQLQueryParser::S_event_nameContext* CEQLQueryParser::S_event_name_with_projectionContext::s_event_name() {
  return getRuleContext<CEQLQueryParser::S_event_nameContext>(0);
}

tree::TerminalNode* CEQLQueryParser::S_event_name_with_projectionContext::LEFT_SQUARE_BRACKET() {
  return getToken(CEQLQueryParser::LEFT_SQUARE_BRACKET, 0);
}

CEQLQueryParser::List_of_attribute_namesContext* CEQLQueryParser::S_event_name_with_projectionContext::list_of_attribute_names() {
  return getRuleContext<CEQLQueryParser::List_of_attribute_namesContext>(0);
}

tree::TerminalNode* CEQLQueryParser::S_event_name_with_projectionContext::RIGHT_SQUARE_BRACKET() {
  return getToken(CEQLQueryParser::RIGHT_SQUARE_BRACKET, 0);
}


size_t CEQLQueryParser::S_event_name_with_projectionContext::getRuleIndex() const {
  return CEQLQueryParser::RuleS_event_name_with_projection;
}


std::any CEQLQueryParser::S_event_name_with_projectionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitS_event_name_with_projection(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::S_event_name_with_projectionContext* CEQLQueryParser::s_event_name_with_projection() {
  S_event_name_with_projectionContext *_localctx = _tracker.createInstance<S_event_name_with_projectionContext>(_ctx, getState());
  enterRule(_localctx, 54, CEQLQueryParser::RuleS_event_name_with_projection);
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
    s_event_name();
    setState(446);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQLQueryParser::LEFT_SQUARE_BRACKET) {
      setState(442);
      match(CEQLQueryParser::LEFT_SQUARE_BRACKET);
      setState(443);
      list_of_attribute_names();
      setState(444);
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
  enterRule(_localctx, 56, CEQLQueryParser::RuleS_event_name);

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
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 40, _ctx)) {
    case 1: {
      setState(448);
      stream_name();
      setState(449);
      match(CEQLQueryParser::GE);
      break;
    }

    default:
      break;
    }
    setState(453);
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
  enterRule(_localctx, 58, CEQLQueryParser::RuleEvent_name);

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
    any_name();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Atomic_cel_formulaContext ------------------------------------------------------------------

CEQLQueryParser::Atomic_cel_formulaContext::Atomic_cel_formulaContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CEQLQueryParser::Atomic_cel_formulaContext::getRuleIndex() const {
  return CEQLQueryParser::RuleAtomic_cel_formula;
}

void CEQLQueryParser::Atomic_cel_formulaContext::copyFrom(Atomic_cel_formulaContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Atomic_cel_formula_filterContext ------------------------------------------------------------------

CEQLQueryParser::Atomic_cel_formulaContext* CEQLQueryParser::Atomic_cel_formula_filterContext::atomic_cel_formula() {
  return getRuleContext<CEQLQueryParser::Atomic_cel_formulaContext>(0);
}

tree::TerminalNode* CEQLQueryParser::Atomic_cel_formula_filterContext::K_FILTER() {
  return getToken(CEQLQueryParser::K_FILTER, 0);
}

CEQLQueryParser::FilterContext* CEQLQueryParser::Atomic_cel_formula_filterContext::filter() {
  return getRuleContext<CEQLQueryParser::FilterContext>(0);
}

CEQLQueryParser::Atomic_cel_formula_filterContext::Atomic_cel_formula_filterContext(Atomic_cel_formulaContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Atomic_cel_formula_filterContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitAtomic_cel_formula_filter(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Atomic_cel_formula_rContext ------------------------------------------------------------------

CEQLQueryParser::S_event_nameContext* CEQLQueryParser::Atomic_cel_formula_rContext::s_event_name() {
  return getRuleContext<CEQLQueryParser::S_event_nameContext>(0);
}

CEQLQueryParser::Atomic_cel_formula_rContext::Atomic_cel_formula_rContext(Atomic_cel_formulaContext *ctx) { copyFrom(ctx); }


std::any CEQLQueryParser::Atomic_cel_formula_rContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitAtomic_cel_formula_r(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::Atomic_cel_formulaContext* CEQLQueryParser::atomic_cel_formula() {
   return atomic_cel_formula(0);
}

CEQLQueryParser::Atomic_cel_formulaContext* CEQLQueryParser::atomic_cel_formula(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  CEQLQueryParser::Atomic_cel_formulaContext *_localctx = _tracker.createInstance<Atomic_cel_formulaContext>(_ctx, parentState);
  CEQLQueryParser::Atomic_cel_formulaContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 60;
  enterRecursionRule(_localctx, 60, CEQLQueryParser::RuleAtomic_cel_formula, precedence);

    

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
    _localctx = _tracker.createInstance<Atomic_cel_formula_rContext>(_localctx);
    _ctx = _localctx;
    previousContext = _localctx;

    setState(458);
    s_event_name();
    _ctx->stop = _input->LT(-1);
    setState(465);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 41, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        auto newContext = _tracker.createInstance<Atomic_cel_formula_filterContext>(_tracker.createInstance<Atomic_cel_formulaContext>(parentContext, parentState));
        _localctx = newContext;
        pushNewRecursionContext(newContext, startState, RuleAtomic_cel_formula);
        setState(460);

        if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
        setState(461);
        match(CEQLQueryParser::K_FILTER);
        setState(462);
        filter(0); 
      }
      setState(467);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 41, _ctx);
    }
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
  enterRule(_localctx, 62, CEQLQueryParser::RuleStream_name);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(468);
    any_name();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- List_of_attribute_namesContext ------------------------------------------------------------------

CEQLQueryParser::List_of_attribute_namesContext::List_of_attribute_namesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<CEQLQueryParser::Attribute_nameContext *> CEQLQueryParser::List_of_attribute_namesContext::attribute_name() {
  return getRuleContexts<CEQLQueryParser::Attribute_nameContext>();
}

CEQLQueryParser::Attribute_nameContext* CEQLQueryParser::List_of_attribute_namesContext::attribute_name(size_t i) {
  return getRuleContext<CEQLQueryParser::Attribute_nameContext>(i);
}

std::vector<tree::TerminalNode *> CEQLQueryParser::List_of_attribute_namesContext::COMMA() {
  return getTokens(CEQLQueryParser::COMMA);
}

tree::TerminalNode* CEQLQueryParser::List_of_attribute_namesContext::COMMA(size_t i) {
  return getToken(CEQLQueryParser::COMMA, i);
}


size_t CEQLQueryParser::List_of_attribute_namesContext::getRuleIndex() const {
  return CEQLQueryParser::RuleList_of_attribute_names;
}


std::any CEQLQueryParser::List_of_attribute_namesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQLQueryParserVisitor*>(visitor))
    return parserVisitor->visitList_of_attribute_names(this);
  else
    return visitor->visitChildren(this);
}

CEQLQueryParser::List_of_attribute_namesContext* CEQLQueryParser::list_of_attribute_names() {
  List_of_attribute_namesContext *_localctx = _tracker.createInstance<List_of_attribute_namesContext>(_ctx, getState());
  enterRule(_localctx, 64, CEQLQueryParser::RuleList_of_attribute_names);
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
    setState(470);
    attribute_name();
    setState(475);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CEQLQueryParser::COMMA) {
      setState(471);
      match(CEQLQueryParser::COMMA);
      setState(472);
      attribute_name();
      setState(477);
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
  enterRule(_localctx, 66, CEQLQueryParser::RuleAttribute_name);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(478);
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
  enterRule(_localctx, 68, CEQLQueryParser::RuleInteger);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(480);
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
  enterRule(_localctx, 70, CEQLQueryParser::RuleDouble);

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
  enterRule(_localctx, 72, CEQLQueryParser::RuleNumber);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(486);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::INTEGER_LITERAL: {
        enterOuterAlt(_localctx, 1);
        setState(484);
        integer();
        break;
      }

      case CEQLQueryParser::DOUBLE_LITERAL: {
        enterOuterAlt(_localctx, 2);
        setState(485);
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
  enterRule(_localctx, 74, CEQLQueryParser::RuleString);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(488);
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
  enterRule(_localctx, 76, CEQLQueryParser::RuleAny_name);

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
  enterRule(_localctx, 78, CEQLQueryParser::RuleKeyword);
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
    setState(492);
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
  enterRule(_localctx, 80, CEQLQueryParser::RuleRegexp);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(494);
    match(CEQLQueryParser::REGEX_START);
    setState(495);
    regexp_alternation();
    setState(496);
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
  enterRule(_localctx, 82, CEQLQueryParser::RuleRegexp_alternation);
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
    regexp_exp();
    setState(503);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CEQLQueryParser::REGEX_PIPE) {
      setState(499);
      match(CEQLQueryParser::REGEX_PIPE);
      setState(500);
      regexp_exp();
      setState(505);
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
  enterRule(_localctx, 84, CEQLQueryParser::RuleRegexp_exp);

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
    setState(507); 
    _errHandler->sync(this);
    alt = 1;
    do {
      switch (alt) {
        case 1: {
              setState(506);
              regexp_element();
              break;
            }

      default:
        throw NoViableAltException(this);
      }
      setState(509); 
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 45, _ctx);
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
  enterRule(_localctx, 86, CEQLQueryParser::RuleRegexp_element);
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
    setState(511);
    regexp_group();
    setState(513);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 68) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 68)) & 225) != 0)) {
      setState(512);
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
  enterRule(_localctx, 88, CEQLQueryParser::RuleRegexp_group);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(517);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::REGEX_L_PAR: {
        enterOuterAlt(_localctx, 1);
        setState(515);
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
        setState(516);
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
  enterRule(_localctx, 90, CEQLQueryParser::RuleParenthesis);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(519);
    match(CEQLQueryParser::REGEX_L_PAR);
    setState(520);
    regexp_alternation();
    setState(521);
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
  enterRule(_localctx, 92, CEQLQueryParser::RuleQuantifier);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(530);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::REGEX_QUESTION: {
        enterOuterAlt(_localctx, 1);
        setState(523);
        match(CEQLQueryParser::REGEX_QUESTION);
        break;
      }

      case CEQLQueryParser::REGEX_PLUS: {
        enterOuterAlt(_localctx, 2);
        setState(524);
        match(CEQLQueryParser::REGEX_PLUS);
        break;
      }

      case CEQLQueryParser::REGEX_STAR: {
        enterOuterAlt(_localctx, 3);
        setState(525);
        match(CEQLQueryParser::REGEX_STAR);
        break;
      }

      case CEQLQueryParser::REGEX_L_CURLY: {
        enterOuterAlt(_localctx, 4);
        setState(526);
        match(CEQLQueryParser::REGEX_L_CURLY);
        setState(527);
        quantity();
        setState(528);
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
  enterRule(_localctx, 94, CEQLQueryParser::RuleQuantity);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(536);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 49, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(532);
      quantExact();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(533);
      quantRange();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(534);
      quantMin();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(535);
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
  enterRule(_localctx, 96, CEQLQueryParser::RuleQuantExact);

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
  enterRule(_localctx, 98, CEQLQueryParser::RuleQuantRange);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(540);
    regexp_number();
    setState(541);
    match(CEQLQueryParser::REGEX_COMMA);
    setState(542);
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
  enterRule(_localctx, 100, CEQLQueryParser::RuleQuantMin);

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
    regexp_number();
    setState(545);
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
  enterRule(_localctx, 102, CEQLQueryParser::RuleQuantMax);

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
    match(CEQLQueryParser::REGEX_COMMA);
    setState(548);
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
  enterRule(_localctx, 104, CEQLQueryParser::RuleAtom);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(553);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 50, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(550);
      characterClass();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(551);
      sharedAtom();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(552);
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
  enterRule(_localctx, 106, CEQLQueryParser::RuleCharacterClass);
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
    setState(555);
    match(CEQLQueryParser::REGEX_L_BRACK);
    setState(557);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQLQueryParser::REGEX_HAT) {
      setState(556);
      match(CEQLQueryParser::REGEX_HAT);
    }
    setState(560); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(559);
      ccAtom();
      setState(562); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -2) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & 268390399) != 0));
    setState(564);
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
  enterRule(_localctx, 108, CEQLQueryParser::RuleCcAtom);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(569);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 53, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(566);
      ccRange();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(567);
      sharedAtom();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(568);
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
  enterRule(_localctx, 110, CEQLQueryParser::RuleCcRange);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(571);
    ccLiteral();
    setState(572);
    match(CEQLQueryParser::REGEX_HYPHEN);
    setState(573);
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
  enterRule(_localctx, 112, CEQLQueryParser::RuleCcSingle);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(575);
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
  enterRule(_localctx, 114, CEQLQueryParser::RuleCcLiteral);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(579);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQLQueryParser::REGEX_BACKSLASH: {
        enterOuterAlt(_localctx, 1);
        setState(577);
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
        setState(578);
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
  enterRule(_localctx, 116, CEQLQueryParser::RuleCcEscapes);
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
    setState(581);
    match(CEQLQueryParser::REGEX_BACKSLASH);
    setState(582);
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
  enterRule(_localctx, 118, CEQLQueryParser::RuleCcOther);
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
    setState(584);
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
  enterRule(_localctx, 120, CEQLQueryParser::RuleLiteral);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(589);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 55, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(586);
      escapes();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(587);
      match(CEQLQueryParser::REGEX_DOT);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(588);
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
  enterRule(_localctx, 122, CEQLQueryParser::RuleEscapes);
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
    setState(591);
    match(CEQLQueryParser::REGEX_BACKSLASH);
    setState(592);
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
  enterRule(_localctx, 124, CEQLQueryParser::RuleOther);
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
    setState(594);
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
  enterRule(_localctx, 126, CEQLQueryParser::RuleSharedAtom);
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
    setState(596);
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
  enterRule(_localctx, 128, CEQLQueryParser::RuleRegexp_number);
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
    setState(599); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(598);
      match(CEQLQueryParser::REGEX_DIGIT);
      setState(601); 
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
    case 30: return atomic_cel_formulaSempred(antlrcpp::downCast<Atomic_cel_formulaContext *>(context), predicateIndex);

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

bool CEQLQueryParser::atomic_cel_formulaSempred(Atomic_cel_formulaContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 13: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

void CEQLQueryParser::initialize() {
  ::antlr4::internal::call_once(ceqlqueryparserParserOnceFlag, ceqlqueryparserParserInitialize);
}
