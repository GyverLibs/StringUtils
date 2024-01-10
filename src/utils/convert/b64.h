#pragma once
#include <Arduino.h>

// #define SU_B64_COMPACT

namespace sutil::b64 {

// размер закодированных данных по размеру исходных
uint16_t encodedLen(uint16_t len);

// будущий размер декодированных данных по строке b64 и её длине
uint16_t decodedLen(const char* b64, uint16_t len);

// закодировать данные в String
void encode(String* b64, uint8_t* data, uint16_t len, bool pgm = false);

// закодировать данные в char[] (библиотека не добавляет '\0' в конец)
void encode(char* b64, uint8_t* data, uint16_t len, bool pgm = false);

// раскодировать данные из строки b64 в буфер data
void decode(uint8_t* data, const char* b64, uint16_t len);

// раскодировать данные из строки b64 в буфер data
void decode(uint8_t* data, const String& b64);

}