
// Generated from StreamDeclarationParser.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"
#include "StreamDeclarationParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by StreamDeclarationParser.
 */
class  StreamDeclarationParserVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by StreamDeclarationParser.
   */
    virtual std::any visitParse(StreamDeclarationParser::ParseContext *context) = 0;

    virtual std::any visitError(StreamDeclarationParser::ErrorContext *context) = 0;

    virtual std::any visitCore_stmt(StreamDeclarationParser::Core_stmtContext *context) = 0;

    virtual std::any visitCore_declaration(StreamDeclarationParser::Core_declarationContext *context) = 0;

    virtual std::any visitStream_declaration(StreamDeclarationParser::Stream_declarationContext *context) = 0;

    virtual std::any visitEvent_list(StreamDeclarationParser::Event_listContext *context) = 0;

    virtual std::any visitEvent_declaration(StreamDeclarationParser::Event_declarationContext *context) = 0;

    virtual std::any visitAttribute_dec_list(StreamDeclarationParser::Attribute_dec_listContext *context) = 0;

    virtual std::any visitAttribute_declaration(StreamDeclarationParser::Attribute_declarationContext *context) = 0;

    virtual std::any visitDatatype(StreamDeclarationParser::DatatypeContext *context) = 0;

    virtual std::any visitEvent_name(StreamDeclarationParser::Event_nameContext *context) = 0;

    virtual std::any visitStream_name(StreamDeclarationParser::Stream_nameContext *context) = 0;

    virtual std::any visitAttribute_name(StreamDeclarationParser::Attribute_nameContext *context) = 0;

    virtual std::any visitAny_name(StreamDeclarationParser::Any_nameContext *context) = 0;

    virtual std::any visitKeyword(StreamDeclarationParser::KeywordContext *context) = 0;


};

