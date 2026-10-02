#pragma once
#include <string>
#include <vector>
#include "token.hpp"

class Lexer {
    std::string src;
    size_t pos  = 0;
    size_t line = 1;
    size_t col  = 1;
    size_t startCol = 1;
    std::vector<Token> tokens;

    bool atEnd() const;
    char adv();
    char peek(int off = 0) const;
    bool match(char expected);
    void add(TT t, std::string lex, Value v = Value());
    static bool isIdChar(char c);
    void scanNumber();
    void scanString(char q);
    void scanIdent(char first);
    void blockComment();

public:
    explicit Lexer(std::string source);
    std::vector<Token> tokenize();
};

