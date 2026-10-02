#pragma once
#include <string>
#include <map>
#include <memory>
#include <utility>
#include "value.hpp"
#include "location.hpp"
#include "error.hpp"

class Environment : public std::enable_shared_from_this<Environment> {
    std::shared_ptr<Environment> parent;
    std::map<std::string, Value> vars;

public:
    Environment() : parent(nullptr) {}
    explicit Environment(std::shared_ptr<Environment> p) : parent(std::move(p)) {}

    void define(const std::string& name, Value v = Value()) {
        vars[name] = std::move(v);
    }

    void assign(const std::string& name, Value v) {
        auto it = vars.find(name);
        if (it != vars.end()) { it->second = std::move(v); return; }
        if (parent)           { parent->assign(name, std::move(v)); return; }
        vars[name] = std::move(v); // auto-define at global scope
    }

    Value get(const std::string& name, SourceLocation loc) const {
        auto it = vars.find(name);
        if (it != vars.end()) return it->second;
        if (parent)           return parent->get(name, loc);
        throw RuntimeError("teyin olunmamis deyisen: " + name, loc);
    }
};

