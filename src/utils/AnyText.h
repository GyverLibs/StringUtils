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
    AnyText(const __FlashStringHelper* str, int16_t len = 0) : _str((PGM_P)str), _len(len), _type(Type::pgmChar) {}
    AnyText(const char* str, bool pgm = 0, int16_t len = 0) : _str(str), _len(len), _type(pgm ? Type::pgmChar : Type::constChar) {}

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
    uint16_t length() {
        if (!valid()) return 0;
        if (!_len) _len = calcLen(true);
        return _len;
    }

    // Длина строки const
    uint16_t calcLen(bool force = false) const {
        if (!valid()) return 0;
        if (_len && !force) return _len;
        return pgm() ? strlen_P(_str) : strlen(str());
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
        uint16_t len = _len ? _len : calcLen();
        if (!valid() || !len) return 0;
        for (uint16_t i = 0; i < len; i++) p.write(_charAt(i));
        return len;
    }

    // ========================== SEARCH ==========================

    // Сравнить со строкой
    bool operator==(const AnyText& s) {
        return compare(s);
    }
    bool operator==(const char* s) {
        return compare(s);
    }
    bool operator==(const __FlashStringHelper* s) {
        return compare(s);
    }
    bool operator==(const String& s) {
        return compare(s);
    }
    bool operator==(String& s) {
        return compare(s.c_str());
    }

    /**
       @brief Сравнить со строкой

       @param s
       @param from с какого индекса начинать сравнение
       @return true строки совпадают
       @return false строки не совпадают
    */
    bool compare(const AnyText& s, uint16_t from = 0) {
        return compareN(s, length(), from);
    }

    /**
       @brief Сравнить со строкой с указанием количества символов

       @param s строка любого типа
       @param amount количество символов
       @param from индекс, откуда начинать сравнение
       @return true строки совпадают
       @return false строки не совпадают
    */
    bool compareN(const AnyText& s, uint16_t amount, uint16_t from = 0) {
        if (!valid() || !s.valid() || !amount || amount + from > length()) return 0;
        uint16_t i = 0;
        while (i != amount) {
            char c1 = _charAt(from + i);
            char c2 = s._charAt(i);
            if (!c1 || c1 != c2) return 0;  // c1 == c2 == 0
            i++;
        }
        return 1;
    }

    /**
       @brief Найти позицию символа в строке

       @param sym символ
       @param from индекс начала поиска
       @return int16_t позиция символа, -1 если не найден
    */
    int16_t indexOf(char sym, uint16_t from = 0) {
        if (!valid() || from > length()) return -1;
        const char* p = str() + from;
        if (pgm()) {
#if (defined(ESP8266) || defined(ESP32))
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
    char charAt(uint16_t idx) {
        if (!valid() || idx >= length()) return 0;
        else return _charAt(idx);
    }

    // Получить символ по индексу
    char operator[](int idx) {
        return charAt(idx);
    }

    // ========================== EXPORT ==========================

    // Добавить к String строке. Вернёт false при неудаче
    bool addString(String& s, bool decodeUnicode = false) {
        if (!valid() || !length()) return 0;
        if (decodeUnicode) {
            if (pgm()) return 0;
            s += unicode::decode(str(), length());
        } else {
            if (!_charAt(length())) {  // null
                if (pgm()) s += (const __FlashStringHelper*)_str;
                else s += str();
            } else {
                if (!s.reserve(s.length() + length())) return 0;
                for (uint16_t i = 0; i < length(); i++) s += _charAt(i);
            }
        }
        return 1;
    }

    // Вывести в String строку. Вернёт false при неудаче
    bool toString(String& s, bool decodeUnicode = false) {
        s = "";
        return addString(s, decodeUnicode);
    }

    // Получить как String строку
    String toString(bool decodeUnicode = false) {
        if (!valid() || !length()) return String();
        String s;
        toString(s, decodeUnicode);
        return s;
    }

    // Вывести в char массив. Вернёт длину строки. terminate - завершить строку нулём
    uint16_t toStr(char* buf, int16_t bufsize = -1, bool terminate = true) const {
        if (!valid() || bufsize == 0) return 0;
        if (_len) {
            if (bufsize > 0 && (int16_t)(_len + 1) >= bufsize) return 0;
            pgm() ? strncpy_P(buf, _str, _len) : strncpy(buf, str(), _len);
            if (terminate) buf[_len] = 0;
            return _len;
        } else {
            int16_t i = 0;
            while (1) {
                char c = _charAt(i);
                if (!c) {
                    if (terminate) buf[i] = 0;
                    return i;
                }
                buf[i] = c;
                i++;
                if (i == bufsize) return 0;
            }
        }
    }

    // получить значение как bool
    bool toBool() {
        return valid() && (charAt(0) == 't' || charAt(0) == '1');
    }

    // получить значение как int 16
    int16_t toInt16() {
        if (!valid()) return 0;
        return pgm() ? strToInt_P<int16_t>(str(), length()) : strToInt<int16_t>(str(), length());
    }

    // получить значение как int 32
    int32_t toInt32() {
        if (!valid()) return 0;
        if (length() < 5) return toInt16();
        return pgm() ? strToInt_P<int32_t>(str(), length()) : strToInt<int32_t>(str(), length());
    }

    // получить значение как int64
    int64_t toInt64() {
        if (!valid()) return 0;
        if (length() < 10) return toInt32();
        return pgm() ? strToInt_P<int64_t>(str(), length()) : strToInt<int64_t>(str(), length());
    }

    // получить значение как float
    float toFloat() {
        if (!valid()) return 0;
        // if (pgm()) return strToFloat_P(str());
        // else return strToFloat(str());
        if (pgm()) {
            char buf[length() + 1];
            buf[length()] = 0;
            strncpy_P(buf, str(), length());
            return atof(buf);
        }
        return atof(str());
    }

    // хэш строки, размер зависит от платформы (size_t)
    size_t hash() {
        if (!valid()) return 0;
        return pgm() ? sutil::hash_P(_str, length()) : sutil::hash(str(), length());
    }

    // хэш строки 32 бит
    uint32_t hash32() {
        if (!valid()) return 0;
        return pgm() ? sutil::hash32_P(_str, length()) : sutil::hash32(str(), length());
    }

    // ================= IMPLICIT =================
    operator bool() {
        return toBool();
    }

    operator signed char() {
        return (char)toInt16();
    }
    operator unsigned char() {
        return toInt16();
    }

    operator short() {
        return toInt16();
    }
    operator unsigned short() {
        return toInt16();
    }

    operator int() {
        return (sizeof(int) == 2) ? toInt16() : toInt32();
    }
    operator unsigned int() {
        return (sizeof(int) == 2) ? toInt16() : toInt32();
    }

    operator long() {
        return toInt32();
    }
    operator unsigned long() {
        return toInt32();
    }

    operator long long() {
        return toInt64();
    }
    operator unsigned long long() {
        return toInt64();
    }

    operator float() {
        return toFloat();
    }

    operator double() {
        return toFloat();
    }

    operator const char*() {
        return str();
    }

    operator String() {
        return toString();
    }

   protected:
    const char* _str = nullptr;
    uint16_t _len = 0;
    const Type _type = Type::constChar;
#if AT_SAFE_STRING == 1
    String* _sptr = nullptr;
#endif

    char _charAt(uint16_t idx) const {
        return pgm() ? (char)pgm_read_byte(_str + idx) : str()[idx];
    }
};

}  // namespace sutil