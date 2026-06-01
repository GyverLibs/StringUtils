#include "b64.h"

// #define SU_B64_COMPACT

namespace su {
namespace b64 {

#ifndef SU_B64_COMPACT
static const char _b64_char[] PROGMEM = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const uint8_t _b64_byte[] PROGMEM = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 62, 0, 0, 0, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0, 0, 0, 0, 0,
    0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51};
#endif

// =======================

char encodeByte(uint8_t n) {
#ifdef SU_B64_COMPACT
    switch (n) {
        case 0 ... 25: return n + 'A';
        case 26 ... 51: return n + 'a' - 26;
        case 52 ... 61: return n + '0' - 52;
        case 62: return '+';
    }
    return '/';
#else
    return pgm_read_byte(_b64_char + n);
#endif
}

int8_t decodeChar(char b) {
#ifdef SU_B64_COMPACT
    switch (b) {
        case 'A' ... 'Z': return b - 'A';
        case 'a' ... 'z': return b - 'a' + 26;
        case '0' ... '9': return b - '0' + 52;
        case '+': return 62;
        case '/': return 63;
    }
    return -1;
#else
    uint8_t c = (uint8_t)b;
    if (c >= sizeof(_b64_byte)) return -1;

    uint8_t v = pgm_read_byte(_b64_byte + c);
    if (v == 0 && b != 'A') return -1;
    return v;
#endif
}

// =======================

size_t encodedLen(size_t len) {
    return ((len + 3 - 1) / 3) * 4;
}

size_t decodedLen(const void* b64) {
    return decodedLen(b64, strlen((const char*)b64));
}
size_t decodedLen(const void* b64, size_t len) {
    if (!len || (len & 3)) return 0;

    const char* s = (const char*)b64;
    uint8_t padd = 0;
    if (s[len - 1] == '=') padd++;
    if (s[len - 2] == '=') padd++;
    return (len / 4) * 3 - padd;
}

// =======================

size_t encode(char* b64, const void* data, size_t len, bool pgm) {
    int8_t valb = -6;
    size_t val = 0;
    char* p = b64;

    for (size_t i = 0; i < len; i++) {
        val = (val << 8) + (pgm ? pgm_read_byte((const uint8_t*)data + i) : ((const uint8_t*)data)[i]);
        valb += 8;
        while (valb >= 0) {
            *p++ = encodeByte((val >> valb) & 0x3F);
            valb -= 6;
        }
    }
    if (valb > -6) *p++ = encodeByte(((val << 8) >> (valb + 8)) & 0x3F);
    while ((p - b64) & 3) *p++ = '=';  // & 3 == % 4
    return p - b64;
}

size_t encode(String* b64, const void* data, size_t len, bool pgm) {
    size_t elen = encodedLen(len);
    if (!elen) return 0;

    size_t oldLen = b64->length();
    if (!b64->reserve(oldLen + elen)) return 0;
    for (size_t i = 0; i < elen; i++) b64->concat(' ');

    char* p = &(*b64)[oldLen];
    return encode(p, data, len, pgm);
}

size_t encode(String& b64, const void* data, size_t len, bool pgm) {
    return encode(&b64, data, len, pgm);
}

// =======================

size_t decode(void* data, const void* b64) {
    return decode(data, b64, strlen((const char*)b64));
}
size_t decode(void* data, const void* b64, size_t len) {
    size_t dlen = decodedLen(b64, len);
    if (!dlen && len) return 0;

    const char* src = (const char*)b64;
    uint8_t* dst = (uint8_t*)data;

    size_t val = 0;
    size_t idx = 0;
    int8_t valb = -8;

    for (size_t i = 0; i < len; i++) {
        char c = src[i];

        if (c == '=') break;

        int8_t decoded = decodeChar(c);
        if (decoded < 0) return 0;

        val = (val << 6) | decoded;
        valb += 6;

        if (valb >= 0) {
            if (idx >= dlen) return 0;

            dst[idx++] = val >> valb;
            valb -= 8;
        }
    }

    return idx == dlen ? idx : 0;
}

size_t decode(void* data, const String& b64) {
    return decode(data, b64.c_str(), b64.length());
}

size_t decode(void* b64) {
    return decode(b64, b64, strlen((char*)b64));
}
size_t decode(void* b64, size_t len) {
    return decode(b64, b64, len);
}

}  // namespace b64
}  // namespace su