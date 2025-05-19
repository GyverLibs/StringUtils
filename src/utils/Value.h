#pragma once
#include <Arduino.h>

#include "./convert/convert.h"
#include "Text.h"

// автоматическое преобразование любых численных данных к строке Text

#ifndef SU_ANYVALUE_BUF_LEN
#define SU_ANYVALUE_BUF_LEN 21
#endif

namespace su {

class Value : public Text {
   public:
    using Text::Text;

    Value() {
        _init();
    }

    Value(const Text& value) : Text(value) {}

    Value(bool value) : Value() {
        buf[0] = value ? '1' : '0';
        buf[1] = 0;
        _len = 1;
    }

    Value(char value) : Value() {
        buf[0] = value;
        buf[1] = 0;
        _len = 1;
    }
    Value(signed char value, uint8_t base = DEC) : Value() {
        _len = intToStr(value, buf, base);
    }
    Value(unsigned char value, uint8_t base = DEC) : Value() {
        _len = uintToStr(value, buf, base);
    }

    Value(short value, uint8_t base = DEC) : Value() {
        _len = intToStr(value, buf, base);
    }
    Value(unsigned short value, uint8_t base = DEC) : Value() {
        _len = uintToStr(value, buf, base);
    }

    Value(int value, uint8_t base = DEC) : Value() {
        _len = intToStr(value, buf, base);
    }
    Value(unsigned int value, uint8_t base = DEC) : Value() {
        _len = uintToStr(value, buf, base);
    }

    Value(long value, uint8_t base = DEC) : Value() {
        _len = intToStr(value, buf, base);
    }
    Value(unsigned long value, uint8_t base = DEC) : Value() {
        _len = uintToStr(value, buf, base);
    }

    Value(long long value, uint8_t base = DEC) : Value() {
        _len = int64ToStr(value, buf, base);
    }
    Value(unsigned long long value, uint8_t base = DEC) : Value() {
        _len = uint64ToStr(value, buf, base);
    }

    Value(double value, uint8_t dec = 2) : Value() {
        _len = floatToStr(value, buf, dec);
    }

    // assign
    Value& operator=(const Text& v) {
        _len = v._len;
        _type = v._type;
        _str = v._str;
        return *this;
    }

    Value& operator=(bool value) {
        buf[0] = value ? '1' : '0';
        buf[1] = 0;
        _len = 1;
        _init();
        return *this;
    }

    Value& operator=(char value) {
        buf[0] = value;
        buf[1] = 0;
        _len = 1;
        _init();
        return *this;
    }
    Value& operator=(signed char value) {
        _len = intToStr(value, buf, DEC);
        _init();
        return *this;
    }
    Value& operator=(unsigned char value) {
        _len = uintToStr(value, buf, DEC);
        _init();
        return *this;
    }

    Value& operator=(short value) {
        _len = intToStr(value, buf, DEC);
        _init();
        return *this;
    }
    Value& operator=(unsigned short value) {
        _len = uintToStr(value, buf, DEC);
        _init();
        return *this;
    }

    Value& operator=(int value) {
        _len = intToStr(value, buf, DEC);
        _init();
        return *this;
    }
    Value& operator=(unsigned int value) {
        _len = uintToStr(value, buf, DEC);
        _init();
        return *this;
    }

    Value& operator=(long value) {
        _len = intToStr(value, buf, DEC);
        _init();
        return *this;
    }
    Value& operator=(unsigned long value) {
        _len = uintToStr(value, buf, DEC);
        _init();
        return *this;
    }

    Value& operator=(long long value) {
        _len = int64ToStr(value, buf, DEC);
        _init();
        return *this;
    }
    Value& operator=(unsigned long long value) {
        _len = uint64ToStr(value, buf, DEC);
        _init();
        return *this;
    }

    Value& operator=(double value) {
        _len = floatToStr(value, buf, 2);
        _init();
        return *this;
    }

    // copy
    Value(const Value& v) : Value() {
        _copy(v);
    }
    Value(Value&& v) : Value() {
        _copy(v);
    }
    Value& operator=(const Value& v) {
        _copy(v);
        return *this;
    }
    Value& operator=(Value&& v) {
        _copy(v);
        return *this;
    }

   protected:
    char buf[SU_ANYVALUE_BUF_LEN] = {0};

   private:
    void _copy(const Value& v) {
        if (v._type == Type::value) {
            memcpy(buf, v.buf, v._len);
            _init();
        } else {
            _type = v._type;
            _str = v._str;
        }
        _len = v._len;
    }
    void _init() {
        _str = buf;
        _type = Type::value;
    }
};

// // размер буфера общий для всех экземпляров и задаётся дефайном SU_ANYVALUE_BUF_LEN
// class Value : public Value<SU_ANYVALUE_BUF_LEN> {
//    public:
//     using Value<SU_ANYVALUE_BUF_LEN>::Value;
// };

}  // namespace su