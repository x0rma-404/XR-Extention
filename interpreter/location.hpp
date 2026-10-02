#pragma once
#include <cstddef>
#include <string>

// Stores source line/column for error reporting.
struct SourceLocation {
    size_t line   = 1;
    size_t column = 1;

    SourceLocation() = default;
    SourceLocation(size_t l, size_t c) : line(l), column(c) {}
};
