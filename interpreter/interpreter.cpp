#include "interpreter.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "utf8.hpp"
#include "error.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

Interpreter::Interpreter(std::ostream& o, std::ostream& e)
    : globalEnv(std::make_shared<Environment>()),
      env(globalEnv),
      out(o), err(e) {
    initBuiltins();
}

void Interpreter::arity(const std::vector<Value>& a, size_t lo, size_t hi,
                        const std::string& name, SourceLocation loc) {
    if (a.size() < lo || a.size() > hi)
        throw RuntimeError(name + ": arqument sayi yanlisdir", loc);
}

void Interpreter::initBuiltins() {
    // uz
    builtins["uz"] = [](std::vector<Value>& a, const SourceLocation& loc) -> Value {
        arity(a, 1, 1, "uz", loc);
        if (a[0].isStr())  return Value((double)UTF8::len(a[0].getStr()));
        if (a[0].isList()) return Value((double)a[0].getList()->size());
        if (a[0].isNum())  return Value((double)a[0].show().size());
        throw RuntimeError("uz: metn ve ya siyahi gozlenilirdi", loc);
    };

    // qat
    builtins["qat"] = [](std::vector<Value>& a, const SourceLocation& loc) -> Value {
        arity(a, 2, 2, "qat", loc);
        if (a[0].isList()) {
            auto l = a[0].getList();
            if (a[1].isList()) {
                for (auto& item : *a[1].getList()) l->push_back(item);
            } else {
                l->push_back(a[1]);
            }
            return a[0];
        }
        return Value(a[0].show() + a[1].show());
    };

    // cixar
    builtins["cixar"] = [](std::vector<Value>& a, const SourceLocation& loc) -> Value {
        arity(a, 1, 2, "cixar", loc);
        if (a[0].isNum() && a.size() == 2 && a[1].isNum())
            return Value(a[0].getNum() - a[1].getNum());
        if (a[0].isList()) {
            auto l = a[0].getList();
            if (l->empty()) throw RuntimeError("cixar: siyahi bosdur", loc);
            long long k = (a.size() == 2) ? a[1].asInt("cixar") : -1;
            size_t i = fixIndex(k, l->size(), loc);
            Value r = (*l)[i];
            l->erase(l->begin() + i);
            return r;
        }
        if (a[0].isStr() && a.size() == 2 && a[1].isStr()) {
            std::string s = a[0].getStr(), sub = a[1].getStr();
            size_t p = s.find(sub);
            if (p != std::string::npos) s.erase(p, sub.size());
            return Value(s);
        }
        throw RuntimeError("cixar: ededler ve ya siyahi gozlenilirdi", loc);
    };

    // metn
    builtins["metn"] = [](std::vector<Value>& a, const SourceLocation& loc) -> Value {
        arity(a, 1, 1, "metn", loc);
        return Value(a[0].show());
    };

    // eded
    builtins["eded"] = [](std::vector<Value>& a, const SourceLocation& loc) -> Value {
        arity(a, 1, 1, "eded", loc);
        if (a[0].isNum()) return a[0];
        const std::string& s = a[0].asStr("eded");
        try {
            size_t p;
            double d = std::stod(s, &p);
            while (p < s.size() && std::isspace((unsigned char)s[p])) p++;
            if (p != s.size()) throw 1;
            return Value(d);
        } catch (...) {
            throw RuntimeError("eded: '" + s + "' edede cevrilmir", loc);
        }
    };

    // sirala
    builtins["sirala"] = [](std::vector<Value>& a, const SourceLocation& loc) -> Value {
        arity(a, 1, 1, "sirala", loc);
        if (a[0].isList()) {
            auto r = std::make_shared<std::vector<Value>>(*a[0].asList("sirala"));
            std::stable_sort(r->begin(), r->end(), [](const Value& x, const Value& y){ return x.cmp(y) < 0; });
            return Value(r);
        }
        if (a[0].isStr()) {
            auto cs = UTF8::chars(a[0].getStr());
            std::stable_sort(cs.begin(), cs.end());
            std::string r; for (auto& c : cs) r += c;
            return Value(r);
        }
        throw RuntimeError("sirala: siyahi ve ya metn gozlenilirdi", loc);
    };

    // boyuk
    builtins["boyuk"] = [](std::vector<Value>& a, const SourceLocation& loc) -> Value {
        arity(a, 1, 2, "boyuk", loc);
        if (a.size() == 2) return a[0].cmp(a[1]) >= 0 ? a[0] : a[1];
        if (a[0].isStr()) return Value(UTF8::toUpper(a[0].getStr()));
        if (a[0].isList()) {
            const auto& l = *a[0].getList();
            if (l.empty()) throw RuntimeError("boyuk: siyahi bosdur", loc);
            Value m = l[0];
            for (size_t i = 1; i < l.size(); i++) if (l[i].cmp(m) > 0) m = l[i];
            return m;
        }
        return a[0];
    };

    // kicik
    builtins["kicik"] = [](std::vector<Value>& a, const SourceLocation& loc) -> Value {
        arity(a, 1, 2, "kicik", loc);
        if (a.size() == 2) return a[0].cmp(a[1]) <= 0 ? a[0] : a[1];
        if (a[0].isStr()) return Value(UTF8::toLower(a[0].getStr()));
        if (a[0].isList()) {
            const auto& l = *a[0].getList();
            if (l.empty()) throw RuntimeError("kicik: siyahi bosdur", loc);
            Value m = l[0];
            for (size_t i = 1; i < l.size(); i++) if (l[i].cmp(m) < 0) m = l[i];
            return m;
        }
        return a[0];
    };

    // bol
    builtins["bol"] = [](std::vector<Value>& a, const SourceLocation& loc) -> Value {
        arity(a, 1, 2, "bol", loc);
        if (a.size() == 2 && a[0].isNum() && a[1].isNum()) {
            double d = a[1].getNum();
            if (d == 0.0) throw RuntimeError("sifira bolme", loc);
            return Value(a[0].getNum() / d);
        }
        const std::string& s = a[0].asStr("bol");
        auto r = std::make_shared<std::vector<Value>>();
        if (a.size() == 1) {
            std::istringstream ss(s); std::string w;
            while (ss >> w) r->push_back(Value(w));
            return Value(r);
        }
        const std::string& sep = a[1].asStr("bol");
        if (sep.empty()) throw RuntimeError("bol: ayirici bos ola bilmez", loc);
        size_t p = 0;
        while (true) {
            size_t f = s.find(sep, p);
            if (f == std::string::npos) { r->push_back(Value(s.substr(p))); break; }
            r->push_back(Value(s.substr(p, f - p)));
            p = f + sep.size();
        }
        return Value(r);
    };

    // yig
    builtins["yig"] = [](std::vector<Value>& a, const SourceLocation& loc) -> Value {
        arity(a, 1, 2, "yig", loc);
        const auto& l = *a[0].asList("yig");
        if (a.size() == 1 && !l.empty()) {
            bool allNum = true;
            for (auto& v : l) if (!v.isNum()) { allNum = false; break; }
            if (allNum) {
                double s = 0; for (auto& v : l) s += v.getNum();
                return Value(s);
            }
        }
        std::string sep = (a.size() == 2) ? a[1].asStr("yig") : "";
        std::string r;
        for (size_t i = 0; i < l.size(); i++) { if (i) r += sep; r += l[i].show(); }
        return Value(r);
    };

    // var
    builtins["var"] = [](std::vector<Value>& a, const SourceLocation& loc) -> Value {
        arity(a, 1, 2, "var", loc);
        if (a.size() == 1) return Value::boolean(a[0].truthy());
        if (a[0].isList()) {
            for (auto& e : *a[0].getList()) if (e.equals(a[1])) return Value::boolean(true);
            return Value::boolean(false);
        }
        if (a[0].isStr() && a[1].isStr())
            return Value::boolean(a[0].getStr().find(a[1].getStr()) != std::string::npos);
        throw RuntimeError("var: (siyahi, deyer) ve ya (metn, metn) gozlenilirdi", loc);
    };
}

Value Interpreter::callBuiltin(const std::string& name, std::vector<Value>& args, SourceLocation loc) {
    auto it = builtins.find(name);
    if (it == builtins.end())
        throw RuntimeError("namelum funksiya: " + name, loc);
    return it->second(args, loc);
}

bool Interpreter::isBuiltin(const std::string& name) const {
    return builtins.count(name) > 0;
}

void Interpreter::exec(const SP& s) {
    if (s) s->exec(*this);
}

Value Interpreter::eval(const EP& e) {
    return e ? e->eval(*this) : Value();
}

void Interpreter::print(const std::string& s) {
    out << s << "\n";
}

void Interpreter::printErr(const std::string& s) {
    err << s << "\n";
}

void Interpreter::run(const std::string& source) {
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    Parser parser(std::move(tokens));
    auto program = parser.parse();
    program->exec(*this);
}

bool Interpreter::runFile(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) { err << "Fayl acilmadi\n"; return false; }
    std::stringstream ss; ss << f.rdbuf();
    try {
        run(ss.str());
        return true;
    } catch (const XRError& e) {
        err << "Xeta: " << e.what() << "\n";
        return false;
    } catch (const std::exception& e) {
        err << "Xeta: " << e.what() << "\n";
        return false;
    }
}

void Interpreter::repl() {
    out << "XR Proqramlasdirma Dili v0.0.1 (OOP Edition)\n";
    out << "Cixis ucun 'cix', komek ucun 'komek' yazin.\n\n";

    std::string line, buf;
    int depth = 0;

    while (true) {
        out << (depth > 0 ? "... " : "xr> ");
        out.flush();
        if (!std::getline(std::cin, line)) break;

        std::string tr = line;
        while (!tr.empty() && std::isspace((unsigned char)tr.front())) tr.erase(tr.begin());
        while (!tr.empty() && std::isspace((unsigned char)tr.back()))  tr.pop_back();

        if (depth == 0) {
            if (tr == "cix" || tr == "exit" || tr == "quit") break;
            if (tr == "komek" || tr == "help") {
                out << "  deyer -> ad;       Deyisen elan et\n"
                       "  5 -> ad;           Deyere qiymet ver\n"
                       "  > ad;              Ekrana yaz\n"
                       "  ? sert { } : { }  Eger / yoxsa\n"
                       "  @ sert { }         Dovr\n"
                       "  @ x : lst { }      For-each\n"
                       "  cix                Cixis\n\n";
                continue;
            }
        }

        for (char c : line) {
            if (c == '{') depth++;
            else if (c == '}') depth = std::max(0, depth - 1);
        }
        buf += line + "\n";

        if (depth == 0 && !tr.empty()) {
            try { run(buf); }
            catch (const XRError& e)      { err << "Xeta: " << e.what() << "\n"; }
            catch (const std::exception& e){ err << "Xeta: " << e.what() << "\n"; }
            buf.clear();
        }
    }
}

