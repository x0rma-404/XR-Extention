#pragma once
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include "value.hpp"
#include "location.hpp"
#include "token.hpp"

class Interpreter;

class Expr {
public:
    SourceLocation loc;
    Expr(SourceLocation l = {}) : loc(l) {}
    virtual ~Expr() = default;
    virtual Value eval(Interpreter& vm) = 0;
};

class Stmt {
public:
    SourceLocation loc;
    Stmt(SourceLocation l = {}) : loc(l) {}
    virtual ~Stmt() = default;
    virtual void exec(Interpreter& vm) = 0;
};

using EP = std::shared_ptr<Expr>;
using SP = std::shared_ptr<Stmt>;

// Expressions

class LitExpr : public Expr {
public:
    Value val;
    LitExpr(Value v, SourceLocation l) : Expr(l), val(std::move(v)) {}
    Value eval(Interpreter&) override { return val; }
};

class VarExpr : public Expr {
public:
    std::string name;
    VarExpr(std::string n, SourceLocation l) : Expr(l), name(std::move(n)) {}
    Value eval(Interpreter& vm) override;
};

class ListLitExpr : public Expr {
public:
    std::vector<EP> items;
    ListLitExpr(std::vector<EP> i, SourceLocation l) : Expr(l), items(std::move(i)) {}
    Value eval(Interpreter& vm) override;
};

class RangeExpr : public Expr {
public:
    EP lo, hi;
    RangeExpr(EP a, EP b, SourceLocation l) : Expr(l), lo(std::move(a)), hi(std::move(b)) {}
    Value eval(Interpreter& vm) override;
};

class UnaryExpr : public Expr {
public:
    TT op;
    EP operand;
    UnaryExpr(TT o, EP e, SourceLocation l) : Expr(l), op(o), operand(std::move(e)) {}
    Value eval(Interpreter& vm) override;
};

class BinExpr : public Expr {
public:
    TT op;
    std::string opStr;
    EP left, right;
    BinExpr(TT o, std::string s, EP l, EP r, SourceLocation loc)
        : Expr(loc), op(o), opStr(std::move(s)), left(std::move(l)), right(std::move(r)) {}
    Value eval(Interpreter& vm) override;
};

class IndexExpr : public Expr {
public:
    EP base, idx;
    IndexExpr(EP b, EP i, SourceLocation l) : Expr(l), base(std::move(b)), idx(std::move(i)) {}
    Value eval(Interpreter& vm) override;
};

class SliceExpr : public Expr {
public:
    EP base, lo, hi;
    SliceExpr(EP b, EP l, EP h, SourceLocation loc)
        : Expr(loc), base(std::move(b)), lo(std::move(l)), hi(std::move(h)) {}
    Value eval(Interpreter& vm) override;
};

class CallExpr : public Expr {
public:
    std::string name;
    std::vector<EP> args;
    CallExpr(std::string n, std::vector<EP> a, SourceLocation l)
        : Expr(l), name(std::move(n)), args(std::move(a)) {}
    Value eval(Interpreter& vm) override;
};

// Statements

class PrintStmt : public Stmt {
public:
    EP expr;
    PrintStmt(EP e, SourceLocation l) : Stmt(l), expr(std::move(e)) {}
    void exec(Interpreter& vm) override;
};

class ExprStmt : public Stmt {
public:
    EP expr;
    ExprStmt(EP e, SourceLocation l) : Stmt(l), expr(std::move(e)) {}
    void exec(Interpreter& vm) override;
};

class VarDeclStmt : public Stmt {
public:
    std::string name;
    VarDeclStmt(std::string n, SourceLocation l) : Stmt(l), name(std::move(n)) {}
    void exec(Interpreter& vm) override;
};

class AssignStmt : public Stmt {
public:
    std::string name;
    EP value;
    AssignStmt(std::string n, EP v, SourceLocation l) : Stmt(l), name(std::move(n)), value(std::move(v)) {}
    void exec(Interpreter& vm) override;
};

class IndexAssignStmt : public Stmt {
public:
    EP base, idx, value;
    IndexAssignStmt(EP b, EP i, EP v, SourceLocation l)
        : Stmt(l), base(std::move(b)), idx(std::move(i)), value(std::move(v)) {}
    void exec(Interpreter& vm) override;
};

class BlockStmt : public Stmt {
public:
    std::vector<SP> stmts;
    BlockStmt(SourceLocation l = {}) : Stmt(l) {}
    void exec(Interpreter& vm) override;
};

class IfStmt : public Stmt {
public:
    EP cond;
    SP yes, no;
    IfStmt(EP c, SP y, SP n, SourceLocation l)
        : Stmt(l), cond(std::move(c)), yes(std::move(y)), no(std::move(n)) {}
    void exec(Interpreter& vm) override;
};

class WhileStmt : public Stmt {
public:
    EP cond;
    SP body;
    WhileStmt(EP c, SP b, SourceLocation l) : Stmt(l), cond(std::move(c)), body(std::move(b)) {}
    void exec(Interpreter& vm) override;
};

class ForEachStmt : public Stmt {
public:
    std::string var;
    EP iter;
    SP body;
    ForEachStmt(std::string v, EP i, SP b, SourceLocation l)
        : Stmt(l), var(std::move(v)), iter(std::move(i)), body(std::move(b)) {}
    void exec(Interpreter& vm) override;
};

