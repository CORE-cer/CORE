
// Generated from OptionDeclarationParser.g4 by ANTLR 4.12.0


#include "OptionDeclarationParserVisitor.h"

#include "OptionDeclarationParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct OptionDeclarationParserStaticData final {
  OptionDeclarationParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  OptionDeclarationParserStaticData(const OptionDeclarationParserStaticData&) = delete;
  OptionDeclarationParserStaticData(OptionDeclarationParserStaticData&&) = delete;
  OptionDeclarationParserStaticData& operator=(const OptionDeclarationParserStaticData&) = delete;
  OptionDeclarationParserStaticData& operator=(OptionDeclarationParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag optiondeclarationparserParserOnceFlag;
OptionDeclarationParserStaticData *optiondeclarationparserParserStaticData = nullptr;

void optiondeclarationparserParserInitialize() {
  assert(optiondeclarationparserParserStaticData == nullptr);
  auto staticData = std::make_unique<OptionDeclarationParserStaticData>(
    std::vector<std::string>{
      "parse", "error", "option_declaration", "quarantine_policy", "stream_names", 
      "time_span", "hour_span", "minute_span", "second_span", "stream_name", 
      "any_name", "number", "integer", "double"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "'{'", "'}'", "','"
    },
    std::vector<std::string>{
      "", "WS", "K_CREATE", "K_QUARANTINE", "K_FIXED_TIME", "K_BOUNDED_TIME", 
      "K_DIRECT", "K_HOURS", "K_MINUTES", "K_SECONDS", "LEFT_CURLY_BRACKET", 
      "RIGHT_CURLY_BRACKET", "COMMA", "DOUBLE_LITERAL", "INTEGER_LITERAL", 
      "NUMERICAL_EXPONENT", "IDENTIFIER", "UNEXPECTED_CHAR"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,17,109,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,1,0,1,0,
  	5,0,31,8,0,10,0,12,0,34,9,0,1,0,1,0,1,1,1,1,1,1,1,2,1,2,1,2,1,2,5,2,45,
  	8,2,10,2,12,2,48,9,2,1,2,1,2,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,
  	1,3,1,3,1,3,1,3,1,3,1,3,1,3,3,3,69,8,3,1,4,1,4,1,4,5,4,74,8,4,10,4,12,
  	4,77,9,4,1,5,3,5,80,8,5,1,5,3,5,83,8,5,1,5,3,5,86,8,5,1,6,1,6,1,6,1,7,
  	1,7,1,7,1,8,1,8,1,8,1,9,1,9,1,10,1,10,1,11,1,11,3,11,103,8,11,1,12,1,
  	12,1,13,1,13,1,13,0,0,14,0,2,4,6,8,10,12,14,16,18,20,22,24,26,0,0,104,
  	0,32,1,0,0,0,2,37,1,0,0,0,4,40,1,0,0,0,6,68,1,0,0,0,8,70,1,0,0,0,10,79,
  	1,0,0,0,12,87,1,0,0,0,14,90,1,0,0,0,16,93,1,0,0,0,18,96,1,0,0,0,20,98,
  	1,0,0,0,22,102,1,0,0,0,24,104,1,0,0,0,26,106,1,0,0,0,28,31,3,4,2,0,29,
  	31,3,2,1,0,30,28,1,0,0,0,30,29,1,0,0,0,31,34,1,0,0,0,32,30,1,0,0,0,32,
  	33,1,0,0,0,33,35,1,0,0,0,34,32,1,0,0,0,35,36,5,0,0,1,36,1,1,0,0,0,37,
  	38,5,17,0,0,38,39,6,1,-1,0,39,3,1,0,0,0,40,41,5,2,0,0,41,42,5,3,0,0,42,
  	46,5,10,0,0,43,45,3,6,3,0,44,43,1,0,0,0,45,48,1,0,0,0,46,44,1,0,0,0,46,
  	47,1,0,0,0,47,49,1,0,0,0,48,46,1,0,0,0,49,50,5,11,0,0,50,5,1,0,0,0,51,
  	52,5,4,0,0,52,53,3,10,5,0,53,54,5,10,0,0,54,55,3,8,4,0,55,56,5,11,0,0,
  	56,69,1,0,0,0,57,58,5,5,0,0,58,59,3,10,5,0,59,60,5,10,0,0,60,61,3,8,4,
  	0,61,62,5,11,0,0,62,69,1,0,0,0,63,64,5,6,0,0,64,65,5,10,0,0,65,66,3,8,
  	4,0,66,67,5,11,0,0,67,69,1,0,0,0,68,51,1,0,0,0,68,57,1,0,0,0,68,63,1,
  	0,0,0,69,7,1,0,0,0,70,75,3,18,9,0,71,72,5,12,0,0,72,74,3,18,9,0,73,71,
  	1,0,0,0,74,77,1,0,0,0,75,73,1,0,0,0,75,76,1,0,0,0,76,9,1,0,0,0,77,75,
  	1,0,0,0,78,80,3,12,6,0,79,78,1,0,0,0,79,80,1,0,0,0,80,82,1,0,0,0,81,83,
  	3,14,7,0,82,81,1,0,0,0,82,83,1,0,0,0,83,85,1,0,0,0,84,86,3,16,8,0,85,
  	84,1,0,0,0,85,86,1,0,0,0,86,11,1,0,0,0,87,88,3,24,12,0,88,89,5,7,0,0,
  	89,13,1,0,0,0,90,91,3,24,12,0,91,92,5,8,0,0,92,15,1,0,0,0,93,94,3,24,
  	12,0,94,95,5,9,0,0,95,17,1,0,0,0,96,97,3,20,10,0,97,19,1,0,0,0,98,99,
  	5,16,0,0,99,21,1,0,0,0,100,103,3,24,12,0,101,103,3,26,13,0,102,100,1,
  	0,0,0,102,101,1,0,0,0,103,23,1,0,0,0,104,105,5,14,0,0,105,25,1,0,0,0,
  	106,107,5,13,0,0,107,27,1,0,0,0,9,30,32,46,68,75,79,82,85,102
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  optiondeclarationparserParserStaticData = staticData.release();
}

}

OptionDeclarationParser::OptionDeclarationParser(TokenStream *input) : OptionDeclarationParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

OptionDeclarationParser::OptionDeclarationParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  OptionDeclarationParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *optiondeclarationparserParserStaticData->atn, optiondeclarationparserParserStaticData->decisionToDFA, optiondeclarationparserParserStaticData->sharedContextCache, options);
}

OptionDeclarationParser::~OptionDeclarationParser() {
  delete _interpreter;
}

const atn::ATN& OptionDeclarationParser::getATN() const {
  return *optiondeclarationparserParserStaticData->atn;
}

std::string OptionDeclarationParser::getGrammarFileName() const {
  return "OptionDeclarationParser.g4";
}

const std::vector<std::string>& OptionDeclarationParser::getRuleNames() const {
  return optiondeclarationparserParserStaticData->ruleNames;
}

const dfa::Vocabulary& OptionDeclarationParser::getVocabulary() const {
  return optiondeclarationparserParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView OptionDeclarationParser::getSerializedATN() const {
  return optiondeclarationparserParserStaticData->serializedATN;
}


//----------------- ParseContext ------------------------------------------------------------------

OptionDeclarationParser::ParseContext::ParseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* OptionDeclarationParser::ParseContext::EOF() {
  return getToken(OptionDeclarationParser::EOF, 0);
}

std::vector<OptionDeclarationParser::Option_declarationContext *> OptionDeclarationParser::ParseContext::option_declaration() {
  return getRuleContexts<OptionDeclarationParser::Option_declarationContext>();
}

OptionDeclarationParser::Option_declarationContext* OptionDeclarationParser::ParseContext::option_declaration(size_t i) {
  return getRuleContext<OptionDeclarationParser::Option_declarationContext>(i);
}

std::vector<OptionDeclarationParser::ErrorContext *> OptionDeclarationParser::ParseContext::error() {
  return getRuleContexts<OptionDeclarationParser::ErrorContext>();
}

OptionDeclarationParser::ErrorContext* OptionDeclarationParser::ParseContext::error(size_t i) {
  return getRuleContext<OptionDeclarationParser::ErrorContext>(i);
}


size_t OptionDeclarationParser::ParseContext::getRuleIndex() const {
  return OptionDeclarationParser::RuleParse;
}


std::any OptionDeclarationParser::ParseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<OptionDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitParse(this);
  else
    return visitor->visitChildren(this);
}

OptionDeclarationParser::ParseContext* OptionDeclarationParser::parse() {
  ParseContext *_localctx = _tracker.createInstance<ParseContext>(_ctx, getState());
  enterRule(_localctx, 0, OptionDeclarationParser::RuleParse);
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
    setState(32);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == OptionDeclarationParser::K_CREATE

    || _la == OptionDeclarationParser::UNEXPECTED_CHAR) {
      setState(30);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case OptionDeclarationParser::K_CREATE: {
          setState(28);
          option_declaration();
          break;
        }

        case OptionDeclarationParser::UNEXPECTED_CHAR: {
          setState(29);
          error();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(34);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(35);
    match(OptionDeclarationParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ErrorContext ------------------------------------------------------------------

OptionDeclarationParser::ErrorContext::ErrorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* OptionDeclarationParser::ErrorContext::UNEXPECTED_CHAR() {
  return getToken(OptionDeclarationParser::UNEXPECTED_CHAR, 0);
}


size_t OptionDeclarationParser::ErrorContext::getRuleIndex() const {
  return OptionDeclarationParser::RuleError;
}


std::any OptionDeclarationParser::ErrorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<OptionDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitError(this);
  else
    return visitor->visitChildren(this);
}

OptionDeclarationParser::ErrorContext* OptionDeclarationParser::error() {
  ErrorContext *_localctx = _tracker.createInstance<ErrorContext>(_ctx, getState());
  enterRule(_localctx, 2, OptionDeclarationParser::RuleError);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(37);
    antlrcpp::downCast<ErrorContext *>(_localctx)->unexpected_charToken = match(OptionDeclarationParser::UNEXPECTED_CHAR);

         throw new RuntimeException("UNEXPECTED_CHAR=" + (antlrcpp::downCast<ErrorContext *>(_localctx)->unexpected_charToken != nullptr ? antlrcpp::downCast<ErrorContext *>(_localctx)->unexpected_charToken->getText() : ""));
       
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Option_declarationContext ------------------------------------------------------------------

OptionDeclarationParser::Option_declarationContext::Option_declarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* OptionDeclarationParser::Option_declarationContext::K_CREATE() {
  return getToken(OptionDeclarationParser::K_CREATE, 0);
}

tree::TerminalNode* OptionDeclarationParser::Option_declarationContext::K_QUARANTINE() {
  return getToken(OptionDeclarationParser::K_QUARANTINE, 0);
}

tree::TerminalNode* OptionDeclarationParser::Option_declarationContext::LEFT_CURLY_BRACKET() {
  return getToken(OptionDeclarationParser::LEFT_CURLY_BRACKET, 0);
}

tree::TerminalNode* OptionDeclarationParser::Option_declarationContext::RIGHT_CURLY_BRACKET() {
  return getToken(OptionDeclarationParser::RIGHT_CURLY_BRACKET, 0);
}

std::vector<OptionDeclarationParser::Quarantine_policyContext *> OptionDeclarationParser::Option_declarationContext::quarantine_policy() {
  return getRuleContexts<OptionDeclarationParser::Quarantine_policyContext>();
}

OptionDeclarationParser::Quarantine_policyContext* OptionDeclarationParser::Option_declarationContext::quarantine_policy(size_t i) {
  return getRuleContext<OptionDeclarationParser::Quarantine_policyContext>(i);
}


size_t OptionDeclarationParser::Option_declarationContext::getRuleIndex() const {
  return OptionDeclarationParser::RuleOption_declaration;
}


std::any OptionDeclarationParser::Option_declarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<OptionDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitOption_declaration(this);
  else
    return visitor->visitChildren(this);
}

OptionDeclarationParser::Option_declarationContext* OptionDeclarationParser::option_declaration() {
  Option_declarationContext *_localctx = _tracker.createInstance<Option_declarationContext>(_ctx, getState());
  enterRule(_localctx, 4, OptionDeclarationParser::RuleOption_declaration);
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
    setState(40);
    match(OptionDeclarationParser::K_CREATE);
    setState(41);
    match(OptionDeclarationParser::K_QUARANTINE);
    setState(42);
    match(OptionDeclarationParser::LEFT_CURLY_BRACKET);
    setState(46);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 112) != 0)) {
      setState(43);
      quarantine_policy();
      setState(48);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(49);
    match(OptionDeclarationParser::RIGHT_CURLY_BRACKET);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Quarantine_policyContext ------------------------------------------------------------------

OptionDeclarationParser::Quarantine_policyContext::Quarantine_policyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t OptionDeclarationParser::Quarantine_policyContext::getRuleIndex() const {
  return OptionDeclarationParser::RuleQuarantine_policy;
}

void OptionDeclarationParser::Quarantine_policyContext::copyFrom(Quarantine_policyContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Bounded_time_policyContext ------------------------------------------------------------------

tree::TerminalNode* OptionDeclarationParser::Bounded_time_policyContext::K_BOUNDED_TIME() {
  return getToken(OptionDeclarationParser::K_BOUNDED_TIME, 0);
}

OptionDeclarationParser::Time_spanContext* OptionDeclarationParser::Bounded_time_policyContext::time_span() {
  return getRuleContext<OptionDeclarationParser::Time_spanContext>(0);
}

tree::TerminalNode* OptionDeclarationParser::Bounded_time_policyContext::LEFT_CURLY_BRACKET() {
  return getToken(OptionDeclarationParser::LEFT_CURLY_BRACKET, 0);
}

OptionDeclarationParser::Stream_namesContext* OptionDeclarationParser::Bounded_time_policyContext::stream_names() {
  return getRuleContext<OptionDeclarationParser::Stream_namesContext>(0);
}

tree::TerminalNode* OptionDeclarationParser::Bounded_time_policyContext::RIGHT_CURLY_BRACKET() {
  return getToken(OptionDeclarationParser::RIGHT_CURLY_BRACKET, 0);
}

OptionDeclarationParser::Bounded_time_policyContext::Bounded_time_policyContext(Quarantine_policyContext *ctx) { copyFrom(ctx); }


std::any OptionDeclarationParser::Bounded_time_policyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<OptionDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitBounded_time_policy(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Direct_policyContext ------------------------------------------------------------------

tree::TerminalNode* OptionDeclarationParser::Direct_policyContext::K_DIRECT() {
  return getToken(OptionDeclarationParser::K_DIRECT, 0);
}

tree::TerminalNode* OptionDeclarationParser::Direct_policyContext::LEFT_CURLY_BRACKET() {
  return getToken(OptionDeclarationParser::LEFT_CURLY_BRACKET, 0);
}

OptionDeclarationParser::Stream_namesContext* OptionDeclarationParser::Direct_policyContext::stream_names() {
  return getRuleContext<OptionDeclarationParser::Stream_namesContext>(0);
}

tree::TerminalNode* OptionDeclarationParser::Direct_policyContext::RIGHT_CURLY_BRACKET() {
  return getToken(OptionDeclarationParser::RIGHT_CURLY_BRACKET, 0);
}

OptionDeclarationParser::Direct_policyContext::Direct_policyContext(Quarantine_policyContext *ctx) { copyFrom(ctx); }


std::any OptionDeclarationParser::Direct_policyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<OptionDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitDirect_policy(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Fixed_time_policyContext ------------------------------------------------------------------

tree::TerminalNode* OptionDeclarationParser::Fixed_time_policyContext::K_FIXED_TIME() {
  return getToken(OptionDeclarationParser::K_FIXED_TIME, 0);
}

OptionDeclarationParser::Time_spanContext* OptionDeclarationParser::Fixed_time_policyContext::time_span() {
  return getRuleContext<OptionDeclarationParser::Time_spanContext>(0);
}

tree::TerminalNode* OptionDeclarationParser::Fixed_time_policyContext::LEFT_CURLY_BRACKET() {
  return getToken(OptionDeclarationParser::LEFT_CURLY_BRACKET, 0);
}

OptionDeclarationParser::Stream_namesContext* OptionDeclarationParser::Fixed_time_policyContext::stream_names() {
  return getRuleContext<OptionDeclarationParser::Stream_namesContext>(0);
}

tree::TerminalNode* OptionDeclarationParser::Fixed_time_policyContext::RIGHT_CURLY_BRACKET() {
  return getToken(OptionDeclarationParser::RIGHT_CURLY_BRACKET, 0);
}

OptionDeclarationParser::Fixed_time_policyContext::Fixed_time_policyContext(Quarantine_policyContext *ctx) { copyFrom(ctx); }


std::any OptionDeclarationParser::Fixed_time_policyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<OptionDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitFixed_time_policy(this);
  else
    return visitor->visitChildren(this);
}
OptionDeclarationParser::Quarantine_policyContext* OptionDeclarationParser::quarantine_policy() {
  Quarantine_policyContext *_localctx = _tracker.createInstance<Quarantine_policyContext>(_ctx, getState());
  enterRule(_localctx, 6, OptionDeclarationParser::RuleQuarantine_policy);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(68);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case OptionDeclarationParser::K_FIXED_TIME: {
        _localctx = _tracker.createInstance<OptionDeclarationParser::Fixed_time_policyContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(51);
        match(OptionDeclarationParser::K_FIXED_TIME);
        setState(52);
        time_span();
        setState(53);
        match(OptionDeclarationParser::LEFT_CURLY_BRACKET);
        setState(54);
        stream_names();
        setState(55);
        match(OptionDeclarationParser::RIGHT_CURLY_BRACKET);
        break;
      }

      case OptionDeclarationParser::K_BOUNDED_TIME: {
        _localctx = _tracker.createInstance<OptionDeclarationParser::Bounded_time_policyContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(57);
        match(OptionDeclarationParser::K_BOUNDED_TIME);
        setState(58);
        time_span();
        setState(59);
        match(OptionDeclarationParser::LEFT_CURLY_BRACKET);
        setState(60);
        stream_names();
        setState(61);
        match(OptionDeclarationParser::RIGHT_CURLY_BRACKET);
        break;
      }

      case OptionDeclarationParser::K_DIRECT: {
        _localctx = _tracker.createInstance<OptionDeclarationParser::Direct_policyContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(63);
        match(OptionDeclarationParser::K_DIRECT);
        setState(64);
        match(OptionDeclarationParser::LEFT_CURLY_BRACKET);
        setState(65);
        stream_names();
        setState(66);
        match(OptionDeclarationParser::RIGHT_CURLY_BRACKET);
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

//----------------- Stream_namesContext ------------------------------------------------------------------

OptionDeclarationParser::Stream_namesContext::Stream_namesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<OptionDeclarationParser::Stream_nameContext *> OptionDeclarationParser::Stream_namesContext::stream_name() {
  return getRuleContexts<OptionDeclarationParser::Stream_nameContext>();
}

OptionDeclarationParser::Stream_nameContext* OptionDeclarationParser::Stream_namesContext::stream_name(size_t i) {
  return getRuleContext<OptionDeclarationParser::Stream_nameContext>(i);
}

std::vector<tree::TerminalNode *> OptionDeclarationParser::Stream_namesContext::COMMA() {
  return getTokens(OptionDeclarationParser::COMMA);
}

tree::TerminalNode* OptionDeclarationParser::Stream_namesContext::COMMA(size_t i) {
  return getToken(OptionDeclarationParser::COMMA, i);
}


size_t OptionDeclarationParser::Stream_namesContext::getRuleIndex() const {
  return OptionDeclarationParser::RuleStream_names;
}


std::any OptionDeclarationParser::Stream_namesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<OptionDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitStream_names(this);
  else
    return visitor->visitChildren(this);
}

OptionDeclarationParser::Stream_namesContext* OptionDeclarationParser::stream_names() {
  Stream_namesContext *_localctx = _tracker.createInstance<Stream_namesContext>(_ctx, getState());
  enterRule(_localctx, 8, OptionDeclarationParser::RuleStream_names);
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
    setState(70);
    stream_name();
    setState(75);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == OptionDeclarationParser::COMMA) {
      setState(71);
      match(OptionDeclarationParser::COMMA);
      setState(72);
      stream_name();
      setState(77);
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

//----------------- Time_spanContext ------------------------------------------------------------------

OptionDeclarationParser::Time_spanContext::Time_spanContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

OptionDeclarationParser::Hour_spanContext* OptionDeclarationParser::Time_spanContext::hour_span() {
  return getRuleContext<OptionDeclarationParser::Hour_spanContext>(0);
}

OptionDeclarationParser::Minute_spanContext* OptionDeclarationParser::Time_spanContext::minute_span() {
  return getRuleContext<OptionDeclarationParser::Minute_spanContext>(0);
}

OptionDeclarationParser::Second_spanContext* OptionDeclarationParser::Time_spanContext::second_span() {
  return getRuleContext<OptionDeclarationParser::Second_spanContext>(0);
}


size_t OptionDeclarationParser::Time_spanContext::getRuleIndex() const {
  return OptionDeclarationParser::RuleTime_span;
}


std::any OptionDeclarationParser::Time_spanContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<OptionDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitTime_span(this);
  else
    return visitor->visitChildren(this);
}

OptionDeclarationParser::Time_spanContext* OptionDeclarationParser::time_span() {
  Time_spanContext *_localctx = _tracker.createInstance<Time_spanContext>(_ctx, getState());
  enterRule(_localctx, 10, OptionDeclarationParser::RuleTime_span);
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
    setState(79);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx)) {
    case 1: {
      setState(78);
      hour_span();
      break;
    }

    default:
      break;
    }
    setState(82);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx)) {
    case 1: {
      setState(81);
      minute_span();
      break;
    }

    default:
      break;
    }
    setState(85);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == OptionDeclarationParser::INTEGER_LITERAL) {
      setState(84);
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

OptionDeclarationParser::Hour_spanContext::Hour_spanContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

OptionDeclarationParser::IntegerContext* OptionDeclarationParser::Hour_spanContext::integer() {
  return getRuleContext<OptionDeclarationParser::IntegerContext>(0);
}

tree::TerminalNode* OptionDeclarationParser::Hour_spanContext::K_HOURS() {
  return getToken(OptionDeclarationParser::K_HOURS, 0);
}


size_t OptionDeclarationParser::Hour_spanContext::getRuleIndex() const {
  return OptionDeclarationParser::RuleHour_span;
}


std::any OptionDeclarationParser::Hour_spanContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<OptionDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitHour_span(this);
  else
    return visitor->visitChildren(this);
}

OptionDeclarationParser::Hour_spanContext* OptionDeclarationParser::hour_span() {
  Hour_spanContext *_localctx = _tracker.createInstance<Hour_spanContext>(_ctx, getState());
  enterRule(_localctx, 12, OptionDeclarationParser::RuleHour_span);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(87);
    integer();
    setState(88);
    match(OptionDeclarationParser::K_HOURS);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Minute_spanContext ------------------------------------------------------------------

OptionDeclarationParser::Minute_spanContext::Minute_spanContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

OptionDeclarationParser::IntegerContext* OptionDeclarationParser::Minute_spanContext::integer() {
  return getRuleContext<OptionDeclarationParser::IntegerContext>(0);
}

tree::TerminalNode* OptionDeclarationParser::Minute_spanContext::K_MINUTES() {
  return getToken(OptionDeclarationParser::K_MINUTES, 0);
}


size_t OptionDeclarationParser::Minute_spanContext::getRuleIndex() const {
  return OptionDeclarationParser::RuleMinute_span;
}


std::any OptionDeclarationParser::Minute_spanContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<OptionDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitMinute_span(this);
  else
    return visitor->visitChildren(this);
}

OptionDeclarationParser::Minute_spanContext* OptionDeclarationParser::minute_span() {
  Minute_spanContext *_localctx = _tracker.createInstance<Minute_spanContext>(_ctx, getState());
  enterRule(_localctx, 14, OptionDeclarationParser::RuleMinute_span);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(90);
    integer();
    setState(91);
    match(OptionDeclarationParser::K_MINUTES);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Second_spanContext ------------------------------------------------------------------

OptionDeclarationParser::Second_spanContext::Second_spanContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

OptionDeclarationParser::IntegerContext* OptionDeclarationParser::Second_spanContext::integer() {
  return getRuleContext<OptionDeclarationParser::IntegerContext>(0);
}

tree::TerminalNode* OptionDeclarationParser::Second_spanContext::K_SECONDS() {
  return getToken(OptionDeclarationParser::K_SECONDS, 0);
}


size_t OptionDeclarationParser::Second_spanContext::getRuleIndex() const {
  return OptionDeclarationParser::RuleSecond_span;
}


std::any OptionDeclarationParser::Second_spanContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<OptionDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitSecond_span(this);
  else
    return visitor->visitChildren(this);
}

OptionDeclarationParser::Second_spanContext* OptionDeclarationParser::second_span() {
  Second_spanContext *_localctx = _tracker.createInstance<Second_spanContext>(_ctx, getState());
  enterRule(_localctx, 16, OptionDeclarationParser::RuleSecond_span);

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
    integer();
    setState(94);
    match(OptionDeclarationParser::K_SECONDS);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Stream_nameContext ------------------------------------------------------------------

OptionDeclarationParser::Stream_nameContext::Stream_nameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

OptionDeclarationParser::Any_nameContext* OptionDeclarationParser::Stream_nameContext::any_name() {
  return getRuleContext<OptionDeclarationParser::Any_nameContext>(0);
}


size_t OptionDeclarationParser::Stream_nameContext::getRuleIndex() const {
  return OptionDeclarationParser::RuleStream_name;
}


std::any OptionDeclarationParser::Stream_nameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<OptionDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitStream_name(this);
  else
    return visitor->visitChildren(this);
}

OptionDeclarationParser::Stream_nameContext* OptionDeclarationParser::stream_name() {
  Stream_nameContext *_localctx = _tracker.createInstance<Stream_nameContext>(_ctx, getState());
  enterRule(_localctx, 18, OptionDeclarationParser::RuleStream_name);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(96);
    any_name();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Any_nameContext ------------------------------------------------------------------

OptionDeclarationParser::Any_nameContext::Any_nameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* OptionDeclarationParser::Any_nameContext::IDENTIFIER() {
  return getToken(OptionDeclarationParser::IDENTIFIER, 0);
}


size_t OptionDeclarationParser::Any_nameContext::getRuleIndex() const {
  return OptionDeclarationParser::RuleAny_name;
}


std::any OptionDeclarationParser::Any_nameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<OptionDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitAny_name(this);
  else
    return visitor->visitChildren(this);
}

OptionDeclarationParser::Any_nameContext* OptionDeclarationParser::any_name() {
  Any_nameContext *_localctx = _tracker.createInstance<Any_nameContext>(_ctx, getState());
  enterRule(_localctx, 20, OptionDeclarationParser::RuleAny_name);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(98);
    match(OptionDeclarationParser::IDENTIFIER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NumberContext ------------------------------------------------------------------

OptionDeclarationParser::NumberContext::NumberContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

OptionDeclarationParser::IntegerContext* OptionDeclarationParser::NumberContext::integer() {
  return getRuleContext<OptionDeclarationParser::IntegerContext>(0);
}

OptionDeclarationParser::DoubleContext* OptionDeclarationParser::NumberContext::double_() {
  return getRuleContext<OptionDeclarationParser::DoubleContext>(0);
}


size_t OptionDeclarationParser::NumberContext::getRuleIndex() const {
  return OptionDeclarationParser::RuleNumber;
}


std::any OptionDeclarationParser::NumberContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<OptionDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitNumber(this);
  else
    return visitor->visitChildren(this);
}

OptionDeclarationParser::NumberContext* OptionDeclarationParser::number() {
  NumberContext *_localctx = _tracker.createInstance<NumberContext>(_ctx, getState());
  enterRule(_localctx, 22, OptionDeclarationParser::RuleNumber);

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
    switch (_input->LA(1)) {
      case OptionDeclarationParser::INTEGER_LITERAL: {
        enterOuterAlt(_localctx, 1);
        setState(100);
        integer();
        break;
      }

      case OptionDeclarationParser::DOUBLE_LITERAL: {
        enterOuterAlt(_localctx, 2);
        setState(101);
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

//----------------- IntegerContext ------------------------------------------------------------------

OptionDeclarationParser::IntegerContext::IntegerContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* OptionDeclarationParser::IntegerContext::INTEGER_LITERAL() {
  return getToken(OptionDeclarationParser::INTEGER_LITERAL, 0);
}


size_t OptionDeclarationParser::IntegerContext::getRuleIndex() const {
  return OptionDeclarationParser::RuleInteger;
}


std::any OptionDeclarationParser::IntegerContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<OptionDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitInteger(this);
  else
    return visitor->visitChildren(this);
}

OptionDeclarationParser::IntegerContext* OptionDeclarationParser::integer() {
  IntegerContext *_localctx = _tracker.createInstance<IntegerContext>(_ctx, getState());
  enterRule(_localctx, 24, OptionDeclarationParser::RuleInteger);

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
    match(OptionDeclarationParser::INTEGER_LITERAL);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DoubleContext ------------------------------------------------------------------

OptionDeclarationParser::DoubleContext::DoubleContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* OptionDeclarationParser::DoubleContext::DOUBLE_LITERAL() {
  return getToken(OptionDeclarationParser::DOUBLE_LITERAL, 0);
}


size_t OptionDeclarationParser::DoubleContext::getRuleIndex() const {
  return OptionDeclarationParser::RuleDouble;
}


std::any OptionDeclarationParser::DoubleContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<OptionDeclarationParserVisitor*>(visitor))
    return parserVisitor->visitDouble(this);
  else
    return visitor->visitChildren(this);
}

OptionDeclarationParser::DoubleContext* OptionDeclarationParser::double_() {
  DoubleContext *_localctx = _tracker.createInstance<DoubleContext>(_ctx, getState());
  enterRule(_localctx, 26, OptionDeclarationParser::RuleDouble);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(106);
    match(OptionDeclarationParser::DOUBLE_LITERAL);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

void OptionDeclarationParser::initialize() {
  ::antlr4::internal::call_once(optiondeclarationparserParserOnceFlag, optiondeclarationparserParserInitialize);
}
