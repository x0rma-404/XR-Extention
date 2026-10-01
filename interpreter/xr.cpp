// Oz dilimiz v3 - Python-a oxsamayan sintaksis

//
//   deyer -> ad;              menimsetme elan etme
//   5 -> ad;                  deyere qiymet ver
//   > deyer;                  ekrana yaz
//   ? sert { } : { }          eger / yoxsa
//   @ sert { }                dovr
//   @ x : siyahi { }          her element uzre
//   // serh
//   'metn'                    metn
//   #[1, 2, 3]               siyahi
//   1..5  5..1                araliq (her iki ucu daxil)
//   a.0  a.i  a.(i+1)  a.$    indeks
//   a.(1..3)                  dilim
//
//   ~                         birlesdirme
//   ^                         quvvet / tekrar
//   =                         beraberdir
//   <>                        ferqlidir
//   &                         ve
//   |                         veya
//   !                         deyil
//   + - * / %                 riyazi operatorlar
//
//   hazir funksiyalar:
//   uz qat cixar metn eded sirala boyuk kicik bol yig var
//
// Derleme:
// g++ -std=c++17 xr.cpp -o xr.exe

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cstdio>
#include <map>
#include <memory>
#include <vector>
#include <string>
#include <variant>
#include <functional>
#include <algorithm>
#include <cctype>
#include <stdexcept>

using namespace std;


// ============================================================
// VALUE
// ============================================================

struct Value;

using List = shared_ptr<vector<Value>>;

struct Value {

    variant<double, string, List> v;

    Value() : v(0.0) {}

    Value(double d) : v(d) {}

    Value(string s) : v(move(s)) {}

    Value(List l) : v(move(l)) {}
};


template <class T>
bool isT(const Value& x) {
    return holds_alternative<T>(x.v);
}


template <class T>
const T& getT(const Value& x) {
    return get<T>(x.v);
}


Value B(bool b) {
    return Value(b ? 1.0 : 0.0);
}


string typeName(const Value& x) {

    if (isT<double>(x))
        return "eded";

    if (isT<string>(x))
        return "metn";

    return "siyahi";
}


string fmtNum(double d) {

    if (d == floor(d) && fabs(d) < 1e15) {

        char buf[32];

        snprintf(
            buf,
            sizeof buf,
            "%lld",
            (long long)d
        );

        return buf;
    }

    ostringstream o;

    o << setprecision(10) << d;

    return o.str();
}


string show(const Value& x, bool quote = false) {

    if (isT<double>(x))
        return fmtNum(getT<double>(x));

    if (isT<string>(x)) {

        return quote
            ? "'" + getT<string>(x) + "'"
            : getT<string>(x);
    }

    string r = "#[";

    auto& l = *getT<List>(x);

    for (size_t i = 0; i < l.size(); i++) {

        if (i)
            r += ", ";

        r += show(l[i], true);
    }

    return r + "]";
}


bool truthy(const Value& x) {

    if (isT<double>(x))
        return getT<double>(x) != 0;

    if (isT<string>(x))
        return !getT<string>(x).empty();

    return !getT<List>(x)->empty();
}


bool equalv(const Value& a, const Value& b) {

    if (a.v.index() != b.v.index())
        return false;

    if (isT<double>(a))
        return getT<double>(a) == getT<double>(b);

    if (isT<string>(a))
        return getT<string>(a) == getT<string>(b);

    auto& x = *getT<List>(a);
    auto& y = *getT<List>(b);

    if (x.size() != y.size())
        return false;

    for (size_t i = 0; i < x.size(); i++) {

        if (!equalv(x[i], y[i]))
            return false;
    }

    return true;
}


int cmpv(const Value& a, const Value& b) {

    if (isT<double>(a) && isT<double>(b)) {

        double x = getT<double>(a);
        double y = getT<double>(b);

        return x < y ? -1 :
               x > y ? 1 : 0;
    }

    if (isT<string>(a) && isT<string>(b)) {

        int c =
            getT<string>(a).compare(
                getT<string>(b)
            );

        return c < 0 ? -1 :
               c > 0 ? 1 : 0;
    }

    throw runtime_error(
        "muqayise: " +
        typeName(a) +
        " ile " +
        typeName(b) +
        " muqayise olunmur"
    );
}


double asNum(
    const Value& x,
    const string& ctx
) {

    if (!isT<double>(x)) {

        throw runtime_error(
            ctx +
            ": eded gozlenilirdi, " +
            typeName(x) +
            " tapildi"
        );
    }

    return getT<double>(x);
}


long long asInt(
    const Value& x,
    const string& ctx
) {

    double d = asNum(x, ctx);

    if (d != floor(d))
        throw runtime_error(
            ctx +
            ": tam eded gozlenilirdi"
        );

    return (long long)d;
}


const string& asStr(
    const Value& x,
    const string& ctx
) {

    if (!isT<string>(x)) {

        throw runtime_error(
            ctx +
            ": metn gozlenilirdi, " +
            typeName(x) +
            " tapildi"
        );
    }

    return getT<string>(x);
}


const List& asList(
    const Value& x,
    const string& ctx
) {

    if (!isT<List>(x)) {

        throw runtime_error(
            ctx +
            ": siyahi gozlenilirdi, " +
            typeName(x) +
            " tapildi"
        );
    }

    return getT<List>(x);
}


size_t fixIndex(
    long long i,
    size_t n
) {

    long long original = i;

    if (i < 0)
        i += (long long)n;

    if (
        i < 0 ||
        i >= (long long)n
    ) {

        throw runtime_error(
            "indeks hududdan kenardadir: " +
            to_string(original)
        );
    }

    return (size_t)i;
}


// ============================================================
// UTF-8 CHARACTERS
// ============================================================

vector<string> chars(const string& s) {

    vector<string> r;

    for (size_t i = 0; i < s.size();) {

        unsigned char c = s[i];

        size_t n =
            c < 0x80 ? 1 :
            (c >> 5) == 6 ? 2 :
            (c >> 4) == 14 ? 3 :
            (c >> 3) == 30 ? 4 :
            1;

        if (i + n > s.size())
            n = s.size() - i;

        r.push_back(
            s.substr(i, n)
        );

        i += n;
    }

    return r;
}


// ============================================================
// BUILT-IN FUNCTIONS
// ============================================================

using Fn =
    function<Value(vector<Value>&)>;

map<string, Fn> builtins;


void arity(
    vector<Value>& a,
    size_t lo,
    size_t hi,
    const string& n
) {

    if (
        a.size() < lo ||
        a.size() > hi
    ) {

        throw runtime_error(
            n +
            ": arqument sayi yanlisdir"
        );
    }
}


void initBuiltins() {

    // uz[siyahi] / uz[metn]

    builtins["uz"] =
        [](vector<Value>& a) -> Value {

            arity(a, 1, 1, "uz");

            if (isT<string>(a[0])) {

                return Value(
                    (double)chars(
                        getT<string>(a[0])
                    ).size()
                );
            }

            if (isT<List>(a[0])) {

                return Value(
                    (double)getT<List>(a[0])->size()
                );
            }

            throw runtime_error(
                "uz: metn ve ya siyahi gozlenilirdi"
            );
        };


    // qat[siyahi, deyer]

    builtins["qat"] =
        [](vector<Value>& a) -> Value {

            arity(a, 2, 2, "qat");

            asList(a[0], "qat")
                ->push_back(a[1]);

            return a[0];
        };


    // cixar[siyahi]
    // cixar[siyahi, i]

    builtins["cixar"] =
        [](vector<Value>& a) -> Value {

            arity(a, 1, 2, "cixar");

            auto l =
                asList(a[0], "cixar");

            if (l->empty())
                throw runtime_error(
                    "cixar: siyahi bosdur"
                );

            size_t k =
                fixIndex(
                    a.size() == 2
                        ? asInt(a[1], "cixar")
                        : -1,
                    l->size()
                );

            Value r = (*l)[k];

            l->erase(
                l->begin() + k
            );

            return r;
        };


    // metn[deyer]

    builtins["metn"] =
        [](vector<Value>& a) -> Value {

            arity(a, 1, 1, "metn");

            return Value(
                show(a[0])
            );
        };


    // eded[deyer]

    builtins["eded"] =
        [](vector<Value>& a) -> Value {

            arity(a, 1, 1, "eded");

            if (isT<double>(a[0]))
                return a[0];

            const string& s =
                asStr(a[0], "eded");

            try {

                size_t pos;

                double d =
                    stod(s, &pos);

                while (
                    pos < s.size() &&
                    isspace(
                        (unsigned char)s[pos]
                    )
                )
                    pos++;

                if (pos != s.size())
                    throw 1;

                return Value(d);

            } catch (...) {

                throw runtime_error(
                    "eded: '" +
                    s +
                    "' edede cevrilmir"
                );
            }
        };


    // sirala[siyahi]

    builtins["sirala"] =
        [](vector<Value>& a) -> Value {

            arity(a, 1, 1, "sirala");

            auto r =
                make_shared<vector<Value>>(
                    *asList(a[0], "sirala")
                );

            stable_sort(
                r->begin(),
                r->end(),
                [](const Value& x,
                   const Value& y) {

                    return cmpv(x, y) < 0;
                }
            );

            return Value(r);
        };


    // boyuk[metn]

    builtins["boyuk"] =
        [](vector<Value>& a) -> Value {

            arity(a, 1, 1, "boyuk");

            string s =
                asStr(a[0], "boyuk");

            for (auto& c : s)
                c = toupper(
                    (unsigned char)c
                );

            return Value(s);
        };


    // kicik[metn]

    builtins["kicik"] =
        [](vector<Value>& a) -> Value {

            arity(a, 1, 1, "kicik");

            string s =
                asStr(a[0], "kicik");

            for (auto& c : s)
                c = tolower(
                    (unsigned char)c
                );

            return Value(s);
        };


    // bol[metn]
    // bol[metn, ayirici]

    builtins["bol"] =
        [](vector<Value>& a) -> Value {

            arity(a, 1, 2, "bol");

            string s =
                asStr(a[0], "bol");

            auto r =
                make_shared<vector<Value>>();

            if (a.size() == 1) {

                istringstream in(s);

                string w;

                while (in >> w)
                    r->push_back(
                        Value(w)
                    );

                return Value(r);
            }

            string sep =
                asStr(a[1], "bol");

            if (sep.empty())
                throw runtime_error(
                    "bol: ayirici bos ola bilmez"
                );

            size_t pos = 0;

            while (true) {

                size_t f =
                    s.find(sep, pos);

                if (f == string::npos) {

                    r->push_back(
                        Value(
                            s.substr(pos)
                        )
                    );

                    break;
                }

                r->push_back(
                    Value(
                        s.substr(
                            pos,
                            f - pos
                        )
                    )
                );

                pos =
                    f + sep.size();
            }

            return Value(r);
        };


    // yig[siyahi]
    // yig[siyahi, ayirici]

    builtins["yig"] =
        [](vector<Value>& a) -> Value {

            arity(a, 1, 2, "yig");

            auto l =
                asList(a[0], "yig");

            string sep =
                a.size() == 2
                    ? asStr(a[1], "yig")
                    : string();

            string r;

            for (size_t i = 0; i < l->size(); i++) {

                if (i)
                    r += sep;

                r += show(
                    (*l)[i]
                );
            }

            return Value(r);
        };


    // var[siyahi, deyer]
    // var[metn, metn]

    builtins["var"] =
        [](vector<Value>& a) -> Value {

            arity(a, 2, 2, "var");

            if (isT<List>(a[0])) {

                for (
                    auto& e :
                    *getT<List>(a[0])
                ) {

                    if (equalv(e, a[1]))
                        return B(true);
                }

                return B(false);
            }

            if (
                isT<string>(a[0]) &&
                isT<string>(a[1])
            ) {

                return B(
                    getT<string>(a[0]).find(
                        getT<string>(a[1])
                    ) != string::npos
                );
            }

            throw runtime_error(
                "var: (siyahi, deyer) ve ya "
                "(metn, metn) gozlenilirdi"
            );
        };
}


// ============================================================
// LEXER
// ============================================================

enum Type {
    NUM,
    STR,
    ID,
    OP,
    END
};


struct Token {

    Type type;

    string text;

    double num = 0;
};


vector<Token> lex(const string& s) {

    vector<Token> out;

    size_t i = 0;

    while (i < s.size()) {

        char c = s[i];

        // whitespace

        if (isspace((unsigned char)c)) {

            i++;

            continue;
        }


        // comment

        if (
            c == '/' &&
            i + 1 < s.size() &&
            s[i + 1] == '/'
        ) {

            while (
                i < s.size() &&
                s[i] != '\n'
            )
                i++;

            continue;
        }


        // string

        if (c == '\'') {

            string t;

            bool closed = false;

            i++;

            while (i < s.size()) {

                char d = s[i];

                if (d == '\'') {

                    closed = true;

                    i++;

                    break;
                }

                if (
                    d == '\\' &&
                    i + 1 < s.size()
                ) {

                    char e =
                        s[i + 1];

                    t +=
                        e == 'n' ? '\n' :
                        e == 't' ? '\t' :
                        e;

                    i += 2;

                    continue;
                }

                t += d;

                i++;
            }

            if (!closed)
                throw runtime_error(
                    "metn baglanmayib (' yoxdur)"
                );

            out.push_back(
                {STR, t}
            );

            continue;
        }


        // number

        if (isdigit((unsigned char)c)) {

            bool afterDot =
                !out.empty() &&
                out.back().type == OP &&
                out.back().text == ".";

            size_t j = i;

            bool dot = false;

            while (j < s.size()) {

                if (
                    isdigit(
                        (unsigned char)s[j]
                    )
                ) {

                    j++;

                } else if (
                    s[j] == '.' &&
                    !dot &&
                    !afterDot &&
                    j + 1 < s.size() &&
                    isdigit(
                        (unsigned char)s[j + 1]
                    )
                ) {

                    dot = true;

                    j++;

                } else {

                    break;
                }
            }

            string t =
                s.substr(
                    i,
                    j - i
                );

            out.push_back(
                {
                    NUM,
                    t,
                    stod(t)
                }
            );

            i = j;

            continue;
        }


        // identifier

        auto idc =
            [](char ch) {

                return
                    isalnum(
                        (unsigned char)ch
                    ) ||
                    ch == '_' ||
                    (unsigned char)ch >= 0x80;
            };


        if (
            isalpha(
                (unsigned char)c
            ) ||
            c == '_' ||
            (unsigned char)c >= 0x80
        ) {

            size_t j = i;

            while (
                j < s.size() &&
                idc(s[j])
            )
                j++;

            out.push_back(
                {
                    ID,
                    s.substr(
                        i,
                        j - i
                    )
                }
            );

            i = j;

            continue;
        }


        // two-character operators

        string two =
            s.substr(i, 2);

        if (
            two == "->" ||
            two == ".." ||
            two == "<>" ||
            two == "<=" ||
            two == ">="
        ) {

            out.push_back(
                {
                    OP,
                    two
                }
            );

            i += 2;

            continue;
        }


        // one-character operator

        out.push_back(
            {
                OP,
                string(1, c)
            }
        );

        i++;
    }


    out.push_back(
        {
            END,
            ""
        }
    );

    return out;
}


// ============================================================
// AST
// ============================================================

map<string, Value> vars;


struct Expr {

    virtual Value eval() = 0;

    virtual ~Expr() {}
};


using E = shared_ptr<Expr>;


struct Lit : Expr {

    Value v;

    Lit(Value v)
        : v(v) {}

    Value eval() override {
        return v;
    }
};


struct Var : Expr {

    string n;

    Var(string n)
        : n(n) {}

    Value eval() override {

        auto it =
            vars.find(n);

        if (it == vars.end()) {

            throw runtime_error(
                "teyin olunmamis deyisen: " +
                n
            );
        }

        return it->second;
    }
};


struct ListLit : Expr {

    vector<E> items;

    ListLit(vector<E> i)
        : items(i) {}

    Value eval() override {

        auto r =
            make_shared<vector<Value>>();

        for (auto& e : items)
            r->push_back(
                e->eval()
            );

        return Value(r);
    }
};


struct Range : Expr {

    E lo;
    E hi;

    Range(E l, E h)
        : lo(l), hi(h) {}

    Value eval() override {

        long long a =
            asInt(
                lo->eval(),
                ".."
            );

        long long b =
            asInt(
                hi->eval(),
                ".."
            );

        long long step =
            a <= b ? 1 : -1;

        if (
            (b > a ? b - a : a - b)
            > 10000000
        ) {

            throw runtime_error(
                "..: araliq cox boyukdur"
            );
        }

        auto r =
            make_shared<vector<Value>>();

        for (
            long long k = a;;
            k += step
        ) {

            r->push_back(
                Value((double)k)
            );

            if (k == b)
                break;
        }

        return Value(r);
    }
};


struct Neg : Expr {

    E e;

    Neg(E e)
        : e(e) {}

    Value eval() override {

        return Value(
            -asNum(
                e->eval(),
                "-"
            )
        );
    }
};


struct Not : Expr {

    E e;

    Not(E e)
        : e(e) {}

    Value eval() override {

        return B(
            !truthy(
                e->eval()
            )
        );
    }
};


struct Index : Expr {

    E base;
    E idx;

    Index(E b, E i)
        : base(b), idx(i) {}

    Value eval() override {

        Value b =
            base->eval();

        long long i =
            asInt(
                idx->eval(),
                "indeks"
            );

        if (isT<List>(b)) {

            auto& l =
                *getT<List>(b);

            return l[
                fixIndex(
                    i,
                    l.size()
                )
            ];
        }

        if (isT<string>(b)) {

            auto c =
                chars(
                    getT<string>(b)
                );

            return Value(
                c[
                    fixIndex(
                        i,
                        c.size()
                    )
                ]
            );
        }

        throw runtime_error(
            "indeks yalniz siyahi ve metn "
            "ucun islenir, " +
            typeName(b) +
            " tapildi"
        );
    }
};


struct Slice : Expr {

    E base;
    E lo;
    E hi;

    Slice(E b, E l, E h)
        : base(b), lo(l), hi(h) {}

    Value eval() override {

        Value b =
            base->eval();

        size_t n;

        vector<string> cs;

        if (isT<List>(b)) {

            n =
                getT<List>(b)->size();

        } else if (isT<string>(b)) {

            cs =
                chars(
                    getT<string>(b)
                );

            n = cs.size();

        } else {

            throw runtime_error(
                "dilim yalniz siyahi ve metn "
                "ucun islenir"
            );
        }

        long long N =
            (long long)n;

        long long a =
            lo
                ? asInt(
                    lo->eval(),
                    "dilim"
                )
                : 0;

        long long z =
            hi
                ? asInt(
                    hi->eval(),
                    "dilim"
                )
                : N - 1;

        if (a < 0)
            a += N;

        if (z < 0)
            z += N;

        a = max(0LL, a);

        z = min(z, N - 1);

        if (isT<List>(b)) {

            auto r =
                make_shared<vector<Value>>();

            auto& l =
                *getT<List>(b);

            for (
                long long k = a;
                k <= z;
                k++
            )
                r->push_back(l[k]);

            return Value(r);
        }

        string r;

        for (
            long long k = a;
            k <= z;
            k++
        )
            r += cs[k];

        return Value(r);
    }
};


struct Call : Expr {

    string name;

    vector<E> args;

    Call(
        string n,
        vector<E> a
    )
        : name(n), args(a) {}

    Value eval() override {

        auto it =
            builtins.find(name);

        if (it == builtins.end()) {

            throw runtime_error(
                "namelum funksiya: " +
                name
            );
        }

        vector<Value> a;

        for (auto& e : args)
            a.push_back(
                e->eval()
            );

        return it->second(a);
    }
};


struct Bin : Expr {

    string op;

    E l;
    E r;

    Bin(
        string op,
        E l,
        E r
    )
        : op(op), l(l), r(r) {}

    Value eval() override {

        // AND

        if (op == "&") {

            if (
                !truthy(
                    l->eval()
                )
            )
                return B(false);

            return B(
                truthy(
                    r->eval()
                )
            );
        }


        // OR

        if (op == "|") {

            if (
                truthy(
                    l->eval()
                )
            )
                return B(true);

            return B(
                truthy(
                    r->eval()
                )
            );
        }


        Value a =
            l->eval();

        Value b =
            r->eval();


        // equality

        if (op == "=")
            return B(
                equalv(a, b)
            );


        // not equal

        if (op == "<>")
            return B(
                !equalv(a, b)
            );


        // comparison

        if (op == "<")
            return B(
                cmpv(a, b) < 0
            );

        if (op == ">")
            return B(
                cmpv(a, b) > 0
            );

        if (op == "<=")
            return B(
                cmpv(a, b) <= 0
            );

        if (op == ">=")
            return B(
                cmpv(a, b) >= 0
            );


        // concatenation

        if (op == "~") {

            if (
                isT<List>(a) &&
                isT<List>(b)
            ) {

                auto r =
                    make_shared<vector<Value>>(
                        *getT<List>(a)
                    );

                r->insert(
                    r->end(),
                    getT<List>(b)->begin(),
                    getT<List>(b)->end()
                );

                return Value(r);
            }

            return Value(
                show(a) +
                show(b)
            );
        }


        // power / repeat

        if (op == "^") {

            if (
                isT<double>(a) &&
                isT<double>(b)
            ) {

                return Value(
                    pow(
                        getT<double>(a),
                        getT<double>(b)
                    )
                );
            }

            if (
                isT<double>(a) ||
                !isT<double>(b)
            ) {

                throw runtime_error(
                    "^: (eded ^ eded) ve ya "
                    "(metn/siyahi ^ eded) gozlenilirdi"
                );
            }

            long long k =
                max(
                    0LL,
                    asInt(b, "^")
                );

            if (isT<string>(a)) {

                if (
                    (long long)
                    getT<string>(a).size()
                    * k
                    > 10000000
                )
                    throw runtime_error(
                        "^: netice cox boyukdur"
                    );

                string r;

                for (
                    long long i = 0;
                    i < k;
                    i++
                )
                    r += getT<string>(a);

                return Value(r);
            }

            auto& src =
                *getT<List>(a);

            if (
                (long long)
                src.size() * k
                > 10000000
            )
                throw runtime_error(
                    "^: netice cox boyukdur"
                );

            auto r =
                make_shared<vector<Value>>();

            for (
                long long i = 0;
                i < k;
                i++
            )
                r->insert(
                    r->end(),
                    src.begin(),
                    src.end()
                );

            return Value(r);
        }


        // numeric operators

        if (
            !isT<double>(a) ||
            !isT<double>(b)
        ) {

            throw runtime_error(
                op +
                ": yalniz ededler ucundur (" +
                typeName(a) +
                " ile " +
                typeName(b) +
                "). Metn/siyahi ucun ~ istifade edin"
            );
        }

        double x =
            getT<double>(a);

        double y =
            getT<double>(b);


        if (op == "+")
            return Value(x + y);

        if (op == "-")
            return Value(x - y);

        if (op == "*")
            return Value(x * y);

        if (op == "/") {

            if (y == 0)
                throw runtime_error(
                    "sifira bolme"
                );

            return Value(x / y);
        }

        if (op == "%") {

            if (y == 0)
                throw runtime_error(
                    "sifira bolme"
                );

            return Value(
                fmod(x, y)
            );
        }


        throw runtime_error(
            "namelum operator: " +
            op
        );
    }
};


// ============================================================
// STATEMENTS
// ============================================================

struct Stmt {

    virtual void run() = 0;

    virtual ~Stmt() {}
};


using S = shared_ptr<Stmt>;


struct Assign : Stmt {

    string n;

    E e;

    Assign(
        string n,
        E e
    )
        : n(n), e(e) {}

    void run() override {

        vars[n] =
            e->eval();
    }
};


struct IndexAssign : Stmt {

    E base;

    E idx;

    E e;

    IndexAssign(
        E b,
        E i,
        E e
    )
        : base(b), idx(i), e(e) {}

    void run() override {

        Value val =
            e->eval();

        Value b =
            base->eval();

        long long i =
            asInt(
                idx->eval(),
                "indeks"
            );

        if (isT<string>(b))
            throw runtime_error(
                "metn deyisdirile bilmez"
            );

        auto l =
            asList(
                b,
                "indeks"
            );

        (*l)[
            fixIndex(
                i,
                l->size()
            )
        ] = val;
    }
};


struct ExprStmt : Stmt {

    E e;

    ExprStmt(E e)
        : e(e) {}

    void run() override {

        e->eval();
    }
};


struct Print : Stmt {

    E e;

    Print(E e)
        : e(e) {}

    void run() override {

        cout <<
            show(
                e->eval()
            ) <<
            "\n";
    }
};


struct Block : Stmt {

    vector<S> list;

    void run() override {

        for (auto& s : list)
            s->run();
    }
};


struct If : Stmt {

    E c;

    S yes;

    S no;

    void run() override {

        if (
            truthy(
                c->eval()
            )
        )
            yes->run();

        else if (no)
            no->run();
    }
};


struct While : Stmt {

    E c;

    S body;

    void run() override {

        while (
            truthy(
                c->eval()
            )
        )
            body->run();
    }
};


struct ForEach : Stmt {

    string n;

    E it;

    S body;

    void run() override {

        Value v =
            it->eval();

        if (isT<List>(v)) {

            auto l =
                getT<List>(v);

            for (
                size_t i = 0;
                i < l->size();
                i++
            ) {

                vars[n] =
                    (*l)[i];

                body->run();
            }

        } else if (isT<string>(v)) {

            for (
                auto& c :
                chars(
                    getT<string>(v)
                )
            ) {

                vars[n] =
                    Value(c);

                body->run();
            }

        } else {

            throw runtime_error(
                "dovr ucun siyahi ve ya metn "
                "lazimdir, " +
                typeName(v) +
                " tapildi"
            );
        }
    }
};


// ============================================================
// PARSER
// ============================================================

struct Parser {

    vector<Token> t;

    size_t p = 0;


    Parser(vector<Token> t)
        : t(t) {}


    Token& cur() {

        return t[p];
    }


    bool is(const string& s) {

        return
            (
                cur().type == OP ||
                cur().type == ID
            ) &&
            cur().text == s;
    }


    bool accept(const string& s) {

        if (is(s)) {

            p++;

            return true;
        }

        return false;
    }


    void expect(const string& s) {

        if (!accept(s)) {

            throw runtime_error(
                "'" +
                s +
                "' gozlenilirdi, tapildi: '" +
                cur().text +
                "'"
            );
        }
    }


    // --------------------------------------------------------
    // PRIMARY
    // --------------------------------------------------------

    E primary() {

        Token k =
            cur();


        // number

        if (k.type == NUM) {

            p++;

            return make_shared<Lit>(
                Value(k.num)
            );
        }


        // string

        if (k.type == STR) {

            p++;

            return make_shared<Lit>(
                Value(k.text)
            );
        }


        // identifier / function

        if (k.type == ID) {

            p++;

            // function call:
            // uz[a]
            // bol[a, b]

            if (accept("[")) {

                vector<E> args;

                if (!is("]")) {

                    do {

                        args.push_back(
                            expr()
                        );

                    } while (
                        accept(",")
                    );
                }

                expect("]");

                return make_shared<Call>(
                    k.text,
                    args
                );
            }

            return make_shared<Var>(
                k.text
            );
        }


        // parentheses

        if (accept("(")) {

            E e =
                expr();

            expect(")");

            return e;
        }


        // list

        if (accept("#")) {

            expect("[");

            vector<E> items;

            if (!is("]")) {

                do {

                    items.push_back(
                        expr()
                    );

                } while (
                    accept(",")
                );
            }

            expect("]");

            return make_shared<ListLit>(
                items
            );
        }


        // negative

        if (accept("-"))
            return make_shared<Neg>(
                postfix()
            );


        // not

        if (accept("!"))
            return make_shared<Not>(
                postfix()
            );


        throw runtime_error(
            "gozlenilmeyen token: '" +
            cur().text +
            "'"
        );
    }


    // --------------------------------------------------------
    // RANGE BOUND
    // --------------------------------------------------------

    E bound() {

        if (accept("$"))
            return make_shared<Lit>(
                Value(-1.0)
            );

        return arith();
    }


    // --------------------------------------------------------
    // DOT INDEX
    // --------------------------------------------------------

    E dotOperand() {

        if (accept("$"))
            return make_shared<Lit>(
                Value(-1.0)
            );


        Token k =
            cur();


        if (k.type == NUM) {

            p++;

            return make_shared<Lit>(
                Value(k.num)
            );
        }


        if (k.type == ID) {

            p++;

            return make_shared<Var>(
                k.text
            );
        }


        throw runtime_error(
            "'.' sonrasi indeks gozlenilirdi, tapildi: '" +
            k.text +
            "'"
        );
    }


    // --------------------------------------------------------
    // POSTFIX
    // --------------------------------------------------------

    E postfix() {

        E e =
            primary();


        while (accept(".")) {

            if (accept("(")) {

                // a.(i)
                // a.(i..j)
                // a.(..j)

                E lo =
                    is("..")
                        ? nullptr
                        : bound();


                if (accept("..")) {

                    E hi =
                        is(")")
                            ? nullptr
                            : bound();

                    expect(")");

                    e =
                        make_shared<Slice>(
                            e,
                            lo,
                            hi
                        );

                } else {

                    expect(")");

                    if (!lo)
                        throw runtime_error(
                            "bos indeks"
                        );

                    e =
                        make_shared<Index>(
                            e,
                            lo
                        );
                }

            } else {

                e =
                    make_shared<Index>(
                        e,
                        dotOperand()
                    );
            }
        }

        return e;
    }


    // --------------------------------------------------------
    // POWER
    // --------------------------------------------------------

    E pw() {

        E l =
            postfix();

        if (accept("^"))

            return make_shared<Bin>(
                "^",
                l,
                pw()
            );

        return l;
    }


    // --------------------------------------------------------
    // MULTIPLICATION
    // --------------------------------------------------------

    E term() {

        E l =
            pw();

        while (
            is("*") ||
            is("/") ||
            is("%")
        ) {

            string op =
                cur().text;

            p++;

            l =
                make_shared<Bin>(
                    op,
                    l,
                    pw()
                );
        }

        return l;
    }


    // --------------------------------------------------------
    // ADDITION
    // --------------------------------------------------------

    E arith() {

        E l =
            term();

        while (
            is("+") ||
            is("-")
        ) {

            string op =
                cur().text;

            p++;

            l =
                make_shared<Bin>(
                    op,
                    l,
                    term()
                );
        }

        return l;
    }


    // --------------------------------------------------------
    // RANGE
    // --------------------------------------------------------

    E rng() {

        E l =
            arith();

        if (accept(".."))

            return make_shared<Range>(
                l,
                arith()
            );

        return l;
    }


    // --------------------------------------------------------
    // CONCATENATION
    // --------------------------------------------------------

    E cat() {

        E l =
            rng();

        while (is("~")) {

            p++;

            l =
                make_shared<Bin>(
                    "~",
                    l,
                    rng()
                );
        }

        return l;
    }


    // --------------------------------------------------------
    // COMPARISON
    // --------------------------------------------------------

    E cmp() {

        E l =
            cat();

        while (
            is("<") ||
            is(">") ||
            is("<=") ||
            is(">=") ||
            is("=") ||
            is("<>")
        ) {

            string op =
                cur().text;

            p++;

            l =
                make_shared<Bin>(
                    op,
                    l,
                    cat()
                );
        }

        return l;
    }


    // --------------------------------------------------------
    // LOGIC
    // --------------------------------------------------------

    E expr() {

        E l =
            cmp();

        while (
            is("&") ||
            is("|")
        ) {

            string op =
                cur().text;

            p++;

            l =
                make_shared<Bin>(
                    op,
                    l,
                    cmp()
                );
        }

        return l;
    }


    // --------------------------------------------------------
    // BLOCK
    // --------------------------------------------------------

    S block() {

        expect("{");

        auto b =
            make_shared<Block>();

        while (!is("}")) {

            if (
                cur().type == END
            ) {

                throw runtime_error(
                    "'}' gozlenilirdi, fayl bitdi"
                );
            }

            b->list.push_back(
                stmt()
            );
        }

        expect("}");

        return b;
    }


    // --------------------------------------------------------
    // STATEMENT
    // --------------------------------------------------------

    S stmt() {

        // ----------------------------------------------------
        // PRINT
        // ----------------------------------------------------

        if (accept(">")) {

            E e =
                expr();

            expect(";");

            return make_shared<Print>(
                e
            );
        }


        // ----------------------------------------------------
        // IF
        // ----------------------------------------------------

        if (accept("?")) {

            auto s =
                make_shared<If>();

            s->c =
                expr();

            s->yes =
                block();

            if (accept(":")) {

                s->no =
                    is("?")
                        ? stmt()
                        : block();
            }

            return s;
        }


        // ----------------------------------------------------
        // LOOP
        // ----------------------------------------------------

        if (accept("@")) {

            // @ x : siyahi { }

            if (
                cur().type == ID &&
                p + 1 < t.size() &&
                t[p + 1].type == OP &&
                t[p + 1].text == ":"
            ) {

                auto s =
                    make_shared<ForEach>();

                s->n =
                    cur().text;

                p += 2;

                s->it =
                    expr();

                s->body =
                    block();

                return s;
            }


            // @ sert { }

            auto s =
                make_shared<While>();

            s->c =
                expr();

            s->body =
                block();

            return s;
        }


        // ----------------------------------------------------
        // DEYER DECLARATION
        //
        // deyer -> ad;
        //
        // Bu keyword-dur.
        // Deyisen yaradilir ve ilkin qiymeti 0 olur.
        // ----------------------------------------------------

        if (accept("deyer")) {

            expect("->");


            if (cur().type != ID) {

                throw runtime_error(
                    "'deyer ->' sonrasi "
                    "deyisen adi gozlenilirdi"
                );
            }


            string name =
                cur().text;

            p++;


            expect(";");


            // Deyiseni yarat.

            vars[name] =
                Value();


            // Heqiqi output yoxdur.
            // Sadəcə declaration statement.
            return make_shared<ExprStmt>(
                make_shared<Lit>(
                    Value()
                )
            );
        }


        // ----------------------------------------------------
        // NORMAL ASSIGNMENT
        //
        // 5 -> a;
        // 'Xorma' -> ad;
        // 5 -> a.0;
        // ----------------------------------------------------

        E e =
            expr();


        if (accept("->")) {

            E target =
                postfix();

            expect(";");


            // variable assignment

            if (
                auto v =
                    dynamic_pointer_cast<Var>(
                        target
                    )
            ) {

                return make_shared<Assign>(
                    v->n,
                    e
                );
            }


            // list index assignment

            if (
                auto ix =
                    dynamic_pointer_cast<Index>(
                        target
                    )
            ) {

                return make_shared<IndexAssign>(
                    ix->base,
                    ix->idx,
                    e
                );
            }


            throw runtime_error(
                "-> sonrasi deyisen ve ya "
                "a.i gozlenilirdi"
            );
        }


        // '=' is comparison, not assignment

        if (
            auto b =
                dynamic_pointer_cast<Bin>(e)
        ) {

            if (b->op == "=") {

                throw runtime_error(
                    "'=' beraberlik yoxlayir. "
                    "Menimsetme ucun: "
                    "deyer -> ad; ve ya "
                    "5 -> ad;"
                );
            }
        }


        expect(";");


        return make_shared<ExprStmt>(
            e
        );
    }


    // --------------------------------------------------------
    // PROGRAM
    // --------------------------------------------------------

    S program() {

        auto b =
            make_shared<Block>();

        while (
            cur().type != END
        ) {

            b->list.push_back(
                stmt()
            );
        }

        return b;
    }
};


// ============================================================
// MAIN
// ============================================================

int main(
    int argc,
    char** argv
) {

    if (argc < 2) {

        cerr <<
            "Istifade: xr fayl.xr\n";

        return 1;
    }


    ifstream f(
        argv[1]
    );


    if (!f) {

        cerr <<
            "Fayl acilmadi\n";

        return 1;
    }


    stringstream ss;

    ss << f.rdbuf();


    initBuiltins();


    try {

        Parser parser(
            lex(
                ss.str()
            )
        );

        parser
            .program()
            ->run();

    } catch (
        exception& e
    ) {

        cerr <<
            "Xeta: " <<
            e.what() <<
            "\n";

        return 1;
    }


    return 0;
}