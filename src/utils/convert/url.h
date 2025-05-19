#pragma once
#include <Arduino.h>

namespace su {
namespace url {

// символ должен быть urlencoded
bool needsEncode(char c);

// длина urlencoded строки
size_t encodedLen(const char* str);
size_t encodedLen(const char* str, size_t len);

// закодировать в char[encodedLen()] (не добавляет '\0' в конец)
size_t encode(char* url, const char* str);
size_t encode(char* url, const char* str, size_t len);

// закодировать в String
void encode(String* url, const char* str);
void encode(String* url, const char* str, size_t len);
void encode(String& url, const char* str);
void encode(String& url, const char* str, size_t len);

String encode(const char* str);
String encode(const char* str, size_t len);

String encode(const String& str);

//

// длина urldecoded строки
size_t decodedLen(const char* url);
size_t decodedLen(const char* url, size_t len);

// раскодировать url (не добавляет '\0' в конец)
size_t decode(char* str, const char* url);
size_t decode(char* str, const char* url, size_t len);

// раскодировать в String
size_t decode(String* str, const char* url);
size_t decode(String* str, const char* url, size_t len);
size_t decode(String& str, const char* url);
size_t decode(String& str, const char* url, size_t len);

String decode(const char* url);
String decode(const char* url, size_t len);
String decode(const String& url);

// раскодировать url саму в себя
size_t decodeSelf(char* url);
size_t decodeSelf(char* url, size_t len);

///////////////////////
size_t decode(const char* src, int16_t len, String& dest);
void decode(const String& src, String& dest);
void encode(const char* str, uint16_t len, String& url);
void encode(const String& str, String& url);

}  // namespace url
}  // namespace su