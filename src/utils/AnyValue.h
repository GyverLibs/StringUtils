#pragma once
#include <Arduino.h>

#include "AnyText.h"
#include "convert.h"

// автоматическое преобразование любых данных к строке AnyText

#ifndef SU_ANYVALUE_BUF_LEN
#define SU_ANYVALUE_BUF_LEN 22
#endif

namespace sutil {

// с указанием размера буфера
template <uint8_t bufsize = 22>
class AnyValueT : public AnyText {
   public:
    using AnyText::AnyText;

    AnyValueT(const char& value) {
        buf[0] = value;
        buf[1] = 0;
        _len = 1;
        _str = buf;
    }

    AnyValueT(const bool& value) {
        buf[0] = value + '0';
        buf[1] = 0;
        _len = 1;
        _str = buf;
    }

    AnyValueT(const int8_t& value, uint8_t base = DEC) {
        _makeInt(value, base);
    }
    AnyValueT(const uint8_t& value, uint8_t base = DEC) {
        _makeUint(value, base);
    }

    AnyValueT(const int16_t& value, uint8_t base = DEC) {
        _makeInt(value, base);
    }
    AnyValueT(const uint16_t& value, uint8_t base = DEC) {
        _makeUint(value, base);
    }

    AnyValueT(const int32_t& value, uint8_t base = DEC) {
        _makeInt(value, base);
    }
    AnyValueT(const uint32_t& value, uint8_t base = DEC) {
        _makeUint(value, base);
    }

    AnyValueT(const int64_t& value, uint8_t base = DEC) {
        if (value >= 0 && value < UINT32_MAX) _len = uintToStr(value, buf, base);
        else if (value < 0 && value > -INT32_MAX) _len = intToStr(value, buf, base);
        else _len = int64ToStr(value, buf, base);
        _str = buf;
    }
    AnyValueT(const uint64_t& value, uint8_t base = DEC) {
        _len = (value <= UINT32_MAX) ? uintToStr(value, buf, base) : uint64ToStr(value, buf, base);
        _str = buf;
    }

    AnyValueT(const double& value, uint8_t dec = 2) {
        _len = floatToStr(value, buf, dec);
        _str = buf;
    }

   private:
    char buf[bufsize] = {0};

    void _makeInt(int32_t value, uint8_t base) {
        _len = intToStr(value, buf, base);
        _str = buf;
    }
    void _makeUint(uint32_t value, uint8_t base) {
        _len = uintToStr(value, buf, base);
        _str = buf;
    }
};

// размер буфера общий для всех экземпляров и задаётся дефайном SU_ANYVALUE_BUF_LEN
class AnyValue : public AnyValueT<SU_ANYVALUE_BUF_LEN> {
    using AnyValueT<SU_ANYVALUE_BUF_LEN>::AnyValueT;
};

}  // namespace sutil