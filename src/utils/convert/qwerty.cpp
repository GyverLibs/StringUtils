#include "qwerty.h"

namespace su {

static const char _qwerty_ru[] PROGMEM = "F<DULT:PBQRKVYJGHCNEA{WXIO}SM\">Zf,dult;pbqrkvyjghcnea[wxio]sm'.z~`";

static char ruUtf8ToQwertyChar(uint8_t first, uint8_t second) {
    int8_t index = -1;

    // Ё
    if (first == 0xD0 && second == 0x81) {
        index = 64;
    }
    // ё
    else if (first == 0xD1 && second == 0x91) {
        index = 65;
    }
    // А..п
    else if (first == 0xD0 && second >= 0x90 && second <= 0xBF) {
        index = second - 0x90;
    }
    // р..я
    else if (first == 0xD1 && second >= 0x80 && second <= 0x8F) {
        index = second - 0x50;  // 0x80 -> 48
    }

    if (index < 0) return 0;

    return (char)pgm_read_byte(&_qwerty_ru[index]);
}

String toQwerty(const String& ru) {
    String qw;
    qw.reserve(ru.length());

    for (size_t i = 0; i < ru.length(); i++) {
        uint8_t cur = (uint8_t)ru[i];

        if (cur < 0x80) {
            qw += (char)cur;
            continue;
        }

        if ((cur == 0xD0 || cur == 0xD1) && i + 1 < ru.length()) {
            char ch = ruUtf8ToQwertyChar(cur, (uint8_t)ru[++i]);
            if (ch) qw += ch;
        }
    }

    return qw;
}

char* toQwerty(const char* ru, char* qw) {
    char* out = qw;

    while (*ru) {
        uint8_t cur = (uint8_t)*ru++;

        if (cur < 0x80) {
            *out++ = (char)cur;
            continue;
        }

        if ((cur == 0xD0 || cur == 0xD1) && *ru) {
            char ch = ruUtf8ToQwertyChar(cur, (uint8_t)*ru++);
            if (ch) *out++ = ch;
        }
    }

    *out = '\0';
    return qw;
}

}  // namespace su