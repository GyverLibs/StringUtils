#include "url.h"

namespace su {
namespace url {

static inline char _encodeNibble(uint8_t b) {
    return b + ((b > 9) ? 55 : '0');
}
static inline uint8_t _decodeNibble(char c) {
    return c - ((c <= '9') ? '0' : ((c <= 'F') ? 55 : 87));
}

// ====================

bool needsEncode(char c) {
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
            return false;
    }
    return true;
}

// ====================

size_t encodedLen(const char* str) {
    return encodedLen(str, strlen(str));
}
size_t encodedLen(const char* str, size_t len) {
    size_t elen = 0;
    while (len--) elen += needsEncode(*str++) ? 3 : 1;
    return elen;
}

// ====================

size_t encode(char* url, const char* str, size_t len) {
    char* w = url;
    while (len--) {
        char c = *str++;
        if (needsEncode(c)) {
            *w++ = '%';
            *w++ = _encodeNibble(c >> 4);
            *w++ = _encodeNibble(c & 0xF);
        } else {
            *w++ = c;
        }
    }
    return w - url;
}
size_t encode(char* url, const char* str) {
    return encode(url, str, strlen(str));
}

//

void encode(String& url, const char* str, size_t len) {
    url.reserve(url.length() + len);
    while (len--) {
        char c = *str++;
        if (needsEncode(c)) {
            url += '%';
            url += _encodeNibble(c >> 4);
            url += _encodeNibble(c & 0xF);
        } else {
            url += c;
        }
    }
}
void encode(String& url, const char* str) {
    encode(url, str, strlen(str));
}
void encode(String* url, const char* str) {
    encode(*url, str);
}
void encode(String* url, const char* str, size_t len) {
    encode(*url, str, len);
}

String encode(const char* str, size_t len) {
    String url;
    encode(url, str, len);
    return url;
}
String encode(const char* str) {
    return encode(str, strlen(str));
}

String encode(const String& str) {
    String url;
    encode(url, str.c_str(), str.length());
    return url;
}

// ====================

size_t decodedLen(const char* url) {
    return decodedLen(url, strlen(url));
}
size_t decodedLen(const char* url, size_t len) {
    size_t dlen = 0;
    while (len > 0) {
        if (*url == '%') len -= 3, url += 3;
        else --len, ++url;
        ++dlen;
    }
    return dlen;
}

// ====================

size_t decode(char* str, const char* url, size_t len) {
    char* w = str;
    while (len-- > 0) {
        char c = *url++;
        if (c != '%') {
            *w++ = (c == '+') ? ' ' : c;
        } else {
            if (len < 2) break;
            *w++ = char(_decodeNibble(url[1]) | (_decodeNibble(url[0]) << 4));
            url += 2;
            len -= 2;
        }
    }
    return w - str;
}
size_t decode(char* str, const char* url) {
    return decode(str, url, strlen(url));
}

//

size_t decode(String& str, const char* url, size_t len) {
    if (len < 0) len = strlen(url);
    str.reserve(len);
    while (len-- > 0) {
        char c = *url++;
        if (c != '%') {
            str += (c == '+') ? ' ' : c;
        } else {
            if (len < 2) break;
            str += char(_decodeNibble(url[1]) | (_decodeNibble(url[0]) << 4));
            url += 2;
            len -= 2;
        }
    }
    return str.length();
}
size_t decode(String& str, const char* url) {
    return decode(str, url, strlen(url));
}
size_t decode(String* str, const char* url) {
    return decode(*str, url);
}
size_t decode(String* str, const char* url, size_t len) {
    return decode(*str, url, len);
}

String decode(const char* url, size_t len) {
    String str;
    decode(str, url, len);
    return str;
}
String decode(const char* url) {
    return decode(url, strlen(url));
}
String decode(const String& url) {
    String str;
    decode(str, url.c_str(), url.length());
    return str;
}

size_t decodeSelf(char* url) {
    return decode(url, url, strlen(url));
}
size_t decodeSelf(char* url, size_t len) {
    return decode(url, url, len);
}

/////////////////////////////////////////
size_t decode(const char* url, int16_t len, String& str) {
    return decode(str, url, len);
}
void decode(const String& src, String& dest) {
    decode(src.c_str(), src.length(), dest);
}
void encode(const char* str, uint16_t len, String& url) {
    encode(url, str, len);
}
void encode(const String& str, String& url) {
    encode(str.c_str(), str.length(), url);
}

}  // namespace url
}  // namespace su