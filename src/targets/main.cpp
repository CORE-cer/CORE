#include <iostream>
#include "antlr4-runtime/antlr4-runtime.h"

//#include "core_server/GramLexer.h"
//#include "Parser/GramParser.h"

// Based on: https://www.codeproject.com/Articles/5308882/ANTLR-Parsing-and-Cplusplus-Part-1-Introduction

int main(int argc, const char *args[])
{
    // Provide the input text in a stream
    //antlr4::ANTLRInputStream input("6*(2+3)");

    // Create a lexer from the input
    //GramLexer lexer(&input);

    // Create a token stream from the lexer
    //antlr4::CommonTokenStream tokens(&lexer);

    // Create a parser from the token stream
    //GramParser parser(&tokens);

    // Display the parse tree
    std::cout << "Hello World!" << std::endl;

    return 0;
}
