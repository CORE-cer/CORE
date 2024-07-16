
// Generated from OptionDeclarationParser.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"
#include "OptionDeclarationParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by OptionDeclarationParser.
 */
class  OptionDeclarationParserVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by OptionDeclarationParser.
   */
    virtual std::any visitParse(OptionDeclarationParser::ParseContext *context) = 0;

    virtual std::any visitError(OptionDeclarationParser::ErrorContext *context) = 0;

    virtual std::any visitOption_declaration(OptionDeclarationParser::Option_declarationContext *context) = 0;

    virtual std::any visitFixed_time_policy(OptionDeclarationParser::Fixed_time_policyContext *context) = 0;

    virtual std::any visitDirect_policy(OptionDeclarationParser::Direct_policyContext *context) = 0;

    virtual std::any visitStream_names(OptionDeclarationParser::Stream_namesContext *context) = 0;

    virtual std::any visitTime_span(OptionDeclarationParser::Time_spanContext *context) = 0;

    virtual std::any visitHour_span(OptionDeclarationParser::Hour_spanContext *context) = 0;

    virtual std::any visitMinute_span(OptionDeclarationParser::Minute_spanContext *context) = 0;

    virtual std::any visitSecond_span(OptionDeclarationParser::Second_spanContext *context) = 0;

    virtual std::any visitStream_name(OptionDeclarationParser::Stream_nameContext *context) = 0;

    virtual std::any visitAny_name(OptionDeclarationParser::Any_nameContext *context) = 0;

    virtual std::any visitNumber(OptionDeclarationParser::NumberContext *context) = 0;

    virtual std::any visitInteger(OptionDeclarationParser::IntegerContext *context) = 0;

    virtual std::any visitDouble(OptionDeclarationParser::DoubleContext *context) = 0;


};

