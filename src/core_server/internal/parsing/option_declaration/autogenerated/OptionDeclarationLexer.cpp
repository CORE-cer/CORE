
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
      "WS", "K_DECLARE", "K_QUARANTINE", "K_FIXED_TIME", "K_DIRECT", "K_HOURS", 
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
      "", "", "", "", "", "", "", "", "", "'{'", "'}'", "','"
    },
    std::vector<std::string>{
      "", "WS", "K_DECLARE", "K_QUARANTINE", "K_FIXED_TIME", "K_DIRECT", 
      "K_HOURS", "K_MINUTES", "K_SECONDS", "LEFT_CURLY_BRACKET", "RIGHT_CURLY_BRACKET", 
      "COMMA", "DOUBLE_LITERAL", "INTEGER_LITERAL", "NUMERICAL_EXPONENT", 
      "IDENTIFIER", "UNEXPECTED_CHAR"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,0,16,277,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,
  	6,2,7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,
  	7,14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,
  	7,21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,
  	7,28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,
  	7,35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,2,40,7,40,2,41,7,41,2,42,
  	7,42,1,0,4,0,89,8,0,11,0,12,0,90,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  	1,1,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,3,1,3,1,3,1,3,1,3,1,
  	3,1,3,1,3,1,3,1,3,1,3,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,5,1,5,1,5,1,5,1,5,
  	3,5,137,8,5,1,6,1,6,1,6,1,6,1,6,1,6,1,6,3,6,146,8,6,1,7,1,7,1,7,1,7,1,
  	7,1,7,1,7,3,7,155,8,7,1,8,1,8,1,9,1,9,1,10,1,10,1,11,1,11,1,11,1,11,1,
  	11,3,11,168,8,11,1,11,1,11,4,11,172,8,11,11,11,12,11,173,1,11,3,11,177,
  	8,11,1,11,1,11,4,11,181,8,11,11,11,12,11,182,1,11,1,11,3,11,187,8,11,
  	1,12,4,12,190,8,12,11,12,12,12,191,1,13,1,13,3,13,196,8,13,1,13,4,13,
  	199,8,13,11,13,12,13,200,1,14,1,14,1,14,1,14,5,14,207,8,14,10,14,12,14,
  	210,9,14,1,14,1,14,1,14,5,14,215,8,14,10,14,12,14,218,9,14,3,14,220,8,
  	14,1,15,1,15,1,16,1,16,1,17,1,17,1,18,1,18,1,19,1,19,1,20,1,20,1,21,1,
  	21,1,22,1,22,1,23,1,23,1,24,1,24,1,25,1,25,1,26,1,26,1,27,1,27,1,28,1,
  	28,1,29,1,29,1,30,1,30,1,31,1,31,1,32,1,32,1,33,1,33,1,34,1,34,1,35,1,
  	35,1,36,1,36,1,37,1,37,1,38,1,38,1,39,1,39,1,40,1,40,1,41,1,41,1,42,1,
  	42,0,0,43,1,1,3,2,5,3,7,4,9,5,11,6,13,7,15,8,17,9,19,10,21,11,23,12,25,
  	13,27,14,29,15,31,16,33,0,35,0,37,0,39,0,41,0,43,0,45,0,47,0,49,0,51,
  	0,53,0,55,0,57,0,59,0,61,0,63,0,65,0,67,0,69,0,71,0,73,0,75,0,77,0,79,
  	0,81,0,83,0,85,0,1,0,31,3,0,9,10,13,13,32,32,1,0,96,96,3,0,65,90,95,95,
  	97,122,4,0,48,57,65,90,95,95,97,122,1,0,48,57,2,0,65,65,97,97,2,0,66,
  	66,98,98,2,0,67,67,99,99,2,0,68,68,100,100,2,0,69,69,101,101,2,0,70,70,
  	102,102,2,0,71,71,103,103,2,0,72,72,104,104,2,0,73,73,105,105,2,0,74,
  	74,106,106,2,0,75,75,107,107,2,0,76,76,108,108,2,0,77,77,109,109,2,0,
  	78,78,110,110,2,0,79,79,111,111,2,0,80,80,112,112,2,0,81,81,113,113,2,
  	0,82,82,114,114,2,0,83,83,115,115,2,0,84,84,116,116,2,0,85,85,117,117,
  	2,0,86,86,118,118,2,0,87,87,119,119,2,0,88,88,120,120,2,0,89,89,121,121,
  	2,0,90,90,122,122,266,0,1,1,0,0,0,0,3,1,0,0,0,0,5,1,0,0,0,0,7,1,0,0,0,
  	0,9,1,0,0,0,0,11,1,0,0,0,0,13,1,0,0,0,0,15,1,0,0,0,0,17,1,0,0,0,0,19,
  	1,0,0,0,0,21,1,0,0,0,0,23,1,0,0,0,0,25,1,0,0,0,0,27,1,0,0,0,0,29,1,0,
  	0,0,0,31,1,0,0,0,1,88,1,0,0,0,3,94,1,0,0,0,5,102,1,0,0,0,7,113,1,0,0,
  	0,9,124,1,0,0,0,11,131,1,0,0,0,13,138,1,0,0,0,15,147,1,0,0,0,17,156,1,
  	0,0,0,19,158,1,0,0,0,21,160,1,0,0,0,23,186,1,0,0,0,25,189,1,0,0,0,27,
  	193,1,0,0,0,29,219,1,0,0,0,31,221,1,0,0,0,33,223,1,0,0,0,35,225,1,0,0,
  	0,37,227,1,0,0,0,39,229,1,0,0,0,41,231,1,0,0,0,43,233,1,0,0,0,45,235,
  	1,0,0,0,47,237,1,0,0,0,49,239,1,0,0,0,51,241,1,0,0,0,53,243,1,0,0,0,55,
  	245,1,0,0,0,57,247,1,0,0,0,59,249,1,0,0,0,61,251,1,0,0,0,63,253,1,0,0,
  	0,65,255,1,0,0,0,67,257,1,0,0,0,69,259,1,0,0,0,71,261,1,0,0,0,73,263,
  	1,0,0,0,75,265,1,0,0,0,77,267,1,0,0,0,79,269,1,0,0,0,81,271,1,0,0,0,83,
  	273,1,0,0,0,85,275,1,0,0,0,87,89,7,0,0,0,88,87,1,0,0,0,89,90,1,0,0,0,
  	90,88,1,0,0,0,90,91,1,0,0,0,91,92,1,0,0,0,92,93,6,0,0,0,93,2,1,0,0,0,
  	94,95,3,41,20,0,95,96,3,43,21,0,96,97,3,39,19,0,97,98,3,57,28,0,98,99,
  	3,35,17,0,99,100,3,69,34,0,100,101,3,43,21,0,101,4,1,0,0,0,102,103,3,
  	67,33,0,103,104,3,75,37,0,104,105,3,35,17,0,105,106,3,69,34,0,106,107,
  	3,35,17,0,107,108,3,61,30,0,108,109,3,73,36,0,109,110,3,51,25,0,110,111,
  	3,61,30,0,111,112,3,43,21,0,112,6,1,0,0,0,113,114,3,45,22,0,114,115,3,
  	51,25,0,115,116,3,81,40,0,116,117,3,43,21,0,117,118,3,41,20,0,118,119,
  	5,95,0,0,119,120,3,73,36,0,120,121,3,51,25,0,121,122,3,59,29,0,122,123,
  	3,43,21,0,123,8,1,0,0,0,124,125,3,41,20,0,125,126,3,51,25,0,126,127,3,
  	69,34,0,127,128,3,43,21,0,128,129,3,39,19,0,129,130,3,73,36,0,130,10,
  	1,0,0,0,131,132,3,49,24,0,132,133,3,63,31,0,133,134,3,75,37,0,134,136,
  	3,69,34,0,135,137,3,71,35,0,136,135,1,0,0,0,136,137,1,0,0,0,137,12,1,
  	0,0,0,138,139,3,59,29,0,139,140,3,51,25,0,140,141,3,61,30,0,141,142,3,
  	75,37,0,142,143,3,73,36,0,143,145,3,43,21,0,144,146,3,71,35,0,145,144,
  	1,0,0,0,145,146,1,0,0,0,146,14,1,0,0,0,147,148,3,71,35,0,148,149,3,43,
  	21,0,149,150,3,39,19,0,150,151,3,63,31,0,151,152,3,61,30,0,152,154,3,
  	41,20,0,153,155,3,71,35,0,154,153,1,0,0,0,154,155,1,0,0,0,155,16,1,0,
  	0,0,156,157,5,123,0,0,157,18,1,0,0,0,158,159,5,125,0,0,159,20,1,0,0,0,
  	160,161,5,44,0,0,161,22,1,0,0,0,162,163,3,25,12,0,163,164,5,46,0,0,164,
  	165,3,27,13,0,165,187,1,0,0,0,166,168,3,25,12,0,167,166,1,0,0,0,167,168,
  	1,0,0,0,168,169,1,0,0,0,169,171,5,46,0,0,170,172,3,33,16,0,171,170,1,
  	0,0,0,172,173,1,0,0,0,173,171,1,0,0,0,173,174,1,0,0,0,174,187,1,0,0,0,
  	175,177,3,25,12,0,176,175,1,0,0,0,176,177,1,0,0,0,177,178,1,0,0,0,178,
  	180,5,46,0,0,179,181,3,33,16,0,180,179,1,0,0,0,181,182,1,0,0,0,182,180,
  	1,0,0,0,182,183,1,0,0,0,183,184,1,0,0,0,184,185,3,27,13,0,185,187,1,0,
  	0,0,186,162,1,0,0,0,186,167,1,0,0,0,186,176,1,0,0,0,187,24,1,0,0,0,188,
  	190,3,33,16,0,189,188,1,0,0,0,190,191,1,0,0,0,191,189,1,0,0,0,191,192,
  	1,0,0,0,192,26,1,0,0,0,193,195,3,43,21,0,194,196,5,45,0,0,195,194,1,0,
  	0,0,195,196,1,0,0,0,196,198,1,0,0,0,197,199,3,33,16,0,198,197,1,0,0,0,
  	199,200,1,0,0,0,200,198,1,0,0,0,200,201,1,0,0,0,201,28,1,0,0,0,202,208,
  	5,96,0,0,203,207,8,1,0,0,204,205,5,96,0,0,205,207,5,96,0,0,206,203,1,
  	0,0,0,206,204,1,0,0,0,207,210,1,0,0,0,208,206,1,0,0,0,208,209,1,0,0,0,
  	209,211,1,0,0,0,210,208,1,0,0,0,211,220,5,96,0,0,212,216,7,2,0,0,213,
  	215,7,3,0,0,214,213,1,0,0,0,215,218,1,0,0,0,216,214,1,0,0,0,216,217,1,
  	0,0,0,217,220,1,0,0,0,218,216,1,0,0,0,219,202,1,0,0,0,219,212,1,0,0,0,
  	220,30,1,0,0,0,221,222,9,0,0,0,222,32,1,0,0,0,223,224,7,4,0,0,224,34,
  	1,0,0,0,225,226,7,5,0,0,226,36,1,0,0,0,227,228,7,6,0,0,228,38,1,0,0,0,
  	229,230,7,7,0,0,230,40,1,0,0,0,231,232,7,8,0,0,232,42,1,0,0,0,233,234,
  	7,9,0,0,234,44,1,0,0,0,235,236,7,10,0,0,236,46,1,0,0,0,237,238,7,11,0,
  	0,238,48,1,0,0,0,239,240,7,12,0,0,240,50,1,0,0,0,241,242,7,13,0,0,242,
  	52,1,0,0,0,243,244,7,14,0,0,244,54,1,0,0,0,245,246,7,15,0,0,246,56,1,
  	0,0,0,247,248,7,16,0,0,248,58,1,0,0,0,249,250,7,17,0,0,250,60,1,0,0,0,
  	251,252,7,18,0,0,252,62,1,0,0,0,253,254,7,19,0,0,254,64,1,0,0,0,255,256,
  	7,20,0,0,256,66,1,0,0,0,257,258,7,21,0,0,258,68,1,0,0,0,259,260,7,22,
  	0,0,260,70,1,0,0,0,261,262,7,23,0,0,262,72,1,0,0,0,263,264,7,24,0,0,264,
  	74,1,0,0,0,265,266,7,25,0,0,266,76,1,0,0,0,267,268,7,26,0,0,268,78,1,
  	0,0,0,269,270,7,27,0,0,270,80,1,0,0,0,271,272,7,28,0,0,272,82,1,0,0,0,
  	273,274,7,29,0,0,274,84,1,0,0,0,275,276,7,30,0,0,276,86,1,0,0,0,17,0,
  	90,136,145,154,167,173,176,182,186,191,195,200,206,208,216,219,1,6,0,
  	0
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
