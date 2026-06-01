#pragma once
#include <stddef.h>

namespace su {

// StrSplit
class StrSplit {
   public:
    StrSplit(const char* str) : p(str) {}

    bool split(char div) {
        if (!*p) return false;
        s = s ? ++p : p;
        while (*p && *p != div) ++p;
        return true;
    }
    size_t len() const {
        return p - s;
    }
    const char* str() const {
        return s;
    }

   private:
    const char* p;
    const char* s = nullptr;
};
/*
while (sp.split(';')) {
    Serial.write(sp.str(), sp.len());
    Serial.println();
}
*/

// StrSplit2
struct StrSplit2 {
    StrSplit2(const char* str) : str(str) {}

    bool split(char div) {
        if (len >= 0) str += len;
        if (!*str) return false;

        if (len >= 0) ++str;
        const char* p = str;
        while (*p && *p != div) ++p;
        len = p - str;
        return true;
    }

    const char* str;
    int len = -1;
};
/*
while (sp.split(';')) {
    Serial.write(sp.str, sp.len);
    Serial.println();
}
*/

// splitStr
void splitStr(const char* str, char div, void (*cb)(const char* chunk, size_t len));
/*
splitStr(topic, ';', [](const char* chunk, size_t len) {
    Serial.write(chunk, len);
    Serial.println();
});
*/

}