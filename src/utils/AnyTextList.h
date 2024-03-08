#pragma once
#include <Arduino.h>

#include "AnyText.h"

namespace sutil {

template <int16_t cap>
class AnyTextListT {
   public:
    AnyTextListT(const AnyText& list, char div) {
        len = list.split(arr, cap, div);
    }

    // количество построк
    uint16_t length() const {
        return len;
    }

    // размер буфера
    uint16_t capacity() const {
        return cap;
    }

    // получить подстроку под индексом
    const AnyText& get(uint16_t idx) const {
        return arr[idx < len ? idx : 0];
    }

    // получить подстроку под индексом
    const AnyText& operator[](int idx) const {
        return get(idx);
    }

   private:
    AnyText arr[cap];
    uint16_t len;
};

class AnyTextList {
   public:
    AnyTextList(const AnyText& list, char div) {
        len = list.count(div);
        arr = (AnyText*)malloc(len * sizeof(AnyText));
        list.split(arr, len, div);
    }

    ~AnyTextList() {
        if (arr) free(arr);
    }

    // количество построк
    uint16_t length() const {
        return len;
    }

    // получить подстроку под индексом
    const AnyText get(uint16_t idx) const {
        return (idx < len && arr) ? arr[idx] : AnyText();
    }

    // получить подстроку под индексом
    const AnyText operator[](int idx) const {
        return get(idx);
    }

   private:
    AnyText* arr = nullptr;
    uint16_t len = 0;
};

}  // namespace sutil