#include "convert.h"

namespace su {

uint8_t _swapBuf(char* p, char* buf);

// быстрое возведение 10 в степень
uint32_t getPow10(const uint8_t value) {
    switch (value) {
        case 0:
            return 1;
        case 1:
            return 10;
        case 2:
            return 100;
        case 3:
            return 1000;
        case 4:
            return 10000;
        case 5:
            return 100000;
        case 6:
            return 1000000;
        case 7:
            return 10000000;
        case 8:
            return 100000000;
        case 9:
            return 1000000000;
    }
    return 0;
}

// быстрый целочисленный логарифм 10 (длина числа в кол-ве символов)
uint8_t getLog10(const uint32_t value) {
    switch (value) {
        case 0 ... 9:
            return 1;
        case 10 ... 99:
            return 2;
        case 100 ... 999:
            return 3;
        case 1000 ... 9999:
            return 4;
        case 10000 ... 99999:
            return 5;
        case 100000 ... 999999:
            return 6;
        case 1000000 ... 9999999:
            return 7;
        case 10000000 ... 99999999:
            return 8;
        case 100000000 ... 999999999:
            return 9;
        case 1000000000 ... UINT32_MAX:
            return 10;
    }
    return 1;
}

uint8_t getLog10(const int32_t value) {
    return getLog10((value < 0) ? (uint32_t)(-value) : (uint32_t)value);
}

/**
 * @brief Длина строки с русскими символами
 *
 * @param str строка
 * @return uint16_t длина
 */
uint16_t strlenRu(const char* str) {
    uint16_t count = 0;
    while (*str) {
        if ((*str & 0xc0) != 0x80) count++;
        ++str;
    }
    return count;
}

/**
 * @brief Получить длину целого числа (с учётом знака -)
 *
 * @param val
 * @return uint8_t
 */
uint8_t intLen(const int32_t val) {
    return getLog10(val) + (val < 0 ? 1 : 0);
}

/**
 * @brief Получить длину float числа (с учётом знака -)
 *
 * @param val
 * @param dec
 * @return uint8_t
 */
uint8_t floatLen(float val, const uint8_t dec) {
    return intLen((int32_t)val) + (dec ? (dec + 1) : 0);
}
uint8_t floatLenNanInf(float val, const uint8_t dec) {
    return (isnan(val) || isinf(val)) ? 3 : floatLen(val, dec);
}

/**
 * @brief Преобразовать float в строку с указанием кол-ва знаков после точки
 *
 * @param val
 * @param buf
 * @param dec кол-во знаков после точки
 * @return uint8_t длина полученной строки
 */
uint8_t floatToStr(float val, char* buf, const uint8_t dec) {
    dtostrf(val, dec ? dec + 2 : 1, dec, buf);
    return floatLen(val, dec);
}

uint8_t floatToStrFast(float val, char* buf, uint8_t dec) {
    char* p = buf;
    if (val < 0) {
        *p++ = '-';
        val = -val;
    }
    p += uintToStr(val, p);
    if (dec) {
        *p++ = '.';
        p += dec;
        char* pp = p;
        uint32_t f = (val - uint32_t(val)) * getPow10(dec);
        fdiv10 div;
        while (dec--) {
            f = div.div10(f);
            *(--pp) = div.rem + '0';
        }
    }
    *p = 0;
    return p - buf;
}

/**
 * @brief Преобразовать HEX строку в целое число
 *
 * @param str строка
 * @param len длина числа в строке (не указывать, если строка заканчивается '\0')
 * @return uint32_t
 */
uint32_t strToIntHex(const char* str, int8_t len) {
    if (!str || !*str) return 0;
    uint32_t v = 0;
    if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) str += 2;
    while (*str && len) {
        switch (*str) {
            case '0' ... '9':
            case 'a' ... 'f':
            case 'A' ... 'F':
                break;
            default:
                return v;
        }
        v <<= 4;
        v += (*str & 0xf) + (*str > '9' ? 9 : 0);
        ++str;
        if (len > 0) --len;
    }
    return v;
}

/**
 * @brief Длина символа в количестве байт
 *
 * @param sym символ utf-8
 * @return uint8_t 0 если некорректный символ или продолжение предыдущего
 */
uint8_t charSize(const unsigned char sym) {
    if ((sym & 0x80) == 0x00) return 1;       // ASCII (0xxxxxxx)
    else if ((sym & 0xE0) == 0xC0) return 2;  // 110xxxxx
    else if ((sym & 0xF0) == 0xE0) return 3;  // 1110xxxx
    else if ((sym & 0xF8) == 0xF0) return 4;  // 11110xxx
    return 0;
}

/**
 * @brief Быстрая конвертация числа в char* массив (в 3-8 раз быстрее ltoa)
 *
 * @param num число
 * @param buf char массив
 * @param base основание (DEC, HEX, OCT, BIN)
 * @return uint8_t длина числа
 */
uint8_t uintToStr(uint32_t n, char* buf, const uint8_t base) {
    if (n < 10 && base == 10) {
        *buf++ = n + '0';
        *buf = 0;
        return 1;
    }

    char* p = buf;
    if (base == 10) {
        fdiv10 div;
        do {
            n = div.div10(n);
            *p++ = div.rem + '0';
        } while (n);
    } else {
        do {
            uint8_t c = n & (base - 1);
            n >>= (base == 16) ? 4 : 1;
            *p++ = (c < 10) ? (c + '0') : (c + 'a' - 10);
        } while (n);
    }
    *p = 0;
    return _swapBuf(p, buf);
}

/**
 * @brief Быстрая конвертация числа в char* массив (в 3-8 раз быстрее ltoa)
 *
 * @param num число
 * @param buf char массив
 * @param base основание (DEC, HEX, OCT, BIN)
 * @return uint8_t длина числа
 */
uint8_t intToStr(int32_t n, char* buf, const uint8_t base) {
    if (n < 0 && base == 10) *buf++ = '-';
    return uintToStr((n < 0 && base == 10) ? -n : n, buf, base) + (n < 0);
}

/**
 * @brief Конвертация числа в char* массив
 *
 * @param num число
 * @param buf char массив
 * @param base основание (DEC, HEX, OCT, BIN)
 * @return uint8_t длина числа
 */
uint8_t uint64ToStr(uint64_t n, char* buf, const uint8_t base) {
    if (n <= UINT32_MAX) return uintToStr(n, buf, base);

    char* p = buf;
    if (base == 10) {
        do {
            uint8_t mod = n % base;
            n /= base;
            *p++ = mod + '0';
        } while (n);
    } else {
        do {
            uint8_t c = n & (base - 1);
            n >>= (base == 16) ? 4 : 1;
            *p++ = (c < 10) ? (c + '0') : (c + 'a' - 10);
        } while (n);
    }
    *p = 0;
    return _swapBuf(p, buf);
}

/**
 * @brief Конвертация числа в char* массив
 *
 * @param num число
 * @param buf char массив
 * @param base основание (DEC, HEX, OCT, BIN)
 * @return uint8_t длина числа
 */
uint8_t int64ToStr(int64_t n, char* buf, const uint8_t base) {
    switch (n) {
        case INT32_MIN ...(-1): return intToStr(n, buf, base);
        case 0 ... UINT32_MAX: return uintToStr(n, buf, base);
    }

    if (n < 0 && base == 10) *buf++ = '-';
    return uint64ToStr((n < 0) ? -n : n, buf, base) + (n < 0);
}

// конвертация из строки во float
float strToFloat(const char* s) {
    if (!*s) return 0.0f;

    bool neg = (*s == '-');
    float f = strToInt<int32_t>(s);
    const char* d = strchr(s, '.');

    if (d && *(d + 1)) {
        int32_t frac = strToInt<int32_t>(d + 1);
        uint8_t fracLen = strlen(d + 1);
        float fraction = (float)frac / getPow10(fracLen);
        f += fraction * (neg ? -1.0f : 1.0f);
    }

    return f;
}

// конвертация из строки во float
float strToFloat_P(PGM_P s) {
    bool neg = (pgm_read_byte(s) == '-');
    float f = strToInt_P<int32_t>(s);

#ifdef __AVR__
    const char* d = strchr_P(s, '.');
#else
    const char* d = strchr(s, '.');
#endif

    if (d) {
        uint8_t fracLen = strlen_P(d + 1);
        int32_t frac = strToInt_P<int32_t>(d + 1);
        f += ((float)frac / getPow10(fracLen)) * (neg ? -1.0f : 1.0f);
    }

    return f;
}

uint8_t _swapBuf(char* p, char* buf) {
    uint8_t len = p - buf;
    --p;
    char b;
    while (p > buf) {
        b = *buf;
        *buf = *p;
        *p = b;
        ++buf;
        --p;
    }
    return len;
}

}  // namespace su