#pragma once
#include <Arduino.h>

#include "convert/convert.h"
#include "convert/unicode.h"
#include "hash.h"

#ifndef AT_SAFE_STRING
#define AT_SAFE_STRING 1
#endif

namespace sutil {

class AnyText : public Printable {
   private:
    enum class Type : uint8_t {
        constChar,  // const char*
        pgmChar,    // PROGMEM
        StringRef,  // ссылка на строку
        StringDup,  // копия String-строки
    };

   public:
    // ========================== CONSTRUCTOR ==========================
    AnyText() {}
    AnyText(const __FlashStringHelper* str, int16_t len = -1) : _str((PGM_P)str), _len(len >= 0 ? len : strlen_P((PGM_P)str)), _type(Type::pgmChar) {}
    AnyText(const char* str, bool pgm = 0, int16_t len = -1) : _str(str), _len(len >= 0 ? len : (pgm ? strlen_P(str) : strlen(str ? str : ""))), _type(pgm ? Type::pgmChar : Type::constChar) {}

#if AT_SAFE_STRING == 1
    AnyText(String& str) : _str(str.c_str()), _len(str.length()), _type(Type::StringRef), _sptr(&str) {}
    AnyText(const String& str) : _str(strdup(str.c_str())), _len(str.length()), _type(Type::StringDup) {}
    AnyText(const AnyText& s) : _str(s.valid() ? s.str() : nullptr), _len(s._len), _type(s._type == Type::StringDup ? Type::constChar : s._type), _sptr(s._sptr) {}
    ~AnyText() {
        if (_str && _type == Type::StringDup) free((char*)_str);
    }
#else
    AnyText(String& str) : _str(str.c_str()), _len(str.length()), _type(Type::StringRef) {}
    AnyText(const String& str) : _str(str.c_str()), _len(str.length()), _type(Type::StringDup) {}
    AnyText(const AnyText& s) : _str(s._str), _len(s._len), _type(s._type) {}
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

    // пересчитать длину строки
    void calcLen() {
        if (valid()) _len = pgm() ? strlen_P(_str) : strlen(str());
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
    const char* end() {
        return valid() ? str() + _len : nullptr;
    }

    // Статус строки
    bool valid() const {
#if AT_SAFE_STRING == 1
        return (_type == Type::StringRef) || _str;
#else
        return _str;
#endif
    }

    // Напечатать в Print
    size_t printTo(Print& p) const {
        if (!valid()) return 0;
        size_t ret = 0;
        for (uint16_t i = 0; i < _len; i++) {
            ret += p.write(_charAt(i));
        }
        return ret;
    }

    // ========================== SEARCH ==========================

    // Сравнить со строкой
    bool operator==(const AnyText& s) const {
        return compare(s);
    }
    bool operator==(const char* s) const {
        return compare(s);
    }
    bool operator==(const __FlashStringHelper* s) const {
        return compare(s);
    }
    bool operator==(const String& s) const {
        return compare(s);
    }
    bool operator==(String& s) const {
        return compare(s.c_str());
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

    // Получить символ по индексу
    char charAt(uint16_t idx) const {
        return (valid() && idx < _len) ? _charAt(idx) : 0;
    }

    // Получить символ по индексу
    char operator[](int idx) const {
        return charAt(idx);
    }

    // ========================== EXPORT ==========================

    // Добавить к String строке. Вернёт false при неудаче
    bool addString(String& s, bool decode = false) const {
        if (!valid() || !_len) return 0;
        if (decode) {
            if (pgm()) {
                char str[_len + 1];
                strncpy_P(str, _str, _len);
                str[_len] = 0;
                s += unicode::decode(str, _len);
            } else {
                s += unicode::decode(str(), _len);
            }
        } else {
            if (!s.reserve(s.length() + _len)) return 0;
            if (pgm()) {
                if (!_charAt(_len)) {
                    s += (const __FlashStringHelper*)_str;
                } else {
                    for (uint16_t i = 0; i < _len; i++) s += _charAt(i);
                }
            } else {
#if defined(ESP8266) || defined(ESP32)
                s.concat(str(), _len);
#else
                if (!_charAt(_len)) {
                    s.concat(str());
                } else {
                    for (uint16_t i = 0; i < _len; i++) s += str()[i];
                }
#endif
            }
        }
        return 1;
    }

    // Вывести в String строку. Вернёт false при неудаче
    bool toString(String& s, bool decode = false) const {
        s = "";
        return addString(s, decode);
    }

    // Получить как String строку
    String toString(bool decode = false) const {
        if (!valid() || !_len) return String();
        String s;
        toString(s, decode);
        return s;
    }

    // Вывести в char массив. Вернёт длину строки. terminate - завершить строку нулём
    uint16_t toStr(char* buf, int16_t bufsize = -1, bool terminate = true) const {
        if (!valid() || !bufsize || !_len) return 0;
        if (bufsize > 0 && (int16_t)(_len + terminate) > bufsize) return 0;
        pgm() ? strncpy_P(buf, _str, _len) : strncpy(buf, str(), _len);
        if (terminate) buf[_len] = 0;
        return _len;
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
    operator bool() const {
        return toBool();
    }

    operator signed char() const {
        return (char)toInt16();
    }
    operator unsigned char() const {
        return toInt16();
    }

    operator short() const {
        return toInt16();
    }
    operator unsigned short() const {
        return toInt16();
    }

    operator int() const {
        return (sizeof(int) == 2) ? toInt16() : toInt32();
    }
    operator unsigned int() const {
        return (sizeof(int) == 2) ? toInt16() : toInt32();
    }

    operator long() const {
        return toInt32();
    }
    operator unsigned long() const {
        return toInt32();
    }

    operator long long() const {
        return toInt64();
    }
    operator unsigned long long() const {
        return toInt64();
    }

    operator float() const {
        return toFloat();
    }

    operator double() const {
        return toFloat();
    }

    operator const char*() const {
        return str();
    }

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
        return pgm() ? (char)pgm_read_byte(_str + idx) : str()[idx];
    }
};

}  // namespace sutil