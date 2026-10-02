#pragma once
#include <string>
#include <stdexcept>
#include <utility>
#include "location.hpp"

class XRError : public std::exception {
protected:
    std::string msg;
    SourceLocation loc;
    mutable std::string full;

public:
    XRError(std::string m, SourceLocation l = {}) : msg(std::move(m)), loc(l) {}

    const char* what() const noexcept override {
        if (full.empty()) {
            if (loc.line > 0 && msg.rfind("teyin olunmamis deyisen:", 0) == std::string::npos)
                full = "[Setir " + std::to_string(loc.line) + "] " + msg;
            else
                full = msg;
        }
        return full.c_str();
    }

    const SourceLocation& where() const { return loc; }
};

struct LexError     : XRError { using XRError::XRError; };
struct ParseError   : XRError { using XRError::XRError; };
struct RuntimeError : XRError { using XRError::XRError; };

