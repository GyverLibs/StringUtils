#include "b64.h"

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
        case 26 ... 51: return n - 26 + 'a';
        case 52 ... 61: return n - 52 + '0';
        case 62: return '+';
    }
    return '/';
#else
    return pgm_read_byte(_b64_char + n);
#endif
}

uint8_t decodeChar(char b) {
#ifdef SU_B64_COMPACT
    switch (b) {
        case 0 ... 42: return 0;
        case 43: return 62;
        case 44 ... 46: return 0;
        case 47: return 63;
        case 48 ... 57: return b + 4;
        case 58 ... 65: return 0;
        case 66 ... 90: return b - 65;
        case 91 ... 96: return 0;
    }
    return b - 71;
#else
    return pgm_read_byte(_b64_byte + b);
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
    if (len < 4) return 0;

    uint8_t padd = 0;
    if (((const char*)b64)[len - 2] == '=') padd = 2;
    else if (((const char*)b64)[len - 1] == '=') padd = 1;
    return ((len + 3) >> 2) * 3 - padd;
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
    b64->reserve(b64->length() + elen);
    char* p = (char*)b64->end();
    while (elen--) b64->concat(' ');
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
    if (!dlen) return 0;

    size_t val = 0, idx = 0;
    int8_t valb = -8;

    for (size_t i = 0; i < len; i++) {
        if (((const char*)b64)[i] == '=') break;
        val = (val << 6) + decodeChar(((const char*)b64)[i]);
        valb += 6;
        if (valb >= 0) {
            ((uint8_t*)data)[idx++] = val >> valb;
            valb -= 8;
        }
    }
    return dlen;
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