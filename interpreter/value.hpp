#pragma once
#include <string>
#include <vector>
#include <memory>
#include <variant>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <utility>
#include "error.hpp"
#include "location.hpp"

class Value {
public:
    using List = std::shared_ptr<std::vector<Value>>;

private:
    std::variant<double, std::string, List> data;

public:
    Value()                         : data(0.0) {}
    explicit Value(double d)        : data(d) {}
    explicit Value(long long v)     : data((double)v) {}
    explicit Value(bool b)          : data(b ? 1.0 : 0.0) {}
    Value(std::string s)            : data(std::move(s)) {}
    Value(const char* s)            : data(std::string(s)) {}
    Value(List l)                   : data(std::move(l)) {}

    bool isNum()  const { return std::holds_alternative<double>(data); }
    bool isStr()  const { return std::holds_alternative<std::string>(data); }
    bool isList() const { return std::holds_alternative<List>(data); }

    std::string typeName() const {
        if (isNum())  return "eded";
        if (isStr())  return "metn";
        return "siyahi";
    }

    double getNum() const { return std::get<double>(data); }
    const std::string& getStr() const { return std::get<std::string>(data); }
    const List& getList() const { return std::get<List>(data); }

    double asNum(const std::string& ctx) const {
        if (!isNum()) throw RuntimeError(ctx + ": eded gozlenilirdi, " + typeName() + " tapildi");
        return getNum();
    }

    long long asInt(const std::string& ctx) const {
        double d = asNum(ctx);
        if (d != std::floor(d)) throw RuntimeError(ctx + ": tam eded gozlenilirdi");
        return (long long)d;
    }

    const std::string& asStr(const std::string& ctx) const {
        if (!isStr()) throw RuntimeError(ctx + ": metn gozlenilirdi, " + typeName() + " tapildi");
        return getStr();
    }

    const List& asList(const std::string& ctx) const {
        if (!isList()) throw RuntimeError(ctx + ": siyahi gozlenilirdi, " + typeName() + " tapildi");
        return getList();
    }

    bool truthy() const {
        if (isNum())  return getNum() != 0.0;
        if (isStr())  return !getStr().empty();
        return !getList()->empty();
    }

    static std::string fmtNum(double d) {
        if (d == std::floor(d) && std::fabs(d) < 1e15) {
            char buf[32];
            std::snprintf(buf, sizeof buf, "%lld", (long long)d);
            return buf;
        }
        std::ostringstream o;
        o << std::setprecision(10) << d;
        return o.str();
    }

    std::string show(bool quote = false) const {
        if (isNum()) return fmtNum(getNum());
        if (isStr()) return quote ? "'" + getStr() + "'" : getStr();
        const auto& l = *getList();
        std::string r = "#[";
        for (size_t i = 0; i < l.size(); i++) {
            if (i) r += ", ";
            r += l[i].show(true);
        }
        return r + "]";
    }

    bool equals(const Value& o) const {
        if (data.index() != o.data.index()) return false;
        if (isNum())  return getNum() == o.getNum();
        if (isStr())  return getStr() == o.getStr();
        auto& x = *getList(); auto& y = *o.getList();
        if (x.size() != y.size()) return false;
        for (size_t i = 0; i < x.size(); i++) if (!x[i].equals(y[i])) return false;
        return true;
    }

    int cmp(const Value& o) const {
        if (isNum() && o.isNum()) {
            double a = getNum(), b = o.getNum();
            return a < b ? -1 : a > b ? 1 : 0;
        }
        if (isStr() && o.isStr()) {
            int c = getStr().compare(o.getStr());
            return c < 0 ? -1 : c > 0 ? 1 : 0;
        }
        throw RuntimeError("muqayise: " + typeName() + " ile " + o.typeName() + " muqayise olunmur");
    }

    static Value boolean(bool b) { return Value(b ? 1.0 : 0.0); }

    static Value makeList() {
        return Value(std::make_shared<std::vector<Value>>());
    }
};

inline size_t fixIndex(long long i, size_t n, SourceLocation loc = {}) {
    long long orig = i;
    if (i < 0) i += (long long)n;
    if (i < 0 || i >= (long long)n)
        throw RuntimeError("indeks hududdan kenardadir: " + std::to_string(orig), loc);
    return (size_t)i;
}

