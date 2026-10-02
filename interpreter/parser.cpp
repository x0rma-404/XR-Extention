#include "parser.hpp"
#include "error.hpp"

Parser::Parser(std::vector<Token> t) : toks(std::move(t)) {}

const Token& Parser::cur() const { return toks[pos]; }
const Token& Parser::next() const { return pos + 1 < toks.size() ? toks[pos + 1] : toks.back(); }
const Token& Parser::previous() const { return toks[pos > 0 ? pos - 1 : 0]; }
bool Parser::atEnd() const { return cur().type == TT::END; }

Token Parser::advance() {
    Token t = toks[pos];
    if (!atEnd()) pos++;
    return t;
}

bool Parser::check(TT t) const { return !atEnd() && cur().type == t; }

bool Parser::match(TT t) {
    if (check(t)) { advance(); return true; }
    return false;
}

bool Parser::match(std::initializer_list<TT> ts) {
    for (TT t : ts) if (check(t)) { advance(); return true; }
    return false;
}

Token Parser::consume(TT t, const std::string& msg) {
    if (check(t)) return advance();
    throw ParseError(msg + ", tapildi: '" + cur().lexeme + "'", cur().loc);
}

EP Parser::primary() {
    Token k = cur();

    if (match(TT::NUMBER)) return std::make_shared<LitExpr>(previous().literal, previous().loc);
    if (match(TT::STRING)) return std::make_shared<LitExpr>(previous().literal, previous().loc);

    if (match(TT::IDENT)) {
        Token id = previous();
        // function call: id[...] or id(...)
        if (match(TT::LBRACKET) || match(TT::LPAREN)) {
            TT close = (previous().type == TT::LBRACKET) ? TT::RBRACKET : TT::RPAREN;
            std::vector<EP> args;
            if (!check(close)) {
                do { args.push_back(expr()); } while (match(TT::COMMA));
            }
            consume(close, (close == TT::RBRACKET) ? "']' gozlenilirdi" : "')' gozlenilirdi");
            return std::make_shared<CallExpr>(id.lexeme, args, id.loc);
        }
        return std::make_shared<VarExpr>(id.lexeme, id.loc);
    }

    if (match(TT::LPAREN)) {
        EP e = expr();
        consume(TT::RPAREN, "')' gozlenilirdi");
        return e;
    }

    if (match(TT::HASH)) {
        SourceLocation l = previous().loc;
        consume(TT::LBRACKET, "'[' gozlenilirdi");
        std::vector<EP> items;
        if (!check(TT::RBRACKET)) {
            do { items.push_back(expr()); } while (match(TT::COMMA));
        }
        consume(TT::RBRACKET, "']' gozlenilirdi");
        return std::make_shared<ListLitExpr>(items, l);
    }

    if (match(TT::DOLLAR)) return std::make_shared<LitExpr>(Value(-1.0), previous().loc);
    if (match(TT::MINUS))  return std::make_shared<UnaryExpr>(TT::MINUS, postfix(), previous().loc);
    if (match(TT::NOT))    return std::make_shared<UnaryExpr>(TT::NOT,   postfix(), previous().loc);

    throw ParseError("gozlenilmeyen token: '" + cur().lexeme + "'", cur().loc);
}

EP Parser::dotOperand() {
    if (match(TT::DOLLAR)) return std::make_shared<LitExpr>(Value(-1.0), previous().loc);
    if (match(TT::NUMBER)) return std::make_shared<LitExpr>(previous().literal, previous().loc);
    if (match(TT::IDENT))  return std::make_shared<VarExpr>(previous().lexeme, previous().loc);
    throw ParseError("'.' sonrasi indeks gozlenilirdi, tapildi: '" + cur().lexeme + "'", cur().loc);
}

EP Parser::bound() {
    if (match(TT::DOLLAR)) return std::make_shared<LitExpr>(Value(-1.0), previous().loc);
    return arith();
}

EP Parser::postfix() {
    EP e = primary();
    while (match(TT::DOT)) {
        SourceLocation dl = previous().loc;
        if (match(TT::LPAREN)) {
            EP lo = check(TT::RANGE) ? nullptr : bound();
            if (match(TT::RANGE)) {
                EP hi = check(TT::RPAREN) ? nullptr : bound();
                consume(TT::RPAREN, "')' gozlenilirdi");
                e = std::make_shared<SliceExpr>(e, lo, hi, dl);
            } else {
                consume(TT::RPAREN, "')' gozlenilirdi");
                if (!lo) throw ParseError("bos indeks", dl);
                e = std::make_shared<IndexExpr>(e, lo, dl);
            }
        } else {
            e = std::make_shared<IndexExpr>(e, dotOperand(), dl);
        }
    }
    return e;
}

EP Parser::pw() {
    EP l = postfix();
    if (match(TT::CARET)) {
        Token op = previous();
        return std::make_shared<BinExpr>(op.type, op.lexeme, l, pw(), op.loc);
    }
    return l;
}

EP Parser::term() {
    EP l = pw();
    while (match({TT::STAR, TT::SLASH, TT::PERCENT})) {
        Token op = previous();
        l = std::make_shared<BinExpr>(op.type, op.lexeme, l, pw(), op.loc);
    }
    return l;
}

EP Parser::arith() {
    EP l = term();
    while (match({TT::PLUS, TT::MINUS})) {
        Token op = previous();
        l = std::make_shared<BinExpr>(op.type, op.lexeme, l, term(), op.loc);
    }
    return l;
}

EP Parser::rng() {
    EP l = arith();
    if (match(TT::RANGE)) {
        Token op = previous();
        return std::make_shared<RangeExpr>(l, arith(), op.loc);
    }
    return l;
}

EP Parser::cat() {
    EP l = rng();
    while (match(TT::TILDE)) {
        Token op = previous();
        l = std::make_shared<BinExpr>(op.type, op.lexeme, l, rng(), op.loc);
    }
    return l;
}

EP Parser::cmp() {
    EP l = cat();
    while (match({TT::LT, TT::GT, TT::LTE, TT::GTE, TT::EQ, TT::NEQ})) {
        Token op = previous();
        l = std::make_shared<BinExpr>(op.type, op.lexeme, l, cat(), op.loc);
    }
    return l;
}

EP Parser::expr() {
    EP l = cmp();
    while (match({TT::AND, TT::OR})) {
        Token op = previous();
        l = std::make_shared<BinExpr>(op.type, op.lexeme, l, cmp(), op.loc);
    }
    return l;
}

SP Parser::block() {
    consume(TT::LBRACE, "'{' gozlenilirdi");
    auto b = std::make_shared<BlockStmt>(previous().loc);
    while (!check(TT::RBRACE)) {
        if (atEnd()) throw ParseError("'}' gozlenilirdi, fayl bitdi", cur().loc);
        b->stmts.push_back(stmt());
    }
    consume(TT::RBRACE, "'}' gozlenilirdi");
    return b;
}

SP Parser::stmt() {
    // print
    if (match(TT::GT)) {
        SourceLocation l = previous().loc;
        EP e = expr();
        consume(TT::SEMI, "';' gozlenilirdi");
        return std::make_shared<PrintStmt>(e, l);
    }

    // if
    if (match(TT::QUESTION)) {
        SourceLocation l = previous().loc;
        EP cond = expr();
        SP yes  = block();
        SP no   = nullptr;
        if (match(TT::COLON))
            no = check(TT::QUESTION) ? stmt() : block();
        return std::make_shared<IfStmt>(cond, yes, no, l);
    }

    // loop
    if (match(TT::AT)) {
        SourceLocation l = previous().loc;
        if (check(TT::IDENT) && next().type == TT::COLON) {
            Token var = advance(); advance(); // skip ':'
            EP iter = expr();
            SP body = block();
            return std::make_shared<ForEachStmt>(var.lexeme, iter, body, l);
        }
        EP cond = expr();
        SP body = block();
        return std::make_shared<WhileStmt>(cond, body, l);
    }

    // deyer -> ad;
    if (match(TT::DEYER)) {
        SourceLocation l = previous().loc;
        consume(TT::ARROW, "'->' gozlenilirdi");
        if (!check(TT::IDENT))
            throw ParseError("'deyer ->' sonrasi deyisen adi gozlenilirdi", cur().loc);
        Token name = advance();
        consume(TT::SEMI, "';' gozlenilirdi");
        return std::make_shared<VarDeclStmt>(name.lexeme, l);
    }

    // expr -> target;  or  expr;
    EP e = expr();
    if (match(TT::ARROW)) {
        SourceLocation l = previous().loc;
        EP target = postfix();
        consume(TT::SEMI, "';' gozlenilirdi");
        if (auto v = std::dynamic_pointer_cast<VarExpr>(target))
            return std::make_shared<AssignStmt>(v->name, e, l);
        if (auto ix = std::dynamic_pointer_cast<IndexExpr>(target))
            return std::make_shared<IndexAssignStmt>(ix->base, ix->idx, e, l);
        throw ParseError("-> sonrasi deyisen ve ya a.i gozlenilirdi", l);
    }

    // guard: warn if user writes '=' trying to assign
    if (auto b = std::dynamic_pointer_cast<BinExpr>(e))
        if (b->op == TT::EQ)
            throw ParseError("'=' beraberlik yoxlayir. Menimsetme ucun: deyer -> ad; ve ya 5 -> ad;", b->loc);

    consume(TT::SEMI, "';' gozlenilirdi");
    return std::make_shared<ExprStmt>(e, e->loc);
}

SP Parser::parse() {
    auto program = std::make_shared<BlockStmt>();
    while (!atEnd()) program->stmts.push_back(stmt());
    return program;
}

