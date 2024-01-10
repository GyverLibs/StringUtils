#pragma once
#include <Arduino.h>

namespace sutil {
// хэш строки, выполняется на этапе компиляции. Размер зависит от платформы и соответствует size_t
constexpr size_t SH(const char* str);

// хэш строки, выполняется на этапе компиляции. Размер 32 бит
constexpr uint32_t SH32(const char* str);

// =====================================================

// хэш строки, выполняется в рантайме. Размер зависит от платформы и соответствует size_t
size_t hash(const char* str, int16_t len = -1);

// хэш строки, выполняется в рантайме. Размер 32 бит
uint32_t hash32(const char* str, int16_t len = -1);

// хэш PROGMEM строки, выполняется в рантайме. Размер зависит от платформы и соответствует size_t
size_t hash_P(PGM_P str, int16_t len = -1);

// хэш PROGMEM строки, выполняется в рантайме. Размер 32 бит
uint32_t hash32_P(PGM_P str, int16_t len = -1);

}  // namespace sutil