#pragma once
#include <string>
#include <utility>
#include "value.hpp"
#include "location.hpp"

enum class TT {
    // keywords
    DEYER,
    // operators
    ARROW, RANGE, DOT, EQ, NEQ, LT, GT, LTE, GTE,
    AND, OR, NOT, PLUS, MINUS, STAR, SLASH, PERCENT, CARET, TILDE,
    // control
    QUESTION, COLON, AT, DOLLAR, HASH,
    // delimiters
    SEMI, COMMA, LPAREN, RPAREN, LBRACKET, RBRACKET, LBRACE, RBRACE,
    // values
    NUMBER, STRING, IDENT,
    // end
    END
};

struct Token {
    TT          type = TT::END;
    std::string lexeme;
    Value       literal;
    SourceLocation loc;

    Token() = default;
    Token(TT t, std::string lex, Value lit, SourceLocation l)
        : type(t), lexeme(std::move(lex)), literal(std::move(lit)), loc(l) {}
};

