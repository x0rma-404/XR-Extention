#pragma once
#include <vector>
#include <memory>
#include <string>
#include <initializer_list>
#include "token.hpp"
#include "ast.hpp"

class Parser {
    std::vector<Token> toks;
    size_t pos = 0;

    const Token& cur()  const;
    const Token& next() const;
    const Token& previous() const;
    bool atEnd() const;
    Token advance();
    bool check(TT t) const;
    bool match(TT t);
    bool match(std::initializer_list<TT> ts);
    Token consume(TT t, const std::string& msg);

    // Expressions
    EP primary();
    EP dotOperand();
    EP bound();
    EP postfix();
    EP pw();
    EP term();
    EP arith();
    EP rng();
    EP cat();
    EP cmp();
    EP expr();

    // Statements
    SP block();
    SP stmt();

public:
    explicit Parser(std::vector<Token> t);
    SP parse();
};

