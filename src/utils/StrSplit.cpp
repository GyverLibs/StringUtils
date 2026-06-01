#include "StrSplit.h"

namespace su {

void splitStr(const char* str, char div, void (*cb)(const char* chunk, size_t len)) {
    while (true) {
        const char* p = str;
        while (*p && *p != div) ++p;
        cb(str, p - str);
        if (!*p) break;
        str = p + 1;
    }
}

}