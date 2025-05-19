#pragma once
#include <Arduino.h>

// #define SU_B64_COMPACT

namespace su {
namespace b64 {

// закодировать в b64
char encodeByte(uint8_t n);

// размер закодированных данных по размеру исходных
size_t encodedLen(size_t len);

// закодировать в char[encodedLen()] (не добавляет '\0' в конец)
size_t encode(char* b64, const void* data, size_t len, bool pgm = false);

// закодировать в String
size_t encode(String* b64, const void* data, size_t len, bool pgm = false);
size_t encode(String& b64, const void* data, size_t len, bool pgm = false);


// раскодировать из b64
uint8_t decodeChar(char b);

// будущий размер декодированных данных по строке b64 и её длине
size_t decodedLen(const void* b64);
size_t decodedLen(const void* b64, size_t len);

// раскодировать данные из строки b64 длиной len в буфер data
size_t decode(void* data, const void* b64);
size_t decode(void* data, const void* b64, size_t len);

// раскодировать данные из строки b64 в буфер data
size_t decode(void* data, const String& b64);

// раскодировать данные из строки b64 длиной len в саму себя
size_t decode(void* b64);
size_t decode(void* b64, size_t len);

////////////////////////////
inline char getChar(uint8_t n) { return encodeByte(n); }
inline uint8_t getByte(char b) { return decodeChar(b); }

}  // namespace b64
}  // namespace su