#include "list.h"

namespace sutil::list {

/**
 * @brief Получить количество подстрок в списке
 *
 * @param list строка любого типа
 * @param div символ-разделитель (умолч. ';')
 * @return uint16_t
 */
uint16_t length(AnyText list, char div) {
    if (!list.valid() || !list.length()) return 0;
    uint16_t am = 0;
    int16_t p = -1;
    while (1) {
        am++;
        p = list.indexOf(div, p + 1);
        if (p < 0) return am;
    }
}

/**
 * @brief Получить индекс подстроки в списке
 *
 * @param list строка любого типа
 * @param str строка любого типа
 * @param div символ-разделитель (умолч. ';')
 * @return int16_t индекс в строке. -1 если не найдена
 */
int16_t indexOf(AnyText list, AnyText str, char div) {
    if (!list.valid() || !str.valid() || !list.length() || !str.length()) return -1;
    int16_t idx = 0;
    int16_t st = 0, end = -1;
    bool stop = 0;
    while (1) {
        if (stop) return -1;
        st = ++end;
        end = list.indexOf(div, end);
        if (end < 0) {
            end = list.length();
            stop = 1;
        }
        if (str.length() == (uint16_t)(end - st) && list.compareN(str, str.length(), st)) return idx;
        idx++;
    }
}

/**
 * @brief Проверка содержит ли список подстроку
 *
 * @param list строка любого типа
 * @param str строка любого типа
 * @param div символ-разделитель (умолч. ';')
 * @return true содержит
 * @return false не содержит
 */
bool includes(AnyText list, AnyText str, char div) {
    return indexOf(list, str, div) >= 0;
}

/**
 * @brief Получить подстроку из списка по индексу
 *
 * @param list строка любого типа
 * @param idx строка любого типа
 * @param div символ-разделитель (умолч. ';')
 * @return AnyText подстрока
 */
AnyText get(AnyText list, uint16_t idx, char div) {
    if (!list.valid() || !list.length()) return AnyText();
    int16_t spos = 0, epos = -1;
    bool stop = 0;
    while (1) {
        epos = list.indexOf(div, epos + 1);
        if (epos < 0) {
            epos = list.length();
            stop = 1;
        }
        if (!idx--) return AnyText(list.str() + spos, list.pgm(), epos - spos);
        if (stop) break;
        spos = epos + 1;
    }
    return AnyText();
}

// распарсить в массив указанного типа и размера. Вернёт количество записанных подстрок
template <typename T>
uint16_t parse(AnyText list, T* buf, uint16_t len, char div) {
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

}  // namespace sutil::list