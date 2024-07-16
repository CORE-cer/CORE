
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
      "K_DECLARE", "K_QUARANTINE", "K_FIXED_TIME", "K_HOURS", "K_MINUTES", 
      "K_SECONDS", "LEFT_CURLY_BRACKET", "RIGHT_CURLY_BRACKET", "DOUBLE_LITERAL", 
      "INTEGER_LITERAL", "NUMERICAL_EXPONENT", "IDENTIFIER", "UNEXPECTED_CHAR", 
      "DIGIT", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", 
      "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "'{'", "'}'"
    },
    std::vector<std::string>{
      "", "K_DECLARE", "K_QUARANTINE", "K_FIXED_TIME", "K_HOURS", "K_MINUTES", 
      "K_SECONDS", "LEFT_CURLY_BRACKET", "RIGHT_CURLY_BRACKET", "DOUBLE_LITERAL", 
      "INTEGER_LITERAL", "NUMERICAL_EXPONENT", "IDENTIFIER", "UNEXPECTED_CHAR"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,0,13,255,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,
  	6,2,7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,
  	7,14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,
  	7,21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,
  	7,28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,
  	7,35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,1,0,1,0,1,0,1,0,1,0,1,0,
  	1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,2,1,2,1,2,1,
  	2,1,2,1,2,1,2,1,2,1,2,1,2,1,3,1,3,1,3,1,3,1,3,3,3,117,8,3,1,4,1,4,1,4,
  	1,4,1,4,1,4,1,4,3,4,126,8,4,1,5,1,5,1,5,1,5,1,5,1,5,1,5,3,5,135,8,5,1,
  	6,1,6,1,7,1,7,1,8,1,8,1,8,1,8,1,8,3,8,146,8,8,1,8,1,8,4,8,150,8,8,11,
  	8,12,8,151,1,8,3,8,155,8,8,1,8,1,8,4,8,159,8,8,11,8,12,8,160,1,8,1,8,
  	3,8,165,8,8,1,9,4,9,168,8,9,11,9,12,9,169,1,10,1,10,3,10,174,8,10,1,10,
  	4,10,177,8,10,11,10,12,10,178,1,11,1,11,1,11,1,11,5,11,185,8,11,10,11,
  	12,11,188,9,11,1,11,1,11,1,11,5,11,193,8,11,10,11,12,11,196,9,11,3,11,
  	198,8,11,1,12,1,12,1,13,1,13,1,14,1,14,1,15,1,15,1,16,1,16,1,17,1,17,
  	1,18,1,18,1,19,1,19,1,20,1,20,1,21,1,21,1,22,1,22,1,23,1,23,1,24,1,24,
  	1,25,1,25,1,26,1,26,1,27,1,27,1,28,1,28,1,29,1,29,1,30,1,30,1,31,1,31,
  	1,32,1,32,1,33,1,33,1,34,1,34,1,35,1,35,1,36,1,36,1,37,1,37,1,38,1,38,
  	1,39,1,39,0,0,40,1,1,3,2,5,3,7,4,9,5,11,6,13,7,15,8,17,9,19,10,21,11,
  	23,12,25,13,27,0,29,0,31,0,33,0,35,0,37,0,39,0,41,0,43,0,45,0,47,0,49,
  	0,51,0,53,0,55,0,57,0,59,0,61,0,63,0,65,0,67,0,69,0,71,0,73,0,75,0,77,
  	0,79,0,1,0,30,1,0,96,96,3,0,65,90,95,95,97,122,4,0,48,57,65,90,95,95,
  	97,122,1,0,48,57,2,0,65,65,97,97,2,0,66,66,98,98,2,0,67,67,99,99,2,0,
  	68,68,100,100,2,0,69,69,101,101,2,0,70,70,102,102,2,0,71,71,103,103,2,
  	0,72,72,104,104,2,0,73,73,105,105,2,0,74,74,106,106,2,0,75,75,107,107,
  	2,0,76,76,108,108,2,0,77,77,109,109,2,0,78,78,110,110,2,0,79,79,111,111,
  	2,0,80,80,112,112,2,0,81,81,113,113,2,0,82,82,114,114,2,0,83,83,115,115,
  	2,0,84,84,116,116,2,0,85,85,117,117,2,0,86,86,118,118,2,0,87,87,119,119,
  	2,0,88,88,120,120,2,0,89,89,121,121,2,0,90,90,122,122,243,0,1,1,0,0,0,
  	0,3,1,0,0,0,0,5,1,0,0,0,0,7,1,0,0,0,0,9,1,0,0,0,0,11,1,0,0,0,0,13,1,0,
  	0,0,0,15,1,0,0,0,0,17,1,0,0,0,0,19,1,0,0,0,0,21,1,0,0,0,0,23,1,0,0,0,
  	0,25,1,0,0,0,1,81,1,0,0,0,3,89,1,0,0,0,5,100,1,0,0,0,7,111,1,0,0,0,9,
  	118,1,0,0,0,11,127,1,0,0,0,13,136,1,0,0,0,15,138,1,0,0,0,17,164,1,0,0,
  	0,19,167,1,0,0,0,21,171,1,0,0,0,23,197,1,0,0,0,25,199,1,0,0,0,27,201,
  	1,0,0,0,29,203,1,0,0,0,31,205,1,0,0,0,33,207,1,0,0,0,35,209,1,0,0,0,37,
  	211,1,0,0,0,39,213,1,0,0,0,41,215,1,0,0,0,43,217,1,0,0,0,45,219,1,0,0,
  	0,47,221,1,0,0,0,49,223,1,0,0,0,51,225,1,0,0,0,53,227,1,0,0,0,55,229,
  	1,0,0,0,57,231,1,0,0,0,59,233,1,0,0,0,61,235,1,0,0,0,63,237,1,0,0,0,65,
  	239,1,0,0,0,67,241,1,0,0,0,69,243,1,0,0,0,71,245,1,0,0,0,73,247,1,0,0,
  	0,75,249,1,0,0,0,77,251,1,0,0,0,79,253,1,0,0,0,81,82,3,35,17,0,82,83,
  	3,37,18,0,83,84,3,33,16,0,84,85,3,51,25,0,85,86,3,29,14,0,86,87,3,63,
  	31,0,87,88,3,37,18,0,88,2,1,0,0,0,89,90,3,61,30,0,90,91,3,69,34,0,91,
  	92,3,29,14,0,92,93,3,63,31,0,93,94,3,29,14,0,94,95,3,55,27,0,95,96,3,
  	67,33,0,96,97,3,45,22,0,97,98,3,55,27,0,98,99,3,37,18,0,99,4,1,0,0,0,
  	100,101,3,39,19,0,101,102,3,45,22,0,102,103,3,75,37,0,103,104,3,37,18,
  	0,104,105,3,35,17,0,105,106,5,95,0,0,106,107,3,67,33,0,107,108,3,45,22,
  	0,108,109,3,53,26,0,109,110,3,37,18,0,110,6,1,0,0,0,111,112,3,43,21,0,
  	112,113,3,57,28,0,113,114,3,69,34,0,114,116,3,63,31,0,115,117,3,65,32,
  	0,116,115,1,0,0,0,116,117,1,0,0,0,117,8,1,0,0,0,118,119,3,53,26,0,119,
  	120,3,45,22,0,120,121,3,55,27,0,121,122,3,69,34,0,122,123,3,67,33,0,123,
  	125,3,37,18,0,124,126,3,65,32,0,125,124,1,0,0,0,125,126,1,0,0,0,126,10,
  	1,0,0,0,127,128,3,65,32,0,128,129,3,37,18,0,129,130,3,33,16,0,130,131,
  	3,57,28,0,131,132,3,55,27,0,132,134,3,35,17,0,133,135,3,65,32,0,134,133,
  	1,0,0,0,134,135,1,0,0,0,135,12,1,0,0,0,136,137,5,123,0,0,137,14,1,0,0,
  	0,138,139,5,125,0,0,139,16,1,0,0,0,140,141,3,19,9,0,141,142,5,46,0,0,
  	142,143,3,21,10,0,143,165,1,0,0,0,144,146,3,19,9,0,145,144,1,0,0,0,145,
  	146,1,0,0,0,146,147,1,0,0,0,147,149,5,46,0,0,148,150,3,27,13,0,149,148,
  	1,0,0,0,150,151,1,0,0,0,151,149,1,0,0,0,151,152,1,0,0,0,152,165,1,0,0,
  	0,153,155,3,19,9,0,154,153,1,0,0,0,154,155,1,0,0,0,155,156,1,0,0,0,156,
  	158,5,46,0,0,157,159,3,27,13,0,158,157,1,0,0,0,159,160,1,0,0,0,160,158,
  	1,0,0,0,160,161,1,0,0,0,161,162,1,0,0,0,162,163,3,21,10,0,163,165,1,0,
  	0,0,164,140,1,0,0,0,164,145,1,0,0,0,164,154,1,0,0,0,165,18,1,0,0,0,166,
  	168,3,27,13,0,167,166,1,0,0,0,168,169,1,0,0,0,169,167,1,0,0,0,169,170,
  	1,0,0,0,170,20,1,0,0,0,171,173,3,37,18,0,172,174,5,45,0,0,173,172,1,0,
  	0,0,173,174,1,0,0,0,174,176,1,0,0,0,175,177,3,27,13,0,176,175,1,0,0,0,
  	177,178,1,0,0,0,178,176,1,0,0,0,178,179,1,0,0,0,179,22,1,0,0,0,180,186,
  	5,96,0,0,181,185,8,0,0,0,182,183,5,96,0,0,183,185,5,96,0,0,184,181,1,
  	0,0,0,184,182,1,0,0,0,185,188,1,0,0,0,186,184,1,0,0,0,186,187,1,0,0,0,
  	187,189,1,0,0,0,188,186,1,0,0,0,189,198,5,96,0,0,190,194,7,1,0,0,191,
  	193,7,2,0,0,192,191,1,0,0,0,193,196,1,0,0,0,194,192,1,0,0,0,194,195,1,
  	0,0,0,195,198,1,0,0,0,196,194,1,0,0,0,197,180,1,0,0,0,197,190,1,0,0,0,
  	198,24,1,0,0,0,199,200,9,0,0,0,200,26,1,0,0,0,201,202,7,3,0,0,202,28,
  	1,0,0,0,203,204,7,4,0,0,204,30,1,0,0,0,205,206,7,5,0,0,206,32,1,0,0,0,
  	207,208,7,6,0,0,208,34,1,0,0,0,209,210,7,7,0,0,210,36,1,0,0,0,211,212,
  	7,8,0,0,212,38,1,0,0,0,213,214,7,9,0,0,214,40,1,0,0,0,215,216,7,10,0,
  	0,216,42,1,0,0,0,217,218,7,11,0,0,218,44,1,0,0,0,219,220,7,12,0,0,220,
  	46,1,0,0,0,221,222,7,13,0,0,222,48,1,0,0,0,223,224,7,14,0,0,224,50,1,
  	0,0,0,225,226,7,15,0,0,226,52,1,0,0,0,227,228,7,16,0,0,228,54,1,0,0,0,
  	229,230,7,17,0,0,230,56,1,0,0,0,231,232,7,18,0,0,232,58,1,0,0,0,233,234,
  	7,19,0,0,234,60,1,0,0,0,235,236,7,20,0,0,236,62,1,0,0,0,237,238,7,21,
  	0,0,238,64,1,0,0,0,239,240,7,22,0,0,240,66,1,0,0,0,241,242,7,23,0,0,242,
  	68,1,0,0,0,243,244,7,24,0,0,244,70,1,0,0,0,245,246,7,25,0,0,246,72,1,
  	0,0,0,247,248,7,26,0,0,248,74,1,0,0,0,249,250,7,27,0,0,250,76,1,0,0,0,
  	251,252,7,28,0,0,252,78,1,0,0,0,253,254,7,29,0,0,254,80,1,0,0,0,16,0,
  	116,125,134,145,151,154,160,164,169,173,178,184,186,194,197,0
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
