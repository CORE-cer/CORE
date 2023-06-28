
// Generated from StreamDeclarationParser.g4 by ANTLR 4.12.0


#include "StreamDeclarationParserVisitor.h"

#include "StreamDeclarationParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct StreamDeclarationParserStaticData final {
  StreamDeclarationParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  StreamDeclarationParserStaticData(const StreamDeclarationParserStaticData&) = delete;
  StreamDeclarationParserStaticData(StreamDeclarationParserStaticData&&) = delete;
  StreamDeclarationParserStaticData& operator=(const StreamDeclarationParserStaticData&) = delete;
  StreamDeclarationParserStaticData& operator=(StreamDeclarationParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag streamdeclarationparserParserOnceFlag;
StreamDeclarationParserStaticData *streamdeclarationparserParserStaticData = nullptr;

void streamdeclarationparserParserInitialize() {
  assert(streamdeclarationparserParserStaticData == nullptr);
  auto staticData = std::make_unique<StreamDeclarationParserStaticData>(
    std::vector<std::string>{
      "parse", "error", "core_stmt", "core_declaration", "event_declaration", 
      "attribute_dec_list", "attribute_declaration", "datatype", "stream_declaration", 
      "event_list", "core_query", "selection_strategy", "result_values", 
      "core_pattern", "partition_list", "attribute_list", "consumption_policy", 
      "filter", "bool_expr", "string_literal", "string_literal_or_regexp", 
      "math_expr", "value_seq", "number_seq", "string_seq", "time_window", 
      "event_span", "time_span", "hour_span", "minute_span", "second_span", 
      "custom_span", "named_event", "s_event_name", "event_name", "stream_name", 
      "attribute_name", "integer", "number", "string", "any_name", "keyword"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "'%'", "'+'", 
      "'-'", "'*'", "'/'", "'<'", "'<='", "'>'", "'>='", "", "", "'int'", 
      "'string'", "'double'", "'long'", "'boolean'", "';'", "','", "'..'", 
      "'('", "')'", "'['", "']'", "'{'", "'}'"
    },
    std::vector<std::string>{
      "", "K_ALL", "K_AND", "K_ANY", "K_AS", "K_BY", "K_CONSUME", "K_DECLARE", 
      "K_DISTINCT", "K_EVENT", "K_EVENTS", "K_FILTER", "K_FROM", "K_HOURS", 
      "K_IN", "K_LAST", "K_LIKE", "K_MAX", "K_MINUTES", "K_NEXT", "K_NONE", 
      "K_NOT", "K_OR", "K_PARTITION", "K_SECONDS", "K_SELECT", "K_STREAM", 
      "K_STRICT", "K_UNLESS", "K_WHERE", "K_WITHIN", "PERCENT", "PLUS", 
      "MINUS", "STAR", "SLASH", "LE", "LEQ", "GE", "GEQ", "EQ", "NEQ", "INT", 
      "STRING", "DOUBLE", "LONG", "BOOLEAN", "SEMICOLON", "COMMA", "DOUBLE_DOT", 
      "LEFT_PARENTHESIS", "RIGHT_PARENTHESIS", "LEFT_SQUARE_BRACKET", "RIGHT_SQUARE_BRACKET", 
      "LEFT_CURLY_BRACKET", "RIGHT_CURLY_BRACKET", "IDENTIFIER", "FLOAT_LITERAL", 
      "INTEGER_LITERAL", "NUMERICAL_EXPONENT", "STRING_LITERAL", "REGEXP", 
      "SINGLE_LINE_COMMENT", "MULTILINE_COMMENT", "SPACES", "UNEXPECTED_CHAR"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,65,437,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,7,
  	35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,2,40,7,40,2,41,7,41,1,0,1,
  	0,5,0,87,8,0,10,0,12,0,90,9,0,1,0,1,0,1,1,1,1,1,1,1,2,1,2,3,2,99,8,2,
  	1,3,1,3,3,3,103,8,3,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,5,1,5,1,5,1,5,5,5,116,
  	8,5,10,5,12,5,119,9,5,3,5,121,8,5,1,6,1,6,1,6,1,7,1,7,1,8,1,8,1,8,1,8,
  	1,8,3,8,133,8,8,1,8,1,8,1,9,1,9,1,9,5,9,140,8,9,10,9,12,9,143,9,9,1,10,
  	1,10,3,10,147,8,10,1,10,1,10,1,10,1,10,1,10,5,10,154,8,10,10,10,12,10,
  	157,9,10,3,10,159,8,10,1,10,1,10,1,10,1,10,1,10,3,10,166,8,10,1,10,1,
  	10,3,10,170,8,10,1,10,1,10,1,10,3,10,175,8,10,1,11,1,11,1,11,1,11,1,11,
  	1,11,3,11,183,8,11,1,12,1,12,1,12,1,12,5,12,189,8,12,10,12,12,12,192,
  	9,12,3,12,194,8,12,1,13,1,13,1,13,1,13,1,13,1,13,3,13,202,8,13,1,13,1,
  	13,1,13,1,13,1,13,1,13,1,13,1,13,1,13,1,13,1,13,5,13,215,8,13,10,13,12,
  	13,218,9,13,1,14,1,14,1,14,1,14,1,14,1,14,1,14,1,14,3,14,228,8,14,1,15,
  	1,15,1,15,5,15,233,8,15,10,15,12,15,236,9,15,1,16,1,16,1,16,3,16,241,
  	8,16,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,17,3,17,253,8,17,
  	1,17,1,17,1,17,1,17,1,17,1,17,5,17,261,8,17,10,17,12,17,264,9,17,1,18,
  	1,18,1,18,1,18,1,18,1,18,1,18,1,18,1,18,1,18,1,18,1,18,1,18,1,18,1,18,
  	1,18,1,18,1,18,1,18,1,18,1,18,1,18,1,18,3,18,289,8,18,1,18,1,18,1,18,
  	1,18,1,18,1,18,3,18,297,8,18,1,18,1,18,1,18,1,18,1,18,1,18,5,18,305,8,
  	18,10,18,12,18,308,9,18,1,19,1,19,3,19,312,8,19,1,20,1,20,1,20,3,20,317,
  	8,20,1,21,1,21,1,21,1,21,1,21,1,21,1,21,1,21,1,21,3,21,328,8,21,1,21,
  	1,21,1,21,1,21,1,21,1,21,5,21,336,8,21,10,21,12,21,339,9,21,1,22,1,22,
  	1,22,1,22,1,22,1,22,1,22,1,22,3,22,349,8,22,1,23,1,23,1,23,5,23,354,8,
  	23,10,23,12,23,357,9,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,1,23,
  	3,23,368,8,23,1,24,1,24,1,24,5,24,373,8,24,10,24,12,24,376,9,24,1,25,
  	1,25,1,25,3,25,381,8,25,1,26,1,26,1,26,1,27,3,27,387,8,27,1,27,3,27,390,
  	8,27,1,27,3,27,393,8,27,1,28,1,28,1,28,1,29,1,29,1,29,1,30,1,30,1,30,
  	1,31,1,31,1,31,1,31,1,31,1,32,1,32,1,32,3,32,412,8,32,1,33,1,33,1,33,
  	3,33,417,8,33,1,33,1,33,1,34,1,34,1,35,1,35,1,36,1,36,1,37,1,37,1,38,
  	1,38,1,39,1,39,1,40,1,40,1,41,1,41,1,41,0,4,26,34,36,42,42,0,2,4,6,8,
  	10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,
  	56,58,60,62,64,66,68,70,72,74,76,78,80,82,0,8,2,0,42,46,56,56,2,0,22,
  	22,47,47,1,0,36,39,1,0,40,41,1,0,32,33,2,0,31,31,34,35,1,0,57,58,1,0,
  	1,30,457,0,88,1,0,0,0,2,93,1,0,0,0,4,98,1,0,0,0,6,102,1,0,0,0,8,104,1,
  	0,0,0,10,120,1,0,0,0,12,122,1,0,0,0,14,125,1,0,0,0,16,127,1,0,0,0,18,
  	136,1,0,0,0,20,144,1,0,0,0,22,182,1,0,0,0,24,193,1,0,0,0,26,201,1,0,0,
  	0,28,219,1,0,0,0,30,229,1,0,0,0,32,240,1,0,0,0,34,252,1,0,0,0,36,296,
  	1,0,0,0,38,311,1,0,0,0,40,316,1,0,0,0,42,327,1,0,0,0,44,348,1,0,0,0,46,
  	367,1,0,0,0,48,369,1,0,0,0,50,380,1,0,0,0,52,382,1,0,0,0,54,386,1,0,0,
  	0,56,394,1,0,0,0,58,397,1,0,0,0,60,400,1,0,0,0,62,403,1,0,0,0,64,408,
  	1,0,0,0,66,416,1,0,0,0,68,420,1,0,0,0,70,422,1,0,0,0,72,424,1,0,0,0,74,
  	426,1,0,0,0,76,428,1,0,0,0,78,430,1,0,0,0,80,432,1,0,0,0,82,434,1,0,0,
  	0,84,87,3,4,2,0,85,87,3,2,1,0,86,84,1,0,0,0,86,85,1,0,0,0,87,90,1,0,0,
  	0,88,86,1,0,0,0,88,89,1,0,0,0,89,91,1,0,0,0,90,88,1,0,0,0,91,92,5,0,0,
  	1,92,1,1,0,0,0,93,94,5,65,0,0,94,95,6,1,-1,0,95,3,1,0,0,0,96,99,3,20,
  	10,0,97,99,3,6,3,0,98,96,1,0,0,0,98,97,1,0,0,0,99,5,1,0,0,0,100,103,3,
  	8,4,0,101,103,3,16,8,0,102,100,1,0,0,0,102,101,1,0,0,0,103,7,1,0,0,0,
  	104,105,5,7,0,0,105,106,5,9,0,0,106,107,3,68,34,0,107,108,5,50,0,0,108,
  	109,3,10,5,0,109,110,5,51,0,0,110,9,1,0,0,0,111,121,1,0,0,0,112,117,3,
  	12,6,0,113,114,5,48,0,0,114,116,3,12,6,0,115,113,1,0,0,0,116,119,1,0,
  	0,0,117,115,1,0,0,0,117,118,1,0,0,0,118,121,1,0,0,0,119,117,1,0,0,0,120,
  	111,1,0,0,0,120,112,1,0,0,0,121,11,1,0,0,0,122,123,3,72,36,0,123,124,
  	3,14,7,0,124,13,1,0,0,0,125,126,7,0,0,0,126,15,1,0,0,0,127,128,5,7,0,
  	0,128,129,5,26,0,0,129,130,3,70,35,0,130,132,5,50,0,0,131,133,3,18,9,
  	0,132,131,1,0,0,0,132,133,1,0,0,0,133,134,1,0,0,0,134,135,5,51,0,0,135,
  	17,1,0,0,0,136,141,3,68,34,0,137,138,5,48,0,0,138,140,3,68,34,0,139,137,
  	1,0,0,0,140,143,1,0,0,0,141,139,1,0,0,0,141,142,1,0,0,0,142,19,1,0,0,
  	0,143,141,1,0,0,0,144,146,5,25,0,0,145,147,3,22,11,0,146,145,1,0,0,0,
  	146,147,1,0,0,0,147,148,1,0,0,0,148,158,3,24,12,0,149,150,5,12,0,0,150,
  	155,3,70,35,0,151,152,5,48,0,0,152,154,3,70,35,0,153,151,1,0,0,0,154,
  	157,1,0,0,0,155,153,1,0,0,0,155,156,1,0,0,0,156,159,1,0,0,0,157,155,1,
  	0,0,0,158,149,1,0,0,0,158,159,1,0,0,0,159,160,1,0,0,0,160,161,5,29,0,
  	0,161,165,3,26,13,0,162,163,5,23,0,0,163,164,5,5,0,0,164,166,3,28,14,
  	0,165,162,1,0,0,0,165,166,1,0,0,0,166,169,1,0,0,0,167,168,5,30,0,0,168,
  	170,3,50,25,0,169,167,1,0,0,0,169,170,1,0,0,0,170,174,1,0,0,0,171,172,
  	5,6,0,0,172,173,5,5,0,0,173,175,3,32,16,0,174,171,1,0,0,0,174,175,1,0,
  	0,0,175,21,1,0,0,0,176,183,5,1,0,0,177,183,5,3,0,0,178,183,5,15,0,0,179,
  	183,5,17,0,0,180,183,5,19,0,0,181,183,5,27,0,0,182,176,1,0,0,0,182,177,
  	1,0,0,0,182,178,1,0,0,0,182,179,1,0,0,0,182,180,1,0,0,0,182,181,1,0,0,
  	0,183,23,1,0,0,0,184,194,5,34,0,0,185,190,3,66,33,0,186,187,5,48,0,0,
  	187,189,3,66,33,0,188,186,1,0,0,0,189,192,1,0,0,0,190,188,1,0,0,0,190,
  	191,1,0,0,0,191,194,1,0,0,0,192,190,1,0,0,0,193,184,1,0,0,0,193,185,1,
  	0,0,0,194,25,1,0,0,0,195,196,6,13,-1,0,196,197,5,50,0,0,197,198,3,26,
  	13,0,198,199,5,51,0,0,199,202,1,0,0,0,200,202,3,66,33,0,201,195,1,0,0,
  	0,201,200,1,0,0,0,202,216,1,0,0,0,203,204,10,2,0,0,204,205,7,1,0,0,205,
  	215,3,26,13,3,206,207,10,4,0,0,207,208,5,4,0,0,208,215,3,68,34,0,209,
  	210,10,3,0,0,210,215,5,32,0,0,211,212,10,1,0,0,212,213,5,11,0,0,213,215,
  	3,34,17,0,214,203,1,0,0,0,214,206,1,0,0,0,214,209,1,0,0,0,214,211,1,0,
  	0,0,215,218,1,0,0,0,216,214,1,0,0,0,216,217,1,0,0,0,217,27,1,0,0,0,218,
  	216,1,0,0,0,219,220,5,52,0,0,220,221,3,30,15,0,221,227,5,53,0,0,222,223,
  	5,48,0,0,223,224,5,52,0,0,224,225,3,30,15,0,225,226,5,53,0,0,226,228,
  	1,0,0,0,227,222,1,0,0,0,227,228,1,0,0,0,228,29,1,0,0,0,229,234,3,72,36,
  	0,230,231,5,48,0,0,231,233,3,72,36,0,232,230,1,0,0,0,233,236,1,0,0,0,
  	234,232,1,0,0,0,234,235,1,0,0,0,235,31,1,0,0,0,236,234,1,0,0,0,237,241,
  	5,3,0,0,238,241,5,23,0,0,239,241,5,20,0,0,240,237,1,0,0,0,240,238,1,0,
  	0,0,240,239,1,0,0,0,241,33,1,0,0,0,242,243,6,17,-1,0,243,244,5,50,0,0,
  	244,245,3,34,17,0,245,246,5,51,0,0,246,253,1,0,0,0,247,248,3,68,34,0,
  	248,249,5,52,0,0,249,250,3,36,18,0,250,251,5,53,0,0,251,253,1,0,0,0,252,
  	242,1,0,0,0,252,247,1,0,0,0,253,262,1,0,0,0,254,255,10,2,0,0,255,256,
  	5,2,0,0,256,261,3,34,17,3,257,258,10,1,0,0,258,259,5,22,0,0,259,261,3,
  	34,17,2,260,254,1,0,0,0,260,257,1,0,0,0,261,264,1,0,0,0,262,260,1,0,0,
  	0,262,263,1,0,0,0,263,35,1,0,0,0,264,262,1,0,0,0,265,266,6,18,-1,0,266,
  	267,5,50,0,0,267,268,3,36,18,0,268,269,5,51,0,0,269,297,1,0,0,0,270,271,
  	5,21,0,0,271,297,3,36,18,8,272,273,3,42,21,0,273,274,7,2,0,0,274,275,
  	3,42,21,0,275,297,1,0,0,0,276,277,3,42,21,0,277,278,7,3,0,0,278,279,3,
  	42,21,0,279,297,1,0,0,0,280,281,3,72,36,0,281,282,5,16,0,0,282,283,5,
  	61,0,0,283,297,1,0,0,0,284,288,3,72,36,0,285,289,5,14,0,0,286,287,5,21,
  	0,0,287,289,5,14,0,0,288,285,1,0,0,0,288,286,1,0,0,0,289,290,1,0,0,0,
  	290,291,3,44,22,0,291,297,1,0,0,0,292,293,3,38,19,0,293,294,7,3,0,0,294,
  	295,3,40,20,0,295,297,1,0,0,0,296,265,1,0,0,0,296,270,1,0,0,0,296,272,
  	1,0,0,0,296,276,1,0,0,0,296,280,1,0,0,0,296,284,1,0,0,0,296,292,1,0,0,
  	0,297,306,1,0,0,0,298,299,10,5,0,0,299,300,5,2,0,0,300,305,3,36,18,6,
  	301,302,10,4,0,0,302,303,5,22,0,0,303,305,3,36,18,5,304,298,1,0,0,0,304,
  	301,1,0,0,0,305,308,1,0,0,0,306,304,1,0,0,0,306,307,1,0,0,0,307,37,1,
  	0,0,0,308,306,1,0,0,0,309,312,3,78,39,0,310,312,3,72,36,0,311,309,1,0,
  	0,0,311,310,1,0,0,0,312,39,1,0,0,0,313,317,3,78,39,0,314,317,3,72,36,
  	0,315,317,5,61,0,0,316,313,1,0,0,0,316,314,1,0,0,0,316,315,1,0,0,0,317,
  	41,1,0,0,0,318,319,6,21,-1,0,319,320,5,50,0,0,320,321,3,42,21,0,321,322,
  	5,51,0,0,322,328,1,0,0,0,323,328,3,76,38,0,324,328,3,72,36,0,325,326,
  	7,4,0,0,326,328,3,42,21,3,327,318,1,0,0,0,327,323,1,0,0,0,327,324,1,0,
  	0,0,327,325,1,0,0,0,328,337,1,0,0,0,329,330,10,2,0,0,330,331,7,5,0,0,
  	331,336,3,42,21,3,332,333,10,1,0,0,333,334,7,4,0,0,334,336,3,42,21,2,
  	335,329,1,0,0,0,335,332,1,0,0,0,336,339,1,0,0,0,337,335,1,0,0,0,337,338,
  	1,0,0,0,338,43,1,0,0,0,339,337,1,0,0,0,340,341,5,54,0,0,341,342,3,46,
  	23,0,342,343,5,55,0,0,343,349,1,0,0,0,344,345,5,54,0,0,345,346,3,48,24,
  	0,346,347,5,55,0,0,347,349,1,0,0,0,348,340,1,0,0,0,348,344,1,0,0,0,349,
  	45,1,0,0,0,350,355,3,76,38,0,351,352,5,48,0,0,352,354,3,76,38,0,353,351,
  	1,0,0,0,354,357,1,0,0,0,355,353,1,0,0,0,355,356,1,0,0,0,356,368,1,0,0,
  	0,357,355,1,0,0,0,358,359,3,76,38,0,359,360,5,49,0,0,360,361,3,76,38,
  	0,361,368,1,0,0,0,362,363,3,76,38,0,363,364,5,49,0,0,364,368,1,0,0,0,
  	365,366,5,49,0,0,366,368,3,76,38,0,367,350,1,0,0,0,367,358,1,0,0,0,367,
  	362,1,0,0,0,367,365,1,0,0,0,368,47,1,0,0,0,369,374,3,78,39,0,370,371,
  	5,48,0,0,371,373,3,78,39,0,372,370,1,0,0,0,373,376,1,0,0,0,374,372,1,
  	0,0,0,374,375,1,0,0,0,375,49,1,0,0,0,376,374,1,0,0,0,377,381,3,52,26,
  	0,378,381,3,54,27,0,379,381,3,62,31,0,380,377,1,0,0,0,380,378,1,0,0,0,
  	380,379,1,0,0,0,381,51,1,0,0,0,382,383,3,74,37,0,383,384,5,10,0,0,384,
  	53,1,0,0,0,385,387,3,56,28,0,386,385,1,0,0,0,386,387,1,0,0,0,387,389,
  	1,0,0,0,388,390,3,58,29,0,389,388,1,0,0,0,389,390,1,0,0,0,390,392,1,0,
  	0,0,391,393,3,60,30,0,392,391,1,0,0,0,392,393,1,0,0,0,393,55,1,0,0,0,
  	394,395,3,74,37,0,395,396,5,13,0,0,396,57,1,0,0,0,397,398,3,74,37,0,398,
  	399,5,18,0,0,399,59,1,0,0,0,400,401,3,74,37,0,401,402,5,24,0,0,402,61,
  	1,0,0,0,403,404,3,74,37,0,404,405,5,52,0,0,405,406,3,80,40,0,406,407,
  	5,53,0,0,407,63,1,0,0,0,408,411,3,66,33,0,409,410,5,4,0,0,410,412,3,68,
  	34,0,411,409,1,0,0,0,411,412,1,0,0,0,412,65,1,0,0,0,413,414,3,70,35,0,
  	414,415,5,38,0,0,415,417,1,0,0,0,416,413,1,0,0,0,416,417,1,0,0,0,417,
  	418,1,0,0,0,418,419,3,68,34,0,419,67,1,0,0,0,420,421,3,80,40,0,421,69,
  	1,0,0,0,422,423,3,80,40,0,423,71,1,0,0,0,424,425,3,80,40,0,425,73,1,0,
  	0,0,426,427,5,58,0,0,427,75,1,0,0,0,428,429,7,6,0,0,429,77,1,0,0,0,430,
  	431,5,60,0,0,431,79,1,0,0,0,432,433,5,56,0,0,433,81,1,0,0,0,434,435,7,
  	7,0,0,435,83,1,0,0,0,45,86,88,98,102,117,120,132,141,146,155,158,165,
  	169,174,182,190,193,201,214,216,227,234,240,252,260,262,288,296,304,306,
  	311,316,327,335,337,348,355,367,374,380,386,389,392,411,416
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  streamdeclarationparserParserStaticData = staticData.release();
}

}

StreamDeclarationParser::StreamDeclarationParser(TokenStream *input) : StreamDeclarationParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

StreamDeclarationParser::StreamDeclarationParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  StreamDeclarationParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *streamdeclarationparserParserStaticData->atn, streamdeclarationparserParserStaticData->decisionToDFA, streamdeclarationparserParserStaticData->sharedContextCache, options);
}

StreamDeclarationParser::~StreamDeclarationParser() {
  delete _interpreter;
}

const atn::ATN& StreamDeclarationParser::getATN() const {
  return *streamdeclarationparserParserStaticData->atn;
}

std::string StreamDeclarationParser::getGrammarFileName() const {
  return "StreamDeclarationParser.g4";
}

const std::vector<std::string>& StreamDeclarationParser::getRuleNames() const {
  return streamdeclarationparserParserStaticData->ruleNames;
}

const dfa::Vocabulary& StreamDeclarationParser::getVocabulary() const {
  return streamdeclarationparserParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView StreamDeclarationParser::getSerializedATN() const {
  return streamdeclarationparserParserStaticData->serializedATN;
}


//----------------- ParseContext ------------------------------------------------------------------

StreamDeclarationParser::ParseContext::ParseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* StreamDeclarationParser::ParseContext::EOF() {
  return getToken(StreamDeclarationParser::EOF, 0);
}

std::vector<StreamDeclarationParser::Core_stmtContext *> StreamDeclarationParser::ParseContext::core_stmt() {
  return getRuleContexts<StreamDeclarationParser::Core_stmtContext>();
}

StreamDeclarationParser::Core_stmtContext* StreamDeclarationParser::ParseContext::core_stmt(size_t i) {
  return getRuleContext<StreamDeclarationParser::Core_stmtContext>(i);
}

std::vector<StreamDeclarationParser::ErrorContext *> StreamDeclarationParser::ParseContext::error() {
  return getRuleContexts<StreamDeclarationParser::ErrorContext>();
}

StreamDeclarationParser::ErrorContext* StreamDeclarationParser::ParseContext::error(size_t i) {
  return getRuleContext<StreamDeclarationParser::ErrorContext>(i);
}


size_t StreamDeclarationParser::ParseContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleParse;
}


std::any StreamDeclarationParser::ParseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitParse(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::ParseContext* StreamDeclarationParser::parse() {
  ParseContext *_localctx = _tracker.createInstance<ParseContext>(_ctx, getState());
  enterRule(_localctx, 0, StreamDeclarationParser::RuleParse);
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
    setState(88);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 7) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 7)) & 288230376151973889) != 0)) {
      setState(86);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case StreamDeclarationParser::K_DECLARE:
        case StreamDeclarationParser::K_SELECT: {
          setState(84);
          core_stmt();
          break;
        }

        case StreamDeclarationParser::UNEXPECTED_CHAR: {
          setState(85);
          error();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(90);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(91);
    match(StreamDeclarationParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ErrorContext ------------------------------------------------------------------

StreamDeclarationParser::ErrorContext::ErrorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* StreamDeclarationParser::ErrorContext::UNEXPECTED_CHAR() {
  return getToken(StreamDeclarationParser::UNEXPECTED_CHAR, 0);
}


size_t StreamDeclarationParser::ErrorContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleError;
}


std::any StreamDeclarationParser::ErrorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitError(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::ErrorContext* StreamDeclarationParser::error() {
  ErrorContext *_localctx = _tracker.createInstance<ErrorContext>(_ctx, getState());
  enterRule(_localctx, 2, StreamDeclarationParser::RuleError);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(93);
    antlrcpp::downCast<ErrorContext *>(_localctx)->unexpected_charToken = match(StreamDeclarationParser::UNEXPECTED_CHAR);

         throw new RuntimeException("UNEXPECTED_CHAR=" + (antlrcpp::downCast<ErrorContext *>(_localctx)->unexpected_charToken != nullptr ? antlrcpp::downCast<ErrorContext *>(_localctx)->unexpected_charToken->getText() : ""));
       
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Core_stmtContext ------------------------------------------------------------------

StreamDeclarationParser::Core_stmtContext::Core_stmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

StreamDeclarationParser::Core_queryContext* StreamDeclarationParser::Core_stmtContext::core_query() {
  return getRuleContext<StreamDeclarationParser::Core_queryContext>(0);
}

StreamDeclarationParser::Core_declarationContext* StreamDeclarationParser::Core_stmtContext::core_declaration() {
  return getRuleContext<StreamDeclarationParser::Core_declarationContext>(0);
}


size_t StreamDeclarationParser::Core_stmtContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleCore_stmt;
}


std::any StreamDeclarationParser::Core_stmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitCore_stmt(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::Core_stmtContext* StreamDeclarationParser::core_stmt() {
  Core_stmtContext *_localctx = _tracker.createInstance<Core_stmtContext>(_ctx, getState());
  enterRule(_localctx, 4, StreamDeclarationParser::RuleCore_stmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(98);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case StreamDeclarationParser::K_SELECT: {
        enterOuterAlt(_localctx, 1);
        setState(96);
        core_query();
        break;
      }

      case StreamDeclarationParser::K_DECLARE: {
        enterOuterAlt(_localctx, 2);
        setState(97);
        core_declaration();
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

//----------------- Core_declarationContext ------------------------------------------------------------------

StreamDeclarationParser::Core_declarationContext::Core_declarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

StreamDeclarationParser::Event_declarationContext* StreamDeclarationParser::Core_declarationContext::event_declaration() {
  return getRuleContext<StreamDeclarationParser::Event_declarationContext>(0);
}

StreamDeclarationParser::Stream_declarationContext* StreamDeclarationParser::Core_declarationContext::stream_declaration() {
  return getRuleContext<StreamDeclarationParser::Stream_declarationContext>(0);
}


size_t StreamDeclarationParser::Core_declarationContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleCore_declaration;
}


std::any StreamDeclarationParser::Core_declarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitCore_declaration(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::Core_declarationContext* StreamDeclarationParser::core_declaration() {
  Core_declarationContext *_localctx = _tracker.createInstance<Core_declarationContext>(_ctx, getState());
  enterRule(_localctx, 6, StreamDeclarationParser::RuleCore_declaration);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(102);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(100);
      event_declaration();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(101);
      stream_declaration();
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

//----------------- Event_declarationContext ------------------------------------------------------------------

StreamDeclarationParser::Event_declarationContext::Event_declarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* StreamDeclarationParser::Event_declarationContext::K_DECLARE() {
  return getToken(StreamDeclarationParser::K_DECLARE, 0);
}

tree::TerminalNode* StreamDeclarationParser::Event_declarationContext::K_EVENT() {
  return getToken(StreamDeclarationParser::K_EVENT, 0);
}

StreamDeclarationParser::Event_nameContext* StreamDeclarationParser::Event_declarationContext::event_name() {
  return getRuleContext<StreamDeclarationParser::Event_nameContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::Event_declarationContext::LEFT_PARENTHESIS() {
  return getToken(StreamDeclarationParser::LEFT_PARENTHESIS, 0);
}

StreamDeclarationParser::Attribute_dec_listContext* StreamDeclarationParser::Event_declarationContext::attribute_dec_list() {
  return getRuleContext<StreamDeclarationParser::Attribute_dec_listContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::Event_declarationContext::RIGHT_PARENTHESIS() {
  return getToken(StreamDeclarationParser::RIGHT_PARENTHESIS, 0);
}


size_t StreamDeclarationParser::Event_declarationContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleEvent_declaration;
}


std::any StreamDeclarationParser::Event_declarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitEvent_declaration(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::Event_declarationContext* StreamDeclarationParser::event_declaration() {
  Event_declarationContext *_localctx = _tracker.createInstance<Event_declarationContext>(_ctx, getState());
  enterRule(_localctx, 8, StreamDeclarationParser::RuleEvent_declaration);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(104);
    match(StreamDeclarationParser::K_DECLARE);
    setState(105);
    match(StreamDeclarationParser::K_EVENT);
    setState(106);
    event_name();
    setState(107);
    match(StreamDeclarationParser::LEFT_PARENTHESIS);
    setState(108);
    attribute_dec_list();
    setState(109);
    match(StreamDeclarationParser::RIGHT_PARENTHESIS);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Attribute_dec_listContext ------------------------------------------------------------------

StreamDeclarationParser::Attribute_dec_listContext::Attribute_dec_listContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<StreamDeclarationParser::Attribute_declarationContext *> StreamDeclarationParser::Attribute_dec_listContext::attribute_declaration() {
  return getRuleContexts<StreamDeclarationParser::Attribute_declarationContext>();
}

StreamDeclarationParser::Attribute_declarationContext* StreamDeclarationParser::Attribute_dec_listContext::attribute_declaration(size_t i) {
  return getRuleContext<StreamDeclarationParser::Attribute_declarationContext>(i);
}

std::vector<tree::TerminalNode *> StreamDeclarationParser::Attribute_dec_listContext::COMMA() {
  return getTokens(StreamDeclarationParser::COMMA);
}

tree::TerminalNode* StreamDeclarationParser::Attribute_dec_listContext::COMMA(size_t i) {
  return getToken(StreamDeclarationParser::COMMA, i);
}


size_t StreamDeclarationParser::Attribute_dec_listContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleAttribute_dec_list;
}


std::any StreamDeclarationParser::Attribute_dec_listContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitAttribute_dec_list(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::Attribute_dec_listContext* StreamDeclarationParser::attribute_dec_list() {
  Attribute_dec_listContext *_localctx = _tracker.createInstance<Attribute_dec_listContext>(_ctx, getState());
  enterRule(_localctx, 10, StreamDeclarationParser::RuleAttribute_dec_list);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(120);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case StreamDeclarationParser::RIGHT_PARENTHESIS: {
        enterOuterAlt(_localctx, 1);

        break;
      }

      case StreamDeclarationParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 2);
        setState(112);
        attribute_declaration();
        setState(117);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == StreamDeclarationParser::COMMA) {
          setState(113);
          match(StreamDeclarationParser::COMMA);
          setState(114);
          attribute_declaration();
          setState(119);
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

//----------------- Attribute_declarationContext ------------------------------------------------------------------

StreamDeclarationParser::Attribute_declarationContext::Attribute_declarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

StreamDeclarationParser::Attribute_nameContext* StreamDeclarationParser::Attribute_declarationContext::attribute_name() {
  return getRuleContext<StreamDeclarationParser::Attribute_nameContext>(0);
}

StreamDeclarationParser::DatatypeContext* StreamDeclarationParser::Attribute_declarationContext::datatype() {
  return getRuleContext<StreamDeclarationParser::DatatypeContext>(0);
}


size_t StreamDeclarationParser::Attribute_declarationContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleAttribute_declaration;
}


std::any StreamDeclarationParser::Attribute_declarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitAttribute_declaration(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::Attribute_declarationContext* StreamDeclarationParser::attribute_declaration() {
  Attribute_declarationContext *_localctx = _tracker.createInstance<Attribute_declarationContext>(_ctx, getState());
  enterRule(_localctx, 12, StreamDeclarationParser::RuleAttribute_declaration);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(122);
    attribute_name();
    setState(123);
    datatype();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DatatypeContext ------------------------------------------------------------------

StreamDeclarationParser::DatatypeContext::DatatypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* StreamDeclarationParser::DatatypeContext::INT() {
  return getToken(StreamDeclarationParser::INT, 0);
}

tree::TerminalNode* StreamDeclarationParser::DatatypeContext::STRING() {
  return getToken(StreamDeclarationParser::STRING, 0);
}

tree::TerminalNode* StreamDeclarationParser::DatatypeContext::DOUBLE() {
  return getToken(StreamDeclarationParser::DOUBLE, 0);
}

tree::TerminalNode* StreamDeclarationParser::DatatypeContext::LONG() {
  return getToken(StreamDeclarationParser::LONG, 0);
}

tree::TerminalNode* StreamDeclarationParser::DatatypeContext::BOOLEAN() {
  return getToken(StreamDeclarationParser::BOOLEAN, 0);
}

tree::TerminalNode* StreamDeclarationParser::DatatypeContext::IDENTIFIER() {
  return getToken(StreamDeclarationParser::IDENTIFIER, 0);
}


size_t StreamDeclarationParser::DatatypeContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleDatatype;
}


std::any StreamDeclarationParser::DatatypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitDatatype(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::DatatypeContext* StreamDeclarationParser::datatype() {
  DatatypeContext *_localctx = _tracker.createInstance<DatatypeContext>(_ctx, getState());
  enterRule(_localctx, 14, StreamDeclarationParser::RuleDatatype);
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
    setState(125);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 72193933479772160) != 0))) {
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

//----------------- Stream_declarationContext ------------------------------------------------------------------

StreamDeclarationParser::Stream_declarationContext::Stream_declarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* StreamDeclarationParser::Stream_declarationContext::K_DECLARE() {
  return getToken(StreamDeclarationParser::K_DECLARE, 0);
}

tree::TerminalNode* StreamDeclarationParser::Stream_declarationContext::K_STREAM() {
  return getToken(StreamDeclarationParser::K_STREAM, 0);
}

StreamDeclarationParser::Stream_nameContext* StreamDeclarationParser::Stream_declarationContext::stream_name() {
  return getRuleContext<StreamDeclarationParser::Stream_nameContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::Stream_declarationContext::LEFT_PARENTHESIS() {
  return getToken(StreamDeclarationParser::LEFT_PARENTHESIS, 0);
}

tree::TerminalNode* StreamDeclarationParser::Stream_declarationContext::RIGHT_PARENTHESIS() {
  return getToken(StreamDeclarationParser::RIGHT_PARENTHESIS, 0);
}

StreamDeclarationParser::Event_listContext* StreamDeclarationParser::Stream_declarationContext::event_list() {
  return getRuleContext<StreamDeclarationParser::Event_listContext>(0);
}


size_t StreamDeclarationParser::Stream_declarationContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleStream_declaration;
}


std::any StreamDeclarationParser::Stream_declarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitStream_declaration(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::Stream_declarationContext* StreamDeclarationParser::stream_declaration() {
  Stream_declarationContext *_localctx = _tracker.createInstance<Stream_declarationContext>(_ctx, getState());
  enterRule(_localctx, 16, StreamDeclarationParser::RuleStream_declaration);
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
    setState(127);
    match(StreamDeclarationParser::K_DECLARE);
    setState(128);
    match(StreamDeclarationParser::K_STREAM);
    setState(129);
    stream_name();
    setState(130);
    match(StreamDeclarationParser::LEFT_PARENTHESIS);
    setState(132);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == StreamDeclarationParser::IDENTIFIER) {
      setState(131);
      event_list();
    }
    setState(134);
    match(StreamDeclarationParser::RIGHT_PARENTHESIS);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Event_listContext ------------------------------------------------------------------

StreamDeclarationParser::Event_listContext::Event_listContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<StreamDeclarationParser::Event_nameContext *> StreamDeclarationParser::Event_listContext::event_name() {
  return getRuleContexts<StreamDeclarationParser::Event_nameContext>();
}

StreamDeclarationParser::Event_nameContext* StreamDeclarationParser::Event_listContext::event_name(size_t i) {
  return getRuleContext<StreamDeclarationParser::Event_nameContext>(i);
}

std::vector<tree::TerminalNode *> StreamDeclarationParser::Event_listContext::COMMA() {
  return getTokens(StreamDeclarationParser::COMMA);
}

tree::TerminalNode* StreamDeclarationParser::Event_listContext::COMMA(size_t i) {
  return getToken(StreamDeclarationParser::COMMA, i);
}


size_t StreamDeclarationParser::Event_listContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleEvent_list;
}


std::any StreamDeclarationParser::Event_listContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitEvent_list(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::Event_listContext* StreamDeclarationParser::event_list() {
  Event_listContext *_localctx = _tracker.createInstance<Event_listContext>(_ctx, getState());
  enterRule(_localctx, 18, StreamDeclarationParser::RuleEvent_list);
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
    event_name();
    setState(141);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == StreamDeclarationParser::COMMA) {
      setState(137);
      match(StreamDeclarationParser::COMMA);
      setState(138);
      event_name();
      setState(143);
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

//----------------- Core_queryContext ------------------------------------------------------------------

StreamDeclarationParser::Core_queryContext::Core_queryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* StreamDeclarationParser::Core_queryContext::K_SELECT() {
  return getToken(StreamDeclarationParser::K_SELECT, 0);
}

StreamDeclarationParser::Result_valuesContext* StreamDeclarationParser::Core_queryContext::result_values() {
  return getRuleContext<StreamDeclarationParser::Result_valuesContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::Core_queryContext::K_WHERE() {
  return getToken(StreamDeclarationParser::K_WHERE, 0);
}

StreamDeclarationParser::Core_patternContext* StreamDeclarationParser::Core_queryContext::core_pattern() {
  return getRuleContext<StreamDeclarationParser::Core_patternContext>(0);
}

StreamDeclarationParser::Selection_strategyContext* StreamDeclarationParser::Core_queryContext::selection_strategy() {
  return getRuleContext<StreamDeclarationParser::Selection_strategyContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::Core_queryContext::K_FROM() {
  return getToken(StreamDeclarationParser::K_FROM, 0);
}

std::vector<StreamDeclarationParser::Stream_nameContext *> StreamDeclarationParser::Core_queryContext::stream_name() {
  return getRuleContexts<StreamDeclarationParser::Stream_nameContext>();
}

StreamDeclarationParser::Stream_nameContext* StreamDeclarationParser::Core_queryContext::stream_name(size_t i) {
  return getRuleContext<StreamDeclarationParser::Stream_nameContext>(i);
}

tree::TerminalNode* StreamDeclarationParser::Core_queryContext::K_PARTITION() {
  return getToken(StreamDeclarationParser::K_PARTITION, 0);
}

std::vector<tree::TerminalNode *> StreamDeclarationParser::Core_queryContext::K_BY() {
  return getTokens(StreamDeclarationParser::K_BY);
}

tree::TerminalNode* StreamDeclarationParser::Core_queryContext::K_BY(size_t i) {
  return getToken(StreamDeclarationParser::K_BY, i);
}

StreamDeclarationParser::Partition_listContext* StreamDeclarationParser::Core_queryContext::partition_list() {
  return getRuleContext<StreamDeclarationParser::Partition_listContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::Core_queryContext::K_WITHIN() {
  return getToken(StreamDeclarationParser::K_WITHIN, 0);
}

StreamDeclarationParser::Time_windowContext* StreamDeclarationParser::Core_queryContext::time_window() {
  return getRuleContext<StreamDeclarationParser::Time_windowContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::Core_queryContext::K_CONSUME() {
  return getToken(StreamDeclarationParser::K_CONSUME, 0);
}

StreamDeclarationParser::Consumption_policyContext* StreamDeclarationParser::Core_queryContext::consumption_policy() {
  return getRuleContext<StreamDeclarationParser::Consumption_policyContext>(0);
}

std::vector<tree::TerminalNode *> StreamDeclarationParser::Core_queryContext::COMMA() {
  return getTokens(StreamDeclarationParser::COMMA);
}

tree::TerminalNode* StreamDeclarationParser::Core_queryContext::COMMA(size_t i) {
  return getToken(StreamDeclarationParser::COMMA, i);
}


size_t StreamDeclarationParser::Core_queryContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleCore_query;
}


std::any StreamDeclarationParser::Core_queryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitCore_query(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::Core_queryContext* StreamDeclarationParser::core_query() {
  Core_queryContext *_localctx = _tracker.createInstance<Core_queryContext>(_ctx, getState());
  enterRule(_localctx, 20, StreamDeclarationParser::RuleCore_query);
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
    setState(144);
    match(StreamDeclarationParser::K_SELECT);
    setState(146);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 134905866) != 0)) {
      setState(145);
      selection_strategy();
    }
    setState(148);
    result_values();
    setState(158);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == StreamDeclarationParser::K_FROM) {
      setState(149);
      match(StreamDeclarationParser::K_FROM);
      setState(150);
      stream_name();
      setState(155);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == StreamDeclarationParser::COMMA) {
        setState(151);
        match(StreamDeclarationParser::COMMA);
        setState(152);
        stream_name();
        setState(157);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(160);
    match(StreamDeclarationParser::K_WHERE);
    setState(161);
    core_pattern(0);
    setState(165);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == StreamDeclarationParser::K_PARTITION) {
      setState(162);
      match(StreamDeclarationParser::K_PARTITION);
      setState(163);
      match(StreamDeclarationParser::K_BY);
      setState(164);
      partition_list();
    }
    setState(169);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == StreamDeclarationParser::K_WITHIN) {
      setState(167);
      match(StreamDeclarationParser::K_WITHIN);
      setState(168);
      time_window();
    }
    setState(174);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == StreamDeclarationParser::K_CONSUME) {
      setState(171);
      match(StreamDeclarationParser::K_CONSUME);
      setState(172);
      match(StreamDeclarationParser::K_BY);
      setState(173);
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

StreamDeclarationParser::Selection_strategyContext::Selection_strategyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t StreamDeclarationParser::Selection_strategyContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleSelection_strategy;
}

void StreamDeclarationParser::Selection_strategyContext::copyFrom(Selection_strategyContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Ss_lastContext ------------------------------------------------------------------

tree::TerminalNode* StreamDeclarationParser::Ss_lastContext::K_LAST() {
  return getToken(StreamDeclarationParser::K_LAST, 0);
}

StreamDeclarationParser::Ss_lastContext::Ss_lastContext(Selection_strategyContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Ss_lastContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitSs_last(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Ss_strictContext ------------------------------------------------------------------

tree::TerminalNode* StreamDeclarationParser::Ss_strictContext::K_STRICT() {
  return getToken(StreamDeclarationParser::K_STRICT, 0);
}

StreamDeclarationParser::Ss_strictContext::Ss_strictContext(Selection_strategyContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Ss_strictContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitSs_strict(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Ss_anyContext ------------------------------------------------------------------

tree::TerminalNode* StreamDeclarationParser::Ss_anyContext::K_ANY() {
  return getToken(StreamDeclarationParser::K_ANY, 0);
}

StreamDeclarationParser::Ss_anyContext::Ss_anyContext(Selection_strategyContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Ss_anyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitSs_any(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Ss_nextContext ------------------------------------------------------------------

tree::TerminalNode* StreamDeclarationParser::Ss_nextContext::K_NEXT() {
  return getToken(StreamDeclarationParser::K_NEXT, 0);
}

StreamDeclarationParser::Ss_nextContext::Ss_nextContext(Selection_strategyContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Ss_nextContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitSs_next(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Ss_maxContext ------------------------------------------------------------------

tree::TerminalNode* StreamDeclarationParser::Ss_maxContext::K_MAX() {
  return getToken(StreamDeclarationParser::K_MAX, 0);
}

StreamDeclarationParser::Ss_maxContext::Ss_maxContext(Selection_strategyContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Ss_maxContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitSs_max(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Ss_allContext ------------------------------------------------------------------

tree::TerminalNode* StreamDeclarationParser::Ss_allContext::K_ALL() {
  return getToken(StreamDeclarationParser::K_ALL, 0);
}

StreamDeclarationParser::Ss_allContext::Ss_allContext(Selection_strategyContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Ss_allContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitSs_all(this);
  else
    return visitor->visitChildren(this);
}
StreamDeclarationParser::Selection_strategyContext* StreamDeclarationParser::selection_strategy() {
  Selection_strategyContext *_localctx = _tracker.createInstance<Selection_strategyContext>(_ctx, getState());
  enterRule(_localctx, 22, StreamDeclarationParser::RuleSelection_strategy);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(182);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case StreamDeclarationParser::K_ALL: {
        _localctx = _tracker.createInstance<StreamDeclarationParser::Ss_allContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(176);
        match(StreamDeclarationParser::K_ALL);
        break;
      }

      case StreamDeclarationParser::K_ANY: {
        _localctx = _tracker.createInstance<StreamDeclarationParser::Ss_anyContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(177);
        match(StreamDeclarationParser::K_ANY);
        break;
      }

      case StreamDeclarationParser::K_LAST: {
        _localctx = _tracker.createInstance<StreamDeclarationParser::Ss_lastContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(178);
        match(StreamDeclarationParser::K_LAST);
        break;
      }

      case StreamDeclarationParser::K_MAX: {
        _localctx = _tracker.createInstance<StreamDeclarationParser::Ss_maxContext>(_localctx);
        enterOuterAlt(_localctx, 4);
        setState(179);
        match(StreamDeclarationParser::K_MAX);
        break;
      }

      case StreamDeclarationParser::K_NEXT: {
        _localctx = _tracker.createInstance<StreamDeclarationParser::Ss_nextContext>(_localctx);
        enterOuterAlt(_localctx, 5);
        setState(180);
        match(StreamDeclarationParser::K_NEXT);
        break;
      }

      case StreamDeclarationParser::K_STRICT: {
        _localctx = _tracker.createInstance<StreamDeclarationParser::Ss_strictContext>(_localctx);
        enterOuterAlt(_localctx, 6);
        setState(181);
        match(StreamDeclarationParser::K_STRICT);
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

StreamDeclarationParser::Result_valuesContext::Result_valuesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* StreamDeclarationParser::Result_valuesContext::STAR() {
  return getToken(StreamDeclarationParser::STAR, 0);
}

std::vector<StreamDeclarationParser::S_event_nameContext *> StreamDeclarationParser::Result_valuesContext::s_event_name() {
  return getRuleContexts<StreamDeclarationParser::S_event_nameContext>();
}

StreamDeclarationParser::S_event_nameContext* StreamDeclarationParser::Result_valuesContext::s_event_name(size_t i) {
  return getRuleContext<StreamDeclarationParser::S_event_nameContext>(i);
}

std::vector<tree::TerminalNode *> StreamDeclarationParser::Result_valuesContext::COMMA() {
  return getTokens(StreamDeclarationParser::COMMA);
}

tree::TerminalNode* StreamDeclarationParser::Result_valuesContext::COMMA(size_t i) {
  return getToken(StreamDeclarationParser::COMMA, i);
}


size_t StreamDeclarationParser::Result_valuesContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleResult_values;
}


std::any StreamDeclarationParser::Result_valuesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitResult_values(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::Result_valuesContext* StreamDeclarationParser::result_values() {
  Result_valuesContext *_localctx = _tracker.createInstance<Result_valuesContext>(_ctx, getState());
  enterRule(_localctx, 24, StreamDeclarationParser::RuleResult_values);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(193);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case StreamDeclarationParser::STAR: {
        enterOuterAlt(_localctx, 1);
        setState(184);
        match(StreamDeclarationParser::STAR);
        break;
      }

      case StreamDeclarationParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 2);
        setState(185);
        s_event_name();
        setState(190);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == StreamDeclarationParser::COMMA) {
          setState(186);
          match(StreamDeclarationParser::COMMA);
          setState(187);
          s_event_name();
          setState(192);
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

StreamDeclarationParser::Core_patternContext::Core_patternContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t StreamDeclarationParser::Core_patternContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleCore_pattern;
}

void StreamDeclarationParser::Core_patternContext::copyFrom(Core_patternContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Event_core_patternContext ------------------------------------------------------------------

StreamDeclarationParser::S_event_nameContext* StreamDeclarationParser::Event_core_patternContext::s_event_name() {
  return getRuleContext<StreamDeclarationParser::S_event_nameContext>(0);
}

StreamDeclarationParser::Event_core_patternContext::Event_core_patternContext(Core_patternContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Event_core_patternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitEvent_core_pattern(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Kleene_core_patternContext ------------------------------------------------------------------

StreamDeclarationParser::Core_patternContext* StreamDeclarationParser::Kleene_core_patternContext::core_pattern() {
  return getRuleContext<StreamDeclarationParser::Core_patternContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::Kleene_core_patternContext::PLUS() {
  return getToken(StreamDeclarationParser::PLUS, 0);
}

StreamDeclarationParser::Kleene_core_patternContext::Kleene_core_patternContext(Core_patternContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Kleene_core_patternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitKleene_core_pattern(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Assign_core_patternContext ------------------------------------------------------------------

StreamDeclarationParser::Core_patternContext* StreamDeclarationParser::Assign_core_patternContext::core_pattern() {
  return getRuleContext<StreamDeclarationParser::Core_patternContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::Assign_core_patternContext::K_AS() {
  return getToken(StreamDeclarationParser::K_AS, 0);
}

StreamDeclarationParser::Event_nameContext* StreamDeclarationParser::Assign_core_patternContext::event_name() {
  return getRuleContext<StreamDeclarationParser::Event_nameContext>(0);
}

StreamDeclarationParser::Assign_core_patternContext::Assign_core_patternContext(Core_patternContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Assign_core_patternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitAssign_core_pattern(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Binary_core_patternContext ------------------------------------------------------------------

std::vector<StreamDeclarationParser::Core_patternContext *> StreamDeclarationParser::Binary_core_patternContext::core_pattern() {
  return getRuleContexts<StreamDeclarationParser::Core_patternContext>();
}

StreamDeclarationParser::Core_patternContext* StreamDeclarationParser::Binary_core_patternContext::core_pattern(size_t i) {
  return getRuleContext<StreamDeclarationParser::Core_patternContext>(i);
}

tree::TerminalNode* StreamDeclarationParser::Binary_core_patternContext::K_OR() {
  return getToken(StreamDeclarationParser::K_OR, 0);
}

tree::TerminalNode* StreamDeclarationParser::Binary_core_patternContext::SEMICOLON() {
  return getToken(StreamDeclarationParser::SEMICOLON, 0);
}

StreamDeclarationParser::Binary_core_patternContext::Binary_core_patternContext(Core_patternContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Binary_core_patternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitBinary_core_pattern(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Par_core_patternContext ------------------------------------------------------------------

tree::TerminalNode* StreamDeclarationParser::Par_core_patternContext::LEFT_PARENTHESIS() {
  return getToken(StreamDeclarationParser::LEFT_PARENTHESIS, 0);
}

StreamDeclarationParser::Core_patternContext* StreamDeclarationParser::Par_core_patternContext::core_pattern() {
  return getRuleContext<StreamDeclarationParser::Core_patternContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::Par_core_patternContext::RIGHT_PARENTHESIS() {
  return getToken(StreamDeclarationParser::RIGHT_PARENTHESIS, 0);
}

StreamDeclarationParser::Par_core_patternContext::Par_core_patternContext(Core_patternContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Par_core_patternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitPar_core_pattern(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Filter_core_patternContext ------------------------------------------------------------------

StreamDeclarationParser::Core_patternContext* StreamDeclarationParser::Filter_core_patternContext::core_pattern() {
  return getRuleContext<StreamDeclarationParser::Core_patternContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::Filter_core_patternContext::K_FILTER() {
  return getToken(StreamDeclarationParser::K_FILTER, 0);
}

StreamDeclarationParser::FilterContext* StreamDeclarationParser::Filter_core_patternContext::filter() {
  return getRuleContext<StreamDeclarationParser::FilterContext>(0);
}

StreamDeclarationParser::Filter_core_patternContext::Filter_core_patternContext(Core_patternContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Filter_core_patternContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitFilter_core_pattern(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::Core_patternContext* StreamDeclarationParser::core_pattern() {
   return core_pattern(0);
}

StreamDeclarationParser::Core_patternContext* StreamDeclarationParser::core_pattern(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  StreamDeclarationParser::Core_patternContext *_localctx = _tracker.createInstance<Core_patternContext>(_ctx, parentState);
  StreamDeclarationParser::Core_patternContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 26;
  enterRecursionRule(_localctx, 26, StreamDeclarationParser::RuleCore_pattern, precedence);

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
    setState(201);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case StreamDeclarationParser::LEFT_PARENTHESIS: {
        _localctx = _tracker.createInstance<Par_core_patternContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(196);
        match(StreamDeclarationParser::LEFT_PARENTHESIS);
        setState(197);
        core_pattern(0);
        setState(198);
        match(StreamDeclarationParser::RIGHT_PARENTHESIS);
        break;
      }

      case StreamDeclarationParser::IDENTIFIER: {
        _localctx = _tracker.createInstance<Event_core_patternContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(200);
        s_event_name();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(216);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 19, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(214);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 18, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<Binary_core_patternContext>(_tracker.createInstance<Core_patternContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleCore_pattern);
          setState(203);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(204);
          _la = _input->LA(1);
          if (!(_la == StreamDeclarationParser::K_OR

          || _la == StreamDeclarationParser::SEMICOLON)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(205);
          core_pattern(3);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<Assign_core_patternContext>(_tracker.createInstance<Core_patternContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleCore_pattern);
          setState(206);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(207);
          match(StreamDeclarationParser::K_AS);
          setState(208);
          event_name();
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<Kleene_core_patternContext>(_tracker.createInstance<Core_patternContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleCore_pattern);
          setState(209);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(210);
          match(StreamDeclarationParser::PLUS);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<Filter_core_patternContext>(_tracker.createInstance<Core_patternContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleCore_pattern);
          setState(211);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(212);
          match(StreamDeclarationParser::K_FILTER);
          setState(213);
          filter(0);
          break;
        }

        default:
          break;
        } 
      }
      setState(218);
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

//----------------- Partition_listContext ------------------------------------------------------------------

StreamDeclarationParser::Partition_listContext::Partition_listContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> StreamDeclarationParser::Partition_listContext::LEFT_SQUARE_BRACKET() {
  return getTokens(StreamDeclarationParser::LEFT_SQUARE_BRACKET);
}

tree::TerminalNode* StreamDeclarationParser::Partition_listContext::LEFT_SQUARE_BRACKET(size_t i) {
  return getToken(StreamDeclarationParser::LEFT_SQUARE_BRACKET, i);
}

std::vector<StreamDeclarationParser::Attribute_listContext *> StreamDeclarationParser::Partition_listContext::attribute_list() {
  return getRuleContexts<StreamDeclarationParser::Attribute_listContext>();
}

StreamDeclarationParser::Attribute_listContext* StreamDeclarationParser::Partition_listContext::attribute_list(size_t i) {
  return getRuleContext<StreamDeclarationParser::Attribute_listContext>(i);
}

std::vector<tree::TerminalNode *> StreamDeclarationParser::Partition_listContext::RIGHT_SQUARE_BRACKET() {
  return getTokens(StreamDeclarationParser::RIGHT_SQUARE_BRACKET);
}

tree::TerminalNode* StreamDeclarationParser::Partition_listContext::RIGHT_SQUARE_BRACKET(size_t i) {
  return getToken(StreamDeclarationParser::RIGHT_SQUARE_BRACKET, i);
}

tree::TerminalNode* StreamDeclarationParser::Partition_listContext::COMMA() {
  return getToken(StreamDeclarationParser::COMMA, 0);
}


size_t StreamDeclarationParser::Partition_listContext::getRuleIndex() const {
  return StreamDeclarationParser::RulePartition_list;
}


std::any StreamDeclarationParser::Partition_listContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitPartition_list(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::Partition_listContext* StreamDeclarationParser::partition_list() {
  Partition_listContext *_localctx = _tracker.createInstance<Partition_listContext>(_ctx, getState());
  enterRule(_localctx, 28, StreamDeclarationParser::RulePartition_list);
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
    setState(219);
    match(StreamDeclarationParser::LEFT_SQUARE_BRACKET);
    setState(220);
    attribute_list();
    setState(221);
    match(StreamDeclarationParser::RIGHT_SQUARE_BRACKET);
    setState(227);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == StreamDeclarationParser::COMMA) {
      setState(222);
      match(StreamDeclarationParser::COMMA);
      setState(223);
      match(StreamDeclarationParser::LEFT_SQUARE_BRACKET);
      setState(224);
      attribute_list();
      setState(225);
      match(StreamDeclarationParser::RIGHT_SQUARE_BRACKET);
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

StreamDeclarationParser::Attribute_listContext::Attribute_listContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<StreamDeclarationParser::Attribute_nameContext *> StreamDeclarationParser::Attribute_listContext::attribute_name() {
  return getRuleContexts<StreamDeclarationParser::Attribute_nameContext>();
}

StreamDeclarationParser::Attribute_nameContext* StreamDeclarationParser::Attribute_listContext::attribute_name(size_t i) {
  return getRuleContext<StreamDeclarationParser::Attribute_nameContext>(i);
}

std::vector<tree::TerminalNode *> StreamDeclarationParser::Attribute_listContext::COMMA() {
  return getTokens(StreamDeclarationParser::COMMA);
}

tree::TerminalNode* StreamDeclarationParser::Attribute_listContext::COMMA(size_t i) {
  return getToken(StreamDeclarationParser::COMMA, i);
}


size_t StreamDeclarationParser::Attribute_listContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleAttribute_list;
}


std::any StreamDeclarationParser::Attribute_listContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitAttribute_list(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::Attribute_listContext* StreamDeclarationParser::attribute_list() {
  Attribute_listContext *_localctx = _tracker.createInstance<Attribute_listContext>(_ctx, getState());
  enterRule(_localctx, 30, StreamDeclarationParser::RuleAttribute_list);
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
    setState(229);
    attribute_name();
    setState(234);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == StreamDeclarationParser::COMMA) {
      setState(230);
      match(StreamDeclarationParser::COMMA);
      setState(231);
      attribute_name();
      setState(236);
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

StreamDeclarationParser::Consumption_policyContext::Consumption_policyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t StreamDeclarationParser::Consumption_policyContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleConsumption_policy;
}

void StreamDeclarationParser::Consumption_policyContext::copyFrom(Consumption_policyContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Cp_partitionContext ------------------------------------------------------------------

tree::TerminalNode* StreamDeclarationParser::Cp_partitionContext::K_PARTITION() {
  return getToken(StreamDeclarationParser::K_PARTITION, 0);
}

StreamDeclarationParser::Cp_partitionContext::Cp_partitionContext(Consumption_policyContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Cp_partitionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitCp_partition(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Cp_anyContext ------------------------------------------------------------------

tree::TerminalNode* StreamDeclarationParser::Cp_anyContext::K_ANY() {
  return getToken(StreamDeclarationParser::K_ANY, 0);
}

StreamDeclarationParser::Cp_anyContext::Cp_anyContext(Consumption_policyContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Cp_anyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitCp_any(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Cp_noneContext ------------------------------------------------------------------

tree::TerminalNode* StreamDeclarationParser::Cp_noneContext::K_NONE() {
  return getToken(StreamDeclarationParser::K_NONE, 0);
}

StreamDeclarationParser::Cp_noneContext::Cp_noneContext(Consumption_policyContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Cp_noneContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitCp_none(this);
  else
    return visitor->visitChildren(this);
}
StreamDeclarationParser::Consumption_policyContext* StreamDeclarationParser::consumption_policy() {
  Consumption_policyContext *_localctx = _tracker.createInstance<Consumption_policyContext>(_ctx, getState());
  enterRule(_localctx, 32, StreamDeclarationParser::RuleConsumption_policy);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(240);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case StreamDeclarationParser::K_ANY: {
        _localctx = _tracker.createInstance<StreamDeclarationParser::Cp_anyContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(237);
        match(StreamDeclarationParser::K_ANY);
        break;
      }

      case StreamDeclarationParser::K_PARTITION: {
        _localctx = _tracker.createInstance<StreamDeclarationParser::Cp_partitionContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(238);
        match(StreamDeclarationParser::K_PARTITION);
        break;
      }

      case StreamDeclarationParser::K_NONE: {
        _localctx = _tracker.createInstance<StreamDeclarationParser::Cp_noneContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(239);
        match(StreamDeclarationParser::K_NONE);
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

StreamDeclarationParser::FilterContext::FilterContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t StreamDeclarationParser::FilterContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleFilter;
}

void StreamDeclarationParser::FilterContext::copyFrom(FilterContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Par_filterContext ------------------------------------------------------------------

tree::TerminalNode* StreamDeclarationParser::Par_filterContext::LEFT_PARENTHESIS() {
  return getToken(StreamDeclarationParser::LEFT_PARENTHESIS, 0);
}

StreamDeclarationParser::FilterContext* StreamDeclarationParser::Par_filterContext::filter() {
  return getRuleContext<StreamDeclarationParser::FilterContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::Par_filterContext::RIGHT_PARENTHESIS() {
  return getToken(StreamDeclarationParser::RIGHT_PARENTHESIS, 0);
}

StreamDeclarationParser::Par_filterContext::Par_filterContext(FilterContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Par_filterContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitPar_filter(this);
  else
    return visitor->visitChildren(this);
}
//----------------- And_filterContext ------------------------------------------------------------------

std::vector<StreamDeclarationParser::FilterContext *> StreamDeclarationParser::And_filterContext::filter() {
  return getRuleContexts<StreamDeclarationParser::FilterContext>();
}

StreamDeclarationParser::FilterContext* StreamDeclarationParser::And_filterContext::filter(size_t i) {
  return getRuleContext<StreamDeclarationParser::FilterContext>(i);
}

tree::TerminalNode* StreamDeclarationParser::And_filterContext::K_AND() {
  return getToken(StreamDeclarationParser::K_AND, 0);
}

StreamDeclarationParser::And_filterContext::And_filterContext(FilterContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::And_filterContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitAnd_filter(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Event_filterContext ------------------------------------------------------------------

StreamDeclarationParser::Event_nameContext* StreamDeclarationParser::Event_filterContext::event_name() {
  return getRuleContext<StreamDeclarationParser::Event_nameContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::Event_filterContext::LEFT_SQUARE_BRACKET() {
  return getToken(StreamDeclarationParser::LEFT_SQUARE_BRACKET, 0);
}

StreamDeclarationParser::Bool_exprContext* StreamDeclarationParser::Event_filterContext::bool_expr() {
  return getRuleContext<StreamDeclarationParser::Bool_exprContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::Event_filterContext::RIGHT_SQUARE_BRACKET() {
  return getToken(StreamDeclarationParser::RIGHT_SQUARE_BRACKET, 0);
}

StreamDeclarationParser::Event_filterContext::Event_filterContext(FilterContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Event_filterContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitEvent_filter(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Or_filterContext ------------------------------------------------------------------

std::vector<StreamDeclarationParser::FilterContext *> StreamDeclarationParser::Or_filterContext::filter() {
  return getRuleContexts<StreamDeclarationParser::FilterContext>();
}

StreamDeclarationParser::FilterContext* StreamDeclarationParser::Or_filterContext::filter(size_t i) {
  return getRuleContext<StreamDeclarationParser::FilterContext>(i);
}

tree::TerminalNode* StreamDeclarationParser::Or_filterContext::K_OR() {
  return getToken(StreamDeclarationParser::K_OR, 0);
}

StreamDeclarationParser::Or_filterContext::Or_filterContext(FilterContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Or_filterContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitOr_filter(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::FilterContext* StreamDeclarationParser::filter() {
   return filter(0);
}

StreamDeclarationParser::FilterContext* StreamDeclarationParser::filter(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  StreamDeclarationParser::FilterContext *_localctx = _tracker.createInstance<FilterContext>(_ctx, parentState);
  StreamDeclarationParser::FilterContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 34;
  enterRecursionRule(_localctx, 34, StreamDeclarationParser::RuleFilter, precedence);

    

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
    setState(252);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case StreamDeclarationParser::LEFT_PARENTHESIS: {
        _localctx = _tracker.createInstance<Par_filterContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(243);
        match(StreamDeclarationParser::LEFT_PARENTHESIS);
        setState(244);
        filter(0);
        setState(245);
        match(StreamDeclarationParser::RIGHT_PARENTHESIS);
        break;
      }

      case StreamDeclarationParser::IDENTIFIER: {
        _localctx = _tracker.createInstance<Event_filterContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(247);
        event_name();
        setState(248);
        match(StreamDeclarationParser::LEFT_SQUARE_BRACKET);
        setState(249);
        bool_expr(0);
        setState(250);
        match(StreamDeclarationParser::RIGHT_SQUARE_BRACKET);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(262);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 25, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(260);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 24, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<And_filterContext>(_tracker.createInstance<FilterContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleFilter);
          setState(254);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(255);
          match(StreamDeclarationParser::K_AND);
          setState(256);
          filter(3);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<Or_filterContext>(_tracker.createInstance<FilterContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleFilter);
          setState(257);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(258);
          match(StreamDeclarationParser::K_OR);
          setState(259);
          filter(2);
          break;
        }

        default:
          break;
        } 
      }
      setState(264);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 25, _ctx);
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

StreamDeclarationParser::Bool_exprContext::Bool_exprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t StreamDeclarationParser::Bool_exprContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleBool_expr;
}

void StreamDeclarationParser::Bool_exprContext::copyFrom(Bool_exprContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Not_exprContext ------------------------------------------------------------------

tree::TerminalNode* StreamDeclarationParser::Not_exprContext::K_NOT() {
  return getToken(StreamDeclarationParser::K_NOT, 0);
}

StreamDeclarationParser::Bool_exprContext* StreamDeclarationParser::Not_exprContext::bool_expr() {
  return getRuleContext<StreamDeclarationParser::Bool_exprContext>(0);
}

StreamDeclarationParser::Not_exprContext::Not_exprContext(Bool_exprContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Not_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitNot_expr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Equality_string_exprContext ------------------------------------------------------------------

StreamDeclarationParser::String_literalContext* StreamDeclarationParser::Equality_string_exprContext::string_literal() {
  return getRuleContext<StreamDeclarationParser::String_literalContext>(0);
}

StreamDeclarationParser::String_literal_or_regexpContext* StreamDeclarationParser::Equality_string_exprContext::string_literal_or_regexp() {
  return getRuleContext<StreamDeclarationParser::String_literal_or_regexpContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::Equality_string_exprContext::EQ() {
  return getToken(StreamDeclarationParser::EQ, 0);
}

tree::TerminalNode* StreamDeclarationParser::Equality_string_exprContext::NEQ() {
  return getToken(StreamDeclarationParser::NEQ, 0);
}

StreamDeclarationParser::Equality_string_exprContext::Equality_string_exprContext(Bool_exprContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Equality_string_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitEquality_string_expr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- And_exprContext ------------------------------------------------------------------

std::vector<StreamDeclarationParser::Bool_exprContext *> StreamDeclarationParser::And_exprContext::bool_expr() {
  return getRuleContexts<StreamDeclarationParser::Bool_exprContext>();
}

StreamDeclarationParser::Bool_exprContext* StreamDeclarationParser::And_exprContext::bool_expr(size_t i) {
  return getRuleContext<StreamDeclarationParser::Bool_exprContext>(i);
}

tree::TerminalNode* StreamDeclarationParser::And_exprContext::K_AND() {
  return getToken(StreamDeclarationParser::K_AND, 0);
}

StreamDeclarationParser::And_exprContext::And_exprContext(Bool_exprContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::And_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitAnd_expr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Par_bool_exprContext ------------------------------------------------------------------

tree::TerminalNode* StreamDeclarationParser::Par_bool_exprContext::LEFT_PARENTHESIS() {
  return getToken(StreamDeclarationParser::LEFT_PARENTHESIS, 0);
}

StreamDeclarationParser::Bool_exprContext* StreamDeclarationParser::Par_bool_exprContext::bool_expr() {
  return getRuleContext<StreamDeclarationParser::Bool_exprContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::Par_bool_exprContext::RIGHT_PARENTHESIS() {
  return getToken(StreamDeclarationParser::RIGHT_PARENTHESIS, 0);
}

StreamDeclarationParser::Par_bool_exprContext::Par_bool_exprContext(Bool_exprContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Par_bool_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitPar_bool_expr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Containment_exprContext ------------------------------------------------------------------

StreamDeclarationParser::Attribute_nameContext* StreamDeclarationParser::Containment_exprContext::attribute_name() {
  return getRuleContext<StreamDeclarationParser::Attribute_nameContext>(0);
}

StreamDeclarationParser::Value_seqContext* StreamDeclarationParser::Containment_exprContext::value_seq() {
  return getRuleContext<StreamDeclarationParser::Value_seqContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::Containment_exprContext::K_IN() {
  return getToken(StreamDeclarationParser::K_IN, 0);
}

tree::TerminalNode* StreamDeclarationParser::Containment_exprContext::K_NOT() {
  return getToken(StreamDeclarationParser::K_NOT, 0);
}

StreamDeclarationParser::Containment_exprContext::Containment_exprContext(Bool_exprContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Containment_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitContainment_expr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Inequality_exprContext ------------------------------------------------------------------

std::vector<StreamDeclarationParser::Math_exprContext *> StreamDeclarationParser::Inequality_exprContext::math_expr() {
  return getRuleContexts<StreamDeclarationParser::Math_exprContext>();
}

StreamDeclarationParser::Math_exprContext* StreamDeclarationParser::Inequality_exprContext::math_expr(size_t i) {
  return getRuleContext<StreamDeclarationParser::Math_exprContext>(i);
}

tree::TerminalNode* StreamDeclarationParser::Inequality_exprContext::LE() {
  return getToken(StreamDeclarationParser::LE, 0);
}

tree::TerminalNode* StreamDeclarationParser::Inequality_exprContext::LEQ() {
  return getToken(StreamDeclarationParser::LEQ, 0);
}

tree::TerminalNode* StreamDeclarationParser::Inequality_exprContext::GE() {
  return getToken(StreamDeclarationParser::GE, 0);
}

tree::TerminalNode* StreamDeclarationParser::Inequality_exprContext::GEQ() {
  return getToken(StreamDeclarationParser::GEQ, 0);
}

StreamDeclarationParser::Inequality_exprContext::Inequality_exprContext(Bool_exprContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Inequality_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitInequality_expr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Or_exprContext ------------------------------------------------------------------

std::vector<StreamDeclarationParser::Bool_exprContext *> StreamDeclarationParser::Or_exprContext::bool_expr() {
  return getRuleContexts<StreamDeclarationParser::Bool_exprContext>();
}

StreamDeclarationParser::Bool_exprContext* StreamDeclarationParser::Or_exprContext::bool_expr(size_t i) {
  return getRuleContext<StreamDeclarationParser::Bool_exprContext>(i);
}

tree::TerminalNode* StreamDeclarationParser::Or_exprContext::K_OR() {
  return getToken(StreamDeclarationParser::K_OR, 0);
}

StreamDeclarationParser::Or_exprContext::Or_exprContext(Bool_exprContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Or_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitOr_expr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Equality_math_exprContext ------------------------------------------------------------------

std::vector<StreamDeclarationParser::Math_exprContext *> StreamDeclarationParser::Equality_math_exprContext::math_expr() {
  return getRuleContexts<StreamDeclarationParser::Math_exprContext>();
}

StreamDeclarationParser::Math_exprContext* StreamDeclarationParser::Equality_math_exprContext::math_expr(size_t i) {
  return getRuleContext<StreamDeclarationParser::Math_exprContext>(i);
}

tree::TerminalNode* StreamDeclarationParser::Equality_math_exprContext::EQ() {
  return getToken(StreamDeclarationParser::EQ, 0);
}

tree::TerminalNode* StreamDeclarationParser::Equality_math_exprContext::NEQ() {
  return getToken(StreamDeclarationParser::NEQ, 0);
}

StreamDeclarationParser::Equality_math_exprContext::Equality_math_exprContext(Bool_exprContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Equality_math_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitEquality_math_expr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Regex_exprContext ------------------------------------------------------------------

StreamDeclarationParser::Attribute_nameContext* StreamDeclarationParser::Regex_exprContext::attribute_name() {
  return getRuleContext<StreamDeclarationParser::Attribute_nameContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::Regex_exprContext::K_LIKE() {
  return getToken(StreamDeclarationParser::K_LIKE, 0);
}

tree::TerminalNode* StreamDeclarationParser::Regex_exprContext::REGEXP() {
  return getToken(StreamDeclarationParser::REGEXP, 0);
}

StreamDeclarationParser::Regex_exprContext::Regex_exprContext(Bool_exprContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Regex_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitRegex_expr(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::Bool_exprContext* StreamDeclarationParser::bool_expr() {
   return bool_expr(0);
}

StreamDeclarationParser::Bool_exprContext* StreamDeclarationParser::bool_expr(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  StreamDeclarationParser::Bool_exprContext *_localctx = _tracker.createInstance<Bool_exprContext>(_ctx, parentState);
  StreamDeclarationParser::Bool_exprContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 36;
  enterRecursionRule(_localctx, 36, StreamDeclarationParser::RuleBool_expr, precedence);

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
    setState(296);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 27, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<Par_bool_exprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(266);
      match(StreamDeclarationParser::LEFT_PARENTHESIS);
      setState(267);
      bool_expr(0);
      setState(268);
      match(StreamDeclarationParser::RIGHT_PARENTHESIS);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<Not_exprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(270);
      match(StreamDeclarationParser::K_NOT);
      setState(271);
      bool_expr(8);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<Inequality_exprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(272);
      math_expr(0);
      setState(273);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 1030792151040) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(274);
      math_expr(0);
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<Equality_math_exprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(276);
      math_expr(0);
      setState(277);
      _la = _input->LA(1);
      if (!(_la == StreamDeclarationParser::EQ

      || _la == StreamDeclarationParser::NEQ)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(278);
      math_expr(0);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<Regex_exprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(280);
      attribute_name();
      setState(281);
      match(StreamDeclarationParser::K_LIKE);
      setState(282);
      match(StreamDeclarationParser::REGEXP);
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<Containment_exprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(284);
      attribute_name();
      setState(288);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case StreamDeclarationParser::K_IN: {
          setState(285);
          match(StreamDeclarationParser::K_IN);
          break;
        }

        case StreamDeclarationParser::K_NOT: {
          setState(286);
          match(StreamDeclarationParser::K_NOT);
          setState(287);
          match(StreamDeclarationParser::K_IN);
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(290);
      value_seq();
      break;
    }

    case 7: {
      _localctx = _tracker.createInstance<Equality_string_exprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(292);
      string_literal();
      setState(293);
      _la = _input->LA(1);
      if (!(_la == StreamDeclarationParser::EQ

      || _la == StreamDeclarationParser::NEQ)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(294);
      string_literal_or_regexp();
      break;
    }

    default:
      break;
    }
    _ctx->stop = _input->LT(-1);
    setState(306);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 29, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(304);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 28, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<And_exprContext>(_tracker.createInstance<Bool_exprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleBool_expr);
          setState(298);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(299);
          match(StreamDeclarationParser::K_AND);
          setState(300);
          bool_expr(6);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<Or_exprContext>(_tracker.createInstance<Bool_exprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleBool_expr);
          setState(301);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(302);
          match(StreamDeclarationParser::K_OR);
          setState(303);
          bool_expr(5);
          break;
        }

        default:
          break;
        } 
      }
      setState(308);
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

//----------------- String_literalContext ------------------------------------------------------------------

StreamDeclarationParser::String_literalContext::String_literalContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

StreamDeclarationParser::StringContext* StreamDeclarationParser::String_literalContext::string() {
  return getRuleContext<StreamDeclarationParser::StringContext>(0);
}

StreamDeclarationParser::Attribute_nameContext* StreamDeclarationParser::String_literalContext::attribute_name() {
  return getRuleContext<StreamDeclarationParser::Attribute_nameContext>(0);
}


size_t StreamDeclarationParser::String_literalContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleString_literal;
}


std::any StreamDeclarationParser::String_literalContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitString_literal(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::String_literalContext* StreamDeclarationParser::string_literal() {
  String_literalContext *_localctx = _tracker.createInstance<String_literalContext>(_ctx, getState());
  enterRule(_localctx, 38, StreamDeclarationParser::RuleString_literal);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(311);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case StreamDeclarationParser::STRING_LITERAL: {
        enterOuterAlt(_localctx, 1);
        setState(309);
        string();
        break;
      }

      case StreamDeclarationParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 2);
        setState(310);
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

StreamDeclarationParser::String_literal_or_regexpContext::String_literal_or_regexpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

StreamDeclarationParser::StringContext* StreamDeclarationParser::String_literal_or_regexpContext::string() {
  return getRuleContext<StreamDeclarationParser::StringContext>(0);
}

StreamDeclarationParser::Attribute_nameContext* StreamDeclarationParser::String_literal_or_regexpContext::attribute_name() {
  return getRuleContext<StreamDeclarationParser::Attribute_nameContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::String_literal_or_regexpContext::REGEXP() {
  return getToken(StreamDeclarationParser::REGEXP, 0);
}


size_t StreamDeclarationParser::String_literal_or_regexpContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleString_literal_or_regexp;
}


std::any StreamDeclarationParser::String_literal_or_regexpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitString_literal_or_regexp(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::String_literal_or_regexpContext* StreamDeclarationParser::string_literal_or_regexp() {
  String_literal_or_regexpContext *_localctx = _tracker.createInstance<String_literal_or_regexpContext>(_ctx, getState());
  enterRule(_localctx, 40, StreamDeclarationParser::RuleString_literal_or_regexp);

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
    switch (_input->LA(1)) {
      case StreamDeclarationParser::STRING_LITERAL: {
        enterOuterAlt(_localctx, 1);
        setState(313);
        string();
        break;
      }

      case StreamDeclarationParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 2);
        setState(314);
        attribute_name();
        break;
      }

      case StreamDeclarationParser::REGEXP: {
        enterOuterAlt(_localctx, 3);
        setState(315);
        match(StreamDeclarationParser::REGEXP);
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

StreamDeclarationParser::Math_exprContext::Math_exprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t StreamDeclarationParser::Math_exprContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleMath_expr;
}

void StreamDeclarationParser::Math_exprContext::copyFrom(Math_exprContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Mul_math_exprContext ------------------------------------------------------------------

std::vector<StreamDeclarationParser::Math_exprContext *> StreamDeclarationParser::Mul_math_exprContext::math_expr() {
  return getRuleContexts<StreamDeclarationParser::Math_exprContext>();
}

StreamDeclarationParser::Math_exprContext* StreamDeclarationParser::Mul_math_exprContext::math_expr(size_t i) {
  return getRuleContext<StreamDeclarationParser::Math_exprContext>(i);
}

tree::TerminalNode* StreamDeclarationParser::Mul_math_exprContext::STAR() {
  return getToken(StreamDeclarationParser::STAR, 0);
}

tree::TerminalNode* StreamDeclarationParser::Mul_math_exprContext::SLASH() {
  return getToken(StreamDeclarationParser::SLASH, 0);
}

tree::TerminalNode* StreamDeclarationParser::Mul_math_exprContext::PERCENT() {
  return getToken(StreamDeclarationParser::PERCENT, 0);
}

StreamDeclarationParser::Mul_math_exprContext::Mul_math_exprContext(Math_exprContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Mul_math_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitMul_math_expr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Sum_math_exprContext ------------------------------------------------------------------

std::vector<StreamDeclarationParser::Math_exprContext *> StreamDeclarationParser::Sum_math_exprContext::math_expr() {
  return getRuleContexts<StreamDeclarationParser::Math_exprContext>();
}

StreamDeclarationParser::Math_exprContext* StreamDeclarationParser::Sum_math_exprContext::math_expr(size_t i) {
  return getRuleContext<StreamDeclarationParser::Math_exprContext>(i);
}

tree::TerminalNode* StreamDeclarationParser::Sum_math_exprContext::PLUS() {
  return getToken(StreamDeclarationParser::PLUS, 0);
}

tree::TerminalNode* StreamDeclarationParser::Sum_math_exprContext::MINUS() {
  return getToken(StreamDeclarationParser::MINUS, 0);
}

StreamDeclarationParser::Sum_math_exprContext::Sum_math_exprContext(Math_exprContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Sum_math_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitSum_math_expr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Number_math_exprContext ------------------------------------------------------------------

StreamDeclarationParser::NumberContext* StreamDeclarationParser::Number_math_exprContext::number() {
  return getRuleContext<StreamDeclarationParser::NumberContext>(0);
}

StreamDeclarationParser::Number_math_exprContext::Number_math_exprContext(Math_exprContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Number_math_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitNumber_math_expr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Unary_math_exprContext ------------------------------------------------------------------

StreamDeclarationParser::Math_exprContext* StreamDeclarationParser::Unary_math_exprContext::math_expr() {
  return getRuleContext<StreamDeclarationParser::Math_exprContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::Unary_math_exprContext::PLUS() {
  return getToken(StreamDeclarationParser::PLUS, 0);
}

tree::TerminalNode* StreamDeclarationParser::Unary_math_exprContext::MINUS() {
  return getToken(StreamDeclarationParser::MINUS, 0);
}

StreamDeclarationParser::Unary_math_exprContext::Unary_math_exprContext(Math_exprContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Unary_math_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitUnary_math_expr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Attribute_math_exprContext ------------------------------------------------------------------

StreamDeclarationParser::Attribute_nameContext* StreamDeclarationParser::Attribute_math_exprContext::attribute_name() {
  return getRuleContext<StreamDeclarationParser::Attribute_nameContext>(0);
}

StreamDeclarationParser::Attribute_math_exprContext::Attribute_math_exprContext(Math_exprContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Attribute_math_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitAttribute_math_expr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Par_math_exprContext ------------------------------------------------------------------

tree::TerminalNode* StreamDeclarationParser::Par_math_exprContext::LEFT_PARENTHESIS() {
  return getToken(StreamDeclarationParser::LEFT_PARENTHESIS, 0);
}

StreamDeclarationParser::Math_exprContext* StreamDeclarationParser::Par_math_exprContext::math_expr() {
  return getRuleContext<StreamDeclarationParser::Math_exprContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::Par_math_exprContext::RIGHT_PARENTHESIS() {
  return getToken(StreamDeclarationParser::RIGHT_PARENTHESIS, 0);
}

StreamDeclarationParser::Par_math_exprContext::Par_math_exprContext(Math_exprContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Par_math_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitPar_math_expr(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::Math_exprContext* StreamDeclarationParser::math_expr() {
   return math_expr(0);
}

StreamDeclarationParser::Math_exprContext* StreamDeclarationParser::math_expr(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  StreamDeclarationParser::Math_exprContext *_localctx = _tracker.createInstance<Math_exprContext>(_ctx, parentState);
  StreamDeclarationParser::Math_exprContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 42;
  enterRecursionRule(_localctx, 42, StreamDeclarationParser::RuleMath_expr, precedence);

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
    setState(327);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case StreamDeclarationParser::LEFT_PARENTHESIS: {
        _localctx = _tracker.createInstance<Par_math_exprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(319);
        match(StreamDeclarationParser::LEFT_PARENTHESIS);
        setState(320);
        math_expr(0);
        setState(321);
        match(StreamDeclarationParser::RIGHT_PARENTHESIS);
        break;
      }

      case StreamDeclarationParser::FLOAT_LITERAL:
      case StreamDeclarationParser::INTEGER_LITERAL: {
        _localctx = _tracker.createInstance<Number_math_exprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(323);
        number();
        break;
      }

      case StreamDeclarationParser::IDENTIFIER: {
        _localctx = _tracker.createInstance<Attribute_math_exprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(324);
        attribute_name();
        break;
      }

      case StreamDeclarationParser::PLUS:
      case StreamDeclarationParser::MINUS: {
        _localctx = _tracker.createInstance<Unary_math_exprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(325);
        _la = _input->LA(1);
        if (!(_la == StreamDeclarationParser::PLUS

        || _la == StreamDeclarationParser::MINUS)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(326);
        math_expr(3);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(337);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 34, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(335);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 33, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<Mul_math_exprContext>(_tracker.createInstance<Math_exprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleMath_expr);
          setState(329);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(330);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & 53687091200) != 0))) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(331);
          math_expr(3);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<Sum_math_exprContext>(_tracker.createInstance<Math_exprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleMath_expr);
          setState(332);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(333);
          _la = _input->LA(1);
          if (!(_la == StreamDeclarationParser::PLUS

          || _la == StreamDeclarationParser::MINUS)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(334);
          math_expr(2);
          break;
        }

        default:
          break;
        } 
      }
      setState(339);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 34, _ctx);
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

StreamDeclarationParser::Value_seqContext::Value_seqContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* StreamDeclarationParser::Value_seqContext::LEFT_CURLY_BRACKET() {
  return getToken(StreamDeclarationParser::LEFT_CURLY_BRACKET, 0);
}

StreamDeclarationParser::Number_seqContext* StreamDeclarationParser::Value_seqContext::number_seq() {
  return getRuleContext<StreamDeclarationParser::Number_seqContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::Value_seqContext::RIGHT_CURLY_BRACKET() {
  return getToken(StreamDeclarationParser::RIGHT_CURLY_BRACKET, 0);
}

StreamDeclarationParser::String_seqContext* StreamDeclarationParser::Value_seqContext::string_seq() {
  return getRuleContext<StreamDeclarationParser::String_seqContext>(0);
}


size_t StreamDeclarationParser::Value_seqContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleValue_seq;
}


std::any StreamDeclarationParser::Value_seqContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitValue_seq(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::Value_seqContext* StreamDeclarationParser::value_seq() {
  Value_seqContext *_localctx = _tracker.createInstance<Value_seqContext>(_ctx, getState());
  enterRule(_localctx, 44, StreamDeclarationParser::RuleValue_seq);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(348);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 35, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(340);
      match(StreamDeclarationParser::LEFT_CURLY_BRACKET);
      setState(341);
      number_seq();
      setState(342);
      match(StreamDeclarationParser::RIGHT_CURLY_BRACKET);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(344);
      match(StreamDeclarationParser::LEFT_CURLY_BRACKET);
      setState(345);
      string_seq();
      setState(346);
      match(StreamDeclarationParser::RIGHT_CURLY_BRACKET);
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

StreamDeclarationParser::Number_seqContext::Number_seqContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t StreamDeclarationParser::Number_seqContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleNumber_seq;
}

void StreamDeclarationParser::Number_seqContext::copyFrom(Number_seqContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Number_listContext ------------------------------------------------------------------

std::vector<StreamDeclarationParser::NumberContext *> StreamDeclarationParser::Number_listContext::number() {
  return getRuleContexts<StreamDeclarationParser::NumberContext>();
}

StreamDeclarationParser::NumberContext* StreamDeclarationParser::Number_listContext::number(size_t i) {
  return getRuleContext<StreamDeclarationParser::NumberContext>(i);
}

std::vector<tree::TerminalNode *> StreamDeclarationParser::Number_listContext::COMMA() {
  return getTokens(StreamDeclarationParser::COMMA);
}

tree::TerminalNode* StreamDeclarationParser::Number_listContext::COMMA(size_t i) {
  return getToken(StreamDeclarationParser::COMMA, i);
}

StreamDeclarationParser::Number_listContext::Number_listContext(Number_seqContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Number_listContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitNumber_list(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Number_rangeContext ------------------------------------------------------------------

std::vector<StreamDeclarationParser::NumberContext *> StreamDeclarationParser::Number_rangeContext::number() {
  return getRuleContexts<StreamDeclarationParser::NumberContext>();
}

StreamDeclarationParser::NumberContext* StreamDeclarationParser::Number_rangeContext::number(size_t i) {
  return getRuleContext<StreamDeclarationParser::NumberContext>(i);
}

tree::TerminalNode* StreamDeclarationParser::Number_rangeContext::DOUBLE_DOT() {
  return getToken(StreamDeclarationParser::DOUBLE_DOT, 0);
}

StreamDeclarationParser::Number_rangeContext::Number_rangeContext(Number_seqContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Number_rangeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitNumber_range(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Number_range_lowerContext ------------------------------------------------------------------

StreamDeclarationParser::NumberContext* StreamDeclarationParser::Number_range_lowerContext::number() {
  return getRuleContext<StreamDeclarationParser::NumberContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::Number_range_lowerContext::DOUBLE_DOT() {
  return getToken(StreamDeclarationParser::DOUBLE_DOT, 0);
}

StreamDeclarationParser::Number_range_lowerContext::Number_range_lowerContext(Number_seqContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Number_range_lowerContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitNumber_range_lower(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Number_range_upperContext ------------------------------------------------------------------

tree::TerminalNode* StreamDeclarationParser::Number_range_upperContext::DOUBLE_DOT() {
  return getToken(StreamDeclarationParser::DOUBLE_DOT, 0);
}

StreamDeclarationParser::NumberContext* StreamDeclarationParser::Number_range_upperContext::number() {
  return getRuleContext<StreamDeclarationParser::NumberContext>(0);
}

StreamDeclarationParser::Number_range_upperContext::Number_range_upperContext(Number_seqContext *ctx) { copyFrom(ctx); }


std::any StreamDeclarationParser::Number_range_upperContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitNumber_range_upper(this);
  else
    return visitor->visitChildren(this);
}
StreamDeclarationParser::Number_seqContext* StreamDeclarationParser::number_seq() {
  Number_seqContext *_localctx = _tracker.createInstance<Number_seqContext>(_ctx, getState());
  enterRule(_localctx, 46, StreamDeclarationParser::RuleNumber_seq);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(367);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 37, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<StreamDeclarationParser::Number_listContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(350);
      number();
      setState(355);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == StreamDeclarationParser::COMMA) {
        setState(351);
        match(StreamDeclarationParser::COMMA);
        setState(352);
        number();
        setState(357);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<StreamDeclarationParser::Number_rangeContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(358);
      number();
      setState(359);
      match(StreamDeclarationParser::DOUBLE_DOT);
      setState(360);
      number();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<StreamDeclarationParser::Number_range_lowerContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(362);
      number();
      setState(363);
      match(StreamDeclarationParser::DOUBLE_DOT);
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<StreamDeclarationParser::Number_range_upperContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(365);
      match(StreamDeclarationParser::DOUBLE_DOT);
      setState(366);
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

StreamDeclarationParser::String_seqContext::String_seqContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<StreamDeclarationParser::StringContext *> StreamDeclarationParser::String_seqContext::string() {
  return getRuleContexts<StreamDeclarationParser::StringContext>();
}

StreamDeclarationParser::StringContext* StreamDeclarationParser::String_seqContext::string(size_t i) {
  return getRuleContext<StreamDeclarationParser::StringContext>(i);
}

std::vector<tree::TerminalNode *> StreamDeclarationParser::String_seqContext::COMMA() {
  return getTokens(StreamDeclarationParser::COMMA);
}

tree::TerminalNode* StreamDeclarationParser::String_seqContext::COMMA(size_t i) {
  return getToken(StreamDeclarationParser::COMMA, i);
}


size_t StreamDeclarationParser::String_seqContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleString_seq;
}


std::any StreamDeclarationParser::String_seqContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitString_seq(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::String_seqContext* StreamDeclarationParser::string_seq() {
  String_seqContext *_localctx = _tracker.createInstance<String_seqContext>(_ctx, getState());
  enterRule(_localctx, 48, StreamDeclarationParser::RuleString_seq);
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
    setState(369);
    string();
    setState(374);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == StreamDeclarationParser::COMMA) {
      setState(370);
      match(StreamDeclarationParser::COMMA);
      setState(371);
      string();
      setState(376);
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

StreamDeclarationParser::Time_windowContext::Time_windowContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

StreamDeclarationParser::Event_spanContext* StreamDeclarationParser::Time_windowContext::event_span() {
  return getRuleContext<StreamDeclarationParser::Event_spanContext>(0);
}

StreamDeclarationParser::Time_spanContext* StreamDeclarationParser::Time_windowContext::time_span() {
  return getRuleContext<StreamDeclarationParser::Time_spanContext>(0);
}

StreamDeclarationParser::Custom_spanContext* StreamDeclarationParser::Time_windowContext::custom_span() {
  return getRuleContext<StreamDeclarationParser::Custom_spanContext>(0);
}


size_t StreamDeclarationParser::Time_windowContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleTime_window;
}


std::any StreamDeclarationParser::Time_windowContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitTime_window(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::Time_windowContext* StreamDeclarationParser::time_window() {
  Time_windowContext *_localctx = _tracker.createInstance<Time_windowContext>(_ctx, getState());
  enterRule(_localctx, 50, StreamDeclarationParser::RuleTime_window);

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
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 39, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(377);
      event_span();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(378);
      time_span();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(379);
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

StreamDeclarationParser::Event_spanContext::Event_spanContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

StreamDeclarationParser::IntegerContext* StreamDeclarationParser::Event_spanContext::integer() {
  return getRuleContext<StreamDeclarationParser::IntegerContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::Event_spanContext::K_EVENTS() {
  return getToken(StreamDeclarationParser::K_EVENTS, 0);
}


size_t StreamDeclarationParser::Event_spanContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleEvent_span;
}


std::any StreamDeclarationParser::Event_spanContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitEvent_span(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::Event_spanContext* StreamDeclarationParser::event_span() {
  Event_spanContext *_localctx = _tracker.createInstance<Event_spanContext>(_ctx, getState());
  enterRule(_localctx, 52, StreamDeclarationParser::RuleEvent_span);

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
    integer();
    setState(383);
    match(StreamDeclarationParser::K_EVENTS);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Time_spanContext ------------------------------------------------------------------

StreamDeclarationParser::Time_spanContext::Time_spanContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

StreamDeclarationParser::Hour_spanContext* StreamDeclarationParser::Time_spanContext::hour_span() {
  return getRuleContext<StreamDeclarationParser::Hour_spanContext>(0);
}

StreamDeclarationParser::Minute_spanContext* StreamDeclarationParser::Time_spanContext::minute_span() {
  return getRuleContext<StreamDeclarationParser::Minute_spanContext>(0);
}

StreamDeclarationParser::Second_spanContext* StreamDeclarationParser::Time_spanContext::second_span() {
  return getRuleContext<StreamDeclarationParser::Second_spanContext>(0);
}


size_t StreamDeclarationParser::Time_spanContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleTime_span;
}


std::any StreamDeclarationParser::Time_spanContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitTime_span(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::Time_spanContext* StreamDeclarationParser::time_span() {
  Time_spanContext *_localctx = _tracker.createInstance<Time_spanContext>(_ctx, getState());
  enterRule(_localctx, 54, StreamDeclarationParser::RuleTime_span);
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
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 40, _ctx)) {
    case 1: {
      setState(385);
      hour_span();
      break;
    }

    default:
      break;
    }
    setState(389);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 41, _ctx)) {
    case 1: {
      setState(388);
      minute_span();
      break;
    }

    default:
      break;
    }
    setState(392);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == StreamDeclarationParser::INTEGER_LITERAL) {
      setState(391);
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

StreamDeclarationParser::Hour_spanContext::Hour_spanContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

StreamDeclarationParser::IntegerContext* StreamDeclarationParser::Hour_spanContext::integer() {
  return getRuleContext<StreamDeclarationParser::IntegerContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::Hour_spanContext::K_HOURS() {
  return getToken(StreamDeclarationParser::K_HOURS, 0);
}


size_t StreamDeclarationParser::Hour_spanContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleHour_span;
}


std::any StreamDeclarationParser::Hour_spanContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitHour_span(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::Hour_spanContext* StreamDeclarationParser::hour_span() {
  Hour_spanContext *_localctx = _tracker.createInstance<Hour_spanContext>(_ctx, getState());
  enterRule(_localctx, 56, StreamDeclarationParser::RuleHour_span);

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
    match(StreamDeclarationParser::K_HOURS);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Minute_spanContext ------------------------------------------------------------------

StreamDeclarationParser::Minute_spanContext::Minute_spanContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

StreamDeclarationParser::IntegerContext* StreamDeclarationParser::Minute_spanContext::integer() {
  return getRuleContext<StreamDeclarationParser::IntegerContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::Minute_spanContext::K_MINUTES() {
  return getToken(StreamDeclarationParser::K_MINUTES, 0);
}


size_t StreamDeclarationParser::Minute_spanContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleMinute_span;
}


std::any StreamDeclarationParser::Minute_spanContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitMinute_span(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::Minute_spanContext* StreamDeclarationParser::minute_span() {
  Minute_spanContext *_localctx = _tracker.createInstance<Minute_spanContext>(_ctx, getState());
  enterRule(_localctx, 58, StreamDeclarationParser::RuleMinute_span);

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
    match(StreamDeclarationParser::K_MINUTES);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Second_spanContext ------------------------------------------------------------------

StreamDeclarationParser::Second_spanContext::Second_spanContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

StreamDeclarationParser::IntegerContext* StreamDeclarationParser::Second_spanContext::integer() {
  return getRuleContext<StreamDeclarationParser::IntegerContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::Second_spanContext::K_SECONDS() {
  return getToken(StreamDeclarationParser::K_SECONDS, 0);
}


size_t StreamDeclarationParser::Second_spanContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleSecond_span;
}


std::any StreamDeclarationParser::Second_spanContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitSecond_span(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::Second_spanContext* StreamDeclarationParser::second_span() {
  Second_spanContext *_localctx = _tracker.createInstance<Second_spanContext>(_ctx, getState());
  enterRule(_localctx, 60, StreamDeclarationParser::RuleSecond_span);

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
    match(StreamDeclarationParser::K_SECONDS);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Custom_spanContext ------------------------------------------------------------------

StreamDeclarationParser::Custom_spanContext::Custom_spanContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

StreamDeclarationParser::IntegerContext* StreamDeclarationParser::Custom_spanContext::integer() {
  return getRuleContext<StreamDeclarationParser::IntegerContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::Custom_spanContext::LEFT_SQUARE_BRACKET() {
  return getToken(StreamDeclarationParser::LEFT_SQUARE_BRACKET, 0);
}

StreamDeclarationParser::Any_nameContext* StreamDeclarationParser::Custom_spanContext::any_name() {
  return getRuleContext<StreamDeclarationParser::Any_nameContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::Custom_spanContext::RIGHT_SQUARE_BRACKET() {
  return getToken(StreamDeclarationParser::RIGHT_SQUARE_BRACKET, 0);
}


size_t StreamDeclarationParser::Custom_spanContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleCustom_span;
}


std::any StreamDeclarationParser::Custom_spanContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitCustom_span(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::Custom_spanContext* StreamDeclarationParser::custom_span() {
  Custom_spanContext *_localctx = _tracker.createInstance<Custom_spanContext>(_ctx, getState());
  enterRule(_localctx, 62, StreamDeclarationParser::RuleCustom_span);

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
    integer();
    setState(404);
    match(StreamDeclarationParser::LEFT_SQUARE_BRACKET);
    setState(405);
    any_name();
    setState(406);
    match(StreamDeclarationParser::RIGHT_SQUARE_BRACKET);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Named_eventContext ------------------------------------------------------------------

StreamDeclarationParser::Named_eventContext::Named_eventContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

StreamDeclarationParser::S_event_nameContext* StreamDeclarationParser::Named_eventContext::s_event_name() {
  return getRuleContext<StreamDeclarationParser::S_event_nameContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::Named_eventContext::K_AS() {
  return getToken(StreamDeclarationParser::K_AS, 0);
}

StreamDeclarationParser::Event_nameContext* StreamDeclarationParser::Named_eventContext::event_name() {
  return getRuleContext<StreamDeclarationParser::Event_nameContext>(0);
}


size_t StreamDeclarationParser::Named_eventContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleNamed_event;
}


std::any StreamDeclarationParser::Named_eventContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitNamed_event(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::Named_eventContext* StreamDeclarationParser::named_event() {
  Named_eventContext *_localctx = _tracker.createInstance<Named_eventContext>(_ctx, getState());
  enterRule(_localctx, 64, StreamDeclarationParser::RuleNamed_event);
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
    setState(408);
    s_event_name();
    setState(411);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == StreamDeclarationParser::K_AS) {
      setState(409);
      match(StreamDeclarationParser::K_AS);
      setState(410);
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

StreamDeclarationParser::S_event_nameContext::S_event_nameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

StreamDeclarationParser::Event_nameContext* StreamDeclarationParser::S_event_nameContext::event_name() {
  return getRuleContext<StreamDeclarationParser::Event_nameContext>(0);
}

StreamDeclarationParser::Stream_nameContext* StreamDeclarationParser::S_event_nameContext::stream_name() {
  return getRuleContext<StreamDeclarationParser::Stream_nameContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::S_event_nameContext::GE() {
  return getToken(StreamDeclarationParser::GE, 0);
}


size_t StreamDeclarationParser::S_event_nameContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleS_event_name;
}


std::any StreamDeclarationParser::S_event_nameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitS_event_name(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::S_event_nameContext* StreamDeclarationParser::s_event_name() {
  S_event_nameContext *_localctx = _tracker.createInstance<S_event_nameContext>(_ctx, getState());
  enterRule(_localctx, 66, StreamDeclarationParser::RuleS_event_name);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(416);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 44, _ctx)) {
    case 1: {
      setState(413);
      stream_name();
      setState(414);
      match(StreamDeclarationParser::GE);
      break;
    }

    default:
      break;
    }
    setState(418);
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

StreamDeclarationParser::Event_nameContext::Event_nameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

StreamDeclarationParser::Any_nameContext* StreamDeclarationParser::Event_nameContext::any_name() {
  return getRuleContext<StreamDeclarationParser::Any_nameContext>(0);
}


size_t StreamDeclarationParser::Event_nameContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleEvent_name;
}


std::any StreamDeclarationParser::Event_nameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitEvent_name(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::Event_nameContext* StreamDeclarationParser::event_name() {
  Event_nameContext *_localctx = _tracker.createInstance<Event_nameContext>(_ctx, getState());
  enterRule(_localctx, 68, StreamDeclarationParser::RuleEvent_name);

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

StreamDeclarationParser::Stream_nameContext::Stream_nameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

StreamDeclarationParser::Any_nameContext* StreamDeclarationParser::Stream_nameContext::any_name() {
  return getRuleContext<StreamDeclarationParser::Any_nameContext>(0);
}


size_t StreamDeclarationParser::Stream_nameContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleStream_name;
}


std::any StreamDeclarationParser::Stream_nameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitStream_name(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::Stream_nameContext* StreamDeclarationParser::stream_name() {
  Stream_nameContext *_localctx = _tracker.createInstance<Stream_nameContext>(_ctx, getState());
  enterRule(_localctx, 70, StreamDeclarationParser::RuleStream_name);

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

StreamDeclarationParser::Attribute_nameContext::Attribute_nameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

StreamDeclarationParser::Any_nameContext* StreamDeclarationParser::Attribute_nameContext::any_name() {
  return getRuleContext<StreamDeclarationParser::Any_nameContext>(0);
}


size_t StreamDeclarationParser::Attribute_nameContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleAttribute_name;
}


std::any StreamDeclarationParser::Attribute_nameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitAttribute_name(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::Attribute_nameContext* StreamDeclarationParser::attribute_name() {
  Attribute_nameContext *_localctx = _tracker.createInstance<Attribute_nameContext>(_ctx, getState());
  enterRule(_localctx, 72, StreamDeclarationParser::RuleAttribute_name);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(424);
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

StreamDeclarationParser::IntegerContext::IntegerContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* StreamDeclarationParser::IntegerContext::INTEGER_LITERAL() {
  return getToken(StreamDeclarationParser::INTEGER_LITERAL, 0);
}


size_t StreamDeclarationParser::IntegerContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleInteger;
}


std::any StreamDeclarationParser::IntegerContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitInteger(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::IntegerContext* StreamDeclarationParser::integer() {
  IntegerContext *_localctx = _tracker.createInstance<IntegerContext>(_ctx, getState());
  enterRule(_localctx, 74, StreamDeclarationParser::RuleInteger);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(426);
    match(StreamDeclarationParser::INTEGER_LITERAL);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NumberContext ------------------------------------------------------------------

StreamDeclarationParser::NumberContext::NumberContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* StreamDeclarationParser::NumberContext::FLOAT_LITERAL() {
  return getToken(StreamDeclarationParser::FLOAT_LITERAL, 0);
}

tree::TerminalNode* StreamDeclarationParser::NumberContext::INTEGER_LITERAL() {
  return getToken(StreamDeclarationParser::INTEGER_LITERAL, 0);
}


size_t StreamDeclarationParser::NumberContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleNumber;
}


std::any StreamDeclarationParser::NumberContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitNumber(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::NumberContext* StreamDeclarationParser::number() {
  NumberContext *_localctx = _tracker.createInstance<NumberContext>(_ctx, getState());
  enterRule(_localctx, 76, StreamDeclarationParser::RuleNumber);
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
    setState(428);
    _la = _input->LA(1);
    if (!(_la == StreamDeclarationParser::FLOAT_LITERAL

    || _la == StreamDeclarationParser::INTEGER_LITERAL)) {
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

StreamDeclarationParser::StringContext::StringContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* StreamDeclarationParser::StringContext::STRING_LITERAL() {
  return getToken(StreamDeclarationParser::STRING_LITERAL, 0);
}


size_t StreamDeclarationParser::StringContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleString;
}


std::any StreamDeclarationParser::StringContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitString(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::StringContext* StreamDeclarationParser::string() {
  StringContext *_localctx = _tracker.createInstance<StringContext>(_ctx, getState());
  enterRule(_localctx, 78, StreamDeclarationParser::RuleString);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(430);
    match(StreamDeclarationParser::STRING_LITERAL);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Any_nameContext ------------------------------------------------------------------

StreamDeclarationParser::Any_nameContext::Any_nameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* StreamDeclarationParser::Any_nameContext::IDENTIFIER() {
  return getToken(StreamDeclarationParser::IDENTIFIER, 0);
}


size_t StreamDeclarationParser::Any_nameContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleAny_name;
}


std::any StreamDeclarationParser::Any_nameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitAny_name(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::Any_nameContext* StreamDeclarationParser::any_name() {
  Any_nameContext *_localctx = _tracker.createInstance<Any_nameContext>(_ctx, getState());
  enterRule(_localctx, 80, StreamDeclarationParser::RuleAny_name);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(432);
    match(StreamDeclarationParser::IDENTIFIER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- KeywordContext ------------------------------------------------------------------

StreamDeclarationParser::KeywordContext::KeywordContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* StreamDeclarationParser::KeywordContext::K_ALL() {
  return getToken(StreamDeclarationParser::K_ALL, 0);
}

tree::TerminalNode* StreamDeclarationParser::KeywordContext::K_AND() {
  return getToken(StreamDeclarationParser::K_AND, 0);
}

tree::TerminalNode* StreamDeclarationParser::KeywordContext::K_ANY() {
  return getToken(StreamDeclarationParser::K_ANY, 0);
}

tree::TerminalNode* StreamDeclarationParser::KeywordContext::K_AS() {
  return getToken(StreamDeclarationParser::K_AS, 0);
}

tree::TerminalNode* StreamDeclarationParser::KeywordContext::K_BY() {
  return getToken(StreamDeclarationParser::K_BY, 0);
}

tree::TerminalNode* StreamDeclarationParser::KeywordContext::K_CONSUME() {
  return getToken(StreamDeclarationParser::K_CONSUME, 0);
}

tree::TerminalNode* StreamDeclarationParser::KeywordContext::K_DECLARE() {
  return getToken(StreamDeclarationParser::K_DECLARE, 0);
}

tree::TerminalNode* StreamDeclarationParser::KeywordContext::K_DISTINCT() {
  return getToken(StreamDeclarationParser::K_DISTINCT, 0);
}

tree::TerminalNode* StreamDeclarationParser::KeywordContext::K_EVENT() {
  return getToken(StreamDeclarationParser::K_EVENT, 0);
}

tree::TerminalNode* StreamDeclarationParser::KeywordContext::K_EVENTS() {
  return getToken(StreamDeclarationParser::K_EVENTS, 0);
}

tree::TerminalNode* StreamDeclarationParser::KeywordContext::K_FILTER() {
  return getToken(StreamDeclarationParser::K_FILTER, 0);
}

tree::TerminalNode* StreamDeclarationParser::KeywordContext::K_FROM() {
  return getToken(StreamDeclarationParser::K_FROM, 0);
}

tree::TerminalNode* StreamDeclarationParser::KeywordContext::K_HOURS() {
  return getToken(StreamDeclarationParser::K_HOURS, 0);
}

tree::TerminalNode* StreamDeclarationParser::KeywordContext::K_IN() {
  return getToken(StreamDeclarationParser::K_IN, 0);
}

tree::TerminalNode* StreamDeclarationParser::KeywordContext::K_LAST() {
  return getToken(StreamDeclarationParser::K_LAST, 0);
}

tree::TerminalNode* StreamDeclarationParser::KeywordContext::K_LIKE() {
  return getToken(StreamDeclarationParser::K_LIKE, 0);
}

tree::TerminalNode* StreamDeclarationParser::KeywordContext::K_MAX() {
  return getToken(StreamDeclarationParser::K_MAX, 0);
}

tree::TerminalNode* StreamDeclarationParser::KeywordContext::K_MINUTES() {
  return getToken(StreamDeclarationParser::K_MINUTES, 0);
}

tree::TerminalNode* StreamDeclarationParser::KeywordContext::K_NEXT() {
  return getToken(StreamDeclarationParser::K_NEXT, 0);
}

tree::TerminalNode* StreamDeclarationParser::KeywordContext::K_NONE() {
  return getToken(StreamDeclarationParser::K_NONE, 0);
}

tree::TerminalNode* StreamDeclarationParser::KeywordContext::K_NOT() {
  return getToken(StreamDeclarationParser::K_NOT, 0);
}

tree::TerminalNode* StreamDeclarationParser::KeywordContext::K_OR() {
  return getToken(StreamDeclarationParser::K_OR, 0);
}

tree::TerminalNode* StreamDeclarationParser::KeywordContext::K_PARTITION() {
  return getToken(StreamDeclarationParser::K_PARTITION, 0);
}

tree::TerminalNode* StreamDeclarationParser::KeywordContext::K_SECONDS() {
  return getToken(StreamDeclarationParser::K_SECONDS, 0);
}

tree::TerminalNode* StreamDeclarationParser::KeywordContext::K_SELECT() {
  return getToken(StreamDeclarationParser::K_SELECT, 0);
}

tree::TerminalNode* StreamDeclarationParser::KeywordContext::K_STREAM() {
  return getToken(StreamDeclarationParser::K_STREAM, 0);
}

tree::TerminalNode* StreamDeclarationParser::KeywordContext::K_STRICT() {
  return getToken(StreamDeclarationParser::K_STRICT, 0);
}

tree::TerminalNode* StreamDeclarationParser::KeywordContext::K_UNLESS() {
  return getToken(StreamDeclarationParser::K_UNLESS, 0);
}

tree::TerminalNode* StreamDeclarationParser::KeywordContext::K_WHERE() {
  return getToken(StreamDeclarationParser::K_WHERE, 0);
}

tree::TerminalNode* StreamDeclarationParser::KeywordContext::K_WITHIN() {
  return getToken(StreamDeclarationParser::K_WITHIN, 0);
}


size_t StreamDeclarationParser::KeywordContext::getRuleIndex() const {
  return StreamDeclarationParser::RuleKeyword;
}


std::any StreamDeclarationParser::KeywordContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<StreamDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitKeyword(this);
  else
    return visitor->visitChildren(this);
}

StreamDeclarationParser::KeywordContext* StreamDeclarationParser::keyword() {
  KeywordContext *_localctx = _tracker.createInstance<KeywordContext>(_ctx, getState());
  enterRule(_localctx, 82, StreamDeclarationParser::RuleKeyword);
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
    setState(434);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 2147483646) != 0))) {
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

bool StreamDeclarationParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 13: return core_patternSempred(antlrcpp::downCast<Core_patternContext *>(context), predicateIndex);
    case 17: return filterSempred(antlrcpp::downCast<FilterContext *>(context), predicateIndex);
    case 18: return bool_exprSempred(antlrcpp::downCast<Bool_exprContext *>(context), predicateIndex);
    case 21: return math_exprSempred(antlrcpp::downCast<Math_exprContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool StreamDeclarationParser::core_patternSempred(Core_patternContext *_localctx, size_t predicateIndex) {
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

bool StreamDeclarationParser::filterSempred(FilterContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 4: return precpred(_ctx, 2);
    case 5: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

bool StreamDeclarationParser::bool_exprSempred(Bool_exprContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 6: return precpred(_ctx, 5);
    case 7: return precpred(_ctx, 4);

  default:
    break;
  }
  return true;
}

bool StreamDeclarationParser::math_exprSempred(Math_exprContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 8: return precpred(_ctx, 2);
    case 9: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

void StreamDeclarationParser::initialize() {
  ::antlr4::internal::call_once(streamdeclarationparserParserOnceFlag, streamdeclarationparserParserInitialize);
}
