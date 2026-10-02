#pragma once
#include <string>
#include <vector>
#include <cctype>

class UTF8 {
public:
    static std::vector<std::string> chars(const std::string& s) {
        std::vector<std::string> r;
        for (size_t i = 0; i < s.size();) {
            unsigned char c = (unsigned char)s[i];
            size_t n = c < 0x80 ? 1 : (c >> 5) == 6 ? 2 : (c >> 4) == 14 ? 3 : (c >> 3) == 30 ? 4 : 1;
            if (i + n > s.size()) n = s.size() - i;
            r.push_back(s.substr(i, n));
            i += n;
        }
        return r;
    }

    static size_t len(const std::string& s) { return chars(s).size(); }

    static std::string toUpper(const std::string& s) {
        std::string r;
        for (auto& ch : chars(s)) {
            if      (ch == "\xc9\x99") r += "\xc6\x8f"; // ə -> Ə
            else if (ch == "\xc4\xb1") r += "I";         // ı -> I
            else if (ch == "i")        r += "\xc4\xb0";  // i -> İ
            else if (ch == "\xc3\xb6") r += "\xc3\x96";  // ö -> Ö
            else if (ch == "\xc3\xbc") r += "\xc3\x9c";  // ü -> Ü
            else if (ch == "\xc5\x9f") r += "\xc5\x9e";  // ş -> Ş
            else if (ch == "\xc3\xa7") r += "\xc3\x87";  // ç -> Ç
            else if (ch == "\xc4\x9f") r += "\xc4\x9e";  // ğ -> Ğ
            else if (ch.size() == 1)   r += (char)std::toupper((unsigned char)ch[0]);
            else                        r += ch;
        }
        return r;
    }

    static std::string toLower(const std::string& s) {
        std::string r;
        for (auto& ch : chars(s)) {
            if      (ch == "\xc6\x8f") r += "\xc9\x99"; // Ə -> ə
            else if (ch == "I")        r += "\xc4\xb1"; // I -> ı
            else if (ch == "\xc4\xb0") r += "i";         // İ -> i
            else if (ch == "\xc3\x96") r += "\xc3\xb6";  // Ö -> ö
            else if (ch == "\xc3\x9c") r += "\xc3\xbc";  // Ü -> ü
            else if (ch == "\xc5\x9e") r += "\xc5\x9f";  // Ş -> ş
            else if (ch == "\xc3\x87") r += "\xc3\xa7";  // Ç -> ç
            else if (ch == "\xc4\x9e") r += "\xc4\x9f";  // Ğ -> ğ
            else if (ch.size() == 1)   r += (char)std::tolower((unsigned char)ch[0]);
            else                        r += ch;
        }
        return r;
    }
};

