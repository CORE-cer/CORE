
// Generated from OptionDeclarationLexer.g4 by ANTLR 4.12.0


#include "OptionDeclarationLexer.h"


using namespace antlr4;



using namespace antlr4;

namespace {

struct OptionDeclarationLexerStaticData final {
  OptionDeclarationLexerStaticData(std::vector<std::string> ruleNames,
                          std::vector<std::string> channelNames,
                          std::vector<std::string> modeNames,
                          std::vector<std::string> literalNames,
                          std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), channelNames(std::move(channelNames)),
        modeNames(std::move(modeNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  OptionDeclarationLexerStaticData(const OptionDeclarationLexerStaticData&) = delete;
  OptionDeclarationLexerStaticData(OptionDeclarationLexerStaticData&&) = delete;
  OptionDeclarationLexerStaticData& operator=(const OptionDeclarationLexerStaticData&) = delete;
  OptionDeclarationLexerStaticData& operator=(OptionDeclarationLexerStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> channelNames;
  const std::vector<std::string> modeNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag optiondeclarationlexerLexerOnceFlag;
OptionDeclarationLexerStaticData *optiondeclarationlexerLexerStaticData = nullptr;

void optiondeclarationlexerLexerInitialize() {
  assert(optiondeclarationlexerLexerStaticData == nullptr);
  auto staticData = std::make_unique<OptionDeclarationLexerStaticData>(
    std::vector<std::string>{
      "K_DECLARE", "K_QUARANTINE", "K_FIXED_TIME", "K_DIRECT", "K_HOURS", 
      "K_MINUTES", "K_SECONDS", "LEFT_CURLY_BRACKET", "RIGHT_CURLY_BRACKET", 
      "COMMA", "DOUBLE_LITERAL", "INTEGER_LITERAL", "NUMERICAL_EXPONENT", 
      "IDENTIFIER", "UNEXPECTED_CHAR", "DIGIT", "A", "B", "C", "D", "E", 
      "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", 
      "T", "U", "V", "W", "X", "Y", "Z"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "'{'", "'}'", "','"
    },
    std::vector<std::string>{
      "", "K_DECLARE", "K_QUARANTINE", "K_FIXED_TIME", "K_DIRECT", "K_HOURS", 
      "K_MINUTES", "K_SECONDS", "LEFT_CURLY_BRACKET", "RIGHT_CURLY_BRACKET", 
      "COMMA", "DOUBLE_LITERAL", "INTEGER_LITERAL", "NUMERICAL_EXPONENT", 
      "IDENTIFIER", "UNEXPECTED_CHAR"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,0,15,268,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,
  	6,2,7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,
  	7,14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,
  	7,21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,
  	7,28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,
  	7,35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,2,40,7,40,2,41,7,41,1,0,
  	1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  	1,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,3,1,3,1,3,1,3,1,3,1,3,
  	1,3,1,4,1,4,1,4,1,4,1,4,3,4,128,8,4,1,5,1,5,1,5,1,5,1,5,1,5,1,5,3,5,137,
  	8,5,1,6,1,6,1,6,1,6,1,6,1,6,1,6,3,6,146,8,6,1,7,1,7,1,8,1,8,1,9,1,9,1,
  	10,1,10,1,10,1,10,1,10,3,10,159,8,10,1,10,1,10,4,10,163,8,10,11,10,12,
  	10,164,1,10,3,10,168,8,10,1,10,1,10,4,10,172,8,10,11,10,12,10,173,1,10,
  	1,10,3,10,178,8,10,1,11,4,11,181,8,11,11,11,12,11,182,1,12,1,12,3,12,
  	187,8,12,1,12,4,12,190,8,12,11,12,12,12,191,1,13,1,13,1,13,1,13,5,13,
  	198,8,13,10,13,12,13,201,9,13,1,13,1,13,1,13,5,13,206,8,13,10,13,12,13,
  	209,9,13,3,13,211,8,13,1,14,1,14,1,15,1,15,1,16,1,16,1,17,1,17,1,18,1,
  	18,1,19,1,19,1,20,1,20,1,21,1,21,1,22,1,22,1,23,1,23,1,24,1,24,1,25,1,
  	25,1,26,1,26,1,27,1,27,1,28,1,28,1,29,1,29,1,30,1,30,1,31,1,31,1,32,1,
  	32,1,33,1,33,1,34,1,34,1,35,1,35,1,36,1,36,1,37,1,37,1,38,1,38,1,39,1,
  	39,1,40,1,40,1,41,1,41,0,0,42,1,1,3,2,5,3,7,4,9,5,11,6,13,7,15,8,17,9,
  	19,10,21,11,23,12,25,13,27,14,29,15,31,0,33,0,35,0,37,0,39,0,41,0,43,
  	0,45,0,47,0,49,0,51,0,53,0,55,0,57,0,59,0,61,0,63,0,65,0,67,0,69,0,71,
  	0,73,0,75,0,77,0,79,0,81,0,83,0,1,0,30,1,0,96,96,3,0,65,90,95,95,97,122,
  	4,0,48,57,65,90,95,95,97,122,1,0,48,57,2,0,65,65,97,97,2,0,66,66,98,98,
  	2,0,67,67,99,99,2,0,68,68,100,100,2,0,69,69,101,101,2,0,70,70,102,102,
  	2,0,71,71,103,103,2,0,72,72,104,104,2,0,73,73,105,105,2,0,74,74,106,106,
  	2,0,75,75,107,107,2,0,76,76,108,108,2,0,77,77,109,109,2,0,78,78,110,110,
  	2,0,79,79,111,111,2,0,80,80,112,112,2,0,81,81,113,113,2,0,82,82,114,114,
  	2,0,83,83,115,115,2,0,84,84,116,116,2,0,85,85,117,117,2,0,86,86,118,118,
  	2,0,87,87,119,119,2,0,88,88,120,120,2,0,89,89,121,121,2,0,90,90,122,122,
  	256,0,1,1,0,0,0,0,3,1,0,0,0,0,5,1,0,0,0,0,7,1,0,0,0,0,9,1,0,0,0,0,11,
  	1,0,0,0,0,13,1,0,0,0,0,15,1,0,0,0,0,17,1,0,0,0,0,19,1,0,0,0,0,21,1,0,
  	0,0,0,23,1,0,0,0,0,25,1,0,0,0,0,27,1,0,0,0,0,29,1,0,0,0,1,85,1,0,0,0,
  	3,93,1,0,0,0,5,104,1,0,0,0,7,115,1,0,0,0,9,122,1,0,0,0,11,129,1,0,0,0,
  	13,138,1,0,0,0,15,147,1,0,0,0,17,149,1,0,0,0,19,151,1,0,0,0,21,177,1,
  	0,0,0,23,180,1,0,0,0,25,184,1,0,0,0,27,210,1,0,0,0,29,212,1,0,0,0,31,
  	214,1,0,0,0,33,216,1,0,0,0,35,218,1,0,0,0,37,220,1,0,0,0,39,222,1,0,0,
  	0,41,224,1,0,0,0,43,226,1,0,0,0,45,228,1,0,0,0,47,230,1,0,0,0,49,232,
  	1,0,0,0,51,234,1,0,0,0,53,236,1,0,0,0,55,238,1,0,0,0,57,240,1,0,0,0,59,
  	242,1,0,0,0,61,244,1,0,0,0,63,246,1,0,0,0,65,248,1,0,0,0,67,250,1,0,0,
  	0,69,252,1,0,0,0,71,254,1,0,0,0,73,256,1,0,0,0,75,258,1,0,0,0,77,260,
  	1,0,0,0,79,262,1,0,0,0,81,264,1,0,0,0,83,266,1,0,0,0,85,86,3,39,19,0,
  	86,87,3,41,20,0,87,88,3,37,18,0,88,89,3,55,27,0,89,90,3,33,16,0,90,91,
  	3,67,33,0,91,92,3,41,20,0,92,2,1,0,0,0,93,94,3,65,32,0,94,95,3,73,36,
  	0,95,96,3,33,16,0,96,97,3,67,33,0,97,98,3,33,16,0,98,99,3,59,29,0,99,
  	100,3,71,35,0,100,101,3,49,24,0,101,102,3,59,29,0,102,103,3,41,20,0,103,
  	4,1,0,0,0,104,105,3,43,21,0,105,106,3,49,24,0,106,107,3,79,39,0,107,108,
  	3,41,20,0,108,109,3,39,19,0,109,110,5,95,0,0,110,111,3,71,35,0,111,112,
  	3,49,24,0,112,113,3,57,28,0,113,114,3,41,20,0,114,6,1,0,0,0,115,116,3,
  	39,19,0,116,117,3,49,24,0,117,118,3,67,33,0,118,119,3,41,20,0,119,120,
  	3,37,18,0,120,121,3,71,35,0,121,8,1,0,0,0,122,123,3,47,23,0,123,124,3,
  	61,30,0,124,125,3,73,36,0,125,127,3,67,33,0,126,128,3,69,34,0,127,126,
  	1,0,0,0,127,128,1,0,0,0,128,10,1,0,0,0,129,130,3,57,28,0,130,131,3,49,
  	24,0,131,132,3,59,29,0,132,133,3,73,36,0,133,134,3,71,35,0,134,136,3,
  	41,20,0,135,137,3,69,34,0,136,135,1,0,0,0,136,137,1,0,0,0,137,12,1,0,
  	0,0,138,139,3,69,34,0,139,140,3,41,20,0,140,141,3,37,18,0,141,142,3,61,
  	30,0,142,143,3,59,29,0,143,145,3,39,19,0,144,146,3,69,34,0,145,144,1,
  	0,0,0,145,146,1,0,0,0,146,14,1,0,0,0,147,148,5,123,0,0,148,16,1,0,0,0,
  	149,150,5,125,0,0,150,18,1,0,0,0,151,152,5,44,0,0,152,20,1,0,0,0,153,
  	154,3,23,11,0,154,155,5,46,0,0,155,156,3,25,12,0,156,178,1,0,0,0,157,
  	159,3,23,11,0,158,157,1,0,0,0,158,159,1,0,0,0,159,160,1,0,0,0,160,162,
  	5,46,0,0,161,163,3,31,15,0,162,161,1,0,0,0,163,164,1,0,0,0,164,162,1,
  	0,0,0,164,165,1,0,0,0,165,178,1,0,0,0,166,168,3,23,11,0,167,166,1,0,0,
  	0,167,168,1,0,0,0,168,169,1,0,0,0,169,171,5,46,0,0,170,172,3,31,15,0,
  	171,170,1,0,0,0,172,173,1,0,0,0,173,171,1,0,0,0,173,174,1,0,0,0,174,175,
  	1,0,0,0,175,176,3,25,12,0,176,178,1,0,0,0,177,153,1,0,0,0,177,158,1,0,
  	0,0,177,167,1,0,0,0,178,22,1,0,0,0,179,181,3,31,15,0,180,179,1,0,0,0,
  	181,182,1,0,0,0,182,180,1,0,0,0,182,183,1,0,0,0,183,24,1,0,0,0,184,186,
  	3,41,20,0,185,187,5,45,0,0,186,185,1,0,0,0,186,187,1,0,0,0,187,189,1,
  	0,0,0,188,190,3,31,15,0,189,188,1,0,0,0,190,191,1,0,0,0,191,189,1,0,0,
  	0,191,192,1,0,0,0,192,26,1,0,0,0,193,199,5,96,0,0,194,198,8,0,0,0,195,
  	196,5,96,0,0,196,198,5,96,0,0,197,194,1,0,0,0,197,195,1,0,0,0,198,201,
  	1,0,0,0,199,197,1,0,0,0,199,200,1,0,0,0,200,202,1,0,0,0,201,199,1,0,0,
  	0,202,211,5,96,0,0,203,207,7,1,0,0,204,206,7,2,0,0,205,204,1,0,0,0,206,
  	209,1,0,0,0,207,205,1,0,0,0,207,208,1,0,0,0,208,211,1,0,0,0,209,207,1,
  	0,0,0,210,193,1,0,0,0,210,203,1,0,0,0,211,28,1,0,0,0,212,213,9,0,0,0,
  	213,30,1,0,0,0,214,215,7,3,0,0,215,32,1,0,0,0,216,217,7,4,0,0,217,34,
  	1,0,0,0,218,219,7,5,0,0,219,36,1,0,0,0,220,221,7,6,0,0,221,38,1,0,0,0,
  	222,223,7,7,0,0,223,40,1,0,0,0,224,225,7,8,0,0,225,42,1,0,0,0,226,227,
  	7,9,0,0,227,44,1,0,0,0,228,229,7,10,0,0,229,46,1,0,0,0,230,231,7,11,0,
  	0,231,48,1,0,0,0,232,233,7,12,0,0,233,50,1,0,0,0,234,235,7,13,0,0,235,
  	52,1,0,0,0,236,237,7,14,0,0,237,54,1,0,0,0,238,239,7,15,0,0,239,56,1,
  	0,0,0,240,241,7,16,0,0,241,58,1,0,0,0,242,243,7,17,0,0,243,60,1,0,0,0,
  	244,245,7,18,0,0,245,62,1,0,0,0,246,247,7,19,0,0,247,64,1,0,0,0,248,249,
  	7,20,0,0,249,66,1,0,0,0,250,251,7,21,0,0,251,68,1,0,0,0,252,253,7,22,
  	0,0,253,70,1,0,0,0,254,255,7,23,0,0,255,72,1,0,0,0,256,257,7,24,0,0,257,
  	74,1,0,0,0,258,259,7,25,0,0,259,76,1,0,0,0,260,261,7,26,0,0,261,78,1,
  	0,0,0,262,263,7,27,0,0,263,80,1,0,0,0,264,265,7,28,0,0,265,82,1,0,0,0,
  	266,267,7,29,0,0,267,84,1,0,0,0,16,0,127,136,145,158,164,167,173,177,
  	182,186,191,197,199,207,210,0
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  optiondeclarationlexerLexerStaticData = staticData.release();
}

}

OptionDeclarationLexer::OptionDeclarationLexer(CharStream *input) : Lexer(input) {
  OptionDeclarationLexer::initialize();
  _interpreter = new atn::LexerATNSimulator(this, *optiondeclarationlexerLexerStaticData->atn, optiondeclarationlexerLexerStaticData->decisionToDFA, optiondeclarationlexerLexerStaticData->sharedContextCache);
}

OptionDeclarationLexer::~OptionDeclarationLexer() {
  delete _interpreter;
}

std::string OptionDeclarationLexer::getGrammarFileName() const {
  return "OptionDeclarationLexer.g4";
}

const std::vector<std::string>& OptionDeclarationLexer::getRuleNames() const {
  return optiondeclarationlexerLexerStaticData->ruleNames;
}

const std::vector<std::string>& OptionDeclarationLexer::getChannelNames() const {
  return optiondeclarationlexerLexerStaticData->channelNames;
}

const std::vector<std::string>& OptionDeclarationLexer::getModeNames() const {
  return optiondeclarationlexerLexerStaticData->modeNames;
}

const dfa::Vocabulary& OptionDeclarationLexer::getVocabulary() const {
  return optiondeclarationlexerLexerStaticData->vocabulary;
}

antlr4::atn::SerializedATNView OptionDeclarationLexer::getSerializedATN() const {
  return optiondeclarationlexerLexerStaticData->serializedATN;
}

const atn::ATN& OptionDeclarationLexer::getATN() const {
  return *optiondeclarationlexerLexerStaticData->atn;
}




void OptionDeclarationLexer::initialize() {
  ::antlr4::internal::call_once(optiondeclarationlexerLexerOnceFlag, optiondeclarationlexerLexerInitialize);
}