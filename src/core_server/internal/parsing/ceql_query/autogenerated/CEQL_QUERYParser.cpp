
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
      "parse", "error", "core_query", "selection_strategy", "result_values", 
      "core_pattern", "partition_list", "attribute_list", "consumption_policy", 
      "filter", "bool_expr", "string_literal", "string_literal_or_regexp", 
      "math_expr", "value_seq", "number_seq", "string_seq", "time_window", 
      "event_span", "time_span", "hour_span", "minute_span", "second_span", 
      "custom_span", "named_event", "s_event_name", "event_name", "stream_name", 
      "attribute_name", "integer", "number", "string", "any_name", "keyword"
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
      "NEQ", "SEMICOLON", "IDENTIFIER", "FLOAT_LITERAL", "INTEGER_LITERAL", 
      "NUMERICAL_EXPONENT", "STRING_LITERAL", "REGEXP", "SINGLE_LINE_COMMENT", 
      "MULTILINE_COMMENT", "SPACES", "UNEXPECTED_CHAR"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,59,373,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,1,0,1,0,5,0,71,8,
  	0,10,0,12,0,74,9,0,1,0,1,0,1,1,1,1,1,1,1,2,1,2,3,2,83,8,2,1,2,1,2,1,2,
  	1,2,1,2,5,2,90,8,2,10,2,12,2,93,9,2,3,2,95,8,2,1,2,1,2,1,2,1,2,1,2,3,
  	2,102,8,2,1,2,1,2,3,2,106,8,2,1,2,1,2,1,2,3,2,111,8,2,1,3,1,3,1,3,1,3,
  	1,3,1,3,3,3,119,8,3,1,4,1,4,1,4,1,4,5,4,125,8,4,10,4,12,4,128,9,4,3,4,
  	130,8,4,1,5,1,5,1,5,1,5,1,5,1,5,3,5,138,8,5,1,5,1,5,1,5,1,5,1,5,1,5,1,
  	5,1,5,1,5,1,5,1,5,5,5,151,8,5,10,5,12,5,154,9,5,1,6,1,6,1,6,1,6,1,6,1,
  	6,1,6,1,6,3,6,164,8,6,1,7,1,7,1,7,5,7,169,8,7,10,7,12,7,172,9,7,1,8,1,
  	8,1,8,3,8,177,8,8,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,3,9,189,8,9,
  	1,9,1,9,1,9,1,9,1,9,1,9,5,9,197,8,9,10,9,12,9,200,9,9,1,10,1,10,1,10,
  	1,10,1,10,1,10,1,10,1,10,1,10,1,10,1,10,1,10,1,10,1,10,1,10,1,10,1,10,
  	1,10,1,10,1,10,1,10,1,10,1,10,3,10,225,8,10,1,10,1,10,1,10,1,10,1,10,
  	1,10,3,10,233,8,10,1,10,1,10,1,10,1,10,1,10,1,10,5,10,241,8,10,10,10,
  	12,10,244,9,10,1,11,1,11,3,11,248,8,11,1,12,1,12,1,12,3,12,253,8,12,1,
  	13,1,13,1,13,1,13,1,13,1,13,1,13,1,13,1,13,3,13,264,8,13,1,13,1,13,1,
  	13,1,13,1,13,1,13,5,13,272,8,13,10,13,12,13,275,9,13,1,14,1,14,1,14,1,
  	14,1,14,1,14,1,14,1,14,3,14,285,8,14,1,15,1,15,1,15,5,15,290,8,15,10,
  	15,12,15,293,9,15,1,15,1,15,1,15,1,15,1,15,1,15,1,15,1,15,1,15,3,15,304,
  	8,15,1,16,1,16,1,16,5,16,309,8,16,10,16,12,16,312,9,16,1,17,1,17,1,17,
  	3,17,317,8,17,1,18,1,18,1,18,1,19,3,19,323,8,19,1,19,3,19,326,8,19,1,
  	19,3,19,329,8,19,1,20,1,20,1,20,1,21,1,21,1,21,1,22,1,22,1,22,1,23,1,
  	23,1,23,1,23,1,23,1,24,1,24,1,24,3,24,348,8,24,1,25,1,25,1,25,3,25,353,
  	8,25,1,25,1,25,1,26,1,26,1,27,1,27,1,28,1,28,1,29,1,29,1,30,1,30,1,31,
  	1,31,1,32,1,32,1,33,1,33,1,33,0,4,10,18,20,26,34,0,2,4,6,8,10,12,14,16,
  	18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,
  	64,66,0,7,2,0,29,29,49,49,1,0,43,46,1,0,47,48,1,0,39,40,2,0,38,38,41,
  	42,1,0,51,52,1,0,9,37,395,0,72,1,0,0,0,2,77,1,0,0,0,4,80,1,0,0,0,6,118,
  	1,0,0,0,8,129,1,0,0,0,10,137,1,0,0,0,12,155,1,0,0,0,14,165,1,0,0,0,16,
  	176,1,0,0,0,18,188,1,0,0,0,20,232,1,0,0,0,22,247,1,0,0,0,24,252,1,0,0,
  	0,26,263,1,0,0,0,28,284,1,0,0,0,30,303,1,0,0,0,32,305,1,0,0,0,34,316,
  	1,0,0,0,36,318,1,0,0,0,38,322,1,0,0,0,40,330,1,0,0,0,42,333,1,0,0,0,44,
  	336,1,0,0,0,46,339,1,0,0,0,48,344,1,0,0,0,50,352,1,0,0,0,52,356,1,0,0,
  	0,54,358,1,0,0,0,56,360,1,0,0,0,58,362,1,0,0,0,60,364,1,0,0,0,62,366,
  	1,0,0,0,64,368,1,0,0,0,66,370,1,0,0,0,68,71,3,4,2,0,69,71,3,2,1,0,70,
  	68,1,0,0,0,70,69,1,0,0,0,71,74,1,0,0,0,72,70,1,0,0,0,72,73,1,0,0,0,73,
  	75,1,0,0,0,74,72,1,0,0,0,75,76,5,0,0,1,76,1,1,0,0,0,77,78,5,59,0,0,78,
  	79,6,1,-1,0,79,3,1,0,0,0,80,82,5,32,0,0,81,83,3,6,3,0,82,81,1,0,0,0,82,
  	83,1,0,0,0,83,84,1,0,0,0,84,94,3,8,4,0,85,86,5,19,0,0,86,91,3,54,27,0,
  	87,88,5,1,0,0,88,90,3,54,27,0,89,87,1,0,0,0,90,93,1,0,0,0,91,89,1,0,0,
  	0,91,92,1,0,0,0,92,95,1,0,0,0,93,91,1,0,0,0,94,85,1,0,0,0,94,95,1,0,0,
  	0,95,96,1,0,0,0,96,97,5,36,0,0,97,101,3,10,5,0,98,99,5,30,0,0,99,100,
  	5,13,0,0,100,102,3,12,6,0,101,98,1,0,0,0,101,102,1,0,0,0,102,105,1,0,
  	0,0,103,104,5,37,0,0,104,106,3,34,17,0,105,103,1,0,0,0,105,106,1,0,0,
  	0,106,110,1,0,0,0,107,108,5,14,0,0,108,109,5,13,0,0,109,111,3,16,8,0,
  	110,107,1,0,0,0,110,111,1,0,0,0,111,5,1,0,0,0,112,119,5,9,0,0,113,119,
  	5,11,0,0,114,119,5,22,0,0,115,119,5,24,0,0,116,119,5,26,0,0,117,119,5,
  	34,0,0,118,112,1,0,0,0,118,113,1,0,0,0,118,114,1,0,0,0,118,115,1,0,0,
  	0,118,116,1,0,0,0,118,117,1,0,0,0,119,7,1,0,0,0,120,130,5,41,0,0,121,
  	126,3,50,25,0,122,123,5,1,0,0,123,125,3,50,25,0,124,122,1,0,0,0,125,128,
  	1,0,0,0,126,124,1,0,0,0,126,127,1,0,0,0,127,130,1,0,0,0,128,126,1,0,0,
  	0,129,120,1,0,0,0,129,121,1,0,0,0,130,9,1,0,0,0,131,132,6,5,-1,0,132,
  	133,5,2,0,0,133,134,3,10,5,0,134,135,5,3,0,0,135,138,1,0,0,0,136,138,
  	3,50,25,0,137,131,1,0,0,0,137,136,1,0,0,0,138,152,1,0,0,0,139,140,10,
  	2,0,0,140,141,7,0,0,0,141,151,3,10,5,3,142,143,10,4,0,0,143,144,5,12,
  	0,0,144,151,3,52,26,0,145,146,10,3,0,0,146,151,5,39,0,0,147,148,10,1,
  	0,0,148,149,5,18,0,0,149,151,3,18,9,0,150,139,1,0,0,0,150,142,1,0,0,0,
  	150,145,1,0,0,0,150,147,1,0,0,0,151,154,1,0,0,0,152,150,1,0,0,0,152,153,
  	1,0,0,0,153,11,1,0,0,0,154,152,1,0,0,0,155,156,5,4,0,0,156,157,3,14,7,
  	0,157,163,5,5,0,0,158,159,5,1,0,0,159,160,5,4,0,0,160,161,3,14,7,0,161,
  	162,5,5,0,0,162,164,1,0,0,0,163,158,1,0,0,0,163,164,1,0,0,0,164,13,1,
  	0,0,0,165,170,3,56,28,0,166,167,5,1,0,0,167,169,3,56,28,0,168,166,1,0,
  	0,0,169,172,1,0,0,0,170,168,1,0,0,0,170,171,1,0,0,0,171,15,1,0,0,0,172,
  	170,1,0,0,0,173,177,5,11,0,0,174,177,5,30,0,0,175,177,5,27,0,0,176,173,
  	1,0,0,0,176,174,1,0,0,0,176,175,1,0,0,0,177,17,1,0,0,0,178,179,6,9,-1,
  	0,179,180,5,2,0,0,180,181,3,18,9,0,181,182,5,3,0,0,182,189,1,0,0,0,183,
  	184,3,52,26,0,184,185,5,4,0,0,185,186,3,20,10,0,186,187,5,5,0,0,187,189,
  	1,0,0,0,188,178,1,0,0,0,188,183,1,0,0,0,189,198,1,0,0,0,190,191,10,2,
  	0,0,191,192,5,10,0,0,192,197,3,18,9,3,193,194,10,1,0,0,194,195,5,29,0,
  	0,195,197,3,18,9,2,196,190,1,0,0,0,196,193,1,0,0,0,197,200,1,0,0,0,198,
  	196,1,0,0,0,198,199,1,0,0,0,199,19,1,0,0,0,200,198,1,0,0,0,201,202,6,
  	10,-1,0,202,203,5,2,0,0,203,204,3,20,10,0,204,205,5,3,0,0,205,233,1,0,
  	0,0,206,207,5,28,0,0,207,233,3,20,10,8,208,209,3,26,13,0,209,210,7,1,
  	0,0,210,211,3,26,13,0,211,233,1,0,0,0,212,213,3,26,13,0,213,214,7,2,0,
  	0,214,215,3,26,13,0,215,233,1,0,0,0,216,217,3,56,28,0,217,218,5,23,0,
  	0,218,219,5,55,0,0,219,233,1,0,0,0,220,224,3,56,28,0,221,225,5,21,0,0,
  	222,223,5,28,0,0,223,225,5,21,0,0,224,221,1,0,0,0,224,222,1,0,0,0,225,
  	226,1,0,0,0,226,227,3,28,14,0,227,233,1,0,0,0,228,229,3,22,11,0,229,230,
  	7,2,0,0,230,231,3,24,12,0,231,233,1,0,0,0,232,201,1,0,0,0,232,206,1,0,
  	0,0,232,208,1,0,0,0,232,212,1,0,0,0,232,216,1,0,0,0,232,220,1,0,0,0,232,
  	228,1,0,0,0,233,242,1,0,0,0,234,235,10,5,0,0,235,236,5,10,0,0,236,241,
  	3,20,10,6,237,238,10,4,0,0,238,239,5,29,0,0,239,241,3,20,10,5,240,234,
  	1,0,0,0,240,237,1,0,0,0,241,244,1,0,0,0,242,240,1,0,0,0,242,243,1,0,0,
  	0,243,21,1,0,0,0,244,242,1,0,0,0,245,248,3,62,31,0,246,248,3,56,28,0,
  	247,245,1,0,0,0,247,246,1,0,0,0,248,23,1,0,0,0,249,253,3,62,31,0,250,
  	253,3,56,28,0,251,253,5,55,0,0,252,249,1,0,0,0,252,250,1,0,0,0,252,251,
  	1,0,0,0,253,25,1,0,0,0,254,255,6,13,-1,0,255,256,5,2,0,0,256,257,3,26,
  	13,0,257,258,5,3,0,0,258,264,1,0,0,0,259,264,3,60,30,0,260,264,3,56,28,
  	0,261,262,7,3,0,0,262,264,3,26,13,3,263,254,1,0,0,0,263,259,1,0,0,0,263,
  	260,1,0,0,0,263,261,1,0,0,0,264,273,1,0,0,0,265,266,10,2,0,0,266,267,
  	7,4,0,0,267,272,3,26,13,3,268,269,10,1,0,0,269,270,7,3,0,0,270,272,3,
  	26,13,2,271,265,1,0,0,0,271,268,1,0,0,0,272,275,1,0,0,0,273,271,1,0,0,
  	0,273,274,1,0,0,0,274,27,1,0,0,0,275,273,1,0,0,0,276,277,5,6,0,0,277,
  	278,3,30,15,0,278,279,5,7,0,0,279,285,1,0,0,0,280,281,5,6,0,0,281,282,
  	3,32,16,0,282,283,5,7,0,0,283,285,1,0,0,0,284,276,1,0,0,0,284,280,1,0,
  	0,0,285,29,1,0,0,0,286,291,3,60,30,0,287,288,5,1,0,0,288,290,3,60,30,
  	0,289,287,1,0,0,0,290,293,1,0,0,0,291,289,1,0,0,0,291,292,1,0,0,0,292,
  	304,1,0,0,0,293,291,1,0,0,0,294,295,3,60,30,0,295,296,5,8,0,0,296,297,
  	3,60,30,0,297,304,1,0,0,0,298,299,3,60,30,0,299,300,5,8,0,0,300,304,1,
  	0,0,0,301,302,5,8,0,0,302,304,3,60,30,0,303,286,1,0,0,0,303,294,1,0,0,
  	0,303,298,1,0,0,0,303,301,1,0,0,0,304,31,1,0,0,0,305,310,3,62,31,0,306,
  	307,5,1,0,0,307,309,3,62,31,0,308,306,1,0,0,0,309,312,1,0,0,0,310,308,
  	1,0,0,0,310,311,1,0,0,0,311,33,1,0,0,0,312,310,1,0,0,0,313,317,3,36,18,
  	0,314,317,3,38,19,0,315,317,3,46,23,0,316,313,1,0,0,0,316,314,1,0,0,0,
  	316,315,1,0,0,0,317,35,1,0,0,0,318,319,3,58,29,0,319,320,5,17,0,0,320,
  	37,1,0,0,0,321,323,3,40,20,0,322,321,1,0,0,0,322,323,1,0,0,0,323,325,
  	1,0,0,0,324,326,3,42,21,0,325,324,1,0,0,0,325,326,1,0,0,0,326,328,1,0,
  	0,0,327,329,3,44,22,0,328,327,1,0,0,0,328,329,1,0,0,0,329,39,1,0,0,0,
  	330,331,3,58,29,0,331,332,5,20,0,0,332,41,1,0,0,0,333,334,3,58,29,0,334,
  	335,5,25,0,0,335,43,1,0,0,0,336,337,3,58,29,0,337,338,5,31,0,0,338,45,
  	1,0,0,0,339,340,3,58,29,0,340,341,5,4,0,0,341,342,3,64,32,0,342,343,5,
  	5,0,0,343,47,1,0,0,0,344,347,3,50,25,0,345,346,5,12,0,0,346,348,3,52,
  	26,0,347,345,1,0,0,0,347,348,1,0,0,0,348,49,1,0,0,0,349,350,3,54,27,0,
  	350,351,5,45,0,0,351,353,1,0,0,0,352,349,1,0,0,0,352,353,1,0,0,0,353,
  	354,1,0,0,0,354,355,3,52,26,0,355,51,1,0,0,0,356,357,3,64,32,0,357,53,
  	1,0,0,0,358,359,3,64,32,0,359,55,1,0,0,0,360,361,3,64,32,0,361,57,1,0,
  	0,0,362,363,5,52,0,0,363,59,1,0,0,0,364,365,7,5,0,0,365,61,1,0,0,0,366,
  	367,5,54,0,0,367,63,1,0,0,0,368,369,5,50,0,0,369,65,1,0,0,0,370,371,7,
  	6,0,0,371,67,1,0,0,0,39,70,72,82,91,94,101,105,110,118,126,129,137,150,
  	152,163,170,176,188,196,198,224,232,240,242,247,252,263,271,273,284,291,
  	303,310,316,322,325,328,347,352
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
    setState(72);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CEQL_QUERYParser::K_SELECT

    || _la == CEQL_QUERYParser::UNEXPECTED_CHAR) {
      setState(70);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case CEQL_QUERYParser::K_SELECT: {
          setState(68);
          core_query();
          break;
        }

        case CEQL_QUERYParser::UNEXPECTED_CHAR: {
          setState(69);
          error();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(74);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(75);
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
    setState(77);
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

CEQL_QUERYParser::Result_valuesContext* CEQL_QUERYParser::Core_queryContext::result_values() {
  return getRuleContext<CEQL_QUERYParser::Result_valuesContext>(0);
}

tree::TerminalNode* CEQL_QUERYParser::Core_queryContext::K_WHERE() {
  return getToken(CEQL_QUERYParser::K_WHERE, 0);
}

CEQL_QUERYParser::Core_patternContext* CEQL_QUERYParser::Core_queryContext::core_pattern() {
  return getRuleContext<CEQL_QUERYParser::Core_patternContext>(0);
}

CEQL_QUERYParser::Selection_strategyContext* CEQL_QUERYParser::Core_queryContext::selection_strategy() {
  return getRuleContext<CEQL_QUERYParser::Selection_strategyContext>(0);
}

tree::TerminalNode* CEQL_QUERYParser::Core_queryContext::K_FROM() {
  return getToken(CEQL_QUERYParser::K_FROM, 0);
}

std::vector<CEQL_QUERYParser::Stream_nameContext *> CEQL_QUERYParser::Core_queryContext::stream_name() {
  return getRuleContexts<CEQL_QUERYParser::Stream_nameContext>();
}

CEQL_QUERYParser::Stream_nameContext* CEQL_QUERYParser::Core_queryContext::stream_name(size_t i) {
  return getRuleContext<CEQL_QUERYParser::Stream_nameContext>(i);
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
    setState(80);
    match(CEQL_QUERYParser::K_SELECT);
    setState(82);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 17267952128) != 0)) {
      setState(81);
      selection_strategy();
    }
    setState(84);
    result_values();
    setState(94);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQL_QUERYParser::K_FROM) {
      setState(85);
      match(CEQL_QUERYParser::K_FROM);
      setState(86);
      stream_name();
      setState(91);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == CEQL_QUERYParser::T__0) {
        setState(87);
        match(CEQL_QUERYParser::T__0);
        setState(88);
        stream_name();
        setState(93);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(96);
    match(CEQL_QUERYParser::K_WHERE);
    setState(97);
    core_pattern(0);
    setState(101);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQL_QUERYParser::K_PARTITION) {
      setState(98);
      match(CEQL_QUERYParser::K_PARTITION);
      setState(99);
      match(CEQL_QUERYParser::K_BY);
      setState(100);
      partition_list();
    }
    setState(105);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQL_QUERYParser::K_WITHIN) {
      setState(103);
      match(CEQL_QUERYParser::K_WITHIN);
      setState(104);
      time_window();
    }
    setState(110);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQL_QUERYParser::K_CONSUME) {
      setState(107);
      match(CEQL_QUERYParser::K_CONSUME);
      setState(108);
      match(CEQL_QUERYParser::K_BY);
      setState(109);
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
    setState(118);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQL_QUERYParser::K_ALL: {
        _localctx = _tracker.createInstance<CEQL_QUERYParser::Ss_allContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(112);
        match(CEQL_QUERYParser::K_ALL);
        break;
      }

      case CEQL_QUERYParser::K_ANY: {
        _localctx = _tracker.createInstance<CEQL_QUERYParser::Ss_anyContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(113);
        match(CEQL_QUERYParser::K_ANY);
        break;
      }

      case CEQL_QUERYParser::K_LAST: {
        _localctx = _tracker.createInstance<CEQL_QUERYParser::Ss_lastContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(114);
        match(CEQL_QUERYParser::K_LAST);
        break;
      }

      case CEQL_QUERYParser::K_MAX: {
        _localctx = _tracker.createInstance<CEQL_QUERYParser::Ss_maxContext>(_localctx);
        enterOuterAlt(_localctx, 4);
        setState(115);
        match(CEQL_QUERYParser::K_MAX);
        break;
      }

      case CEQL_QUERYParser::K_NEXT: {
        _localctx = _tracker.createInstance<CEQL_QUERYParser::Ss_nextContext>(_localctx);
        enterOuterAlt(_localctx, 5);
        setState(116);
        match(CEQL_QUERYParser::K_NEXT);
        break;
      }

      case CEQL_QUERYParser::K_STRICT: {
        _localctx = _tracker.createInstance<CEQL_QUERYParser::Ss_strictContext>(_localctx);
        enterOuterAlt(_localctx, 6);
        setState(117);
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

//----------------- Result_valuesContext ------------------------------------------------------------------

CEQL_QUERYParser::Result_valuesContext::Result_valuesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CEQL_QUERYParser::Result_valuesContext::STAR() {
  return getToken(CEQL_QUERYParser::STAR, 0);
}

std::vector<CEQL_QUERYParser::S_event_nameContext *> CEQL_QUERYParser::Result_valuesContext::s_event_name() {
  return getRuleContexts<CEQL_QUERYParser::S_event_nameContext>();
}

CEQL_QUERYParser::S_event_nameContext* CEQL_QUERYParser::Result_valuesContext::s_event_name(size_t i) {
  return getRuleContext<CEQL_QUERYParser::S_event_nameContext>(i);
}


size_t CEQL_QUERYParser::Result_valuesContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleResult_values;
}


std::any CEQL_QUERYParser::Result_valuesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitResult_values(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::Result_valuesContext* CEQL_QUERYParser::result_values() {
  Result_valuesContext *_localctx = _tracker.createInstance<Result_valuesContext>(_ctx, getState());
  enterRule(_localctx, 8, CEQL_QUERYParser::RuleResult_values);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(129);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQL_QUERYParser::STAR: {
        enterOuterAlt(_localctx, 1);
        setState(120);
        match(CEQL_QUERYParser::STAR);
        break;
      }

      case CEQL_QUERYParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 2);
        setState(121);
        s_event_name();
        setState(126);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == CEQL_QUERYParser::T__0) {
          setState(122);
          match(CEQL_QUERYParser::T__0);
          setState(123);
          s_event_name();
          setState(128);
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

//----------------- Core_patternContext ------------------------------------------------------------------

CEQL_QUERYParser::Core_patternContext::Core_patternContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CEQL_QUERYParser::Core_patternContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleCore_pattern;
}

void CEQL_QUERYParser::Core_patternContext::copyFrom(Core_patternContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Event_core_patternContext ------------------------------------------------------------------

CEQL_QUERYParser::S_event_nameContext* CEQL_QUERYParser::Event_core_patternContext::s_event_name() {
  return getRuleContext<CEQL_QUERYParser::S_event_nameContext>(0);
}

CEQL_QUERYParser::Event_core_patternContext::Event_core_patternContext(Core_patternContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Event_core_patternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitEvent_core_pattern(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Kleene_core_patternContext ------------------------------------------------------------------

CEQL_QUERYParser::Core_patternContext* CEQL_QUERYParser::Kleene_core_patternContext::core_pattern() {
  return getRuleContext<CEQL_QUERYParser::Core_patternContext>(0);
}

tree::TerminalNode* CEQL_QUERYParser::Kleene_core_patternContext::PLUS() {
  return getToken(CEQL_QUERYParser::PLUS, 0);
}

CEQL_QUERYParser::Kleene_core_patternContext::Kleene_core_patternContext(Core_patternContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Kleene_core_patternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitKleene_core_pattern(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Assign_core_patternContext ------------------------------------------------------------------

CEQL_QUERYParser::Core_patternContext* CEQL_QUERYParser::Assign_core_patternContext::core_pattern() {
  return getRuleContext<CEQL_QUERYParser::Core_patternContext>(0);
}

tree::TerminalNode* CEQL_QUERYParser::Assign_core_patternContext::K_AS() {
  return getToken(CEQL_QUERYParser::K_AS, 0);
}

CEQL_QUERYParser::Event_nameContext* CEQL_QUERYParser::Assign_core_patternContext::event_name() {
  return getRuleContext<CEQL_QUERYParser::Event_nameContext>(0);
}

CEQL_QUERYParser::Assign_core_patternContext::Assign_core_patternContext(Core_patternContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Assign_core_patternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitAssign_core_pattern(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Binary_core_patternContext ------------------------------------------------------------------

std::vector<CEQL_QUERYParser::Core_patternContext *> CEQL_QUERYParser::Binary_core_patternContext::core_pattern() {
  return getRuleContexts<CEQL_QUERYParser::Core_patternContext>();
}

CEQL_QUERYParser::Core_patternContext* CEQL_QUERYParser::Binary_core_patternContext::core_pattern(size_t i) {
  return getRuleContext<CEQL_QUERYParser::Core_patternContext>(i);
}

tree::TerminalNode* CEQL_QUERYParser::Binary_core_patternContext::K_OR() {
  return getToken(CEQL_QUERYParser::K_OR, 0);
}

tree::TerminalNode* CEQL_QUERYParser::Binary_core_patternContext::SEMICOLON() {
  return getToken(CEQL_QUERYParser::SEMICOLON, 0);
}

CEQL_QUERYParser::Binary_core_patternContext::Binary_core_patternContext(Core_patternContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Binary_core_patternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitBinary_core_pattern(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Par_core_patternContext ------------------------------------------------------------------

CEQL_QUERYParser::Core_patternContext* CEQL_QUERYParser::Par_core_patternContext::core_pattern() {
  return getRuleContext<CEQL_QUERYParser::Core_patternContext>(0);
}

CEQL_QUERYParser::Par_core_patternContext::Par_core_patternContext(Core_patternContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Par_core_patternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitPar_core_pattern(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Filter_core_patternContext ------------------------------------------------------------------

CEQL_QUERYParser::Core_patternContext* CEQL_QUERYParser::Filter_core_patternContext::core_pattern() {
  return getRuleContext<CEQL_QUERYParser::Core_patternContext>(0);
}

tree::TerminalNode* CEQL_QUERYParser::Filter_core_patternContext::K_FILTER() {
  return getToken(CEQL_QUERYParser::K_FILTER, 0);
}

CEQL_QUERYParser::FilterContext* CEQL_QUERYParser::Filter_core_patternContext::filter() {
  return getRuleContext<CEQL_QUERYParser::FilterContext>(0);
}

CEQL_QUERYParser::Filter_core_patternContext::Filter_core_patternContext(Core_patternContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Filter_core_patternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitFilter_core_pattern(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::Core_patternContext* CEQL_QUERYParser::core_pattern() {
   return core_pattern(0);
}

CEQL_QUERYParser::Core_patternContext* CEQL_QUERYParser::core_pattern(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  CEQL_QUERYParser::Core_patternContext *_localctx = _tracker.createInstance<Core_patternContext>(_ctx, parentState);
  CEQL_QUERYParser::Core_patternContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 10;
  enterRecursionRule(_localctx, 10, CEQL_QUERYParser::RuleCore_pattern, precedence);

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
    setState(137);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQL_QUERYParser::T__1: {
        _localctx = _tracker.createInstance<Par_core_patternContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(132);
        match(CEQL_QUERYParser::T__1);
        setState(133);
        core_pattern(0);
        setState(134);
        match(CEQL_QUERYParser::T__2);
        break;
      }

      case CEQL_QUERYParser::IDENTIFIER: {
        _localctx = _tracker.createInstance<Event_core_patternContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(136);
        s_event_name();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(152);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 13, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(150);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 12, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<Binary_core_patternContext>(_tracker.createInstance<Core_patternContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleCore_pattern);
          setState(139);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(140);
          _la = _input->LA(1);
          if (!(_la == CEQL_QUERYParser::K_OR

          || _la == CEQL_QUERYParser::SEMICOLON)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(141);
          core_pattern(3);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<Assign_core_patternContext>(_tracker.createInstance<Core_patternContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleCore_pattern);
          setState(142);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(143);
          match(CEQL_QUERYParser::K_AS);
          setState(144);
          event_name();
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<Kleene_core_patternContext>(_tracker.createInstance<Core_patternContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleCore_pattern);
          setState(145);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(146);
          match(CEQL_QUERYParser::PLUS);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<Filter_core_patternContext>(_tracker.createInstance<Core_patternContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleCore_pattern);
          setState(147);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(148);
          match(CEQL_QUERYParser::K_FILTER);
          setState(149);
          filter(0);
          break;
        }

        default:
          break;
        } 
      }
      setState(154);
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
  enterRule(_localctx, 12, CEQL_QUERYParser::RulePartition_list);
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
    setState(155);
    match(CEQL_QUERYParser::T__3);
    setState(156);
    attribute_list();
    setState(157);
    match(CEQL_QUERYParser::T__4);
    setState(163);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQL_QUERYParser::T__0) {
      setState(158);
      match(CEQL_QUERYParser::T__0);
      setState(159);
      match(CEQL_QUERYParser::T__3);
      setState(160);
      attribute_list();
      setState(161);
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
  enterRule(_localctx, 14, CEQL_QUERYParser::RuleAttribute_list);
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
    attribute_name();
    setState(170);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CEQL_QUERYParser::T__0) {
      setState(166);
      match(CEQL_QUERYParser::T__0);
      setState(167);
      attribute_name();
      setState(172);
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
  enterRule(_localctx, 16, CEQL_QUERYParser::RuleConsumption_policy);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(176);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQL_QUERYParser::K_ANY: {
        _localctx = _tracker.createInstance<CEQL_QUERYParser::Cp_anyContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(173);
        match(CEQL_QUERYParser::K_ANY);
        break;
      }

      case CEQL_QUERYParser::K_PARTITION: {
        _localctx = _tracker.createInstance<CEQL_QUERYParser::Cp_partitionContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(174);
        match(CEQL_QUERYParser::K_PARTITION);
        break;
      }

      case CEQL_QUERYParser::K_NONE: {
        _localctx = _tracker.createInstance<CEQL_QUERYParser::Cp_noneContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(175);
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
//----------------- Event_filterContext ------------------------------------------------------------------

CEQL_QUERYParser::Event_nameContext* CEQL_QUERYParser::Event_filterContext::event_name() {
  return getRuleContext<CEQL_QUERYParser::Event_nameContext>(0);
}

CEQL_QUERYParser::Bool_exprContext* CEQL_QUERYParser::Event_filterContext::bool_expr() {
  return getRuleContext<CEQL_QUERYParser::Bool_exprContext>(0);
}

CEQL_QUERYParser::Event_filterContext::Event_filterContext(FilterContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Event_filterContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitEvent_filter(this);
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
  size_t startState = 18;
  enterRecursionRule(_localctx, 18, CEQL_QUERYParser::RuleFilter, precedence);

    

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
    setState(188);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQL_QUERYParser::T__1: {
        _localctx = _tracker.createInstance<Par_filterContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(179);
        match(CEQL_QUERYParser::T__1);
        setState(180);
        filter(0);
        setState(181);
        match(CEQL_QUERYParser::T__2);
        break;
      }

      case CEQL_QUERYParser::IDENTIFIER: {
        _localctx = _tracker.createInstance<Event_filterContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(183);
        event_name();
        setState(184);
        match(CEQL_QUERYParser::T__3);
        setState(185);
        bool_expr(0);
        setState(186);
        match(CEQL_QUERYParser::T__4);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(198);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 19, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(196);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 18, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<And_filterContext>(_tracker.createInstance<FilterContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleFilter);
          setState(190);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(191);
          match(CEQL_QUERYParser::K_AND);
          setState(192);
          filter(3);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<Or_filterContext>(_tracker.createInstance<FilterContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleFilter);
          setState(193);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(194);
          match(CEQL_QUERYParser::K_OR);
          setState(195);
          filter(2);
          break;
        }

        default:
          break;
        } 
      }
      setState(200);
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

//----------------- Bool_exprContext ------------------------------------------------------------------

CEQL_QUERYParser::Bool_exprContext::Bool_exprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CEQL_QUERYParser::Bool_exprContext::getRuleIndex() const {
  return CEQL_QUERYParser::RuleBool_expr;
}

void CEQL_QUERYParser::Bool_exprContext::copyFrom(Bool_exprContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Not_exprContext ------------------------------------------------------------------

tree::TerminalNode* CEQL_QUERYParser::Not_exprContext::K_NOT() {
  return getToken(CEQL_QUERYParser::K_NOT, 0);
}

CEQL_QUERYParser::Bool_exprContext* CEQL_QUERYParser::Not_exprContext::bool_expr() {
  return getRuleContext<CEQL_QUERYParser::Bool_exprContext>(0);
}

CEQL_QUERYParser::Not_exprContext::Not_exprContext(Bool_exprContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Not_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitNot_expr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Equality_string_exprContext ------------------------------------------------------------------

CEQL_QUERYParser::String_literalContext* CEQL_QUERYParser::Equality_string_exprContext::string_literal() {
  return getRuleContext<CEQL_QUERYParser::String_literalContext>(0);
}

CEQL_QUERYParser::String_literal_or_regexpContext* CEQL_QUERYParser::Equality_string_exprContext::string_literal_or_regexp() {
  return getRuleContext<CEQL_QUERYParser::String_literal_or_regexpContext>(0);
}

tree::TerminalNode* CEQL_QUERYParser::Equality_string_exprContext::EQ() {
  return getToken(CEQL_QUERYParser::EQ, 0);
}

tree::TerminalNode* CEQL_QUERYParser::Equality_string_exprContext::NEQ() {
  return getToken(CEQL_QUERYParser::NEQ, 0);
}

CEQL_QUERYParser::Equality_string_exprContext::Equality_string_exprContext(Bool_exprContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Equality_string_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitEquality_string_expr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- And_exprContext ------------------------------------------------------------------

std::vector<CEQL_QUERYParser::Bool_exprContext *> CEQL_QUERYParser::And_exprContext::bool_expr() {
  return getRuleContexts<CEQL_QUERYParser::Bool_exprContext>();
}

CEQL_QUERYParser::Bool_exprContext* CEQL_QUERYParser::And_exprContext::bool_expr(size_t i) {
  return getRuleContext<CEQL_QUERYParser::Bool_exprContext>(i);
}

tree::TerminalNode* CEQL_QUERYParser::And_exprContext::K_AND() {
  return getToken(CEQL_QUERYParser::K_AND, 0);
}

CEQL_QUERYParser::And_exprContext::And_exprContext(Bool_exprContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::And_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitAnd_expr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Par_bool_exprContext ------------------------------------------------------------------

CEQL_QUERYParser::Bool_exprContext* CEQL_QUERYParser::Par_bool_exprContext::bool_expr() {
  return getRuleContext<CEQL_QUERYParser::Bool_exprContext>(0);
}

CEQL_QUERYParser::Par_bool_exprContext::Par_bool_exprContext(Bool_exprContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Par_bool_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitPar_bool_expr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Containment_exprContext ------------------------------------------------------------------

CEQL_QUERYParser::Attribute_nameContext* CEQL_QUERYParser::Containment_exprContext::attribute_name() {
  return getRuleContext<CEQL_QUERYParser::Attribute_nameContext>(0);
}

CEQL_QUERYParser::Value_seqContext* CEQL_QUERYParser::Containment_exprContext::value_seq() {
  return getRuleContext<CEQL_QUERYParser::Value_seqContext>(0);
}

tree::TerminalNode* CEQL_QUERYParser::Containment_exprContext::K_IN() {
  return getToken(CEQL_QUERYParser::K_IN, 0);
}

tree::TerminalNode* CEQL_QUERYParser::Containment_exprContext::K_NOT() {
  return getToken(CEQL_QUERYParser::K_NOT, 0);
}

CEQL_QUERYParser::Containment_exprContext::Containment_exprContext(Bool_exprContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Containment_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitContainment_expr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Inequality_exprContext ------------------------------------------------------------------

std::vector<CEQL_QUERYParser::Math_exprContext *> CEQL_QUERYParser::Inequality_exprContext::math_expr() {
  return getRuleContexts<CEQL_QUERYParser::Math_exprContext>();
}

CEQL_QUERYParser::Math_exprContext* CEQL_QUERYParser::Inequality_exprContext::math_expr(size_t i) {
  return getRuleContext<CEQL_QUERYParser::Math_exprContext>(i);
}

tree::TerminalNode* CEQL_QUERYParser::Inequality_exprContext::LE() {
  return getToken(CEQL_QUERYParser::LE, 0);
}

tree::TerminalNode* CEQL_QUERYParser::Inequality_exprContext::LEQ() {
  return getToken(CEQL_QUERYParser::LEQ, 0);
}

tree::TerminalNode* CEQL_QUERYParser::Inequality_exprContext::GE() {
  return getToken(CEQL_QUERYParser::GE, 0);
}

tree::TerminalNode* CEQL_QUERYParser::Inequality_exprContext::GEQ() {
  return getToken(CEQL_QUERYParser::GEQ, 0);
}

CEQL_QUERYParser::Inequality_exprContext::Inequality_exprContext(Bool_exprContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Inequality_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitInequality_expr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Or_exprContext ------------------------------------------------------------------

std::vector<CEQL_QUERYParser::Bool_exprContext *> CEQL_QUERYParser::Or_exprContext::bool_expr() {
  return getRuleContexts<CEQL_QUERYParser::Bool_exprContext>();
}

CEQL_QUERYParser::Bool_exprContext* CEQL_QUERYParser::Or_exprContext::bool_expr(size_t i) {
  return getRuleContext<CEQL_QUERYParser::Bool_exprContext>(i);
}

tree::TerminalNode* CEQL_QUERYParser::Or_exprContext::K_OR() {
  return getToken(CEQL_QUERYParser::K_OR, 0);
}

CEQL_QUERYParser::Or_exprContext::Or_exprContext(Bool_exprContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Or_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitOr_expr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Equality_math_exprContext ------------------------------------------------------------------

std::vector<CEQL_QUERYParser::Math_exprContext *> CEQL_QUERYParser::Equality_math_exprContext::math_expr() {
  return getRuleContexts<CEQL_QUERYParser::Math_exprContext>();
}

CEQL_QUERYParser::Math_exprContext* CEQL_QUERYParser::Equality_math_exprContext::math_expr(size_t i) {
  return getRuleContext<CEQL_QUERYParser::Math_exprContext>(i);
}

tree::TerminalNode* CEQL_QUERYParser::Equality_math_exprContext::EQ() {
  return getToken(CEQL_QUERYParser::EQ, 0);
}

tree::TerminalNode* CEQL_QUERYParser::Equality_math_exprContext::NEQ() {
  return getToken(CEQL_QUERYParser::NEQ, 0);
}

CEQL_QUERYParser::Equality_math_exprContext::Equality_math_exprContext(Bool_exprContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Equality_math_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitEquality_math_expr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Regex_exprContext ------------------------------------------------------------------

CEQL_QUERYParser::Attribute_nameContext* CEQL_QUERYParser::Regex_exprContext::attribute_name() {
  return getRuleContext<CEQL_QUERYParser::Attribute_nameContext>(0);
}

tree::TerminalNode* CEQL_QUERYParser::Regex_exprContext::K_LIKE() {
  return getToken(CEQL_QUERYParser::K_LIKE, 0);
}

tree::TerminalNode* CEQL_QUERYParser::Regex_exprContext::REGEXP() {
  return getToken(CEQL_QUERYParser::REGEXP, 0);
}

CEQL_QUERYParser::Regex_exprContext::Regex_exprContext(Bool_exprContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Regex_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitRegex_expr(this);
  else
    return visitor->visitChildren(this);
}

CEQL_QUERYParser::Bool_exprContext* CEQL_QUERYParser::bool_expr() {
   return bool_expr(0);
}

CEQL_QUERYParser::Bool_exprContext* CEQL_QUERYParser::bool_expr(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  CEQL_QUERYParser::Bool_exprContext *_localctx = _tracker.createInstance<Bool_exprContext>(_ctx, parentState);
  CEQL_QUERYParser::Bool_exprContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 20;
  enterRecursionRule(_localctx, 20, CEQL_QUERYParser::RuleBool_expr, precedence);

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
    setState(232);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 21, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<Par_bool_exprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(202);
      match(CEQL_QUERYParser::T__1);
      setState(203);
      bool_expr(0);
      setState(204);
      match(CEQL_QUERYParser::T__2);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<Not_exprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(206);
      match(CEQL_QUERYParser::K_NOT);
      setState(207);
      bool_expr(8);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<Inequality_exprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(208);
      math_expr(0);
      setState(209);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 131941395333120) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(210);
      math_expr(0);
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<Equality_math_exprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(212);
      math_expr(0);
      setState(213);
      _la = _input->LA(1);
      if (!(_la == CEQL_QUERYParser::EQ

      || _la == CEQL_QUERYParser::NEQ)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(214);
      math_expr(0);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<Regex_exprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(216);
      attribute_name();
      setState(217);
      match(CEQL_QUERYParser::K_LIKE);
      setState(218);
      match(CEQL_QUERYParser::REGEXP);
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<Containment_exprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(220);
      attribute_name();
      setState(224);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case CEQL_QUERYParser::K_IN: {
          setState(221);
          match(CEQL_QUERYParser::K_IN);
          break;
        }

        case CEQL_QUERYParser::K_NOT: {
          setState(222);
          match(CEQL_QUERYParser::K_NOT);
          setState(223);
          match(CEQL_QUERYParser::K_IN);
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(226);
      value_seq();
      break;
    }

    case 7: {
      _localctx = _tracker.createInstance<Equality_string_exprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(228);
      string_literal();
      setState(229);
      _la = _input->LA(1);
      if (!(_la == CEQL_QUERYParser::EQ

      || _la == CEQL_QUERYParser::NEQ)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(230);
      string_literal_or_regexp();
      break;
    }

    default:
      break;
    }
    _ctx->stop = _input->LT(-1);
    setState(242);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 23, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(240);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 22, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<And_exprContext>(_tracker.createInstance<Bool_exprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleBool_expr);
          setState(234);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(235);
          match(CEQL_QUERYParser::K_AND);
          setState(236);
          bool_expr(6);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<Or_exprContext>(_tracker.createInstance<Bool_exprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleBool_expr);
          setState(237);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(238);
          match(CEQL_QUERYParser::K_OR);
          setState(239);
          bool_expr(5);
          break;
        }

        default:
          break;
        } 
      }
      setState(244);
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
  enterRule(_localctx, 22, CEQL_QUERYParser::RuleString_literal);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(247);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQL_QUERYParser::STRING_LITERAL: {
        enterOuterAlt(_localctx, 1);
        setState(245);
        string();
        break;
      }

      case CEQL_QUERYParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 2);
        setState(246);
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

tree::TerminalNode* CEQL_QUERYParser::String_literal_or_regexpContext::REGEXP() {
  return getToken(CEQL_QUERYParser::REGEXP, 0);
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
  enterRule(_localctx, 24, CEQL_QUERYParser::RuleString_literal_or_regexp);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(252);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQL_QUERYParser::STRING_LITERAL: {
        enterOuterAlt(_localctx, 1);
        setState(249);
        string();
        break;
      }

      case CEQL_QUERYParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 2);
        setState(250);
        attribute_name();
        break;
      }

      case CEQL_QUERYParser::REGEXP: {
        enterOuterAlt(_localctx, 3);
        setState(251);
        match(CEQL_QUERYParser::REGEXP);
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
  size_t startState = 26;
  enterRecursionRule(_localctx, 26, CEQL_QUERYParser::RuleMath_expr, precedence);

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
    setState(263);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CEQL_QUERYParser::T__1: {
        _localctx = _tracker.createInstance<Par_math_exprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(255);
        match(CEQL_QUERYParser::T__1);
        setState(256);
        math_expr(0);
        setState(257);
        match(CEQL_QUERYParser::T__2);
        break;
      }

      case CEQL_QUERYParser::FLOAT_LITERAL:
      case CEQL_QUERYParser::INTEGER_LITERAL: {
        _localctx = _tracker.createInstance<Number_math_exprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(259);
        number();
        break;
      }

      case CEQL_QUERYParser::IDENTIFIER: {
        _localctx = _tracker.createInstance<Attribute_math_exprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(260);
        attribute_name();
        break;
      }

      case CEQL_QUERYParser::PLUS:
      case CEQL_QUERYParser::MINUS: {
        _localctx = _tracker.createInstance<Unary_math_exprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(261);
        _la = _input->LA(1);
        if (!(_la == CEQL_QUERYParser::PLUS

        || _la == CEQL_QUERYParser::MINUS)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(262);
        math_expr(3);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(273);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 28, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(271);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 27, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<Mul_math_exprContext>(_tracker.createInstance<Math_exprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleMath_expr);
          setState(265);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(266);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & 6871947673600) != 0))) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(267);
          math_expr(3);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<Sum_math_exprContext>(_tracker.createInstance<Math_exprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleMath_expr);
          setState(268);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
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
          math_expr(2);
          break;
        }

        default:
          break;
        } 
      }
      setState(275);
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
  enterRule(_localctx, 28, CEQL_QUERYParser::RuleValue_seq);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(284);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 29, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(276);
      match(CEQL_QUERYParser::T__5);
      setState(277);
      number_seq();
      setState(278);
      match(CEQL_QUERYParser::T__6);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(280);
      match(CEQL_QUERYParser::T__5);
      setState(281);
      string_seq();
      setState(282);
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
//----------------- Number_rangeContext ------------------------------------------------------------------

std::vector<CEQL_QUERYParser::NumberContext *> CEQL_QUERYParser::Number_rangeContext::number() {
  return getRuleContexts<CEQL_QUERYParser::NumberContext>();
}

CEQL_QUERYParser::NumberContext* CEQL_QUERYParser::Number_rangeContext::number(size_t i) {
  return getRuleContext<CEQL_QUERYParser::NumberContext>(i);
}

CEQL_QUERYParser::Number_rangeContext::Number_rangeContext(Number_seqContext *ctx) { copyFrom(ctx); }


std::any CEQL_QUERYParser::Number_rangeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CEQL_QUERYVisitor*>(visitor))
    return parserVisitor->visitNumber_range(this);
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
CEQL_QUERYParser::Number_seqContext* CEQL_QUERYParser::number_seq() {
  Number_seqContext *_localctx = _tracker.createInstance<Number_seqContext>(_ctx, getState());
  enterRule(_localctx, 30, CEQL_QUERYParser::RuleNumber_seq);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(303);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 31, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<CEQL_QUERYParser::Number_listContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(286);
      number();
      setState(291);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == CEQL_QUERYParser::T__0) {
        setState(287);
        match(CEQL_QUERYParser::T__0);
        setState(288);
        number();
        setState(293);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<CEQL_QUERYParser::Number_rangeContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(294);
      number();
      setState(295);
      match(CEQL_QUERYParser::T__7);
      setState(296);
      number();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<CEQL_QUERYParser::Number_range_lowerContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(298);
      number();
      setState(299);
      match(CEQL_QUERYParser::T__7);
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<CEQL_QUERYParser::Number_range_upperContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(301);
      match(CEQL_QUERYParser::T__7);
      setState(302);
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
  enterRule(_localctx, 32, CEQL_QUERYParser::RuleString_seq);
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
    setState(305);
    string();
    setState(310);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CEQL_QUERYParser::T__0) {
      setState(306);
      match(CEQL_QUERYParser::T__0);
      setState(307);
      string();
      setState(312);
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
  enterRule(_localctx, 34, CEQL_QUERYParser::RuleTime_window);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(316);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 33, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(313);
      event_span();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(314);
      time_span();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(315);
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
  enterRule(_localctx, 36, CEQL_QUERYParser::RuleEvent_span);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(318);
    integer();
    setState(319);
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
  enterRule(_localctx, 38, CEQL_QUERYParser::RuleTime_span);
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
    setState(322);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 34, _ctx)) {
    case 1: {
      setState(321);
      hour_span();
      break;
    }

    default:
      break;
    }
    setState(325);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 35, _ctx)) {
    case 1: {
      setState(324);
      minute_span();
      break;
    }

    default:
      break;
    }
    setState(328);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQL_QUERYParser::INTEGER_LITERAL) {
      setState(327);
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
  enterRule(_localctx, 40, CEQL_QUERYParser::RuleHour_span);

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
  enterRule(_localctx, 42, CEQL_QUERYParser::RuleMinute_span);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(333);
    integer();
    setState(334);
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
  enterRule(_localctx, 44, CEQL_QUERYParser::RuleSecond_span);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(336);
    integer();
    setState(337);
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
  enterRule(_localctx, 46, CEQL_QUERYParser::RuleCustom_span);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(339);
    integer();
    setState(340);
    match(CEQL_QUERYParser::T__3);
    setState(341);
    any_name();
    setState(342);
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
  enterRule(_localctx, 48, CEQL_QUERYParser::RuleNamed_event);
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
    setState(344);
    s_event_name();
    setState(347);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CEQL_QUERYParser::K_AS) {
      setState(345);
      match(CEQL_QUERYParser::K_AS);
      setState(346);
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
  enterRule(_localctx, 50, CEQL_QUERYParser::RuleS_event_name);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(352);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 38, _ctx)) {
    case 1: {
      setState(349);
      stream_name();
      setState(350);
      match(CEQL_QUERYParser::GE);
      break;
    }

    default:
      break;
    }
    setState(354);
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
  enterRule(_localctx, 52, CEQL_QUERYParser::RuleEvent_name);

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
  enterRule(_localctx, 54, CEQL_QUERYParser::RuleStream_name);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(358);
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
  enterRule(_localctx, 56, CEQL_QUERYParser::RuleAttribute_name);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(360);
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
  enterRule(_localctx, 58, CEQL_QUERYParser::RuleInteger);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(362);
    match(CEQL_QUERYParser::INTEGER_LITERAL);
   
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

tree::TerminalNode* CEQL_QUERYParser::NumberContext::FLOAT_LITERAL() {
  return getToken(CEQL_QUERYParser::FLOAT_LITERAL, 0);
}

tree::TerminalNode* CEQL_QUERYParser::NumberContext::INTEGER_LITERAL() {
  return getToken(CEQL_QUERYParser::INTEGER_LITERAL, 0);
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
  enterRule(_localctx, 60, CEQL_QUERYParser::RuleNumber);
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
    setState(364);
    _la = _input->LA(1);
    if (!(_la == CEQL_QUERYParser::FLOAT_LITERAL

    || _la == CEQL_QUERYParser::INTEGER_LITERAL)) {
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
  enterRule(_localctx, 62, CEQL_QUERYParser::RuleString);

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
  enterRule(_localctx, 64, CEQL_QUERYParser::RuleAny_name);

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
  enterRule(_localctx, 66, CEQL_QUERYParser::RuleKeyword);
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
    setState(370);
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
    case 5: return core_patternSempred(antlrcpp::downCast<Core_patternContext *>(context), predicateIndex);
    case 9: return filterSempred(antlrcpp::downCast<FilterContext *>(context), predicateIndex);
    case 10: return bool_exprSempred(antlrcpp::downCast<Bool_exprContext *>(context), predicateIndex);
    case 13: return math_exprSempred(antlrcpp::downCast<Math_exprContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool CEQL_QUERYParser::core_patternSempred(Core_patternContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 2);
    case 1: return precpred(_ctx, 4);
    case 2: return precpred(_ctx, 3);
    case 3: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

bool CEQL_QUERYParser::filterSempred(FilterContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 4: return precpred(_ctx, 2);
    case 5: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

bool CEQL_QUERYParser::bool_exprSempred(Bool_exprContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 6: return precpred(_ctx, 5);
    case 7: return precpred(_ctx, 4);

  default:
    break;
  }
  return true;
}

bool CEQL_QUERYParser::math_exprSempred(Math_exprContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 8: return precpred(_ctx, 2);
    case 9: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

void CEQL_QUERYParser::initialize() {
  ::antlr4::internal::call_once(ceql_queryParserOnceFlag, ceql_queryParserInitialize);
}
