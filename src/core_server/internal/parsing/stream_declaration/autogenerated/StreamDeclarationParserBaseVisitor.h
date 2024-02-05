
// Generated from StreamDeclarationParser.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"
#include "StreamDeclarationParserVisitor.h"


/**
 * This class provides an empty implementation of StreamDeclarationParserVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  StreamDeclarationParserBaseVisitor : public StreamDeclarationParserVisitor {
public:

  virtual std::any visitParse(StreamDeclarationParser::ParseContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitError(StreamDeclarationParser::ErrorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCore_stmt(StreamDeclarationParser::Core_stmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCore_declaration(StreamDeclarationParser::Core_declarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStream_declaration(StreamDeclarationParser::Stream_declarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEvent_list(StreamDeclarationParser::Event_listContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEvent_declaration(StreamDeclarationParser::Event_declarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAttribute_dec_list(StreamDeclarationParser::Attribute_dec_listContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAttribute_declaration(StreamDeclarationParser::Attribute_declarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDatatype(StreamDeclarationParser::DatatypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEvent_name(StreamDeclarationParser::Event_nameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStream_name(StreamDeclarationParser::Stream_nameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAttribute_name(StreamDeclarationParser::Attribute_nameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAny_name(StreamDeclarationParser::Any_nameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitKeyword(StreamDeclarationParser::KeywordContext *ctx) override {
    return visitChildren(ctx);
  }


};

