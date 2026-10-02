#include "ast.hpp"
#include "interpreter.hpp"
#include "utf8.hpp"
#include <algorithm>
#include <cmath>

Value VarExpr::eval(Interpreter& vm) {
    return vm.getEnv()->get(name, loc);
}

Value ListLitExpr::eval(Interpreter& vm) {
    auto r = std::make_shared<std::vector<Value>>();
    for (auto& item : items) r->push_back(item->eval(vm));
    return Value(r);
}

Value RangeExpr::eval(Interpreter& vm) {
    long long a = lo->eval(vm).asInt("..");
    long long b = hi->eval(vm).asInt("..");
    long long step = (a <= b) ? 1 : -1;
    long long diff = a <= b ? (b - a) : (a - b);
    if (diff > 10000000) throw RuntimeError("..: araliq cox boyukdur", loc);
    auto r = std::make_shared<std::vector<Value>>();
    for (long long k = a;; k += step) {
        r->push_back(Value((double)k));
        if (k == b) break;
    }
    return Value(r);
}

Value UnaryExpr::eval(Interpreter& vm) {
    Value v = operand->eval(vm);
    if (op == TT::MINUS) return Value(-v.asNum("-"));
    if (op == TT::NOT)   return Value::boolean(!v.truthy());
    throw RuntimeError("namelum unar operator", loc);
}

Value BinExpr::eval(Interpreter& vm) {
    if (op == TT::AND) {
        if (!left->eval(vm).truthy()) return Value::boolean(false);
        return Value::boolean(right->eval(vm).truthy());
    }
    if (op == TT::OR) {
        if (left->eval(vm).truthy()) return Value::boolean(true);
        return Value::boolean(right->eval(vm).truthy());
    }

    Value a = left->eval(vm), b = right->eval(vm);

    if (op == TT::EQ)  return Value::boolean(a.equals(b));
    if (op == TT::NEQ) return Value::boolean(!a.equals(b));
    if (op == TT::LT)  return Value::boolean(a.cmp(b) < 0);
    if (op == TT::GT)  return Value::boolean(a.cmp(b) > 0);
    if (op == TT::LTE) return Value::boolean(a.cmp(b) <= 0);
    if (op == TT::GTE) return Value::boolean(a.cmp(b) >= 0);

    if (op == TT::TILDE) {
        if (a.isList() && b.isList()) {
            auto r = std::make_shared<std::vector<Value>>(*a.getList());
            for (auto& x : *b.getList()) r->push_back(x);
            return Value(r);
        }
        return Value(a.show() + b.show());
    }

    if (op == TT::CARET) {
        if (a.isNum() && b.isNum())
            return Value(std::pow(a.getNum(), b.getNum()));
        if (a.isNum() || !b.isNum())
            throw RuntimeError("^: (eded^eded) ve ya (metn/siyahi^eded) gozlenilirdi", loc);
        long long k = std::max(0LL, b.asInt("^"));
        if (a.isStr()) {
            if ((long long)a.getStr().size() * k > 10000000)
                throw RuntimeError("^: netice cox boyukdur", loc);
            std::string r; for (long long i = 0; i < k; i++) r += a.getStr();
            return Value(r);
        }
        auto& src = *a.getList();
        if ((long long)src.size() * k > 10000000)
            throw RuntimeError("^: netice cox boyukdur", loc);
        auto r = std::make_shared<std::vector<Value>>();
        for (long long i = 0; i < k; i++)
            r->insert(r->end(), src.begin(), src.end());
        return Value(r);
    }

    if (!a.isNum() || !b.isNum())
        throw RuntimeError(opStr + ": yalniz ededler ucundur (" + a.typeName() + " ile " + b.typeName() + "). Metn/siyahi ucun ~ istifade edin", loc);

    double x = a.getNum(), y = b.getNum();
    if (op == TT::PLUS)    return Value(x + y);
    if (op == TT::MINUS)   return Value(x - y);
    if (op == TT::STAR)    return Value(x * y);
    if (op == TT::SLASH) {
        if (y == 0.0) throw RuntimeError("sifira bolme", loc);
        return Value(x / y);
    }
    if (op == TT::PERCENT) {
        if (y == 0.0) throw RuntimeError("sifira bolme", loc);
        return Value(std::fmod(x, y));
    }
    throw RuntimeError("namelum operator: " + opStr, loc);
}

Value IndexExpr::eval(Interpreter& vm) {
    Value b = base->eval(vm);
    long long i = idx->eval(vm).asInt("indeks");
    if (b.isList()) {
        auto& l = *b.getList();
        return l[fixIndex(i, l.size(), loc)];
    }
    if (b.isStr()) {
        auto cs = UTF8::chars(b.getStr());
        return Value(cs[fixIndex(i, cs.size(), loc)]);
    }
    throw RuntimeError("indeks yalniz siyahi ve metn ucun islenir, " + b.typeName() + " tapildi", loc);
}

Value SliceExpr::eval(Interpreter& vm) {
    Value b = base->eval(vm);
    size_t n = 0;
    std::vector<std::string> cs;
    if (b.isList())      n = b.getList()->size();
    else if (b.isStr())  { cs = UTF8::chars(b.getStr()); n = cs.size(); }
    else throw RuntimeError("dilim yalniz siyahi ve metn ucun islenir", loc);

    long long N = (long long)n;
    long long a = lo ? lo->eval(vm).asInt("dilim") : 0;
    long long z = hi ? hi->eval(vm).asInt("dilim") : N - 1;
    if (a < 0) a += N;
    if (z < 0) z += N;
    a = std::max(0LL, a);
    z = std::min(z, N - 1);

    if (b.isList()) {
        auto r = std::make_shared<std::vector<Value>>();
        auto& l = *b.getList();
        for (long long k = a; k <= z; k++) r->push_back(l[k]);
        return Value(r);
    }
    std::string r; for (long long k = a; k <= z; k++) r += cs[k];
    return Value(r);
}

Value CallExpr::eval(Interpreter& vm) {
    std::vector<Value> evArgs;
    for (auto& a : args) evArgs.push_back(a->eval(vm));
    return vm.callBuiltin(name, evArgs, loc);
}

void PrintStmt::exec(Interpreter& vm)   { vm.print(expr->eval(vm).show()); }
void ExprStmt::exec(Interpreter& vm)    { expr->eval(vm); }
void VarDeclStmt::exec(Interpreter& vm) { vm.getEnv()->define(name, Value()); }
void AssignStmt::exec(Interpreter& vm)  { vm.getEnv()->assign(name, value->eval(vm)); }

void IndexAssignStmt::exec(Interpreter& vm) {
    Value val = value->eval(vm);
    Value b   = base->eval(vm);
    long long i = idx->eval(vm).asInt("indeks");
    if (b.isStr()) throw RuntimeError("metn deyisdirile bilmez", loc);
    auto l = b.asList("indeks");
    size_t k = fixIndex(i, l->size(), loc);
    (*l)[k] = val;
}

void BlockStmt::exec(Interpreter& vm) {
    for (auto& s : stmts) s->exec(vm);
}

void IfStmt::exec(Interpreter& vm) {
    if (cond->eval(vm).truthy()) yes->exec(vm);
    else if (no) no->exec(vm);
}

void WhileStmt::exec(Interpreter& vm) {
    while (cond->eval(vm).truthy()) body->exec(vm);
}

void ForEachStmt::exec(Interpreter& vm) {
    Value v = iter->eval(vm);
    if (v.isList()) {
        auto copy = *v.getList();
        for (auto& item : copy) {
            vm.getEnv()->assign(var, item);
            body->exec(vm);
        }
    } else if (v.isStr()) {
        for (auto& ch : UTF8::chars(v.getStr())) {
            vm.getEnv()->assign(var, Value(ch));
            body->exec(vm);
        }
    } else {
        throw RuntimeError("dovr ucun siyahi ve ya metn lazimdir, " + v.typeName() + " tapildi", loc);
    }
}

