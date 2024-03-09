#pragma once
#include <Arduino.h>

#include "convert/b64.h"
#include "convert/convert.h"
#include "convert/unicode.h"
#include "hash.h"

#ifndef AT_SAFE_STRING
#define AT_SAFE_STRING 0
#endif

namespace sutil {

class AnyText : public Printable {
   public:
    enum class Type : uint8_t {
        constChar,  // const char*
        pgmChar,    // PROGMEM
        StringRef,  // ссылка на строку
        StringDup,  // копия String-строки
        value,      // буфер AnyValue
    };

    // ========================== CONSTRUCTOR ==========================
    AnyText() {}
    AnyText(const __FlashStringHelper* str, int16_t len = -1) : _str((PGM_P)str), _len(len >= 0 ? len : strlen_P((PGM_P)str)), _type(Type::pgmChar) {}
    AnyText(const char* str, int16_t len = -1, bool pgm = 0) : _str(str), _len(len >= 0 ? len : (pgm ? strlen_P(str) : strlen(str ? str : ""))), _type(pgm ? Type::pgmChar : Type::constChar) {}

#if AT_SAFE_STRING == 1
    AnyText(String& str) : _str(str.c_str()), _len(str.length()), _type(Type::StringRef), _sptr(&str) {}
    AnyText(const String& str) : _str(strdup(str.c_str())), _len(str.length()), _type(Type::StringDup) {}
    AnyText(const AnyText& s) : _str(s.valid() ? s.str() : nullptr), _len(s._len), _type(s._type == Type::StringDup ? Type::constChar : s._type), _sptr(s._sptr) {}
    ~AnyText() {
        if (_str && _type == Type::StringDup) free((char*)_str);
    }
#else
    AnyText(const String& str) : _str(str.c_str()), _len(str.length()), _type(Type::constChar) {}
#endif

    // ========================== SYSTEM ==========================
    // Строка из Flash памяти
    bool pgm() const {
        return _type == Type::pgmChar;
    }

    // Длина строки
    uint16_t length() const {
        return valid() ? _len : 0;
    }

    // посчитать и вернуть длину строки (const)
    uint16_t readLen() const {
        return valid() ? (pgm() ? strlen_P(_str) : strlen(str())) : 0;
    }

    // пересчитать и запомнить длину строки (non-const)
    void calcLen() {
        if (valid()) _len = readLen();
    }

    // Тип строки
    Type type() const {
        return _type;
    }

    // Получить указатель на строку. Всегда вернёт указатель, отличный от nullptr!
    const char* str() const {
#if AT_SAFE_STRING == 1
        return (_type == Type::StringRef) ? _sptr->c_str() : (_str ? _str : "");
#else
        return _str ? _str : "";
#endif
    }

    // указатель на конец строки
    const char* end() const {
        return valid() ? str() + _len : "";
    }

    // Статус строки
    bool valid() const {
#if AT_SAFE_STRING == 1
        return (_type == Type::StringRef) || _str;
#else
        return _str;
#endif
    }

    // строка валидна и оканчивается \0
    bool terminated() const {
        return (valid()) ? (_charAt(_len) == 0) : 0;
    }

    // Напечатать в Print
    size_t printTo(Print& p) const {
        if (!valid()) return 0;
        size_t ret = 0;
        if (pgm()) {
            if (terminated()) {
                ret = p.print((const __FlashStringHelper*)_str);
            } else {
                for (uint16_t i = 0; i < _len; i++) ret += p.write(_charAt(i));
            }
        } else {
            ret = p.write(str(), length());
        }
        return ret;
    }

    // ========================== SEARCH ==========================

    // Сравнить со строкой
    bool operator==(const AnyText& s) const {
        return compare(s);
    }
    bool operator!=(const AnyText& s) const {
        return !compare(s);
    }
    bool operator==(const char* s) const {
        return compare(s);
    }
    bool operator!=(const char* s) const {
        return !compare(s);
    }
    bool operator==(const __FlashStringHelper* s) const {
        return compare(s);
    }
    bool operator!=(const __FlashStringHelper* s) const {
        return !compare(s);
    }
    bool operator==(const String& s) const {
        return compare(s.c_str());
    }
    bool operator!=(const String& s) const {
        return !compare(s.c_str());
    }
    bool operator==(String& s) const {
        return compare(s.c_str());
    }
    bool operator!=(String& s) const {
        return !compare(s.c_str());
    }

    /**
       @brief Сравнить со строкой

       @param s
       @return true строки совпадают
       @return false строки не совпадают
    */
    bool compare(const char* s) const {
        return (pgm() ? !strncmp_P(s, _str, _len) : !strncmp(s, str(), _len)) && !s[_len];
    }

    /**
       @brief Сравнить со строкой

       @param s
       @return true строки совпадают
       @return false строки не совпадают
    */
    bool compare(const AnyText& s) const {
        return (s.length() == _len) ? compareN(s, _len) : 0;
    }

    /**
       @brief Сравнить со строкой с указанием количества символов

       @param s строка любого типа
       @param amount количество символов
       @param from индекс, откуда начинать сравнение
       @return true строки совпадают
       @return false строки не совпадают
    */
    bool compareN(const AnyText& s, uint16_t amount, uint16_t from = 0) const {
        if (!valid() || !s.valid() || !amount || amount > s.length() || from + amount > _len) return 0;
        for (uint16_t i = 0; i < amount; i++) {
            if (_charAt(from + i) != s._charAt(i)) return 0;
        }
        return 1;
    }

    /**
       @brief Найти позицию символа в строке

       @param sym символ
       @param from индекс начала поиска
       @return int16_t позиция символа, -1 если не найден
    */
    int16_t indexOf(char sym, uint16_t from = 0) const {
        if (!valid() || from > _len) return -1;
        const char* p = str() + from;
        if (pgm()) {
#if defined(ESP8266) || defined(ESP32)
            while (1) {
                char b = pgm_read_byte(p);
                if (b == sym) break;
                if (!b) return -1;
                p++;
            }
#else
            p = strchr_P(p, sym);
#endif
        } else {
            p = strchr(p, sym);
        }
        return p ? (p - str()) : -1;
    }

    // найти символ и получить указатель на первое вхождение
    const char* find(char sym, uint16_t from = 0) const {
        int16_t idx = indexOf(sym, from);
        return (idx < 0) ? nullptr : (str() + idx);
    }

    // Посчитать количество подстрок, разделённых символом (количество символов +1)
    uint16_t count(char sym, uint16_t from = 0) const {
        if (!valid()) return 0;
        uint16_t sum = 1;
        for (uint16_t i = from; i < _len; i++) {
            if (_charAt(i) == sym) sum++;
        }
        return sum;
    }

    // ========================== EXPORT ==========================

    /**
       @brief Разделить по символу-разделителю

       @param arr внешний массив строк
       @param len размер массива
       @param sym символ
       @return uint16_t количество найденных подстрок
    */
    uint16_t split(AnyText* arr, uint16_t len, char div) const {
        if (!len || !valid() || !length()) return 0;
        uint16_t i = 0;
        int16_t start = 0, end = -1;

        while (1) {
            end = indexOf(div, end + 1);
            if (end < 0 || i + 1 == len) end = length();
            arr[i++] = AnyText(str() + start, end - start, pgm());
            if (i == len || (uint16_t)end == length()) return i;
            start = end + 1;
        }
    }

    // вернёт новую строку с убранными пробельными символами с начала и конца
    AnyText trim() const {
        if (!length()) return AnyText();
        AnyText txt(*this);
        while (txt._len) {
            uint8_t sym = txt._charAt(0);
            if (sym && (sym <= 0x0F || sym == ' ')) {
                txt._str++;
                txt._len--;
            } else break;
        }
        while (txt._len) {
            uint8_t sym = txt._charAt(txt._len - 1);
            if (sym <= 0x0F || sym == ' ') txt._len--;
            else break;
        }
        return txt;
    }

    /**
     * @brief Получить подстроку из списка по индексу
     *
     * @param idx индекс
     * @param div символ-разделитель
     * @return AnyText подстрока
     */
    AnyText getSub(uint16_t idx, char div) const {
        if (!valid() || !length()) return AnyText();
        int16_t start = 0, end = -1;
        while (1) {
            end = indexOf(div, end + 1);
            if (end < 0) end = length();
            if (!idx--) return AnyText(str() + start, end - start, pgm());
            if ((uint16_t)end == length()) break;
            start = end + 1;
        }
        return AnyText();
    }

    // Получить символ по индексу
    char charAt(uint16_t idx) const {
        return (valid()) ? _charAt(idx) : 0;
    }

    // Получить символ по индексу
    char operator[](int idx) const {
        return charAt(idx);
    }

    // выделить подстроку. Отрицательные индексы работают с конца строки
    AnyText substring(int16_t start, int16_t end = 0) const {
        if (!length()) return AnyText();
        if (start < 0) start += _len;
        if (!end) end = _len - 1;
        else if (end < 0) end += _len;
        if (start > (int16_t)_len || end > (int16_t)_len) return AnyText();

        if (end && end < start) {
            int16_t b = end;
            end = start;
            start = b;
        }

        AnyText t(*this);
        t._str += start;
        t._len = end - start + 1;
        return t;
    }

    // Добавить к String строке. Вернёт false при неудаче
    bool addString(String& s) const {
        if (!valid() || !_len) return 0;
        if (!s.reserve(s.length() + _len)) return 0;
        if (pgm()) {
            if (terminated()) {
                s += (const __FlashStringHelper*)_str;
            } else {
                for (uint16_t i = 0; i < _len; i++) s += _charAt(i);
            }
        } else {
#if defined(ESP8266) || defined(ESP32)
            s.concat(str(), _len);
#else
            if (terminated()) {
                s.concat(str());
            } else {
                const char* p = str();
                for (uint16_t i = 0; i < _len; i++) s += p[i];
            }
#endif
        }

        return 1;
    }

    // Добавить к String строке. Вернёт false при неудаче
    bool addString(String& s, bool decodeUnicode) const {
        if (!valid() || !_len) return 0;
        if (decodeUnicode) {
            if (pgm()) {
                char str[_len + 1];
                strncpy_P(str, _str, _len);
                str[_len] = 0;
                s += unicode::decode(str, _len);
            } else {
                s += unicode::decode(str(), _len);
            }
        } else {
            addString(s);
        }
        return 1;
    }

    // Вывести в String строку. Вернёт false при неудаче
    bool toString(String& s, bool decodeUnicode = false) const {
        s = "";
        return addString(s, decodeUnicode);
    }

    // Получить как String строку
    String toString(bool decodeUnicode = false) const {
        if (!valid() || !_len) return String();
        String s;
        toString(s, decodeUnicode);
        return s;
    }

    // Вывести в char массив. Вернёт длину строки. terminate - завершить строку нулём
    uint16_t toStr(char* buf, int16_t bufsize = -1, bool terminate = true) const {
        if (!bufsize) return 0;
        if (!valid() || !_len) {
            if (terminate) buf[_len] = 0;
            return 0;
        }
        if (bufsize > 0 && (int16_t)(_len + terminate) > bufsize) return 0;
        pgm() ? strncpy_P(buf, _str, _len) : strncpy(buf, str(), _len);
        if (terminate) buf[_len] = 0;
        return _len;
    }

    // размер данных (байт), если они b64
    size_t sizeB64() const {
        return b64::decodedLen(str(), length());
    }

    // вывести в переменную из b64
    bool decodeB64(void* var, size_t size) const {
        if (sizeB64() == size) {
            b64::decode((uint8_t*)var, str(), length());
            return 1;
        }
        return 0;
    }

    // получить значение как bool
    bool toBool() const {
        return valid() && (charAt(0) == 't' || charAt(0) == '1');
    }

    // получить значение как int 16
    int16_t toInt16() const {
        if (!valid()) return 0;
        return pgm() ? strToInt_P<int16_t>(_str, _len) : strToInt<int16_t>(str(), _len);
    }

    // получить значение как int 32
    int32_t toInt32() const {
        if (!valid()) return 0;
        if (_len < 5) return toInt16();
        return pgm() ? strToInt_P<int32_t>(_str, _len) : strToInt<int32_t>(str(), _len);
    }

    // получить значение как uint 32 из HEX строки
    uint32_t toInt32HEX() const {
        if (!length()) return 0;
        uint8_t i = 0;
        uint32_t v = 0;
        if (_len > 2 && _charAt(0) == '0' && _charAt(1) == 'x') i += 2;
        for (; i < _len; i++) {
            char sym = _charAt(i);
            v <<= 4;
            v += (sym & 0xf) + (sym > '9' ? 9 : 0);
        }
        return v;
    }

    // получить значение как int64
    int64_t toInt64() const {
        if (!valid()) return 0;
        if (_len < 10) return toInt32();
        return pgm() ? strToInt_P<int64_t>(_str, _len) : strToInt<int64_t>(str(), _len);
    }

    // получить значение как float
    float toFloat() const {
        if (!valid()) return 0;
        // if (pgm()) return strToFloat_P(str());
        // else return strToFloat(str());
        if (pgm()) {
            char buf[_len + 1];
            strncpy_P(buf, _str, _len);
            buf[_len] = 0;
            return atof(buf);
        } else {
            return atof(str());
        }
    }

    // хэш строки, размер зависит от платформы (size_t)
    size_t hash() const {
        return valid() ? (pgm() ? sutil::hash_P(_str, _len) : sutil::hash(str(), _len)) : 0;
    }

    // хэш строки 32 бит
    uint32_t hash32() const {
        return valid() ? (pgm() ? sutil::hash32_P(_str, _len) : sutil::hash32(str(), _len)) : 0;
    }

    // ================= IMPLICIT =================
    // bool
    operator bool() const {
        return toBool();
    }
    bool operator==(const bool& v) const {
        return toBool() == v;
    }
    bool operator!=(const bool& v) const {
        return toBool() != v;
    }

    // signed char
    operator signed char() const {
        return (char)toInt16();
    }
    bool operator==(const signed char& v) const {
        return toInt16() == v;
    }
    bool operator!=(const signed char& v) const {
        return toInt16() != v;
    }

    // unsigned char
    operator unsigned char() const {
        return toInt16();
    }
    bool operator==(const unsigned char& v) const {
        return (unsigned char)toInt16() == v;
    }
    bool operator!=(const unsigned char& v) const {
        return (unsigned char)toInt16() != v;
    }

    // short
    operator short() const {
        return toInt16();
    }
    bool operator==(const short& v) const {
        return toInt16() == v;
    }
    bool operator!=(const short& v) const {
        return toInt16() != v;
    }

    // unsigned short
    operator unsigned short() const {
        return toInt16();
    }
    bool operator==(const unsigned short& v) const {
        return (unsigned short)toInt16() == v;
    }
    bool operator!=(const unsigned short& v) const {
        return (unsigned short)toInt16() != v;
    }

    // int
    operator int() const {
        return (sizeof(int) == 2) ? toInt16() : toInt32();
    }
    bool operator==(const int& v) const {
        return toInt16() == v;
    }
    bool operator!=(const int& v) const {
        return toInt16() != v;
    }

    // unsigned int
    operator unsigned int() const {
        return (sizeof(int) == 2) ? toInt16() : toInt32();
    }
    bool operator==(const unsigned int& v) const {
        return (unsigned int)toInt16() == v;
    }
    bool operator!=(const unsigned int& v) const {
        return (unsigned int)toInt16() != v;
    }

    // long
    operator long() const {
        return toInt32();
    }
    bool operator==(const long& v) const {
        return toInt32() == v;
    }
    bool operator!=(const long& v) const {
        return toInt32() != v;
    }

    // unsigned long
    operator unsigned long() const {
        return toInt32();
    }
    bool operator==(const unsigned long& v) const {
        return (unsigned long)toInt32() == v;
    }
    bool operator!=(const unsigned long& v) const {
        return (unsigned long)toInt32() != v;
    }

    // long long
    operator long long() const {
        return toInt64();
    }
    bool operator==(const long long& v) const {
        return toInt64() == v;
    }
    bool operator!=(const long long& v) const {
        return toInt64() != v;
    }

    // unsigned long long
    operator unsigned long long() const {
        return toInt64();
    }
    bool operator==(const unsigned long long& v) const {
        return (unsigned long long)toInt64() == v;
    }
    bool operator!=(const unsigned long long& v) const {
        return (unsigned long long)toInt64() != v;
    }

    // float
    operator float() const {
        return toFloat();
    }
    bool operator==(const float& v) const {
        return toFloat() == v;
    }
    bool operator!=(const float& v) const {
        return toFloat() != v;
    }

    // double
    operator double() const {
        return toFloat();
    }
    bool operator==(const double& v) const {
        return toFloat() == v;
    }
    bool operator!=(const double& v) const {
        return toFloat() != v;
    }

    // operator const char*() const {
    //     return str();
    // }

    operator String() const {
        return toString();
    }

    const char* _str = nullptr;
    uint16_t _len = 0;
    Type _type = Type::constChar;

   protected:
#if AT_SAFE_STRING == 1
    String* _sptr = nullptr;
#endif

    char _charAt(uint16_t idx) const {
        if (idx >= _len) return 0;
        return pgm() ? (char)pgm_read_byte(_str + idx) : str()[idx];
    }
};

}  // namespace sutil