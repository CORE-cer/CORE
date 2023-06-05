
// Generated from CEQL_QUERY.g4 by ANTLR 4.12.0


#include "CEQL_QUERYVisitor.h"

#include "CEQL_QUERYParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct CEQL_QUERYParserStaticData final {
  CEQL_QUERYParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  CEQL_QUERYParserStaticData(const CEQL_QUERYParserStaticData&) = delete;
  CEQL_QUERYParserStaticData(CEQL_QUERYParserStaticData&&) = delete;
  CEQL_QUERYParserStaticData& operator=(const CEQL_QUERYParserStaticData&) = delete;
  CEQL_QUERYParserStaticData& operator=(CEQL_QUERYParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag ceql_queryParserOnceFlag;
CEQL_QUERYParserStaticData *ceql_queryParserStaticData = nullptr;

void ceql_queryParserInitialize() {
  assert(ceql_queryParserStaticData == nullptr);
  auto staticData = std::make_unique<CEQL_QUERYParserStaticData>(
    std::vector<std::string>{
      "parse", "error", "core_query", "selection_strategy", "list_of_variables", 
      "from_clause", "cel_formula", "partition_list", "attribute_list", 
      "consumption_policy", "filter", "predicate", "string_literal", "string_literal_or_regexp", 
      "regexp", "math_expr", "value_seq", "number_seq", "string_seq", "time_window", 
      "event_span", "time_span", "hour_span", "minute_span", "second_span", 
      "custom_span", "named_event", "s_event_name", "event_name", "stream_name", 
      "attribute_name", "integer", "double", "number", "string", "any_name", 
      "keyword"
    },
    std::vector<std::string>{
      "", "','", "'('", "')'", "'['", "']'", "'{'", "'}'", "'..'", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "'%'", "'+'", "'-'", "'*'", 
      "'/'", "'<'", "'<='", "'>'", "'>='", "", "", "';'"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "K_ALL", "K_AND", "K_ANY", "K_AS", 
      "K_BY", "K_CONSUME", "K_DISTINCT", "K_EVENT", "K_EVENTS", "K_FILTER", 
      "K_FROM", "K_HOURS", "K_IN", "K_LAST", "K_LIKE", "K_MAX", "K_MINUTES", 
      "K_NEXT", "K_NONE", "K_NOT", "K_OR", "K_PARTITION", "K_SECONDS", "K_SELECT", 
      "K_STREAM", "K_STRICT", "K_UNLESS", "K_WHERE", "K_WITHIN", "PERCENT", 
      "PLUS", "MINUS", "STAR", "SLASH", "LE", "LEQ", "GE", "GEQ", "EQ", 
      "NEQ", "SEMICOLON", "IDENTIFIER", "DOUBLE_LITERAL", "INTEGER_LITERAL", 
      "NUMERICAL_EXPONENT", "STRING_LITERAL", "SINGLE_LINE_COMMENT", "MULTILINE_COMMENT", 
      "SPACES", "UNEXPECTED_CHAR"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,58,389,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,7,
  	35,2,36,7,36,1,0,1,0,5,0,77,8,0,10,0,12,0,80,9,0,1,0,1,0,1,1,1,1,1,1,
  	1,2,1,2,3,2,89,8,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,3,2,98,8,2,1,2,1,2,3,2,
  	102,8,2,1,2,1,2,1,2,3,2,107,8,2,1,3,1,3,1,3,1,3,1,3,1,3,3,3,115,8,3,1,
  	4,1,4,1,4,1,4,5,4,121,8,4,10,4,12,4,124,9,4,3,4,126,8,4,1,5,1,5,1,5,1,
  	5,5,5,132,8,5,10,5,12,5,135,9,5,3,5,137,8,5,1,6,1,6,1,6,1,6,1,6,1,6,3,
  	6,145,8,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,5,6,
  	161,8,6,10,6,12,6,164,9,6,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,3,7,174,8,7,
  	1,8,1,8,1,8,5,8,179,8,8,10,8,12,8,182,9,8,1,9,1,9,1,9,3,9,187,8,9,1,10,
  	1,10,1,10,1,10,1,10,1,10,1,10,1,10,1,10,1,10,3,10,199,8,10,1,10,1,10,
  	1,10,1,10,1,10,1,10,5,10,207,8,10,10,10,12,10,210,9,10,1,11,1,11,1,11,
  	1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,
  	1,11,1,11,1,11,1,11,1,11,1,11,3,11,235,8,11,1,11,1,11,3,11,239,8,11,1,
  	11,1,11,1,11,1,11,1,11,1,11,5,11,247,8,11,10,11,12,11,250,9,11,1,12,1,
  	12,3,12,254,8,12,1,13,1,13,1,13,3,13,259,8,13,1,14,1,14,1,15,1,15,1,15,
  	1,15,1,15,1,15,1,15,1,15,1,15,3,15,272,8,15,1,15,1,15,1,15,1,15,1,15,
  	1,15,5,15,280,8,15,10,15,12,15,283,9,15,1,16,1,16,1,16,1,16,1,16,1,16,
  	1,16,1,16,3,16,293,8,16,1,17,1,17,1,17,5,17,298,8,17,10,17,12,17,301,
  	9,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,
  	3,17,316,8,17,1,18,1,18,1,18,5,18,321,8,18,10,18,12,18,324,9,18,1,19,
  	1,19,1,19,3,19,329,8,19,1,20,1,20,1,20,1,21,3,21,335,8,21,1,21,3,21,338,
  	8,21,1,21,3,21,341,8,21,1,22,1,22,1,22,1,23,1,23,1,23,1,24,1,24,1,24,
  	1,25,1,25,1,25,1,25,1,25,1,26,1,26,1,26,3,26,360,8,26,1,27,1,27,1,27,
  	3,27,365,8,27,1,27,1,27,1,28,1,28,1,29,1,29,1,30,1,30,1,31,1,31,1,32,
  	1,32,1,33,1,33,3,33,381,8,33,1,34,1,34,1,35,1,35,1,36,1,36,1,36,0,4,12,
  	20,22,30,37,0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,
  	42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,72,0,5,1,0,43,48,1,0,47,
  	48,1,0,39,40,2,0,38,38,41,42,1,0,9,37,410,0,78,1,0,0,0,2,83,1,0,0,0,4,
  	86,1,0,0,0,6,114,1,0,0,0,8,125,1,0,0,0,10,136,1,0,0,0,12,144,1,0,0,0,
  	14,165,1,0,0,0,16,175,1,0,0,0,18,186,1,0,0,0,20,198,1,0,0,0,22,238,1,
  	0,0,0,24,253,1,0,0,0,26,258,1,0,0,0,28,260,1,0,0,0,30,271,1,0,0,0,32,
  	292,1,0,0,0,34,315,1,0,0,0,36,317,1,0,0,0,38,328,1,0,0,0,40,330,1,0,0,
  	0,42,334,1,0,0,0,44,342,1,0,0,0,46,345,1,0,0,0,48,348,1,0,0,0,50,351,
  	1,0,0,0,52,356,1,0,0,0,54,364,1,0,0,0,56,368,1,0,0,0,58,370,1,0,0,0,60,
  	372,1,0,0,0,62,374,1,0,0,0,64,376,1,0,0,0,66,380,1,0,0,0,68,382,1,0,0,
  	0,70,384,1,0,0,0,72,386,1,0,0,0,74,77,3,4,2,0,75,77,3,2,1,0,76,74,1,0,
  	0,0,76,75,1,0,0,0,77,80,1,0,0,0,78,76,1,0,0,0,78,79,1,0,0,0,79,81,1,0,
  	0,0,80,78,1,0,0,0,81,82,5,0,0,1,82,1,1,0,0,0,83,84,5,58,0,0,84,85,6,1,
  	-1,0,85,3,1,0,0,0,86,88,5,32,0,0,87,89,3,6,3,0,88,87,1,0,0,0,88,89,1,
  	0,0,0,89,90,1,0,0,0,90,91,3,8,4,0,91,92,3,10,5,0,92,93,5,36,0,0,93,97,
  	3,12,6,0,94,95,5,30,0,0,95,96,5,13,0,0,96,98,3,14,7,0,97,94,1,0,0,0,97,
  	98,1,0,0,0,98,101,1,0,0,0,99,100,5,37,0,0,100,102,3,38,19,0,101,99,1,
  	0,0,0,101,102,1,0,0,0,102,106,1,0,0,0,103,104,5,14,0,0,104,105,5,13,0,
  	0,105,107,3,18,9,0,106,103,1,0,0,0,106,107,1,0,0,0,107,5,1,0,0,0,108,
  	115,5,9,0,0,109,115,5,11,0,0,110,115,5,22,0,0,111,115,5,24,0,0,112,115,
  	5,26,0,0,113,115,5,34,0,0,114,108,1,0,0,0,114,109,1,0,0,0,114,110,1,0,
  	0,0,114,111,1,0,0,0,114,112,1,0,0,0,114,113,1,0,0,0,115,7,1,0,0,0,116,
  	126,5,41,0,0,117,122,3,70,35,0,118,119,5,1,0,0,119,121,3,70,35,0,120,
  	118,1,0,0,0,121,124,1,0,0,0,122,120,1,0,0,0,122,123,1,0,0,0,123,126,1,
  	0,0,0,124,122,1,0,0,0,125,116,1,0,0,0,125,117,1,0,0,0,126,9,1,0,0,0,127,
  	128,5,19,0,0,128,133,3,58,29,0,129,130,5,1,0,0,130,132,3,58,29,0,131,
  	129,1,0,0,0,132,135,1,0,0,0,133,131,1,0,0,0,133,134,1,0,0,0,134,137,1,
  	0,0,0,135,133,1,0,0,0,136,127,1,0,0,0,136,137,1,0,0,0,137,11,1,0,0,0,
  	138,139,6,6,-1,0,139,140,5,2,0,0,140,141,3,12,6,0,141,142,5,3,0,0,142,
  	145,1,0,0,0,143,145,3,54,27,0,144,138,1,0,0,0,144,143,1,0,0,0,145,162,
  	1,0,0,0,146,147,10,3,0,0,147,148,5,49,0,0,148,161,3,12,6,4,149,150,10,
  	2,0,0,150,151,5,29,0,0,151,161,3,12,6,3,152,153,10,5,0,0,153,154,5,12,
  	0,0,154,161,3,56,28,0,155,156,10,4,0,0,156,161,5,39,0,0,157,158,10,1,
  	0,0,158,159,5,18,0,0,159,161,3,20,10,0,160,146,1,0,0,0,160,149,1,0,0,
  	0,160,152,1,0,0,0,160,155,1,0,0,0,160,157,1,0,0,0,161,164,1,0,0,0,162,
  	160,1,0,0,0,162,163,1,0,0,0,163,13,1,0,0,0,164,162,1,0,0,0,165,166,5,
  	4,0,0,166,167,3,16,8,0,167,173,5,5,0,0,168,169,5,1,0,0,169,170,5,4,0,
  	0,170,171,3,16,8,0,171,172,5,5,0,0,172,174,1,0,0,0,173,168,1,0,0,0,173,
  	174,1,0,0,0,174,15,1,0,0,0,175,180,3,60,30,0,176,177,5,1,0,0,177,179,
  	3,60,30,0,178,176,1,0,0,0,179,182,1,0,0,0,180,178,1,0,0,0,180,181,1,0,
  	0,0,181,17,1,0,0,0,182,180,1,0,0,0,183,187,5,11,0,0,184,187,5,30,0,0,
  	185,187,5,27,0,0,186,183,1,0,0,0,186,184,1,0,0,0,186,185,1,0,0,0,187,
  	19,1,0,0,0,188,189,6,10,-1,0,189,190,5,2,0,0,190,191,3,20,10,0,191,192,
  	5,3,0,0,192,199,1,0,0,0,193,194,3,56,28,0,194,195,5,4,0,0,195,196,3,22,
  	11,0,196,197,5,5,0,0,197,199,1,0,0,0,198,188,1,0,0,0,198,193,1,0,0,0,
  	199,208,1,0,0,0,200,201,10,2,0,0,201,202,5,10,0,0,202,207,3,20,10,3,203,
  	204,10,1,0,0,204,205,5,29,0,0,205,207,3,20,10,2,206,200,1,0,0,0,206,203,
  	1,0,0,0,207,210,1,0,0,0,208,206,1,0,0,0,208,209,1,0,0,0,209,21,1,0,0,
  	0,210,208,1,0,0,0,211,212,6,11,-1,0,212,213,5,2,0,0,213,214,3,22,11,0,
  	214,215,5,3,0,0,215,239,1,0,0,0,216,217,5,28,0,0,217,239,3,22,11,7,218,
  	219,3,30,15,0,219,220,7,0,0,0,220,221,3,30,15,0,221,239,1,0,0,0,222,223,
  	3,24,12,0,223,224,7,1,0,0,224,225,3,26,13,0,225,239,1,0,0,0,226,227,3,
  	60,30,0,227,228,5,23,0,0,228,229,3,28,14,0,229,239,1,0,0,0,230,234,3,
  	60,30,0,231,235,5,21,0,0,232,233,5,28,0,0,233,235,5,21,0,0,234,231,1,
  	0,0,0,234,232,1,0,0,0,235,236,1,0,0,0,236,237,3,32,16,0,237,239,1,0,0,
  	0,238,211,1,0,0,0,238,216,1,0,0,0,238,218,1,0,0,0,238,222,1,0,0,0,238,
  	226,1,0,0,0,238,230,1,0,0,0,239,248,1,0,0,0,240,241,10,4,0,0,241,242,
  	5,10,0,0,242,247,3,22,11,5,243,244,10,3,0,0,244,245,5,29,0,0,245,247,
  	3,22,11,4,246,240,1,0,0,0,246,243,1,0,0,0,247,250,1,0,0,0,248,246,1,0,
  	0,0,248,249,1,0,0,0,249,23,1,0,0,0,250,248,1,0,0,0,251,254,3,68,34,0,
  	252,254,3,60,30,0,253,251,1,0,0,0,253,252,1,0,0,0,254,25,1,0,0,0,255,
  	259,3,68,34,0,256,259,3,60,30,0,257,259,3,28,14,0,258,255,1,0,0,0,258,
  	256,1,0,0,0,258,257,1,0,0,0,259,27,1,0,0,0,260,261,5,54,0,0,261,29,1,
  	0,0,0,262,263,6,15,-1,0,263,264,5,2,0,0,264,265,3,30,15,0,265,266,5,3,
  	0,0,266,272,1,0,0,0,267,272,3,66,33,0,268,272,3,60,30,0,269,270,7,2,0,
  	0,270,272,3,30,15,3,271,262,1,0,0,0,271,267,1,0,0,0,271,268,1,0,0,0,271,
  	269,1,0,0,0,272,281,1,0,0,0,273,274,10,2,0,0,274,275,7,3,0,0,275,280,
  	3,30,15,3,276,277,10,1,0,0,277,278,7,2,0,0,278,280,3,30,15,2,279,273,
  	1,0,0,0,279,276,1,0,0,0,280,283,1,0,0,0,281,279,1,0,0,0,281,282,1,0,0,
  	0,282,31,1,0,0,0,283,281,1,0,0,0,284,285,5,6,0,0,285,286,3,34,17,0,286,
  	287,5,7,0,0,287,293,1,0,0,0,288,289,5,6,0,0,289,290,3,36,18,0,290,291,
  	5,7,0,0,291,293,1,0,0,0,292,284,1,0,0,0,292,288,1,0,0,0,293,33,1,0,0,
  	0,294,299,3,66,33,0,295,296,5,1,0,0,296,298,3,66,33,0,297,295,1,0,0,0,
  	298,301,1,0,0,0,299,297,1,0,0,0,299,300,1,0,0,0,300,316,1,0,0,0,301,299,
  	1,0,0,0,302,303,3,62,31,0,303,304,5,8,0,0,304,305,3,62,31,0,305,316,1,
  	0,0,0,306,307,3,64,32,0,307,308,5,8,0,0,308,309,3,64,32,0,309,316,1,0,
  	0,0,310,311,3,66,33,0,311,312,5,8,0,0,312,316,1,0,0,0,313,314,5,8,0,0,
  	314,316,3,66,33,0,315,294,1,0,0,0,315,302,1,0,0,0,315,306,1,0,0,0,315,
  	310,1,0,0,0,315,313,1,0,0,0,316,35,1,0,0,0,317,322,3,68,34,0,318,319,
  	5,1,0,0,319,321,3,68,34,0,320,318,1,0,0,0,321,324,1,0,0,0,322,320,1,0,
  	0,0,322,323,1,0,0,0,323,37,1,0,0,0,324,322,1,0,0,0,325,329,3,40,20,0,
  	326,329,3,42,21,0,327,329,3,50,25,0,328,325,1,0,0,0,328,326,1,0,0,0,328,
  	327,1,0,0,0,329,39,1,0,0,0,330,331,3,62,31,0,331,332,5,17,0,0,332,41,
  	1,0,0,0,333,335,3,44,22,0,334,333,1,0,0,0,334,335,1,0,0,0,335,337,1,0,
  	0,0,336,338,3,46,23,0,337,336,1,0,0,0,337,338,1,0,0,0,338,340,1,0,0,0,
  	339,341,3,48,24,0,340,339,1,0,0,0,340,341,1,0,0,0,341,43,1,0,0,0,342,
  	343,3,62,31,0,343,344,5,20,0,0,344,45,1,0,0,0,345,346,3,62,31,0,346,347,
  	5,25,0,0,347,47,1,0,0,0,348,349,3,62,31,0,349,350,5,31,0,0,350,49,1,0,
  	0,0,351,352,3,62,31,0,352,353,5,4,0,0,353,354,3,70,35,0,354,355,5,5,0,
  	0,355,51,1,0,0,0,356,359,3,54,27,0,357,358,5,12,0,0,358,360,3,56,28,0,
  	359,357,1,0,0,0,359,360,1,0,0,0,360,53,1,0,0,0,361,362,3,58,29,0,362,
  	363,5,45,0,0,363,365,1,0,0,0,364,361,1,0,0,0,364,365,1,0,0,0,365,366,
  	1,0,0,0,366,367,3,56,28,0,367,55,1,0,0,0,368,369,3,70,35,0,369,57,1,0,
  	0,0,370,371,3,70,35,0,371,59,1,0,0,0,372,373,3,70,35,0,373,61,1,0,0,0,
  	374,375,5,52,0,0,375,63,1,0,0,0,376,377,5,51,0,0,377,65,1,0,0,0,378,381,
  	3,62,31,0,379,381,3,64,32,0,380,378,1,0,0,0,380,379,1,0,0,0,381,67,1,
  	0,0,0,382,383,5,54,0,0,383,69,1,0,0,0,384,385,5,50,0,0,385,71,1,0,0,0,
  	386,387,7,4,0,0,387,73,1,0,0,0,40,76,78,88,97,101,106,114,122,125,133,
  	136,144,160,162,173,180,186,198,206,208,234,238,246,248,253,258,271,279,
  	281,292,299,315,322,328,334,337,340,359,364,380
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  ceql_queryParserStaticData = staticData.release();
}

}

CEQL_QUERYParser::CEQL_QUERYParser(TokenStream *input) : CEQL_QUERYParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

CEQL_QUERYParser::CEQL_QUERYParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  CEQL_QUERYParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *ceql_queryParserStaticData->atn, ceql_queryParserStaticData->decisionToDFA, ceql_queryParserStaticData->sharedContextCache, options);
}

CEQL_QUERYParser::~CEQL_QUERYParser() {
  delete _interpreter;
}

const atn::ATN& CEQL_QUERYParser::getATN() const {
  return *ceql_queryParserStaticData->atn;
}

std::string CEQL_QUERYParser::getGrammarFileName() const {
  return "CEQL_QUERY.g4";
}

const std::vector<std::string>& CEQL_QUERYParser::getRuleNames() const {
  return ceql_queryParserStaticData->ruleNames;
}

const dfa::Vocabulary& CEQL_QUERYParser::getVocabulary() const {
  return ceql_queryParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView CEQL_QUERYParser::getSerializedATN() const {
  return ceql_queryParserStaticData->serializedATN;
}


//----------------- ParseContext ------------------------------------------------------------------

CEQL_QUERYParser::ParseContext::ParseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CEQL_QUERYParser::ParseContext::EOF() {
  return getToken(CEQL_QUERYParser::EOF, 0);
}

std::vector<CEQL_QUERYParser::Core_queryContext *> CEQL_QUERYParser::ParseContext::core_query() {
  return getRuleContexts<CEQL_QUERYParser::Core_queryContext>();
}

CEQL_QUERYParser::Core_queryContext* CEQL_QUERYParser::ParseContext::core_query(size_t i) {
  return getRuleContext<CEQL_QUERYParser::Core_queryContext>(i);
}

std::vector<CEQL_QUERYParser::ErrorContext *> CEQL_QUERYParser::ParseContext::error() {
  return getRuleContexts<CEQL_QUERYParser::ErrorContext>();
}

CEQL_QUERYParser::ErrorContext* CEQL_QUERYParser::ParseContext::error(size_t i) {
  return getRuleContext<CEQL_QUERYParser::ErrorContext>(i);
}


size_t CEQL_QUERYParser::ParseContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleParse;
}


std::any CEQL_QUERYParser::ParseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitParse(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::ParseContext* CEQL_QUERYParser::parse() {
  ParseContext *_localctx = _tracker.createInstance<ParseContext>(_ctx, getState());
  enterRule(_localctx, 0, CEQL_QUERYParser::RuleParse);
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
    setState(78);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CEQL_QUERYParser::K_SELECT

    || _la == CEQL_QUERYParser::UNEXPECTED_CHAR) {
      setState(76);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case CEQL_QUERYParser::K_SELECT: {
          setState(74);
          core_query();
          break;
        }

        case CEQL_QUERYParser::UNEXPECTED_CHAR: {
          setState(75);
          error();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(80);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(81);
    match(CEQL_QUERYParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ErrorContext ------------------------------------------------------------------

CEQL_QUERYParser::ErrorContext::ErrorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CEQL_QUERYParser::ErrorContext::UNEXPECTED_CHAR() {
  return getToken(CEQL_QUERYParser::UNEXPECTED_CHAR, 0);
}


size_t CEQL_QUERYParser::ErrorContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleError;
}


std::any CEQL_QUERYParser::ErrorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitError(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::ErrorContext* CEQL_QUERYParser::error() {
  ErrorContext *_localctx = _tracker.createInstance<ErrorContext>(_ctx, getState());
  enterRule(_localctx, 2, CEQL_QUERYParser::RuleError);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(83);
    antlrcpp::downCast<ErrorContext *>(_localctx)->unexpected_charToken = match(CEQL_QUERYParser::UNEXPECTED_CHAR);

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

CEQL_QUERYParser::Core_queryContext::Core_queryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CEQL_QUERYParser::Core_queryContext::K_SELECT() {
  return getToken(CEQL_QUERYParser::K_SELECT, 0);
}

CEQL_QUERYParser::List_of_variablesContext* CEQL_QUERYParser::Core_queryContext::list_of_variables() {
  return getRuleContext<CEQL_QUERYParser::List_of_variablesContext>(0);
}

CEQL_QUERYParser::From_clauseContext* CEQL_QUERYParser::Core_queryContext::from_clause() {
  return getRuleContext<CEQL_QUERYParser::From_clauseContext>(0);
}

tree::TerminalNode* CEQL_QUERYParser::Core_queryContext::K_WHERE() {
  return getToken(CEQL_QUERYParser::K_WHERE, 0);
}

CEQL_QUERYParser::Cel_formulaContext* CEQL_QUERYParser::Core_queryContext::cel_formula() {
  return getRuleContext<CEQL_QUERYParser::Cel_formulaContext>(0);
}

CEQL_QUERYParser::Selection_strategyContext* CEQL_QUERYParser::Core_queryContext::selection_strategy() {
  return getRuleContext<CEQL_QUERYParser::Selection_strategyContext>(0);
}

tree::TerminalNode* CEQL_QUERYParser::Core_queryContext::K_PARTITION() {
  return getToken(CEQL_QUERYParser::K_PARTITION, 0);
}

std::vector<tree::TerminalNode *> CEQL_QUERYParser::Core_queryContext::K_BY() {
  return getTokens(CEQL_QUERYParser::K_BY);
}

tree::TerminalNode* CEQL_QUERYParser::Core_queryContext::K_BY(size_t i) {
  return getToken(CEQL_QUERYParser::K_BY, i);
}

CEQL_QUERYParser::Partition_listContext* CEQL_QUERYParser::Core_queryContext::partition_list() {
  return getRuleContext<CEQL_QUERYParser::Partition_listContext>(0);
}

tree::TerminalNode* CEQL_QUERYParser::Core_queryContext::K_WITHIN() {
  return getToken(CEQL_QUERYParser::K_WITHIN, 0);
}

CEQL_QUERYParser::Time_windowContext* CEQL_QUERYParser::Core_queryContext::time_window() {
  return getRuleContext<CEQL_QUERYParser::Time_windowContext>(0);
}

tree::TerminalNode* CEQL_QUERYParser::Core_queryContext::K_CONSUME() {
  return getToken(CEQL_QUERYParser::K_CONSUME, 0);
}

CEQL_QUERYParser::Consumption_policyContext* CEQL_QUERYParser::Core_queryContext::consumption_policy() {
  return getRuleContext<CEQL_QUERYParser::Consumption_policyContext>(0);
}


size_t CEQL_QUERYParser::Core_queryContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleCore_query;
}


std::any CEQL_QUERYParser::Core_queryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitCore_query(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::Core_queryContext* CEQL_QUERYParser::core_query() {
  Core_queryContext *_localctx = _tracker.createInstance<Core_queryContext>(_ctx, getState());
  enterRule(_localctx, 4, CEQL_QUERYParser::RuleCore_query);
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
    setState(86);
    match(CEQL_QUERYParser::K_SELECT);
    setState(88);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 17267952128) != 0)) {
      setState(87);
      selection_strategy();
    }
    setState(90);
    list_of_variables();
    setState(91);
    from_clause();
    setState(92);
    match(CEQL_QUERYParser::K_WHERE);
    setState(93);
    cel_formula(0);
    setState(97);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQL_QUERYParser::K_PARTITION) {
      setState(94);
      match(CEQL_QUERYParser::K_PARTITION);
      setState(95);
      match(CEQL_QUERYParser::K_BY);
      setState(96);
      partition_list();
    }
    setState(101);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQL_QUERYParser::K_WITHIN) {
      setState(99);
      match(CEQL_QUERYParser::K_WITHIN);
      setState(100);
      time_window();
    }
    setState(106);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQL_QUERYParser::K_CONSUME) {
      setState(103);
      match(CEQL_QUERYParser::K_CONSUME);
      setState(104);
      match(CEQL_QUERYParser::K_BY);
      setState(105);
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

CEQL_QUERYParser::Selection_strategyContext::Selection_strategyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CEQL_QUERYParser::Selection_strategyContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleSelection_strategy;
}

void CEQL_QUERYParser::Selection_strategyContext::copyFrom(Selection_strategyContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Ss_lastContext ------------------------------------------------------------------

tree::TerminalNode* CEQL_QUERYParser::Ss_lastContext::K_LAST() {
  return getToken(CEQL_QUERYParser::K_LAST, 0);
}

CEQL_QUERYParser::Ss_lastContext::Ss_lastContext(Selection_strategyContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Ss_lastContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitSs_last(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Ss_strictContext ------------------------------------------------------------------

tree::TerminalNode* CEQL_QUERYParser::Ss_strictContext::K_STRICT() {
  return getToken(CEQL_QUERYParser::K_STRICT, 0);
}

CEQL_QUERYParser::Ss_strictContext::Ss_strictContext(Selection_strategyContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Ss_strictContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitSs_strict(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Ss_anyContext ------------------------------------------------------------------

tree::TerminalNode* CEQL_QUERYParser::Ss_anyContext::K_ANY() {
  return getToken(CEQL_QUERYParser::K_ANY, 0);
}

CEQL_QUERYParser::Ss_anyContext::Ss_anyContext(Selection_strategyContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Ss_anyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitSs_any(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Ss_nextContext ------------------------------------------------------------------

tree::TerminalNode* CEQL_QUERYParser::Ss_nextContext::K_NEXT() {
  return getToken(CEQL_QUERYParser::K_NEXT, 0);
}

CEQL_QUERYParser::Ss_nextContext::Ss_nextContext(Selection_strategyContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Ss_nextContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitSs_next(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Ss_maxContext ------------------------------------------------------------------

tree::TerminalNode* CEQL_QUERYParser::Ss_maxContext::K_MAX() {
  return getToken(CEQL_QUERYParser::K_MAX, 0);
}

CEQL_QUERYParser::Ss_maxContext::Ss_maxContext(Selection_strategyContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Ss_maxContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitSs_max(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Ss_allContext ------------------------------------------------------------------

tree::TerminalNode* CEQL_QUERYParser::Ss_allContext::K_ALL() {
  return getToken(CEQL_QUERYParser::K_ALL, 0);
}

CEQL_QUERYParser::Ss_allContext::Ss_allContext(Selection_strategyContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Ss_allContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitSs_all(this);
  else
    return visitor->visitChildren(this);
}
CEQL_QUERYParser::Selection_strategyContext* CEQL_QUERYParser::selection_strategy() {
  Selection_strategyContext *_localctx = _tracker.createInstance<Selection_strategyContext>(_ctx, getState());
  enterRule(_localctx, 6, CEQL_QUERYParser::RuleSelection_strategy);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(114);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQL_QUERYParser::K_ALL: {
        _localctx = _tracker.createInstance<CEQL_QUERYParser::Ss_allContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(108);
        match(CEQL_QUERYParser::K_ALL);
        break;
      }

      case CEQL_QUERYParser::K_ANY: {
        _localctx = _tracker.createInstance<CEQL_QUERYParser::Ss_anyContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(109);
        match(CEQL_QUERYParser::K_ANY);
        break;
      }

      case CEQL_QUERYParser::K_LAST: {
        _localctx = _tracker.createInstance<CEQL_QUERYParser::Ss_lastContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(110);
        match(CEQL_QUERYParser::K_LAST);
        break;
      }

      case CEQL_QUERYParser::K_MAX: {
        _localctx = _tracker.createInstance<CEQL_QUERYParser::Ss_maxContext>(_localctx);
        enterOuterAlt(_localctx, 4);
        setState(111);
        match(CEQL_QUERYParser::K_MAX);
        break;
      }

      case CEQL_QUERYParser::K_NEXT: {
        _localctx = _tracker.createInstance<CEQL_QUERYParser::Ss_nextContext>(_localctx);
        enterOuterAlt(_localctx, 5);
        setState(112);
        match(CEQL_QUERYParser::K_NEXT);
        break;
      }

      case CEQL_QUERYParser::K_STRICT: {
        _localctx = _tracker.createInstance<CEQL_QUERYParser::Ss_strictContext>(_localctx);
        enterOuterAlt(_localctx, 6);
        setState(113);
        match(CEQL_QUERYParser::K_STRICT);
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

CEQL_QUERYParser::List_of_variablesContext::List_of_variablesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CEQL_QUERYParser::List_of_variablesContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleList_of_variables;
}

void CEQL_QUERYParser::List_of_variablesContext::copyFrom(List_of_variablesContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- S_starContext ------------------------------------------------------------------

tree::TerminalNode* CEQL_QUERYParser::S_starContext::STAR() {
  return getToken(CEQL_QUERYParser::STAR, 0);
}

CEQL_QUERYParser::S_starContext::S_starContext(List_of_variablesContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::S_starContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitS_star(this);
  else
    return visitor->visitChildren(this);
}
//----------------- S_list_of_variablesContext ------------------------------------------------------------------

std::vector<CEQL_QUERYParser::Any_nameContext *> CEQL_QUERYParser::S_list_of_variablesContext::any_name() {
  return getRuleContexts<CEQL_QUERYParser::Any_nameContext>();
}

CEQL_QUERYParser::Any_nameContext* CEQL_QUERYParser::S_list_of_variablesContext::any_name(size_t i) {
  return getRuleContext<CEQL_QUERYParser::Any_nameContext>(i);
}

CEQL_QUERYParser::S_list_of_variablesContext::S_list_of_variablesContext(List_of_variablesContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::S_list_of_variablesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitS_list_of_variables(this);
  else
    return visitor->visitChildren(this);
}
CEQL_QUERYParser::List_of_variablesContext* CEQL_QUERYParser::list_of_variables() {
  List_of_variablesContext *_localctx = _tracker.createInstance<List_of_variablesContext>(_ctx, getState());
  enterRule(_localctx, 8, CEQL_QUERYParser::RuleList_of_variables);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(125);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQL_QUERYParser::STAR: {
        _localctx = _tracker.createInstance<CEQL_QUERYParser::S_starContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(116);
        match(CEQL_QUERYParser::STAR);
        break;
      }

      case CEQL_QUERYParser::IDENTIFIER: {
        _localctx = _tracker.createInstance<CEQL_QUERYParser::S_list_of_variablesContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(117);
        any_name();
        setState(122);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == CEQL_QUERYParser::T__0) {
          setState(118);
          match(CEQL_QUERYParser::T__0);
          setState(119);
          any_name();
          setState(124);
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

CEQL_QUERYParser::From_clauseContext::From_clauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CEQL_QUERYParser::From_clauseContext::K_FROM() {
  return getToken(CEQL_QUERYParser::K_FROM, 0);
}

std::vector<CEQL_QUERYParser::Stream_nameContext *> CEQL_QUERYParser::From_clauseContext::stream_name() {
  return getRuleContexts<CEQL_QUERYParser::Stream_nameContext>();
}

CEQL_QUERYParser::Stream_nameContext* CEQL_QUERYParser::From_clauseContext::stream_name(size_t i) {
  return getRuleContext<CEQL_QUERYParser::Stream_nameContext>(i);
}


size_t CEQL_QUERYParser::From_clauseContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleFrom_clause;
}


std::any CEQL_QUERYParser::From_clauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitFrom_clause(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::From_clauseContext* CEQL_QUERYParser::from_clause() {
  From_clauseContext *_localctx = _tracker.createInstance<From_clauseContext>(_ctx, getState());
  enterRule(_localctx, 10, CEQL_QUERYParser::RuleFrom_clause);
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
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQL_QUERYParser::K_FROM) {
      setState(127);
      match(CEQL_QUERYParser::K_FROM);
      setState(128);
      stream_name();
      setState(133);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == CEQL_QUERYParser::T__0) {
        setState(129);
        match(CEQL_QUERYParser::T__0);
        setState(130);
        stream_name();
        setState(135);
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

CEQL_QUERYParser::Cel_formulaContext::Cel_formulaContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CEQL_QUERYParser::Cel_formulaContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleCel_formula;
}

void CEQL_QUERYParser::Cel_formulaContext::copyFrom(Cel_formulaContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Event_type_cel_formulaContext ------------------------------------------------------------------

CEQL_QUERYParser::S_event_nameContext* CEQL_QUERYParser::Event_type_cel_formulaContext::s_event_name() {
  return getRuleContext<CEQL_QUERYParser::S_event_nameContext>(0);
}

CEQL_QUERYParser::Event_type_cel_formulaContext::Event_type_cel_formulaContext(Cel_formulaContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Event_type_cel_formulaContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitEvent_type_cel_formula(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Par_cel_formulaContext ------------------------------------------------------------------

CEQL_QUERYParser::Cel_formulaContext* CEQL_QUERYParser::Par_cel_formulaContext::cel_formula() {
  return getRuleContext<CEQL_QUERYParser::Cel_formulaContext>(0);
}

CEQL_QUERYParser::Par_cel_formulaContext::Par_cel_formulaContext(Cel_formulaContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Par_cel_formulaContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitPar_cel_formula(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Sequencing_cel_formulaContext ------------------------------------------------------------------

std::vector<CEQL_QUERYParser::Cel_formulaContext *> CEQL_QUERYParser::Sequencing_cel_formulaContext::cel_formula() {
  return getRuleContexts<CEQL_QUERYParser::Cel_formulaContext>();
}

CEQL_QUERYParser::Cel_formulaContext* CEQL_QUERYParser::Sequencing_cel_formulaContext::cel_formula(size_t i) {
  return getRuleContext<CEQL_QUERYParser::Cel_formulaContext>(i);
}

tree::TerminalNode* CEQL_QUERYParser::Sequencing_cel_formulaContext::SEMICOLON() {
  return getToken(CEQL_QUERYParser::SEMICOLON, 0);
}

CEQL_QUERYParser::Sequencing_cel_formulaContext::Sequencing_cel_formulaContext(Cel_formulaContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Sequencing_cel_formulaContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitSequencing_cel_formula(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Kleene_cel_formulaContext ------------------------------------------------------------------

CEQL_QUERYParser::Cel_formulaContext* CEQL_QUERYParser::Kleene_cel_formulaContext::cel_formula() {
  return getRuleContext<CEQL_QUERYParser::Cel_formulaContext>(0);
}

tree::TerminalNode* CEQL_QUERYParser::Kleene_cel_formulaContext::PLUS() {
  return getToken(CEQL_QUERYParser::PLUS, 0);
}

CEQL_QUERYParser::Kleene_cel_formulaContext::Kleene_cel_formulaContext(Cel_formulaContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Kleene_cel_formulaContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitKleene_cel_formula(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Filter_cel_formulaContext ------------------------------------------------------------------

CEQL_QUERYParser::Cel_formulaContext* CEQL_QUERYParser::Filter_cel_formulaContext::cel_formula() {
  return getRuleContext<CEQL_QUERYParser::Cel_formulaContext>(0);
}

tree::TerminalNode* CEQL_QUERYParser::Filter_cel_formulaContext::K_FILTER() {
  return getToken(CEQL_QUERYParser::K_FILTER, 0);
}

CEQL_QUERYParser::FilterContext* CEQL_QUERYParser::Filter_cel_formulaContext::filter() {
  return getRuleContext<CEQL_QUERYParser::FilterContext>(0);
}

CEQL_QUERYParser::Filter_cel_formulaContext::Filter_cel_formulaContext(Cel_formulaContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Filter_cel_formulaContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitFilter_cel_formula(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Or_cel_formulaContext ------------------------------------------------------------------

std::vector<CEQL_QUERYParser::Cel_formulaContext *> CEQL_QUERYParser::Or_cel_formulaContext::cel_formula() {
  return getRuleContexts<CEQL_QUERYParser::Cel_formulaContext>();
}

CEQL_QUERYParser::Cel_formulaContext* CEQL_QUERYParser::Or_cel_formulaContext::cel_formula(size_t i) {
  return getRuleContext<CEQL_QUERYParser::Cel_formulaContext>(i);
}

tree::TerminalNode* CEQL_QUERYParser::Or_cel_formulaContext::K_OR() {
  return getToken(CEQL_QUERYParser::K_OR, 0);
}

CEQL_QUERYParser::Or_cel_formulaContext::Or_cel_formulaContext(Cel_formulaContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Or_cel_formulaContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitOr_cel_formula(this);
  else
    return visitor->visitChildren(this);
}
//----------------- As_cel_formulaContext ------------------------------------------------------------------

CEQL_QUERYParser::Cel_formulaContext* CEQL_QUERYParser::As_cel_formulaContext::cel_formula() {
  return getRuleContext<CEQL_QUERYParser::Cel_formulaContext>(0);
}

tree::TerminalNode* CEQL_QUERYParser::As_cel_formulaContext::K_AS() {
  return getToken(CEQL_QUERYParser::K_AS, 0);
}

CEQL_QUERYParser::Event_nameContext* CEQL_QUERYParser::As_cel_formulaContext::event_name() {
  return getRuleContext<CEQL_QUERYParser::Event_nameContext>(0);
}

CEQL_QUERYParser::As_cel_formulaContext::As_cel_formulaContext(Cel_formulaContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::As_cel_formulaContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitAs_cel_formula(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::Cel_formulaContext* CEQL_QUERYParser::cel_formula() {
   return cel_formula(0);
}

CEQL_QUERYParser::Cel_formulaContext* CEQL_QUERYParser::cel_formula(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  CEQL_QUERYParser::Cel_formulaContext *_localctx = _tracker.createInstance<Cel_formulaContext>(_ctx, parentState);
  CEQL_QUERYParser::Cel_formulaContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 12;
  enterRecursionRule(_localctx, 12, CEQL_QUERYParser::RuleCel_formula, precedence);

    

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
    setState(144);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQL_QUERYParser::T__1: {
        _localctx = _tracker.createInstance<Par_cel_formulaContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(139);
        match(CEQL_QUERYParser::T__1);
        setState(140);
        cel_formula(0);
        setState(141);
        match(CEQL_QUERYParser::T__2);
        break;
      }

      case CEQL_QUERYParser::IDENTIFIER: {
        _localctx = _tracker.createInstance<Event_type_cel_formulaContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(143);
        s_event_name();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(162);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 13, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(160);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 12, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<Sequencing_cel_formulaContext>(_tracker.createInstance<Cel_formulaContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleCel_formula);
          setState(146);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(147);
          match(CEQL_QUERYParser::SEMICOLON);
          setState(148);
          cel_formula(4);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<Or_cel_formulaContext>(_tracker.createInstance<Cel_formulaContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleCel_formula);
          setState(149);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(150);
          match(CEQL_QUERYParser::K_OR);
          setState(151);
          cel_formula(3);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<As_cel_formulaContext>(_tracker.createInstance<Cel_formulaContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleCel_formula);
          setState(152);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(153);
          match(CEQL_QUERYParser::K_AS);
          setState(154);
          event_name();
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<Kleene_cel_formulaContext>(_tracker.createInstance<Cel_formulaContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleCel_formula);
          setState(155);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(156);
          match(CEQL_QUERYParser::PLUS);
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<Filter_cel_formulaContext>(_tracker.createInstance<Cel_formulaContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleCel_formula);
          setState(157);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(158);
          match(CEQL_QUERYParser::K_FILTER);
          setState(159);
          filter(0);
          break;
        }

        default:
          break;
        } 
      }
      setState(164);
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

CEQL_QUERYParser::Partition_listContext::Partition_listContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<CEQL_QUERYParser::Attribute_listContext *> CEQL_QUERYParser::Partition_listContext::attribute_list() {
  return getRuleContexts<CEQL_QUERYParser::Attribute_listContext>();
}

CEQL_QUERYParser::Attribute_listContext* CEQL_QUERYParser::Partition_listContext::attribute_list(size_t i) {
  return getRuleContext<CEQL_QUERYParser::Attribute_listContext>(i);
}


size_t CEQL_QUERYParser::Partition_listContext::getRuleIndex() const {
  return CEQL_QUERYParser::RulePartition_list;
}


std::any CEQL_QUERYParser::Partition_listContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitPartition_list(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::Partition_listContext* CEQL_QUERYParser::partition_list() {
  Partition_listContext *_localctx = _tracker.createInstance<Partition_listContext>(_ctx, getState());
  enterRule(_localctx, 14, CEQL_QUERYParser::RulePartition_list);
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
    setState(165);
    match(CEQL_QUERYParser::T__3);
    setState(166);
    attribute_list();
    setState(167);
    match(CEQL_QUERYParser::T__4);
    setState(173);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQL_QUERYParser::T__0) {
      setState(168);
      match(CEQL_QUERYParser::T__0);
      setState(169);
      match(CEQL_QUERYParser::T__3);
      setState(170);
      attribute_list();
      setState(171);
      match(CEQL_QUERYParser::T__4);
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

CEQL_QUERYParser::Attribute_listContext::Attribute_listContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<CEQL_QUERYParser::Attribute_nameContext *> CEQL_QUERYParser::Attribute_listContext::attribute_name() {
  return getRuleContexts<CEQL_QUERYParser::Attribute_nameContext>();
}

CEQL_QUERYParser::Attribute_nameContext* CEQL_QUERYParser::Attribute_listContext::attribute_name(size_t i) {
  return getRuleContext<CEQL_QUERYParser::Attribute_nameContext>(i);
}


size_t CEQL_QUERYParser::Attribute_listContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleAttribute_list;
}


std::any CEQL_QUERYParser::Attribute_listContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitAttribute_list(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::Attribute_listContext* CEQL_QUERYParser::attribute_list() {
  Attribute_listContext *_localctx = _tracker.createInstance<Attribute_listContext>(_ctx, getState());
  enterRule(_localctx, 16, CEQL_QUERYParser::RuleAttribute_list);
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
    setState(175);
    attribute_name();
    setState(180);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CEQL_QUERYParser::T__0) {
      setState(176);
      match(CEQL_QUERYParser::T__0);
      setState(177);
      attribute_name();
      setState(182);
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

CEQL_QUERYParser::Consumption_policyContext::Consumption_policyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CEQL_QUERYParser::Consumption_policyContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleConsumption_policy;
}

void CEQL_QUERYParser::Consumption_policyContext::copyFrom(Consumption_policyContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Cp_partitionContext ------------------------------------------------------------------

tree::TerminalNode* CEQL_QUERYParser::Cp_partitionContext::K_PARTITION() {
  return getToken(CEQL_QUERYParser::K_PARTITION, 0);
}

CEQL_QUERYParser::Cp_partitionContext::Cp_partitionContext(Consumption_policyContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Cp_partitionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitCp_partition(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Cp_anyContext ------------------------------------------------------------------

tree::TerminalNode* CEQL_QUERYParser::Cp_anyContext::K_ANY() {
  return getToken(CEQL_QUERYParser::K_ANY, 0);
}

CEQL_QUERYParser::Cp_anyContext::Cp_anyContext(Consumption_policyContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Cp_anyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitCp_any(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Cp_noneContext ------------------------------------------------------------------

tree::TerminalNode* CEQL_QUERYParser::Cp_noneContext::K_NONE() {
  return getToken(CEQL_QUERYParser::K_NONE, 0);
}

CEQL_QUERYParser::Cp_noneContext::Cp_noneContext(Consumption_policyContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Cp_noneContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitCp_none(this);
  else
    return visitor->visitChildren(this);
}
CEQL_QUERYParser::Consumption_policyContext* CEQL_QUERYParser::consumption_policy() {
  Consumption_policyContext *_localctx = _tracker.createInstance<Consumption_policyContext>(_ctx, getState());
  enterRule(_localctx, 18, CEQL_QUERYParser::RuleConsumption_policy);

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
      case CEQL_QUERYParser::K_ANY: {
        _localctx = _tracker.createInstance<CEQL_QUERYParser::Cp_anyContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(183);
        match(CEQL_QUERYParser::K_ANY);
        break;
      }

      case CEQL_QUERYParser::K_PARTITION: {
        _localctx = _tracker.createInstance<CEQL_QUERYParser::Cp_partitionContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(184);
        match(CEQL_QUERYParser::K_PARTITION);
        break;
      }

      case CEQL_QUERYParser::K_NONE: {
        _localctx = _tracker.createInstance<CEQL_QUERYParser::Cp_noneContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(185);
        match(CEQL_QUERYParser::K_NONE);
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

CEQL_QUERYParser::FilterContext::FilterContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CEQL_QUERYParser::FilterContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleFilter;
}

void CEQL_QUERYParser::FilterContext::copyFrom(FilterContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Par_filterContext ------------------------------------------------------------------

CEQL_QUERYParser::FilterContext* CEQL_QUERYParser::Par_filterContext::filter() {
  return getRuleContext<CEQL_QUERYParser::FilterContext>(0);
}

CEQL_QUERYParser::Par_filterContext::Par_filterContext(FilterContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Par_filterContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitPar_filter(this);
  else
    return visitor->visitChildren(this);
}
//----------------- And_filterContext ------------------------------------------------------------------

std::vector<CEQL_QUERYParser::FilterContext *> CEQL_QUERYParser::And_filterContext::filter() {
  return getRuleContexts<CEQL_QUERYParser::FilterContext>();
}

CEQL_QUERYParser::FilterContext* CEQL_QUERYParser::And_filterContext::filter(size_t i) {
  return getRuleContext<CEQL_QUERYParser::FilterContext>(i);
}

tree::TerminalNode* CEQL_QUERYParser::And_filterContext::K_AND() {
  return getToken(CEQL_QUERYParser::K_AND, 0);
}

CEQL_QUERYParser::And_filterContext::And_filterContext(FilterContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::And_filterContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitAnd_filter(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Atomic_filterContext ------------------------------------------------------------------

CEQL_QUERYParser::Event_nameContext* CEQL_QUERYParser::Atomic_filterContext::event_name() {
  return getRuleContext<CEQL_QUERYParser::Event_nameContext>(0);
}

CEQL_QUERYParser::PredicateContext* CEQL_QUERYParser::Atomic_filterContext::predicate() {
  return getRuleContext<CEQL_QUERYParser::PredicateContext>(0);
}

CEQL_QUERYParser::Atomic_filterContext::Atomic_filterContext(FilterContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Atomic_filterContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitAtomic_filter(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Or_filterContext ------------------------------------------------------------------

std::vector<CEQL_QUERYParser::FilterContext *> CEQL_QUERYParser::Or_filterContext::filter() {
  return getRuleContexts<CEQL_QUERYParser::FilterContext>();
}

CEQL_QUERYParser::FilterContext* CEQL_QUERYParser::Or_filterContext::filter(size_t i) {
  return getRuleContext<CEQL_QUERYParser::FilterContext>(i);
}

tree::TerminalNode* CEQL_QUERYParser::Or_filterContext::K_OR() {
  return getToken(CEQL_QUERYParser::K_OR, 0);
}

CEQL_QUERYParser::Or_filterContext::Or_filterContext(FilterContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Or_filterContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitOr_filter(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::FilterContext* CEQL_QUERYParser::filter() {
   return filter(0);
}

CEQL_QUERYParser::FilterContext* CEQL_QUERYParser::filter(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  CEQL_QUERYParser::FilterContext *_localctx = _tracker.createInstance<FilterContext>(_ctx, parentState);
  CEQL_QUERYParser::FilterContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 20;
  enterRecursionRule(_localctx, 20, CEQL_QUERYParser::RuleFilter, precedence);

    

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
    setState(198);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQL_QUERYParser::T__1: {
        _localctx = _tracker.createInstance<Par_filterContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(189);
        match(CEQL_QUERYParser::T__1);
        setState(190);
        filter(0);
        setState(191);
        match(CEQL_QUERYParser::T__2);
        break;
      }

      case CEQL_QUERYParser::IDENTIFIER: {
        _localctx = _tracker.createInstance<Atomic_filterContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(193);
        event_name();
        setState(194);
        match(CEQL_QUERYParser::T__3);
        setState(195);
        predicate(0);
        setState(196);
        match(CEQL_QUERYParser::T__4);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(208);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 19, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(206);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 18, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<And_filterContext>(_tracker.createInstance<FilterContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleFilter);
          setState(200);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(201);
          match(CEQL_QUERYParser::K_AND);
          setState(202);
          filter(3);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<Or_filterContext>(_tracker.createInstance<FilterContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleFilter);
          setState(203);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(204);
          match(CEQL_QUERYParser::K_OR);
          setState(205);
          filter(2);
          break;
        }

        default:
          break;
        } 
      }
      setState(210);
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

CEQL_QUERYParser::PredicateContext::PredicateContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CEQL_QUERYParser::PredicateContext::getRuleIndex() const {
  return CEQL_QUERYParser::RulePredicate;
}

void CEQL_QUERYParser::PredicateContext::copyFrom(PredicateContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Regex_predicateContext ------------------------------------------------------------------

CEQL_QUERYParser::Attribute_nameContext* CEQL_QUERYParser::Regex_predicateContext::attribute_name() {
  return getRuleContext<CEQL_QUERYParser::Attribute_nameContext>(0);
}

tree::TerminalNode* CEQL_QUERYParser::Regex_predicateContext::K_LIKE() {
  return getToken(CEQL_QUERYParser::K_LIKE, 0);
}

CEQL_QUERYParser::RegexpContext* CEQL_QUERYParser::Regex_predicateContext::regexp() {
  return getRuleContext<CEQL_QUERYParser::RegexpContext>(0);
}

CEQL_QUERYParser::Regex_predicateContext::Regex_predicateContext(PredicateContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Regex_predicateContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitRegex_predicate(this);
  else
    return visitor->visitChildren(this);
}
//----------------- In_predicateContext ------------------------------------------------------------------

CEQL_QUERYParser::Attribute_nameContext* CEQL_QUERYParser::In_predicateContext::attribute_name() {
  return getRuleContext<CEQL_QUERYParser::Attribute_nameContext>(0);
}

CEQL_QUERYParser::Value_seqContext* CEQL_QUERYParser::In_predicateContext::value_seq() {
  return getRuleContext<CEQL_QUERYParser::Value_seqContext>(0);
}

tree::TerminalNode* CEQL_QUERYParser::In_predicateContext::K_IN() {
  return getToken(CEQL_QUERYParser::K_IN, 0);
}

tree::TerminalNode* CEQL_QUERYParser::In_predicateContext::K_NOT() {
  return getToken(CEQL_QUERYParser::K_NOT, 0);
}

CEQL_QUERYParser::In_predicateContext::In_predicateContext(PredicateContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::In_predicateContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitIn_predicate(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Not_predicateContext ------------------------------------------------------------------

tree::TerminalNode* CEQL_QUERYParser::Not_predicateContext::K_NOT() {
  return getToken(CEQL_QUERYParser::K_NOT, 0);
}

CEQL_QUERYParser::PredicateContext* CEQL_QUERYParser::Not_predicateContext::predicate() {
  return getRuleContext<CEQL_QUERYParser::PredicateContext>(0);
}

CEQL_QUERYParser::Not_predicateContext::Not_predicateContext(PredicateContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Not_predicateContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitNot_predicate(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Par_predicateContext ------------------------------------------------------------------

CEQL_QUERYParser::PredicateContext* CEQL_QUERYParser::Par_predicateContext::predicate() {
  return getRuleContext<CEQL_QUERYParser::PredicateContext>(0);
}

CEQL_QUERYParser::Par_predicateContext::Par_predicateContext(PredicateContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Par_predicateContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitPar_predicate(this);
  else
    return visitor->visitChildren(this);
}
//----------------- And_predicateContext ------------------------------------------------------------------

std::vector<CEQL_QUERYParser::PredicateContext *> CEQL_QUERYParser::And_predicateContext::predicate() {
  return getRuleContexts<CEQL_QUERYParser::PredicateContext>();
}

CEQL_QUERYParser::PredicateContext* CEQL_QUERYParser::And_predicateContext::predicate(size_t i) {
  return getRuleContext<CEQL_QUERYParser::PredicateContext>(i);
}

tree::TerminalNode* CEQL_QUERYParser::And_predicateContext::K_AND() {
  return getToken(CEQL_QUERYParser::K_AND, 0);
}

CEQL_QUERYParser::And_predicateContext::And_predicateContext(PredicateContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::And_predicateContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitAnd_predicate(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Equality_string_predicateContext ------------------------------------------------------------------

CEQL_QUERYParser::String_literalContext* CEQL_QUERYParser::Equality_string_predicateContext::string_literal() {
  return getRuleContext<CEQL_QUERYParser::String_literalContext>(0);
}

CEQL_QUERYParser::String_literal_or_regexpContext* CEQL_QUERYParser::Equality_string_predicateContext::string_literal_or_regexp() {
  return getRuleContext<CEQL_QUERYParser::String_literal_or_regexpContext>(0);
}

tree::TerminalNode* CEQL_QUERYParser::Equality_string_predicateContext::EQ() {
  return getToken(CEQL_QUERYParser::EQ, 0);
}

tree::TerminalNode* CEQL_QUERYParser::Equality_string_predicateContext::NEQ() {
  return getToken(CEQL_QUERYParser::NEQ, 0);
}

CEQL_QUERYParser::Equality_string_predicateContext::Equality_string_predicateContext(PredicateContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Equality_string_predicateContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitEquality_string_predicate(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Inequality_predicateContext ------------------------------------------------------------------

std::vector<CEQL_QUERYParser::Math_exprContext *> CEQL_QUERYParser::Inequality_predicateContext::math_expr() {
  return getRuleContexts<CEQL_QUERYParser::Math_exprContext>();
}

CEQL_QUERYParser::Math_exprContext* CEQL_QUERYParser::Inequality_predicateContext::math_expr(size_t i) {
  return getRuleContext<CEQL_QUERYParser::Math_exprContext>(i);
}

tree::TerminalNode* CEQL_QUERYParser::Inequality_predicateContext::LE() {
  return getToken(CEQL_QUERYParser::LE, 0);
}

tree::TerminalNode* CEQL_QUERYParser::Inequality_predicateContext::LEQ() {
  return getToken(CEQL_QUERYParser::LEQ, 0);
}

tree::TerminalNode* CEQL_QUERYParser::Inequality_predicateContext::GE() {
  return getToken(CEQL_QUERYParser::GE, 0);
}

tree::TerminalNode* CEQL_QUERYParser::Inequality_predicateContext::GEQ() {
  return getToken(CEQL_QUERYParser::GEQ, 0);
}

tree::TerminalNode* CEQL_QUERYParser::Inequality_predicateContext::EQ() {
  return getToken(CEQL_QUERYParser::EQ, 0);
}

tree::TerminalNode* CEQL_QUERYParser::Inequality_predicateContext::NEQ() {
  return getToken(CEQL_QUERYParser::NEQ, 0);
}

CEQL_QUERYParser::Inequality_predicateContext::Inequality_predicateContext(PredicateContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Inequality_predicateContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitInequality_predicate(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Or_predicateContext ------------------------------------------------------------------

std::vector<CEQL_QUERYParser::PredicateContext *> CEQL_QUERYParser::Or_predicateContext::predicate() {
  return getRuleContexts<CEQL_QUERYParser::PredicateContext>();
}

CEQL_QUERYParser::PredicateContext* CEQL_QUERYParser::Or_predicateContext::predicate(size_t i) {
  return getRuleContext<CEQL_QUERYParser::PredicateContext>(i);
}

tree::TerminalNode* CEQL_QUERYParser::Or_predicateContext::K_OR() {
  return getToken(CEQL_QUERYParser::K_OR, 0);
}

CEQL_QUERYParser::Or_predicateContext::Or_predicateContext(PredicateContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Or_predicateContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitOr_predicate(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::PredicateContext* CEQL_QUERYParser::predicate() {
   return predicate(0);
}

CEQL_QUERYParser::PredicateContext* CEQL_QUERYParser::predicate(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  CEQL_QUERYParser::PredicateContext *_localctx = _tracker.createInstance<PredicateContext>(_ctx, parentState);
  CEQL_QUERYParser::PredicateContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 22;
  enterRecursionRule(_localctx, 22, CEQL_QUERYParser::RulePredicate, precedence);

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
    setState(238);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 21, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<Par_predicateContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(212);
      match(CEQL_QUERYParser::T__1);
      setState(213);
      predicate(0);
      setState(214);
      match(CEQL_QUERYParser::T__2);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<Not_predicateContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(216);
      match(CEQL_QUERYParser::K_NOT);
      setState(217);
      predicate(7);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<Inequality_predicateContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(218);
      math_expr(0);
      setState(219);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 554153860399104) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(220);
      math_expr(0);
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<Equality_string_predicateContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(222);
      string_literal();
      setState(223);
      _la = _input->LA(1);
      if (!(_la == CEQL_QUERYParser::EQ

      || _la == CEQL_QUERYParser::NEQ)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(224);
      string_literal_or_regexp();
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<Regex_predicateContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(226);
      attribute_name();
      setState(227);
      match(CEQL_QUERYParser::K_LIKE);
      setState(228);
      regexp();
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<In_predicateContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(230);
      attribute_name();
      setState(234);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case CEQL_QUERYParser::K_IN: {
          setState(231);
          match(CEQL_QUERYParser::K_IN);
          break;
        }

        case CEQL_QUERYParser::K_NOT: {
          setState(232);
          match(CEQL_QUERYParser::K_NOT);
          setState(233);
          match(CEQL_QUERYParser::K_IN);
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(236);
      value_seq();
      break;
    }

    default:
      break;
    }
    _ctx->stop = _input->LT(-1);
    setState(248);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 23, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(246);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 22, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<And_predicateContext>(_tracker.createInstance<PredicateContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RulePredicate);
          setState(240);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(241);
          match(CEQL_QUERYParser::K_AND);
          setState(242);
          predicate(5);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<Or_predicateContext>(_tracker.createInstance<PredicateContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RulePredicate);
          setState(243);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(244);
          match(CEQL_QUERYParser::K_OR);
          setState(245);
          predicate(4);
          break;
        }

        default:
          break;
        } 
      }
      setState(250);
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

CEQL_QUERYParser::String_literalContext::String_literalContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQL_QUERYParser::StringContext* CEQL_QUERYParser::String_literalContext::string() {
  return getRuleContext<CEQL_QUERYParser::StringContext>(0);
}

CEQL_QUERYParser::Attribute_nameContext* CEQL_QUERYParser::String_literalContext::attribute_name() {
  return getRuleContext<CEQL_QUERYParser::Attribute_nameContext>(0);
}


size_t CEQL_QUERYParser::String_literalContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleString_literal;
}


std::any CEQL_QUERYParser::String_literalContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitString_literal(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::String_literalContext* CEQL_QUERYParser::string_literal() {
  String_literalContext *_localctx = _tracker.createInstance<String_literalContext>(_ctx, getState());
  enterRule(_localctx, 24, CEQL_QUERYParser::RuleString_literal);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(253);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQL_QUERYParser::STRING_LITERAL: {
        enterOuterAlt(_localctx, 1);
        setState(251);
        string();
        break;
      }

      case CEQL_QUERYParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 2);
        setState(252);
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

CEQL_QUERYParser::String_literal_or_regexpContext::String_literal_or_regexpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQL_QUERYParser::StringContext* CEQL_QUERYParser::String_literal_or_regexpContext::string() {
  return getRuleContext<CEQL_QUERYParser::StringContext>(0);
}

CEQL_QUERYParser::Attribute_nameContext* CEQL_QUERYParser::String_literal_or_regexpContext::attribute_name() {
  return getRuleContext<CEQL_QUERYParser::Attribute_nameContext>(0);
}

CEQL_QUERYParser::RegexpContext* CEQL_QUERYParser::String_literal_or_regexpContext::regexp() {
  return getRuleContext<CEQL_QUERYParser::RegexpContext>(0);
}


size_t CEQL_QUERYParser::String_literal_or_regexpContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleString_literal_or_regexp;
}


std::any CEQL_QUERYParser::String_literal_or_regexpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitString_literal_or_regexp(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::String_literal_or_regexpContext* CEQL_QUERYParser::string_literal_or_regexp() {
  String_literal_or_regexpContext *_localctx = _tracker.createInstance<String_literal_or_regexpContext>(_ctx, getState());
  enterRule(_localctx, 26, CEQL_QUERYParser::RuleString_literal_or_regexp);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(258);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 25, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(255);
      string();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(256);
      attribute_name();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(257);
      regexp();
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

//----------------- RegexpContext ------------------------------------------------------------------

CEQL_QUERYParser::RegexpContext::RegexpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CEQL_QUERYParser::RegexpContext::STRING_LITERAL() {
  return getToken(CEQL_QUERYParser::STRING_LITERAL, 0);
}


size_t CEQL_QUERYParser::RegexpContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleRegexp;
}


std::any CEQL_QUERYParser::RegexpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitRegexp(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::RegexpContext* CEQL_QUERYParser::regexp() {
  RegexpContext *_localctx = _tracker.createInstance<RegexpContext>(_ctx, getState());
  enterRule(_localctx, 28, CEQL_QUERYParser::RuleRegexp);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(260);
    match(CEQL_QUERYParser::STRING_LITERAL);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Math_exprContext ------------------------------------------------------------------

CEQL_QUERYParser::Math_exprContext::Math_exprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CEQL_QUERYParser::Math_exprContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleMath_expr;
}

void CEQL_QUERYParser::Math_exprContext::copyFrom(Math_exprContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Mul_math_exprContext ------------------------------------------------------------------

std::vector<CEQL_QUERYParser::Math_exprContext *> CEQL_QUERYParser::Mul_math_exprContext::math_expr() {
  return getRuleContexts<CEQL_QUERYParser::Math_exprContext>();
}

CEQL_QUERYParser::Math_exprContext* CEQL_QUERYParser::Mul_math_exprContext::math_expr(size_t i) {
  return getRuleContext<CEQL_QUERYParser::Math_exprContext>(i);
}

tree::TerminalNode* CEQL_QUERYParser::Mul_math_exprContext::STAR() {
  return getToken(CEQL_QUERYParser::STAR, 0);
}

tree::TerminalNode* CEQL_QUERYParser::Mul_math_exprContext::SLASH() {
  return getToken(CEQL_QUERYParser::SLASH, 0);
}

tree::TerminalNode* CEQL_QUERYParser::Mul_math_exprContext::PERCENT() {
  return getToken(CEQL_QUERYParser::PERCENT, 0);
}

CEQL_QUERYParser::Mul_math_exprContext::Mul_math_exprContext(Math_exprContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Mul_math_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitMul_math_expr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Sum_math_exprContext ------------------------------------------------------------------

std::vector<CEQL_QUERYParser::Math_exprContext *> CEQL_QUERYParser::Sum_math_exprContext::math_expr() {
  return getRuleContexts<CEQL_QUERYParser::Math_exprContext>();
}

CEQL_QUERYParser::Math_exprContext* CEQL_QUERYParser::Sum_math_exprContext::math_expr(size_t i) {
  return getRuleContext<CEQL_QUERYParser::Math_exprContext>(i);
}

tree::TerminalNode* CEQL_QUERYParser::Sum_math_exprContext::PLUS() {
  return getToken(CEQL_QUERYParser::PLUS, 0);
}

tree::TerminalNode* CEQL_QUERYParser::Sum_math_exprContext::MINUS() {
  return getToken(CEQL_QUERYParser::MINUS, 0);
}

CEQL_QUERYParser::Sum_math_exprContext::Sum_math_exprContext(Math_exprContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Sum_math_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitSum_math_expr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Number_math_exprContext ------------------------------------------------------------------

CEQL_QUERYParser::NumberContext* CEQL_QUERYParser::Number_math_exprContext::number() {
  return getRuleContext<CEQL_QUERYParser::NumberContext>(0);
}

CEQL_QUERYParser::Number_math_exprContext::Number_math_exprContext(Math_exprContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Number_math_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitNumber_math_expr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Unary_math_exprContext ------------------------------------------------------------------

CEQL_QUERYParser::Math_exprContext* CEQL_QUERYParser::Unary_math_exprContext::math_expr() {
  return getRuleContext<CEQL_QUERYParser::Math_exprContext>(0);
}

tree::TerminalNode* CEQL_QUERYParser::Unary_math_exprContext::PLUS() {
  return getToken(CEQL_QUERYParser::PLUS, 0);
}

tree::TerminalNode* CEQL_QUERYParser::Unary_math_exprContext::MINUS() {
  return getToken(CEQL_QUERYParser::MINUS, 0);
}

CEQL_QUERYParser::Unary_math_exprContext::Unary_math_exprContext(Math_exprContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Unary_math_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitUnary_math_expr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Attribute_math_exprContext ------------------------------------------------------------------

CEQL_QUERYParser::Attribute_nameContext* CEQL_QUERYParser::Attribute_math_exprContext::attribute_name() {
  return getRuleContext<CEQL_QUERYParser::Attribute_nameContext>(0);
}

CEQL_QUERYParser::Attribute_math_exprContext::Attribute_math_exprContext(Math_exprContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Attribute_math_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitAttribute_math_expr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Par_math_exprContext ------------------------------------------------------------------

CEQL_QUERYParser::Math_exprContext* CEQL_QUERYParser::Par_math_exprContext::math_expr() {
  return getRuleContext<CEQL_QUERYParser::Math_exprContext>(0);
}

CEQL_QUERYParser::Par_math_exprContext::Par_math_exprContext(Math_exprContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Par_math_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitPar_math_expr(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::Math_exprContext* CEQL_QUERYParser::math_expr() {
   return math_expr(0);
}

CEQL_QUERYParser::Math_exprContext* CEQL_QUERYParser::math_expr(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  CEQL_QUERYParser::Math_exprContext *_localctx = _tracker.createInstance<Math_exprContext>(_ctx, parentState);
  CEQL_QUERYParser::Math_exprContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 30;
  enterRecursionRule(_localctx, 30, CEQL_QUERYParser::RuleMath_expr, precedence);

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
    setState(271);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQL_QUERYParser::T__1: {
        _localctx = _tracker.createInstance<Par_math_exprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(263);
        match(CEQL_QUERYParser::T__1);
        setState(264);
        math_expr(0);
        setState(265);
        match(CEQL_QUERYParser::T__2);
        break;
      }

      case CEQL_QUERYParser::DOUBLE_LITERAL:
      case CEQL_QUERYParser::INTEGER_LITERAL: {
        _localctx = _tracker.createInstance<Number_math_exprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(267);
        number();
        break;
      }

      case CEQL_QUERYParser::IDENTIFIER: {
        _localctx = _tracker.createInstance<Attribute_math_exprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(268);
        attribute_name();
        break;
      }

      case CEQL_QUERYParser::PLUS:
      case CEQL_QUERYParser::MINUS: {
        _localctx = _tracker.createInstance<Unary_math_exprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(269);
        _la = _input->LA(1);
        if (!(_la == CEQL_QUERYParser::PLUS

        || _la == CEQL_QUERYParser::MINUS)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(270);
        math_expr(3);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(281);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 28, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(279);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 27, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<Mul_math_exprContext>(_tracker.createInstance<Math_exprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleMath_expr);
          setState(273);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(274);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & 6871947673600) != 0))) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(275);
          math_expr(3);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<Sum_math_exprContext>(_tracker.createInstance<Math_exprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleMath_expr);
          setState(276);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(277);
          _la = _input->LA(1);
          if (!(_la == CEQL_QUERYParser::PLUS

          || _la == CEQL_QUERYParser::MINUS)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(278);
          math_expr(2);
          break;
        }

        default:
          break;
        } 
      }
      setState(283);
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

CEQL_QUERYParser::Value_seqContext::Value_seqContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQL_QUERYParser::Number_seqContext* CEQL_QUERYParser::Value_seqContext::number_seq() {
  return getRuleContext<CEQL_QUERYParser::Number_seqContext>(0);
}

CEQL_QUERYParser::String_seqContext* CEQL_QUERYParser::Value_seqContext::string_seq() {
  return getRuleContext<CEQL_QUERYParser::String_seqContext>(0);
}


size_t CEQL_QUERYParser::Value_seqContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleValue_seq;
}


std::any CEQL_QUERYParser::Value_seqContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitValue_seq(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::Value_seqContext* CEQL_QUERYParser::value_seq() {
  Value_seqContext *_localctx = _tracker.createInstance<Value_seqContext>(_ctx, getState());
  enterRule(_localctx, 32, CEQL_QUERYParser::RuleValue_seq);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(292);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 29, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(284);
      match(CEQL_QUERYParser::T__5);
      setState(285);
      number_seq();
      setState(286);
      match(CEQL_QUERYParser::T__6);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(288);
      match(CEQL_QUERYParser::T__5);
      setState(289);
      string_seq();
      setState(290);
      match(CEQL_QUERYParser::T__6);
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

CEQL_QUERYParser::Number_seqContext::Number_seqContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CEQL_QUERYParser::Number_seqContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleNumber_seq;
}

void CEQL_QUERYParser::Number_seqContext::copyFrom(Number_seqContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Number_listContext ------------------------------------------------------------------

std::vector<CEQL_QUERYParser::NumberContext *> CEQL_QUERYParser::Number_listContext::number() {
  return getRuleContexts<CEQL_QUERYParser::NumberContext>();
}

CEQL_QUERYParser::NumberContext* CEQL_QUERYParser::Number_listContext::number(size_t i) {
  return getRuleContext<CEQL_QUERYParser::NumberContext>(i);
}

CEQL_QUERYParser::Number_listContext::Number_listContext(Number_seqContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Number_listContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitNumber_list(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Double_rangeContext ------------------------------------------------------------------

std::vector<CEQL_QUERYParser::DoubleContext *> CEQL_QUERYParser::Double_rangeContext::double_() {
  return getRuleContexts<CEQL_QUERYParser::DoubleContext>();
}

CEQL_QUERYParser::DoubleContext* CEQL_QUERYParser::Double_rangeContext::double_(size_t i) {
  return getRuleContext<CEQL_QUERYParser::DoubleContext>(i);
}

CEQL_QUERYParser::Double_rangeContext::Double_rangeContext(Number_seqContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Double_rangeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitDouble_range(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Number_range_lowerContext ------------------------------------------------------------------

CEQL_QUERYParser::NumberContext* CEQL_QUERYParser::Number_range_lowerContext::number() {
  return getRuleContext<CEQL_QUERYParser::NumberContext>(0);
}

CEQL_QUERYParser::Number_range_lowerContext::Number_range_lowerContext(Number_seqContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Number_range_lowerContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitNumber_range_lower(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Number_range_upperContext ------------------------------------------------------------------

CEQL_QUERYParser::NumberContext* CEQL_QUERYParser::Number_range_upperContext::number() {
  return getRuleContext<CEQL_QUERYParser::NumberContext>(0);
}

CEQL_QUERYParser::Number_range_upperContext::Number_range_upperContext(Number_seqContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Number_range_upperContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitNumber_range_upper(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Integer_rangeContext ------------------------------------------------------------------

std::vector<CEQL_QUERYParser::IntegerContext *> CEQL_QUERYParser::Integer_rangeContext::integer() {
  return getRuleContexts<CEQL_QUERYParser::IntegerContext>();
}

CEQL_QUERYParser::IntegerContext* CEQL_QUERYParser::Integer_rangeContext::integer(size_t i) {
  return getRuleContext<CEQL_QUERYParser::IntegerContext>(i);
}

CEQL_QUERYParser::Integer_rangeContext::Integer_rangeContext(Number_seqContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Integer_rangeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitInteger_range(this);
  else
    return visitor->visitChildren(this);
}
CEQL_QUERYParser::Number_seqContext* CEQL_QUERYParser::number_seq() {
  Number_seqContext *_localctx = _tracker.createInstance<Number_seqContext>(_ctx, getState());
  enterRule(_localctx, 34, CEQL_QUERYParser::RuleNumber_seq);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(315);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 31, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<CEQL_QUERYParser::Number_listContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(294);
      number();
      setState(299);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == CEQL_QUERYParser::T__0) {
        setState(295);
        match(CEQL_QUERYParser::T__0);
        setState(296);
        number();
        setState(301);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<CEQL_QUERYParser::Integer_rangeContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(302);
      integer();
      setState(303);
      match(CEQL_QUERYParser::T__7);
      setState(304);
      integer();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<CEQL_QUERYParser::Double_rangeContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(306);
      double_();
      setState(307);
      match(CEQL_QUERYParser::T__7);
      setState(308);
      double_();
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<CEQL_QUERYParser::Number_range_lowerContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(310);
      number();
      setState(311);
      match(CEQL_QUERYParser::T__7);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<CEQL_QUERYParser::Number_range_upperContext>(_localctx);
      enterOuterAlt(_localctx, 5);
      setState(313);
      match(CEQL_QUERYParser::T__7);
      setState(314);
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

CEQL_QUERYParser::String_seqContext::String_seqContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<CEQL_QUERYParser::StringContext *> CEQL_QUERYParser::String_seqContext::string() {
  return getRuleContexts<CEQL_QUERYParser::StringContext>();
}

CEQL_QUERYParser::StringContext* CEQL_QUERYParser::String_seqContext::string(size_t i) {
  return getRuleContext<CEQL_QUERYParser::StringContext>(i);
}


size_t CEQL_QUERYParser::String_seqContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleString_seq;
}


std::any CEQL_QUERYParser::String_seqContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitString_seq(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::String_seqContext* CEQL_QUERYParser::string_seq() {
  String_seqContext *_localctx = _tracker.createInstance<String_seqContext>(_ctx, getState());
  enterRule(_localctx, 36, CEQL_QUERYParser::RuleString_seq);
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
    setState(317);
    string();
    setState(322);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CEQL_QUERYParser::T__0) {
      setState(318);
      match(CEQL_QUERYParser::T__0);
      setState(319);
      string();
      setState(324);
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

CEQL_QUERYParser::Time_windowContext::Time_windowContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQL_QUERYParser::Event_spanContext* CEQL_QUERYParser::Time_windowContext::event_span() {
  return getRuleContext<CEQL_QUERYParser::Event_spanContext>(0);
}

CEQL_QUERYParser::Time_spanContext* CEQL_QUERYParser::Time_windowContext::time_span() {
  return getRuleContext<CEQL_QUERYParser::Time_spanContext>(0);
}

CEQL_QUERYParser::Custom_spanContext* CEQL_QUERYParser::Time_windowContext::custom_span() {
  return getRuleContext<CEQL_QUERYParser::Custom_spanContext>(0);
}


size_t CEQL_QUERYParser::Time_windowContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleTime_window;
}


std::any CEQL_QUERYParser::Time_windowContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitTime_window(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::Time_windowContext* CEQL_QUERYParser::time_window() {
  Time_windowContext *_localctx = _tracker.createInstance<Time_windowContext>(_ctx, getState());
  enterRule(_localctx, 38, CEQL_QUERYParser::RuleTime_window);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(328);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 33, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(325);
      event_span();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(326);
      time_span();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(327);
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

CEQL_QUERYParser::Event_spanContext::Event_spanContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQL_QUERYParser::IntegerContext* CEQL_QUERYParser::Event_spanContext::integer() {
  return getRuleContext<CEQL_QUERYParser::IntegerContext>(0);
}

tree::TerminalNode* CEQL_QUERYParser::Event_spanContext::K_EVENTS() {
  return getToken(CEQL_QUERYParser::K_EVENTS, 0);
}


size_t CEQL_QUERYParser::Event_spanContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleEvent_span;
}


std::any CEQL_QUERYParser::Event_spanContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitEvent_span(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::Event_spanContext* CEQL_QUERYParser::event_span() {
  Event_spanContext *_localctx = _tracker.createInstance<Event_spanContext>(_ctx, getState());
  enterRule(_localctx, 40, CEQL_QUERYParser::RuleEvent_span);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(330);
    integer();
    setState(331);
    match(CEQL_QUERYParser::K_EVENTS);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Time_spanContext ------------------------------------------------------------------

CEQL_QUERYParser::Time_spanContext::Time_spanContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQL_QUERYParser::Hour_spanContext* CEQL_QUERYParser::Time_spanContext::hour_span() {
  return getRuleContext<CEQL_QUERYParser::Hour_spanContext>(0);
}

CEQL_QUERYParser::Minute_spanContext* CEQL_QUERYParser::Time_spanContext::minute_span() {
  return getRuleContext<CEQL_QUERYParser::Minute_spanContext>(0);
}

CEQL_QUERYParser::Second_spanContext* CEQL_QUERYParser::Time_spanContext::second_span() {
  return getRuleContext<CEQL_QUERYParser::Second_spanContext>(0);
}


size_t CEQL_QUERYParser::Time_spanContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleTime_span;
}


std::any CEQL_QUERYParser::Time_spanContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitTime_span(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::Time_spanContext* CEQL_QUERYParser::time_span() {
  Time_spanContext *_localctx = _tracker.createInstance<Time_spanContext>(_ctx, getState());
  enterRule(_localctx, 42, CEQL_QUERYParser::RuleTime_span);
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
    setState(334);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 34, _ctx)) {
    case 1: {
      setState(333);
      hour_span();
      break;
    }

    default:
      break;
    }
    setState(337);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 35, _ctx)) {
    case 1: {
      setState(336);
      minute_span();
      break;
    }

    default:
      break;
    }
    setState(340);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQL_QUERYParser::INTEGER_LITERAL) {
      setState(339);
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

CEQL_QUERYParser::Hour_spanContext::Hour_spanContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQL_QUERYParser::IntegerContext* CEQL_QUERYParser::Hour_spanContext::integer() {
  return getRuleContext<CEQL_QUERYParser::IntegerContext>(0);
}

tree::TerminalNode* CEQL_QUERYParser::Hour_spanContext::K_HOURS() {
  return getToken(CEQL_QUERYParser::K_HOURS, 0);
}


size_t CEQL_QUERYParser::Hour_spanContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleHour_span;
}


std::any CEQL_QUERYParser::Hour_spanContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitHour_span(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::Hour_spanContext* CEQL_QUERYParser::hour_span() {
  Hour_spanContext *_localctx = _tracker.createInstance<Hour_spanContext>(_ctx, getState());
  enterRule(_localctx, 44, CEQL_QUERYParser::RuleHour_span);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(342);
    integer();
    setState(343);
    match(CEQL_QUERYParser::K_HOURS);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Minute_spanContext ------------------------------------------------------------------

CEQL_QUERYParser::Minute_spanContext::Minute_spanContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQL_QUERYParser::IntegerContext* CEQL_QUERYParser::Minute_spanContext::integer() {
  return getRuleContext<CEQL_QUERYParser::IntegerContext>(0);
}

tree::TerminalNode* CEQL_QUERYParser::Minute_spanContext::K_MINUTES() {
  return getToken(CEQL_QUERYParser::K_MINUTES, 0);
}


size_t CEQL_QUERYParser::Minute_spanContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleMinute_span;
}


std::any CEQL_QUERYParser::Minute_spanContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitMinute_span(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::Minute_spanContext* CEQL_QUERYParser::minute_span() {
  Minute_spanContext *_localctx = _tracker.createInstance<Minute_spanContext>(_ctx, getState());
  enterRule(_localctx, 46, CEQL_QUERYParser::RuleMinute_span);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(345);
    integer();
    setState(346);
    match(CEQL_QUERYParser::K_MINUTES);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Second_spanContext ------------------------------------------------------------------

CEQL_QUERYParser::Second_spanContext::Second_spanContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQL_QUERYParser::IntegerContext* CEQL_QUERYParser::Second_spanContext::integer() {
  return getRuleContext<CEQL_QUERYParser::IntegerContext>(0);
}

tree::TerminalNode* CEQL_QUERYParser::Second_spanContext::K_SECONDS() {
  return getToken(CEQL_QUERYParser::K_SECONDS, 0);
}


size_t CEQL_QUERYParser::Second_spanContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleSecond_span;
}


std::any CEQL_QUERYParser::Second_spanContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitSecond_span(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::Second_spanContext* CEQL_QUERYParser::second_span() {
  Second_spanContext *_localctx = _tracker.createInstance<Second_spanContext>(_ctx, getState());
  enterRule(_localctx, 48, CEQL_QUERYParser::RuleSecond_span);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(348);
    integer();
    setState(349);
    match(CEQL_QUERYParser::K_SECONDS);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Custom_spanContext ------------------------------------------------------------------

CEQL_QUERYParser::Custom_spanContext::Custom_spanContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQL_QUERYParser::IntegerContext* CEQL_QUERYParser::Custom_spanContext::integer() {
  return getRuleContext<CEQL_QUERYParser::IntegerContext>(0);
}

CEQL_QUERYParser::Any_nameContext* CEQL_QUERYParser::Custom_spanContext::any_name() {
  return getRuleContext<CEQL_QUERYParser::Any_nameContext>(0);
}


size_t CEQL_QUERYParser::Custom_spanContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleCustom_span;
}


std::any CEQL_QUERYParser::Custom_spanContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitCustom_span(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::Custom_spanContext* CEQL_QUERYParser::custom_span() {
  Custom_spanContext *_localctx = _tracker.createInstance<Custom_spanContext>(_ctx, getState());
  enterRule(_localctx, 50, CEQL_QUERYParser::RuleCustom_span);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(351);
    integer();
    setState(352);
    match(CEQL_QUERYParser::T__3);
    setState(353);
    any_name();
    setState(354);
    match(CEQL_QUERYParser::T__4);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Named_eventContext ------------------------------------------------------------------

CEQL_QUERYParser::Named_eventContext::Named_eventContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQL_QUERYParser::S_event_nameContext* CEQL_QUERYParser::Named_eventContext::s_event_name() {
  return getRuleContext<CEQL_QUERYParser::S_event_nameContext>(0);
}

tree::TerminalNode* CEQL_QUERYParser::Named_eventContext::K_AS() {
  return getToken(CEQL_QUERYParser::K_AS, 0);
}

CEQL_QUERYParser::Event_nameContext* CEQL_QUERYParser::Named_eventContext::event_name() {
  return getRuleContext<CEQL_QUERYParser::Event_nameContext>(0);
}


size_t CEQL_QUERYParser::Named_eventContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleNamed_event;
}


std::any CEQL_QUERYParser::Named_eventContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitNamed_event(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::Named_eventContext* CEQL_QUERYParser::named_event() {
  Named_eventContext *_localctx = _tracker.createInstance<Named_eventContext>(_ctx, getState());
  enterRule(_localctx, 52, CEQL_QUERYParser::RuleNamed_event);
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
    setState(356);
    s_event_name();
    setState(359);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQL_QUERYParser::K_AS) {
      setState(357);
      match(CEQL_QUERYParser::K_AS);
      setState(358);
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

CEQL_QUERYParser::S_event_nameContext::S_event_nameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQL_QUERYParser::Event_nameContext* CEQL_QUERYParser::S_event_nameContext::event_name() {
  return getRuleContext<CEQL_QUERYParser::Event_nameContext>(0);
}

CEQL_QUERYParser::Stream_nameContext* CEQL_QUERYParser::S_event_nameContext::stream_name() {
  return getRuleContext<CEQL_QUERYParser::Stream_nameContext>(0);
}

tree::TerminalNode* CEQL_QUERYParser::S_event_nameContext::GE() {
  return getToken(CEQL_QUERYParser::GE, 0);
}


size_t CEQL_QUERYParser::S_event_nameContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleS_event_name;
}


std::any CEQL_QUERYParser::S_event_nameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitS_event_name(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::S_event_nameContext* CEQL_QUERYParser::s_event_name() {
  S_event_nameContext *_localctx = _tracker.createInstance<S_event_nameContext>(_ctx, getState());
  enterRule(_localctx, 54, CEQL_QUERYParser::RuleS_event_name);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(364);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 38, _ctx)) {
    case 1: {
      setState(361);
      stream_name();
      setState(362);
      match(CEQL_QUERYParser::GE);
      break;
    }

    default:
      break;
    }
    setState(366);
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

CEQL_QUERYParser::Event_nameContext::Event_nameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQL_QUERYParser::Any_nameContext* CEQL_QUERYParser::Event_nameContext::any_name() {
  return getRuleContext<CEQL_QUERYParser::Any_nameContext>(0);
}


size_t CEQL_QUERYParser::Event_nameContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleEvent_name;
}


std::any CEQL_QUERYParser::Event_nameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitEvent_name(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::Event_nameContext* CEQL_QUERYParser::event_name() {
  Event_nameContext *_localctx = _tracker.createInstance<Event_nameContext>(_ctx, getState());
  enterRule(_localctx, 56, CEQL_QUERYParser::RuleEvent_name);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(368);
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

CEQL_QUERYParser::Stream_nameContext::Stream_nameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQL_QUERYParser::Any_nameContext* CEQL_QUERYParser::Stream_nameContext::any_name() {
  return getRuleContext<CEQL_QUERYParser::Any_nameContext>(0);
}


size_t CEQL_QUERYParser::Stream_nameContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleStream_name;
}


std::any CEQL_QUERYParser::Stream_nameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitStream_name(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::Stream_nameContext* CEQL_QUERYParser::stream_name() {
  Stream_nameContext *_localctx = _tracker.createInstance<Stream_nameContext>(_ctx, getState());
  enterRule(_localctx, 58, CEQL_QUERYParser::RuleStream_name);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(370);
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

CEQL_QUERYParser::Attribute_nameContext::Attribute_nameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQL_QUERYParser::Any_nameContext* CEQL_QUERYParser::Attribute_nameContext::any_name() {
  return getRuleContext<CEQL_QUERYParser::Any_nameContext>(0);
}


size_t CEQL_QUERYParser::Attribute_nameContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleAttribute_name;
}


std::any CEQL_QUERYParser::Attribute_nameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitAttribute_name(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::Attribute_nameContext* CEQL_QUERYParser::attribute_name() {
  Attribute_nameContext *_localctx = _tracker.createInstance<Attribute_nameContext>(_ctx, getState());
  enterRule(_localctx, 60, CEQL_QUERYParser::RuleAttribute_name);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(372);
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

CEQL_QUERYParser::IntegerContext::IntegerContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CEQL_QUERYParser::IntegerContext::INTEGER_LITERAL() {
  return getToken(CEQL_QUERYParser::INTEGER_LITERAL, 0);
}


size_t CEQL_QUERYParser::IntegerContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleInteger;
}


std::any CEQL_QUERYParser::IntegerContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitInteger(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::IntegerContext* CEQL_QUERYParser::integer() {
  IntegerContext *_localctx = _tracker.createInstance<IntegerContext>(_ctx, getState());
  enterRule(_localctx, 62, CEQL_QUERYParser::RuleInteger);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(374);
    match(CEQL_QUERYParser::INTEGER_LITERAL);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DoubleContext ------------------------------------------------------------------

CEQL_QUERYParser::DoubleContext::DoubleContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CEQL_QUERYParser::DoubleContext::DOUBLE_LITERAL() {
  return getToken(CEQL_QUERYParser::DOUBLE_LITERAL, 0);
}


size_t CEQL_QUERYParser::DoubleContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleDouble;
}


std::any CEQL_QUERYParser::DoubleContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitDouble(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::DoubleContext* CEQL_QUERYParser::double_() {
  DoubleContext *_localctx = _tracker.createInstance<DoubleContext>(_ctx, getState());
  enterRule(_localctx, 64, CEQL_QUERYParser::RuleDouble);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(376);
    match(CEQL_QUERYParser::DOUBLE_LITERAL);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NumberContext ------------------------------------------------------------------

CEQL_QUERYParser::NumberContext::NumberContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CEQL_QUERYParser::IntegerContext* CEQL_QUERYParser::NumberContext::integer() {
  return getRuleContext<CEQL_QUERYParser::IntegerContext>(0);
}

CEQL_QUERYParser::DoubleContext* CEQL_QUERYParser::NumberContext::double_() {
  return getRuleContext<CEQL_QUERYParser::DoubleContext>(0);
}


size_t CEQL_QUERYParser::NumberContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleNumber;
}


std::any CEQL_QUERYParser::NumberContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitNumber(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::NumberContext* CEQL_QUERYParser::number() {
  NumberContext *_localctx = _tracker.createInstance<NumberContext>(_ctx, getState());
  enterRule(_localctx, 66, CEQL_QUERYParser::RuleNumber);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(380);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQL_QUERYParser::INTEGER_LITERAL: {
        enterOuterAlt(_localctx, 1);
        setState(378);
        integer();
        break;
      }

      case CEQL_QUERYParser::DOUBLE_LITERAL: {
        enterOuterAlt(_localctx, 2);
        setState(379);
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

CEQL_QUERYParser::StringContext::StringContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CEQL_QUERYParser::StringContext::STRING_LITERAL() {
  return getToken(CEQL_QUERYParser::STRING_LITERAL, 0);
}


size_t CEQL_QUERYParser::StringContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleString;
}


std::any CEQL_QUERYParser::StringContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitString(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::StringContext* CEQL_QUERYParser::string() {
  StringContext *_localctx = _tracker.createInstance<StringContext>(_ctx, getState());
  enterRule(_localctx, 68, CEQL_QUERYParser::RuleString);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(382);
    match(CEQL_QUERYParser::STRING_LITERAL);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Any_nameContext ------------------------------------------------------------------

CEQL_QUERYParser::Any_nameContext::Any_nameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CEQL_QUERYParser::Any_nameContext::IDENTIFIER() {
  return getToken(CEQL_QUERYParser::IDENTIFIER, 0);
}


size_t CEQL_QUERYParser::Any_nameContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleAny_name;
}


std::any CEQL_QUERYParser::Any_nameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitAny_name(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::Any_nameContext* CEQL_QUERYParser::any_name() {
  Any_nameContext *_localctx = _tracker.createInstance<Any_nameContext>(_ctx, getState());
  enterRule(_localctx, 70, CEQL_QUERYParser::RuleAny_name);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(384);
    match(CEQL_QUERYParser::IDENTIFIER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- KeywordContext ------------------------------------------------------------------

CEQL_QUERYParser::KeywordContext::KeywordContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CEQL_QUERYParser::KeywordContext::K_ALL() {
  return getToken(CEQL_QUERYParser::K_ALL, 0);
}

tree::TerminalNode* CEQL_QUERYParser::KeywordContext::K_AND() {
  return getToken(CEQL_QUERYParser::K_AND, 0);
}

tree::TerminalNode* CEQL_QUERYParser::KeywordContext::K_ANY() {
  return getToken(CEQL_QUERYParser::K_ANY, 0);
}

tree::TerminalNode* CEQL_QUERYParser::KeywordContext::K_AS() {
  return getToken(CEQL_QUERYParser::K_AS, 0);
}

tree::TerminalNode* CEQL_QUERYParser::KeywordContext::K_BY() {
  return getToken(CEQL_QUERYParser::K_BY, 0);
}

tree::TerminalNode* CEQL_QUERYParser::KeywordContext::K_CONSUME() {
  return getToken(CEQL_QUERYParser::K_CONSUME, 0);
}

tree::TerminalNode* CEQL_QUERYParser::KeywordContext::K_DISTINCT() {
  return getToken(CEQL_QUERYParser::K_DISTINCT, 0);
}

tree::TerminalNode* CEQL_QUERYParser::KeywordContext::K_EVENT() {
  return getToken(CEQL_QUERYParser::K_EVENT, 0);
}

tree::TerminalNode* CEQL_QUERYParser::KeywordContext::K_EVENTS() {
  return getToken(CEQL_QUERYParser::K_EVENTS, 0);
}

tree::TerminalNode* CEQL_QUERYParser::KeywordContext::K_FILTER() {
  return getToken(CEQL_QUERYParser::K_FILTER, 0);
}

tree::TerminalNode* CEQL_QUERYParser::KeywordContext::K_FROM() {
  return getToken(CEQL_QUERYParser::K_FROM, 0);
}

tree::TerminalNode* CEQL_QUERYParser::KeywordContext::K_HOURS() {
  return getToken(CEQL_QUERYParser::K_HOURS, 0);
}

tree::TerminalNode* CEQL_QUERYParser::KeywordContext::K_IN() {
  return getToken(CEQL_QUERYParser::K_IN, 0);
}

tree::TerminalNode* CEQL_QUERYParser::KeywordContext::K_LAST() {
  return getToken(CEQL_QUERYParser::K_LAST, 0);
}

tree::TerminalNode* CEQL_QUERYParser::KeywordContext::K_LIKE() {
  return getToken(CEQL_QUERYParser::K_LIKE, 0);
}

tree::TerminalNode* CEQL_QUERYParser::KeywordContext::K_MAX() {
  return getToken(CEQL_QUERYParser::K_MAX, 0);
}

tree::TerminalNode* CEQL_QUERYParser::KeywordContext::K_MINUTES() {
  return getToken(CEQL_QUERYParser::K_MINUTES, 0);
}

tree::TerminalNode* CEQL_QUERYParser::KeywordContext::K_NEXT() {
  return getToken(CEQL_QUERYParser::K_NEXT, 0);
}

tree::TerminalNode* CEQL_QUERYParser::KeywordContext::K_NONE() {
  return getToken(CEQL_QUERYParser::K_NONE, 0);
}

tree::TerminalNode* CEQL_QUERYParser::KeywordContext::K_NOT() {
  return getToken(CEQL_QUERYParser::K_NOT, 0);
}

tree::TerminalNode* CEQL_QUERYParser::KeywordContext::K_OR() {
  return getToken(CEQL_QUERYParser::K_OR, 0);
}

tree::TerminalNode* CEQL_QUERYParser::KeywordContext::K_PARTITION() {
  return getToken(CEQL_QUERYParser::K_PARTITION, 0);
}

tree::TerminalNode* CEQL_QUERYParser::KeywordContext::K_SECONDS() {
  return getToken(CEQL_QUERYParser::K_SECONDS, 0);
}

tree::TerminalNode* CEQL_QUERYParser::KeywordContext::K_SELECT() {
  return getToken(CEQL_QUERYParser::K_SELECT, 0);
}

tree::TerminalNode* CEQL_QUERYParser::KeywordContext::K_STREAM() {
  return getToken(CEQL_QUERYParser::K_STREAM, 0);
}

tree::TerminalNode* CEQL_QUERYParser::KeywordContext::K_STRICT() {
  return getToken(CEQL_QUERYParser::K_STRICT, 0);
}

tree::TerminalNode* CEQL_QUERYParser::KeywordContext::K_UNLESS() {
  return getToken(CEQL_QUERYParser::K_UNLESS, 0);
}

tree::TerminalNode* CEQL_QUERYParser::KeywordContext::K_WHERE() {
  return getToken(CEQL_QUERYParser::K_WHERE, 0);
}

tree::TerminalNode* CEQL_QUERYParser::KeywordContext::K_WITHIN() {
  return getToken(CEQL_QUERYParser::K_WITHIN, 0);
}


size_t CEQL_QUERYParser::KeywordContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleKeyword;
}


std::any CEQL_QUERYParser::KeywordContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitKeyword(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::KeywordContext* CEQL_QUERYParser::keyword() {
  KeywordContext *_localctx = _tracker.createInstance<KeywordContext>(_ctx, getState());
  enterRule(_localctx, 72, CEQL_QUERYParser::RuleKeyword);
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
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 274877906432) != 0))) {
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

bool CEQL_QUERYParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 6: return cel_formulaSempred(antlrcpp::downCast<Cel_formulaContext *>(context), predicateIndex);
    case 10: return filterSempred(antlrcpp::downCast<FilterContext *>(context), predicateIndex);
    case 11: return predicateSempred(antlrcpp::downCast<PredicateContext *>(context), predicateIndex);
    case 15: return math_exprSempred(antlrcpp::downCast<Math_exprContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool CEQL_QUERYParser::cel_formulaSempred(Cel_formulaContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 3);
    case 1: return precpred(_ctx, 2);
    case 2: return precpred(_ctx, 5);
    case 3: return precpred(_ctx, 4);
    case 4: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

bool CEQL_QUERYParser::filterSempred(FilterContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 5: return precpred(_ctx, 2);
    case 6: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

bool CEQL_QUERYParser::predicateSempred(PredicateContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 7: return precpred(_ctx, 4);
    case 8: return precpred(_ctx, 3);

  default:
    break;
  }
  return true;
}

bool CEQL_QUERYParser::math_exprSempred(Math_exprContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 9: return precpred(_ctx, 2);
    case 10: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

void CEQL_QUERYParser::initialize() {
  ::antlr4::internal::call_once(ceql_queryParserOnceFlag, ceql_queryParserInitialize);
}
