#pragma once
#include <Arduino.h>

namespace sutil::url {

// символ должен быть urlencoded
inline bool needsEncode(char c) {
    switch (c) {
        case '0' ... '9':
        case 'a' ... 'z':
        case 'A' ... 'Z':
        case '.':
        case '-':
        case '_':
        case '~':
            // case '!': case '+': case ',':
            // case '*': case ':': case '@':
            return 0;
    }
    return 1;
}

// закодировать в url
inline void encode(const String& src, String& dest) {
    dest.reserve(src.length());
    char c;
    for (uint16_t i = 0; i < src.length(); i++) {
        c = src[i];
        if (needsEncode(c)) {
            dest += '%';
            dest += (char)((c >> 4) + (((c >> 4) > 9) ? 55 : '0'));
            dest += (char)((c & 0xF) + (((c & 0xF) > 9) ? 55 : '0'));
        } else {
            dest += c;
        }
    }
}

// закодировать в url
inline String encode(const String& src) {
    String dest;
    encode(src, dest);
    return dest;
}

inline uint8_t _decodeNibble(char c) {
    return c - ((c <= '9') ? '0' : ((c <= 'F') ? 55 : 87));
}

// раскодировать url
inline void decode(const String& src, String& dest) {
    dest.reserve(src.length());
    for (uint16_t i = 0; i < src.length(); i++) {
        if (src[i] != '%') {
            dest += (src[i] == '+') ? ' ' : src[i];
        } else {
            dest += char(_decodeNibble(src[i + 2]) | (_decodeNibble(src[i + 1]) << 4));
            i += 2;
        }
    }
}

// раскодировать url
inline String decode(const String& src) {
    String dest;
    decode(src, dest);
    return dest;
}

}  // namespace sutil::url
