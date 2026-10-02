#include "lexer.hpp"
#include "error.hpp"
#include <cctype>

Lexer::Lexer(std::string source) : src(std::move(source)) {}

bool Lexer::atEnd() const { return pos >= src.size(); }

char Lexer::adv() {
    char c = src[pos++];
    if (c == '\n') { line++; col = 1; } else { col++; }
    return c;
}

char Lexer::peek(int off) const {
    size_t i = pos + off;
    return i < src.size() ? src[i] : '\0';
}

bool Lexer::match(char expected) {
    if (atEnd() || src[pos] != expected) return false;
    adv();
    return true;
}

void Lexer::add(TT t, std::string lex, Value v) {
    tokens.emplace_back(t, std::move(lex), std::move(v), SourceLocation(line, startCol));
}

bool Lexer::isIdChar(char c) {
    unsigned char uc = (unsigned char)c;
    return std::isalnum(uc) || c == '_' || uc >= 0x80;
}

void Lexer::scanNumber() {
    size_t start = pos - 1;
    bool afterDot = !tokens.empty() && tokens.back().type == TT::DOT;
    while (std::isdigit((unsigned char)peek())) adv();
    if (!afterDot && peek() == '.' && std::isdigit((unsigned char)peek(1))) {
        adv(); // consume '.'
        while (std::isdigit((unsigned char)peek())) adv();
    }
    std::string t = src.substr(start, pos - start);
    add(TT::NUMBER, t, Value(std::stod(t)));
}

void Lexer::scanString(char q) {
    size_t sloc_col = startCol;
    size_t sloc_line = line;
    std::string val;
    bool closed = false;
    while (!atEnd()) {
        char c = adv();
        if (c == q) { closed = true; break; }
        if (c == '\\' && !atEnd()) {
            char n = adv();
            switch (n) {
                case 'n': val += '\n'; break;
                case 't': val += '\t'; break;
                case 'r': val += '\r'; break;
                case '0': val += '\0'; break;
                default:  val += n;   break;
            }
            continue;
        }
        val += c;
    }
    if (!closed)
        throw LexError("metn baglanmayib (' yoxdur)", SourceLocation(sloc_line, sloc_col));
    add(TT::STRING, val, Value(val));
}

void Lexer::scanIdent(char) {
    size_t start = pos - 1;
    while (isIdChar(peek())) adv();
    std::string text = src.substr(start, pos - start);
    if (text == "deyer") add(TT::DEYER, text);
    else                 add(TT::IDENT, text);
}

void Lexer::blockComment() {
    while (!atEnd()) {
        if (peek() == '*' && peek(1) == '/') { adv(); adv(); return; }
        adv();
    }
}

std::vector<Token> Lexer::tokenize() {
    while (!atEnd()) {
        startCol = col;
        char c = adv();
        switch (c) {
            case ' ': case '\t': case '\r': case '\n': break;
            case '/':
                if      (match('/')) { while (!atEnd() && peek() != '\n') adv(); }
                else if (match('*')) blockComment();
                else                 add(TT::SLASH, "/");
                break;
            case '\'': case '"': scanString(c); break;
            case '(': add(TT::LPAREN,   "("); break;
            case ')': add(TT::RPAREN,   ")"); break;
            case '[': add(TT::LBRACKET, "["); break;
            case ']': add(TT::RBRACKET, "]"); break;
            case '{': add(TT::LBRACE,   "{"); break;
            case '}': add(TT::RBRACE,   "}"); break;
            case ';': add(TT::SEMI,      ";"); break;
            case ',': add(TT::COMMA,     ","); break;
            case '+': add(TT::PLUS,      "+"); break;
            case '*': add(TT::STAR,      "*"); break;
            case '%': add(TT::PERCENT,   "%"); break;
            case '^': add(TT::CARET,     "^"); break;
            case '~': add(TT::TILDE,     "~"); break;
            case '?': add(TT::QUESTION,  "?"); break;
            case ':': add(TT::COLON,     ":"); break;
            case '@': add(TT::AT,        "@"); break;
            case '$': add(TT::DOLLAR,    "$"); break;
            case '#': add(TT::HASH,      "#"); break;
            case '&': add(TT::AND,       "&"); break;
            case '|': add(TT::OR,        "|"); break;
            case '!': add(TT::NOT,       "!"); break;
            case '=': add(TT::EQ,        "="); break;
            case '-': match('>') ? add(TT::ARROW, "->") : add(TT::MINUS, "-"); break;
            case '.': match('.') ? add(TT::RANGE, "..") : add(TT::DOT,   "."); break;
            case '<':
                if      (match('>')) add(TT::NEQ, "<>");
                else if (match('=')) add(TT::LTE, "<=");
                else                 add(TT::LT,  "<");
                break;
            case '>':
                match('=') ? add(TT::GTE, ">=") : add(TT::GT, ">");
                break;
            default:
                if (std::isdigit((unsigned char)c)) { scanNumber(); }
                else if (isIdChar(c))               { scanIdent(c); }
                else                                { add(TT::IDENT, std::string(1, c)); }
                break;
        }
    }
    add(TT::END, "", Value());
    return tokens;
}

