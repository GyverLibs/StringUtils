#include "unicode.h"

namespace su {
namespace unicode {

size_t decodeSelf(char* str) {
    return decodeSelf(str, strlen(str));
}

size_t decodeSelf(char* str, size_t len) {
    char* end = str + (len ? len : strlen(str));
    char *w = str, *r = str;
    uint32_t ub = 0, buf = 0;

    while (r < end) {
        if (*r != '\\') {
            if (r != w) *w = *r;
            ++w;
        } else {
            switch (*(++r)) {
                case 'n': *w++ = '\n'; break;
                case 'r': *w++ = '\r'; break;
                case 't': *w++ = '\t'; break;
                default: *w++ = *r; break;

                case 'u':
                    ub = 0;
                    uint8_t i = 4;
                    while (i--) {
                        if (++r >= end) return w - str;
                        ub <<= 4;
                        ub += (*r & 0xf) + (*r > '9' ? 9 : 0);
                    }

                    switch (ub) {
                        case 0x0000 ... 0x07FF:
                            *w++ = (0b11000000 | ((ub >> 6) & 0b11111));
                            *w++ = (0b10000000 | (ub & 0b111111));
                            break;

                        case 0x0800 ... 0xD7FF:
                            *w++ = (0b11100000 | ((ub >> 12) & 0b1111));
                            *w++ = (0b10000000 | ((ub >> 6) & 0b111111));
                            *w++ = (0b10000000 | (ub & 0b111111));
                            break;

                        case 0xD800 ... 0xDBFF:
                            buf = ub;
                            break;

                        case 0xDC00 ... 0xDFFF:
                            ub = (0x10000 + ((buf - 0xD800) * 0x0400) + (ub - 0xDC00));
                            *w++ = (0b11110000 | ((ub >> 18) & 0b111));
                            *w++ = (0b10000000 | ((ub >> 12) & 0b111111));
                            *w++ = (0b10000000 | ((ub >> 6) & 0b111111));
                            *w++ = (0b10000000 | (ub & 0b111111));
                            break;
                    }
                    break;
            }
        }
        ++r;
    }
    return w - str;
}

String decode(const char* str) {
    String s(str);
    size_t dlen = decodeSelf((char*)s.c_str(), s.length());
    s.remove(dlen);
    return s;
}

String decode(const char* str, size_t len) {
    String s;
    s.reserve(len);
    while (len--) s += *str++;
    size_t dlen = decodeSelf((char*)s.c_str(), s.length());
    s.remove(dlen);
    return s;
}

String decode(const String& str) {
    String s(str);
    size_t dlen = decodeSelf((char*)s.c_str(), s.length());
    s.remove(dlen);
    return s;
}

uint8_t encode(char* str, uint32_t c, bool terminate) {
    switch (c) {
        case 0x00 ... 0x7F:
            str[0] = c;
            if (terminate) str[1] = 0;
            return 1;

        case 0x80 ... 0x7FF:
            str[0] = 0xC0 | ((c >> 6) & 0x1F);
            str[1] = 0x80 | (c & 0x3F);
            if (terminate) str[2] = 0;
            return 2;

        case 0x0800 ... 0xFFFF:
            str[0] = 0xE0 | ((c >> 12) & 0x0F);
            str[1] = 0x80 | ((c >> 6) & 0x3F);
            str[2] = 0x80 | (c & 0x3F);
            if (terminate) str[3] = 0;
            return 3;

        case 0x10000 ... 0x10FFFF:
            str[0] = 0xF0 | ((c >> 18) & 0x07);
            str[1] = 0x80 | ((c >> 12) & 0x3F);
            str[2] = 0x80 | ((c >> 6) & 0x3F);
            str[3] = 0x80 | (c & 0x3F);
            if (terminate) str[4] = 0;
            return 4;
    }
    if (terminate) str[0] = 0;
    return 0;
}

String encode(uint32_t code) {
    char sym[5];
    encode(sym, code);
    return sym;
}

}  // namespace unicode
}  // namespace su