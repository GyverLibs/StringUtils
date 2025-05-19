#pragma once
#include <Arduino.h>

#include "convert.h"

namespace su {
namespace unicode {

// декодировать строку с unicode символами саму в себя (не добавляет '\0' в конец)
size_t decodeSelf(char* str);
size_t decodeSelf(char* str, size_t len);

// декодировать строку с unicode символами
String decode(const char* str);
String decode(const char* str, size_t len);

// декодировать строку с unicode символами
String decode(const String& str);

// кодировать unicode символ по его коду. В массиве должно быть 5 ячеек
void encode(char* str, uint32_t c);

// кодировать unicode символ по его коду
String encode(uint32_t code);

}  // namespace unicode
}  // namespace su