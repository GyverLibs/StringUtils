#pragma once
#include <Arduino.h>

#include "AnyText.h"

namespace sutil {
namespace list {

/**
 * @brief Получить количество подстрок в списке
 *
 * @param list строка любого типа
 * @param div символ-разделитель (умолч. ';')
 * @return uint16_t
 */
uint16_t length(AnyText list, char div = ';');

/**
 * @brief Получить индекс подстроки в списке
 *
 * @param list строка любого типа
 * @param str строка любого типа
 * @param div символ-разделитель (умолч. ';')
 * @return int16_t индекс в строке. -1 если не найдена
 */
int16_t indexOf(AnyText list, AnyText str, char div = ';');

/**
 * @brief Проверка содержит ли список подстроку
 *
 * @param list строка любого типа
 * @param str строка любого типа
 * @param div символ-разделитель (умолч. ';')
 * @return true содержит
 * @return false не содержит
 */
bool includes(AnyText list, AnyText str, char div = ';');

/**
 * @brief Получить подстроку из списка по индексу
 *
 * @param list строка любого типа
 * @param idx строка любого типа
 * @param div символ-разделитель (умолч. ';')
 * @return AnyText подстрока
 */
AnyText get(AnyText list, uint16_t idx, char div = ';');

// распарсить в массив указанного типа и размера. Вернёт количество записанных подстрок
template <typename T>
uint16_t parse(AnyText list, T* buf, uint16_t len, char div = ';') {
    if (!list.valid() || !list.length()) return 0;
    uint16_t idx = 0;
    int16_t st = 0, end = -1;
    bool stop = 0;
    while (1) {
        st = ++end;
        end = list.indexOf(div, end);
        if (end < 0) {
            end = list.length();
            stop = 1;
        }
        buf[idx] = AnyText(list.str() + st, list.pgm(), end - st);
        idx++;
        if (stop) return idx;
        if (idx == len) return len;
    }
}

}  // namespace list

class List {
   public:
    List(String& list, char div = ';') : List(list.c_str(), div) {}
    List(const char* list, char div = ';') : _str(list), _div(div) {}

    // установить разделитель
    void setDiv(char div) {
        _div = div;
    }

    // получить размер списка
    uint16_t length() {
        if (!_len) _len = list::length(_str, _div);
        return _len;
    }

    // получить индекс подстроки в списке или -1 если её нет
    int16_t indexOf(AnyText str) const {
        return list::indexOf(_str, str, _div);
    }

    // проверить наличие подстроки в списке
    bool includes(AnyText str) const {
        return list::includes(_str, str, _div);
    }

    // распарсить в массив указанного типа и размера. Вернёт количество записанных подстрок
    template <typename T>
    uint16_t parse(T* buf, uint16_t len) {
        return list::parse<T>(_str, buf, len, _div);
    }

    // получить подстроку под индексом
    AnyText get(uint16_t idx) const {
        return list::get(_str, idx, _div);
    }

    // получить подстроку под индексом
    AnyText operator[](int idx) const {
        return get(idx);
    }

   private:
    const char* _str;
    char _div;
    uint16_t _len = 0;
};

}  // namespace sutil