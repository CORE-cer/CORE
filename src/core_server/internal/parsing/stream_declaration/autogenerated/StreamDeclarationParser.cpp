
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
      "parse", "error", "core_stmt", "core_declaration", "stream_declaration", 
      "event_list", "event_declaration", "attribute_dec_list", "attribute_declaration", 
      "datatype", "event_name", "stream_name", "attribute_name", "any_name", 
      "keyword"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "'%'", "'+'", 
      "'-'", "'*'", "'/'", "'<'", "'<='", "'>'", "'>='", "", "", "'int'", 
      "'string'", "'double'", "'long'", "'boolean'", "'primary_time'", "';'", 
      "','", "'..'", "'('", "')'", "'['", "']'", "'{'", "'}'", "':'"
    },
    std::vector<std::string>{
      "", "K_ALL", "K_AND", "K_ANY", "K_AS", "K_BY", "K_CONSUME", "K_CREATE", 
      "K_DISTINCT", "K_EVENT", "K_EVENTS", "K_FILTER", "K_FROM", "K_HOURS", 
      "K_IN", "K_LAST", "K_LIKE", "K_MAX", "K_MINUTES", "K_NEXT", "K_NONE", 
      "K_NOT", "K_OR", "K_PARTITION", "K_SECONDS", "K_SELECT", "K_STREAM", 
      "K_STRICT", "K_UNLESS", "K_WHERE", "K_WITHIN", "PERCENT", "PLUS", 
      "MINUS", "STAR", "SLASH", "LE", "LEQ", "GE", "GEQ", "EQ", "NEQ", "INT", 
      "STRING", "DOUBLE", "LONG", "BOOLEAN", "PRIMARY_TIME", "SEMICOLON", 
      "COMMA", "DOUBLE_DOT", "LEFT_PARENTHESIS", "RIGHT_PARENTHESIS", "LEFT_SQUARE_BRACKET", 
      "RIGHT_SQUARE_BRACKET", "LEFT_CURLY_BRACKET", "RIGHT_CURLY_BRACKET", 
      "COLON", "IDENTIFIER", "FLOAT_LITERAL", "INTEGER_LITERAL", "NUMERICAL_EXPONENT", 
      "STRING_LITERAL", "REGEXP", "SINGLE_LINE_COMMENT", "MULTILINE_COMMENT", 
      "SPACES", "UNEXPECTED_CHAR"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,67,95,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,7,
  	7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,14,
  	1,0,1,0,5,0,33,8,0,10,0,12,0,36,9,0,1,0,1,0,1,1,1,1,1,1,1,2,1,2,1,3,1,
  	3,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,5,1,5,1,5,5,5,57,8,5,10,5,12,5,60,9,5,
  	1,6,1,6,1,6,1,6,1,6,1,6,1,7,1,7,1,7,1,7,5,7,72,8,7,10,7,12,7,75,9,7,3,
  	7,77,8,7,1,8,1,8,1,8,1,8,1,9,1,9,1,10,1,10,1,11,1,11,1,12,1,12,1,13,1,
  	13,1,14,1,14,1,14,0,0,15,0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,0,2,
  	3,0,42,44,46,47,58,58,1,0,1,30,84,0,34,1,0,0,0,2,39,1,0,0,0,4,42,1,0,
  	0,0,6,44,1,0,0,0,8,46,1,0,0,0,10,53,1,0,0,0,12,61,1,0,0,0,14,76,1,0,0,
  	0,16,78,1,0,0,0,18,82,1,0,0,0,20,84,1,0,0,0,22,86,1,0,0,0,24,88,1,0,0,
  	0,26,90,1,0,0,0,28,92,1,0,0,0,30,33,3,4,2,0,31,33,3,2,1,0,32,30,1,0,0,
  	0,32,31,1,0,0,0,33,36,1,0,0,0,34,32,1,0,0,0,34,35,1,0,0,0,35,37,1,0,0,
  	0,36,34,1,0,0,0,37,38,5,0,0,1,38,1,1,0,0,0,39,40,5,67,0,0,40,41,6,1,-1,
  	0,41,3,1,0,0,0,42,43,3,6,3,0,43,5,1,0,0,0,44,45,3,8,4,0,45,7,1,0,0,0,
  	46,47,5,7,0,0,47,48,5,26,0,0,48,49,3,22,11,0,49,50,5,55,0,0,50,51,3,10,
  	5,0,51,52,5,56,0,0,52,9,1,0,0,0,53,58,3,12,6,0,54,55,5,49,0,0,55,57,3,
  	12,6,0,56,54,1,0,0,0,57,60,1,0,0,0,58,56,1,0,0,0,58,59,1,0,0,0,59,11,
  	1,0,0,0,60,58,1,0,0,0,61,62,5,9,0,0,62,63,3,20,10,0,63,64,5,55,0,0,64,
  	65,3,14,7,0,65,66,5,56,0,0,66,13,1,0,0,0,67,77,1,0,0,0,68,73,3,16,8,0,
  	69,70,5,49,0,0,70,72,3,16,8,0,71,69,1,0,0,0,72,75,1,0,0,0,73,71,1,0,0,
  	0,73,74,1,0,0,0,74,77,1,0,0,0,75,73,1,0,0,0,76,67,1,0,0,0,76,68,1,0,0,
  	0,77,15,1,0,0,0,78,79,3,24,12,0,79,80,5,57,0,0,80,81,3,18,9,0,81,17,1,
  	0,0,0,82,83,7,0,0,0,83,19,1,0,0,0,84,85,3,26,13,0,85,21,1,0,0,0,86,87,
  	3,26,13,0,87,23,1,0,0,0,88,89,3,26,13,0,89,25,1,0,0,0,90,91,5,58,0,0,
  	91,27,1,0,0,0,92,93,7,1,0,0,93,29,1,0,0,0,5,32,34,58,73,76
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
    setState(34);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == StreamDeclarationParser::K_CREATE

    || _la == StreamDeclarationParser::UNEXPECTED_CHAR) {
      setState(32);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case StreamDeclarationParser::K_CREATE: {
          setState(30);
          core_stmt();
          break;
        }

        case StreamDeclarationParser::UNEXPECTED_CHAR: {
          setState(31);
          error();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(36);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(37);
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
    setState(39);
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
    enterOuterAlt(_localctx, 1);
    setState(42);
    core_declaration();
   
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
    enterOuterAlt(_localctx, 1);
    setState(44);
    stream_declaration();
   
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

tree::TerminalNode* StreamDeclarationParser::Stream_declarationContext::K_CREATE() {
  return getToken(StreamDeclarationParser::K_CREATE, 0);
}

tree::TerminalNode* StreamDeclarationParser::Stream_declarationContext::K_STREAM() {
  return getToken(StreamDeclarationParser::K_STREAM, 0);
}

StreamDeclarationParser::Stream_nameContext* StreamDeclarationParser::Stream_declarationContext::stream_name() {
  return getRuleContext<StreamDeclarationParser::Stream_nameContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::Stream_declarationContext::LEFT_CURLY_BRACKET() {
  return getToken(StreamDeclarationParser::LEFT_CURLY_BRACKET, 0);
}

StreamDeclarationParser::Event_listContext* StreamDeclarationParser::Stream_declarationContext::event_list() {
  return getRuleContext<StreamDeclarationParser::Event_listContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::Stream_declarationContext::RIGHT_CURLY_BRACKET() {
  return getToken(StreamDeclarationParser::RIGHT_CURLY_BRACKET, 0);
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
  enterRule(_localctx, 8, StreamDeclarationParser::RuleStream_declaration);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(46);
    match(StreamDeclarationParser::K_CREATE);
    setState(47);
    match(StreamDeclarationParser::K_STREAM);
    setState(48);
    stream_name();
    setState(49);
    match(StreamDeclarationParser::LEFT_CURLY_BRACKET);
    setState(50);
    event_list();
    setState(51);
    match(StreamDeclarationParser::RIGHT_CURLY_BRACKET);
   
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

std::vector<StreamDeclarationParser::Event_declarationContext *> StreamDeclarationParser::Event_listContext::event_declaration() {
  return getRuleContexts<StreamDeclarationParser::Event_declarationContext>();
}

StreamDeclarationParser::Event_declarationContext* StreamDeclarationParser::Event_listContext::event_declaration(size_t i) {
  return getRuleContext<StreamDeclarationParser::Event_declarationContext>(i);
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
  enterRule(_localctx, 10, StreamDeclarationParser::RuleEvent_list);
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
    setState(53);
    event_declaration();
    setState(58);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == StreamDeclarationParser::COMMA) {
      setState(54);
      match(StreamDeclarationParser::COMMA);
      setState(55);
      event_declaration();
      setState(60);
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

//----------------- Event_declarationContext ------------------------------------------------------------------

StreamDeclarationParser::Event_declarationContext::Event_declarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* StreamDeclarationParser::Event_declarationContext::K_EVENT() {
  return getToken(StreamDeclarationParser::K_EVENT, 0);
}

StreamDeclarationParser::Event_nameContext* StreamDeclarationParser::Event_declarationContext::event_name() {
  return getRuleContext<StreamDeclarationParser::Event_nameContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::Event_declarationContext::LEFT_CURLY_BRACKET() {
  return getToken(StreamDeclarationParser::LEFT_CURLY_BRACKET, 0);
}

StreamDeclarationParser::Attribute_dec_listContext* StreamDeclarationParser::Event_declarationContext::attribute_dec_list() {
  return getRuleContext<StreamDeclarationParser::Attribute_dec_listContext>(0);
}

tree::TerminalNode* StreamDeclarationParser::Event_declarationContext::RIGHT_CURLY_BRACKET() {
  return getToken(StreamDeclarationParser::RIGHT_CURLY_BRACKET, 0);
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
  enterRule(_localctx, 12, StreamDeclarationParser::RuleEvent_declaration);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(61);
    match(StreamDeclarationParser::K_EVENT);
    setState(62);
    event_name();
    setState(63);
    match(StreamDeclarationParser::LEFT_CURLY_BRACKET);
    setState(64);
    attribute_dec_list();
    setState(65);
    match(StreamDeclarationParser::RIGHT_CURLY_BRACKET);
   
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
  enterRule(_localctx, 14, StreamDeclarationParser::RuleAttribute_dec_list);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(76);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case StreamDeclarationParser::RIGHT_CURLY_BRACKET: {
        enterOuterAlt(_localctx, 1);

        break;
      }

      case StreamDeclarationParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 2);
        setState(68);
        attribute_declaration();
        setState(73);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == StreamDeclarationParser::COMMA) {
          setState(69);
          match(StreamDeclarationParser::COMMA);
          setState(70);
          attribute_declaration();
          setState(75);
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

tree::TerminalNode* StreamDeclarationParser::Attribute_declarationContext::COLON() {
  return getToken(StreamDeclarationParser::COLON, 0);
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
  enterRule(_localctx, 16, StreamDeclarationParser::RuleAttribute_declaration);

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
    attribute_name();
    setState(79);
    match(StreamDeclarationParser::COLON);
    setState(80);
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

tree::TerminalNode* StreamDeclarationParser::DatatypeContext::BOOLEAN() {
  return getToken(StreamDeclarationParser::BOOLEAN, 0);
}

tree::TerminalNode* StreamDeclarationParser::DatatypeContext::PRIMARY_TIME() {
  return getToken(StreamDeclarationParser::PRIMARY_TIME, 0);
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
  enterRule(_localctx, 18, StreamDeclarationParser::RuleDatatype);
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
    setState(82);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 288472268709822464) != 0))) {
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
  enterRule(_localctx, 20, StreamDeclarationParser::RuleEvent_name);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(84);
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
  enterRule(_localctx, 22, StreamDeclarationParser::RuleStream_name);

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
  enterRule(_localctx, 24, StreamDeclarationParser::RuleAttribute_name);

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
  enterRule(_localctx, 26, StreamDeclarationParser::RuleAny_name);

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

tree::TerminalNode* StreamDeclarationParser::KeywordContext::K_CREATE() {
  return getToken(StreamDeclarationParser::K_CREATE, 0);
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
  enterRule(_localctx, 28, StreamDeclarationParser::RuleKeyword);
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
    setState(92);
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

void StreamDeclarationParser::initialize() {
  ::antlr4::internal::call_once(streamdeclarationparserParserOnceFlag, streamdeclarationparserParserInitialize);
}
