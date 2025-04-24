#pragma once
#include <Arduino.h>

#include "./convert/convert.h"
#include "Text.h"

// автоматическое преобразование любых численных данных к строке Text

#ifndef SU_ANYVALUE_BUF_LEN
#define SU_ANYVALUE_BUF_LEN 21
#endif

namespace su {

// с указанием размера буфера
template <uint8_t bufsize = SU_ANYVALUE_BUF_LEN>
class ValueT : public Text {
   public:
    using Text::Text;

    ValueT() {
        _str = buf;
        _type = Type::value;
    }

    ValueT(const Text& value) : Text(value) {}

    ValueT(bool value) : ValueT() {
        buf[0] = value ? '1' : '0';
        buf[1] = 0;
        _len = 1;
    }

    ValueT(char value) : ValueT() {
        buf[0] = value;
        buf[1] = 0;
        _len = 1;
    }
    ValueT(unsigned char value, uint8_t base = DEC) : ValueT() {
        _len = intToStr(value, buf, base);
    }

    ValueT(short value, uint8_t base = DEC) : ValueT() {
        _len = intToStr(value, buf, base);
    }
    ValueT(unsigned short value, uint8_t base = DEC) : ValueT() {
        _len = uintToStr(value, buf, base);
    }

    ValueT(int value, uint8_t base = DEC) : ValueT() {
        _len = intToStr(value, buf, base);
    }
    ValueT(unsigned int value, uint8_t base = DEC) : ValueT() {
        _len = uintToStr(value, buf, base);
    }

    ValueT(long value, uint8_t base = DEC) : ValueT() {
        _len = intToStr(value, buf, base);
    }
    ValueT(unsigned long value, uint8_t base = DEC) : ValueT() {
        _len = uintToStr(value, buf, base);
    }

    ValueT(long long value, uint8_t base = DEC) : ValueT() {
        switch (value) {
            case INT32_MIN ...(-1): _len = intToStr(value, buf, base); break;
            case 0 ... UINT32_MAX: _len = uintToStr(value, buf, base); break;
            default: _len = int64ToStr(value, buf, base); break;
        }
    }
    ValueT(unsigned long long value, uint8_t base = DEC) : ValueT() {
        switch (value) {
            case 0 ... UINT32_MAX: _len = uintToStr(value, buf, base); break;
            default: _len = uint64ToStr(value, buf, base); break;
        }
    }

    ValueT(double value, uint8_t dec = 2) : ValueT() {
        _len = floatToStr(value, buf, dec);
    }

    // assign
    ValueT& operator=(bool value) {
        buf[0] = value ? '1' : '0';
        buf[1] = 0;
        _len = 1;
        return *this;
    }

    ValueT& operator=(char value) {
        buf[0] = value;
        buf[1] = 0;
        _len = 1;
        return *this;
    }
    ValueT& operator=(unsigned char value) {
        _len = intToStr(value, buf, DEC);
        return *this;
    }

    ValueT& operator=(short value) {
        _len = intToStr(value, buf, DEC);
        return *this;
    }
    ValueT& operator=(unsigned short value) {
        _len = uintToStr(value, buf, DEC);
        return *this;
    }

    ValueT& operator=(int value) {
        _len = intToStr(value, buf, DEC);
        return *this;
    }
    ValueT& operator=(unsigned int value) {
        _len = uintToStr(value, buf, DEC);
        return *this;
    }

    ValueT& operator=(long value) {
        _len = intToStr(value, buf, DEC);
        return *this;
    }
    ValueT& operator=(unsigned long value) {
        _len = uintToStr(value, buf, DEC);
        return *this;
    }

    ValueT& operator=(long long value) {
        switch (value) {
            case INT32_MIN ...(-1): _len = intToStr(value, buf, DEC); break;
            case 0 ... UINT32_MAX: _len = uintToStr(value, buf, DEC); break;
            default: _len = int64ToStr(value, buf, DEC); break;
        }
        return *this;
    }
    ValueT& operator=(unsigned long long value) {
        switch (value) {
            case 0 ... UINT32_MAX: _len = uintToStr(value, buf, DEC); break;
            default: _len = uint64ToStr(value, buf, DEC); break;
        }
        return *this;
    }

    ValueT& operator=(double value) {
        _len = floatToStr(value, buf, 2);
        return *this;
    }

    // copy
    ValueT(const ValueT& v) : ValueT() {
        _copy(v);
    }
    ValueT(ValueT&& v) : ValueT() {
        _copy(v);
    }
    ValueT& operator=(const ValueT& v) {
        _copy(v);
        return *this;
    }
    ValueT& operator=(ValueT&& v) {
        _copy(v);
        return *this;
    }

   protected:
    char buf[bufsize] = {0};

   private:
    void _copy(const ValueT& v) {
        memcpy(buf, v.buf, v._len);
        _len = v._len;
    }
};

// размер буфера общий для всех экземпляров и задаётся дефайном SU_ANYVALUE_BUF_LEN
class Value : public ValueT<> {
    using ValueT<SU_ANYVALUE_BUF_LEN>::ValueT;
};

}  // namespace su