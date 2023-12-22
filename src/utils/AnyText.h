#pragma once
#include <Arduino.h>

#include "convert.h"

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
    // AnyText(const char* str, int16_t _len = -1) : _str(str), _len(_len >= 0 ? _len : strlen(str)) {}
#if AT_SAFE_STRING == 1
    AnyText(String& str) : _str(str.c_str()), _len(str.length()), _type(Type::StringRef), _sptr(&str) {}
    AnyText(const String& str) : _str(strdup(str.c_str())), _len(str.length()), _type(Type::StringDup) {}
#else
    AnyText(String& str) : _str(str.c_str()), _len(str.length()), _type(Type::StringRef) {}
    AnyText(const String& str) : _str(str.c_str()), _len(str.length()), _type(Type::StringDup) {}
#endif
    AnyText(const __FlashStringHelper* str, int16_t _len = -1) : _str((PGM_P)str), _len(_len >= 0 ? _len : strlen_P((PGM_P)str)), _type(Type::pgmChar) {}
    AnyText(const char* str, bool pgm = 0, int16_t _len = -1) : _str(str), _len(_len >= 0 ? _len : (pgm ? strlen_P(str) : strlen(str))), _type(pgm ? Type::pgmChar : Type::constChar) {}

    ~AnyText() {
#if AT_SAFE_STRING == 1
        if (_str && _type == Type::StringDup) free((char*)_str);
#endif
    }

    // ========================== SYSTEM ==========================
    // Строка из Flash памяти
    bool pgm() const {
        return _type == Type::pgmChar;
    }

    // Длина строки
    uint16_t length() const {
        return _len;
    }

    // Тип строки
    Type type() {
        return _type;
    }

    // Получить указатель на строку
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
        if (!valid() || !_len) return 0;
        for (uint16_t i = 0; i < _len; i++) p.write(_charAt(i));
        return _len;
    }

    // ========================== SEARCH ==========================

    // Сравнить со строкой
    bool operator==(const AnyText& s) {
        return compare(s);
    }

    /**
       @brief Сравнить со строкой

       @param s
       @param from с какого индекса начинать сравнение
       @return true строки совпадают
       @return false строки не совпадают
    */
    bool compare(const AnyText& s, uint16_t from = 0) const {
        if (!valid() || !s.valid() || _len != s._len || from > _len) return 0;
        return compareN(s, _len, from);
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
        if (!valid() || !s.str() || !amount || amount > s._len || amount + from > _len) return 0;
        uint16_t i = 0;
        while (i != amount) {
            if (_charAt(from + i) != s._charAt(i)) return 0;
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
    int16_t indexOf(char sym, uint16_t from = 0) const {
        if (!valid() || from > _len) return -1;
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
    char charAt(uint16_t idx) const {
        if (!valid() || idx >= _len) return 0;
        else return _charAt(idx);
    }

    // Получить символ по индексу
    char operator[](uint16_t idx) const {
        return charAt(idx);
    }

    // ========================== EXPORT ==========================

    // Вывести в String строку. Вернёт false при неудаче
    bool toString(String& s) const {
        if (!valid() || !_len) return 0;
        if (!_charAt(_len)) {  // null
            if (pgm()) s += (const __FlashStringHelper*)_str;
            else s += str();
        } else {
            if (!s.reserve(s.length() + _len)) return 0;
            for (uint16_t i = 0; i < _len; i++) s += _charAt(i);
        }
        return 1;
    }

    // Получить как String строку
    String toString() const {
        if (!valid() || !_len) return String();
        String s;
        toString(s);
        return s;
    }

    // Вывести в char массив. Сама добавит '\0' в конце!
    void toStr(char* buf) {
        pgm() ? strncpy_P(buf, str(), _len) : strncpy(buf, str(), _len);
        buf[_len] = 0;
    }

    // получить значение как bool
    bool toBool() {
        return (charAt(0) == 't' || charAt(0) == '1');
    }

    // получить значение как int 16
    int16_t toInt16() {
        if (!valid()) return 0;
        return pgm() ? strToInt_P<int16_t>(str(), _len) : strToInt<int16_t>(str(), _len);
    }

    // получить значение как int 32
    int32_t toInt32() {
        if (!valid()) return 0;
        if (_len < 5) return toInt16();
        return pgm() ? strToInt_P<int32_t>(str(), _len) : strToInt<int32_t>(str(), _len);
    }

    // получить значение как int64
    int64_t toInt64() {
        if (!valid()) return 0;
        if (_len < 10) return toInt32();
        return pgm() ? strToInt_P<int64_t>(str(), _len) : strToInt<int64_t>(str(), _len);
    }

    // получить значение как float
    float toFloat() {
        if (!valid()) return 0;
        // if (pgm()) return strToFloat_P(str());
        // return strToFloat(str());
        if (pgm()) {
            char buf[_len + 1];
            buf[_len] = 0;
            strncpy_P(buf, str(), _len);
            return atof(buf);
        }
        return atof(str());
    }

    // ================= IMPLICIT =================
    operator bool() {
        return toBool();
    }

    operator int8_t() {
        return toInt16();
    }

    operator uint8_t() {
        return toInt16();
    }

    operator int16_t() {
        return toInt16();
    }

    operator uint16_t() {
        return toInt16();
    }

    operator int32_t() {
        return toInt32();
    }

    operator uint32_t() {
        return toInt32();
    }

    operator int64_t() {
        return toInt64();
    }

    operator uint64_t() {
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