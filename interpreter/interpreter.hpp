#pragma once
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include "value.hpp"
#include "location.hpp"
#include "environment.hpp"
#include "ast.hpp"

using BuiltinFn = std::function<Value(std::vector<Value>&, const SourceLocation&)>;

class Interpreter {
    std::shared_ptr<Environment> globalEnv;
    std::shared_ptr<Environment> env;
    std::map<std::string, BuiltinFn> builtins;
    std::ostream& out;
    std::ostream& err;

    static void arity(const std::vector<Value>& a, size_t lo, size_t hi,
                      const std::string& name, SourceLocation loc);
    void initBuiltins();

public:
    Interpreter(std::ostream& o = std::cout, std::ostream& e = std::cerr);

    std::shared_ptr<Environment>& getEnv() { return env; }

    Value callBuiltin(const std::string& name, std::vector<Value>& args, SourceLocation loc);
    bool isBuiltin(const std::string& name) const;

    void exec(const SP& s);
    Value eval(const EP& e);

    void print(const std::string& s);
    void printErr(const std::string& s);

    void run(const std::string& source);
    bool runFile(const std::string& path);
    void repl();
};

