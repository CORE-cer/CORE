#pragma once

#include <ANTLRInputStream.h>
#include <CommonTokenStream.h>
#include <tree/ParseTree.h>

#include <set>
#include <string>

#include "autogenerated/StreamDeclarationLexer.h"
#include "autogenerated/StreamDeclarationParser.h"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/parsing/error_listener.hpp"
#include "shared/datatypes/parsing/stream_info_parsed.hpp"
#include "visitors/stream_visitor.hpp"

namespace CORE::Internal::Parsing {

class StreamParser {
 public:
  static Types::StreamInfoParsed parse_stream(std::string stream, Catalog& catalog) {
    // Convert the input string to a stream
    antlr4::ANTLRInputStream input(stream);

    // Use the ANTLR input stream to create a lexer
    StreamDeclarationLexer lexer(&input);

    // Use the lexer to create a token stream
    antlr4::CommonTokenStream tokens(&lexer);

    // Use the token stream to create a parser
    StreamDeclarationParser parser(&tokens);

    // Add error listener for parsing errors
    parser.removeErrorListeners();
    antlr4::ParseErrorListener error_listener;
    parser.addErrorListener(&error_listener);

    // Parse the input
    antlr4::tree::ParseTree* tree = parser.parse();

    const std::set<std::string>& stream_names = catalog.get_stream_names();
    Declaration::StreamVisitorCatalog stream_visitor(stream_names);
    stream_visitor.visit(tree);
    Types::StreamInfoParsed stream_created = stream_visitor.get_parsed_stream();

    return stream_created;
  }
};
}  // namespace CORE::Internal::Parsing
