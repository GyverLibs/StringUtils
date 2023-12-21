[![latest](https://img.shields.io/github/v/release/GyverLibs/StringUtils.svg?color=brightgreen)](https://github.com/GyverLibs/StringUtils/releases/latest/download/StringUtils.zip)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD$%E2%82%AC%20%D0%9D%D0%B0%20%D0%BF%D0%B8%D0%B2%D0%BE-%D1%81%20%D1%80%D1%8B%D0%B1%D0%BA%D0%BE%D0%B9-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/StringUtils?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# StringUtils
Набор инструментов для работы со строками
- Быстрые функции конвертации
- Парсинг, разбивание по разделителям
- Несколько классов-конвертеров данных в строку и обратно для использования в других библиотеках
- Кодирование и раскодирование base64, url, unicode, йцукен/qwerty-раскладки

### Совместимость
Совместима со всеми Arduino платформами (используются Arduino-функции)

## Содержание
- [Документация](#docs)
- [Версии](#versions)
- [Установка](#install)
- [Баги и обратная связь](#feedback)

<a id="docs"></a>

## Документация
### AnyText
Класс-обёртка для всех типов строк:
- `"const char"` - строки
- `char[]` - строки
- `F("f-строки")`
- `PROGMEM` - строки
- `String` - строки

Во всех случаях, кроме `AnyText(String("строка"))` не создаёт копию строки и работает с оригинальной строкой, т.е. оригинальная строка должна быть в памяти на время существования AnyText. Позволяет печататься и конвертироваться в любой формат.

```cpp
// конструктор
sutil::AnyText(String& str);
sutil::AnyText(const String& str);
sutil::AnyText(const __FlashStringHelper* str, int16_t _len = -1);
sutil::AnyText(const char* str, bool pgm = 0, int16_t _len = -1);

// методы
// Строка из Flash памяти
bool pgm();

// Длина строки
uint16_t length();

// Тип строки
Type type();

// Получить указатель на строку
const char* str();

// Статус строки
bool valid();

// Напечатать в Print (c учётом длины)
size_t printTo(Print& p);

// ========================== SEARCH ==========================
// Сравнить со строкой
bool compare(const AnyText& s, uint16_t from = 0);

// Сравнить со строкой с указанием количества символов
bool compareN(const AnyText& s, uint16_t amount, uint16_t from = 0);

// Найти позицию символа в строке
int16_t indexOf(char sym, uint16_t from = 0);

// Получить символ по индексу
char charAt(uint16_t idx);

// Получить символ по индексу
char operator[](uint16_t idx);

// Вывести в String строку
String toString();

// Вывести в char массив. Сама добавит '\0' в конце!
void toStr(char* buf);

// получить значение как bool
bool toBool();

// получить значение как int 16
int16_t toInt16();

// получить значение как int 32
int32_t toInt32();

// получить значение как int64
int64_t toInt64();

// получить значение как float
float toFloat();

// автоматически конвертируется в
bool
int8_t
uint8_t
int16_t
uint16_t
int32_t
uint32_t
int64_t
uint64_t
float
double
const char*
String
```

### AnyValue
Добавка к `AnyText`, поддерживает все остальные стандартные типы данных. Имеет буфер 22 байта, при создании конвертирует число в него:
```cpp
sutil::AnyValue(const char& value);
sutil::AnyValue(const bool& value);
sutil::AnyValue(const int8_t& value, uint8_t base = DEC);
sutil::AnyValue(const uint8_t& value, uint8_t base = DEC);
sutil::AnyValue(const int16_t& value, uint8_t base = DEC);
sutil::AnyValue(const uint16_t& value, uint8_t base = DEC);
sutil::AnyValue(const int32_t& value, uint8_t base = DEC);
sutil::AnyValue(const uint32_t& value, uint8_t base = DEC);
sutil::AnyValue(const int64_t& value, uint8_t base = DEC);
sutil::AnyValue(const uint64_t& value, uint8_t base = DEC);
sutil::AnyValue(const double& value, uint8_t dec = 2);

// аналогично с ручным размером буфера
sutil::AnyValueT<размер буфера>();
```

#### Пример
```cpp
sutil::AnyValue v0("-123456");   // все строки также можно
sutil::AnyValue v1(123);
sutil::AnyValue v2(3.14);
sutil::AnyValue v3((uint64_t)12345678987654321);

Serial.println(v0);
Serial.println(v1 == v2);

// cast
float f = v2;   // f == 3.14
```

### Parser
Разделение строки на подстроки по разделителю в цикле. **Изменяет** исходную строку, но после завершения возвращает разделители на место.

```cpp
sutil::Parser p(char* str, char div = ';');

// парсить следующую подстроку. Вернёт false, если парсинг закончен
bool next();

// индекс текущей подстроки
uint8_t index();

// получить подстроку
const char* str();

// получить подстроку как AnyText
AnyText get();
```

#### Пример
```cpp
char buf[] = "123;456;abc";

sutil::Parser p(buf);
while (p.next()) {
    Serial.print(p.index());
    Serial.print(": ");
    Serial.println(p.get());
}
```

### Splitter
Разделение строки на подстроки по разделителю в цикле. **Изменяет** исходную строку! После удаления объекта строка восстанавливается, либо вручную вызвать `restore()`
```cpp
sutil::SplitterT<макс. подстрок> spl(char* str, char div = ';');
sutil::Splitter spl(char* str, char div = ';');  // авто-размер (выделяется в heap)

// восстановить строку (вернуть разделители)
void restore();

// количество подстрок
uint8_t length();

// получить подстроку по индексу
const char* str(uint16_t idx);

// получить подстроку по индексу как AnyText
AnyText get(uint16_t idx);
```

#### Пример
```cpp
char buf[] = "123;456;abc";

sutil::Splitter spl(buf);
for (uint8_t i = 0; i < spl.length(); i++) {
    Serial.print(i);
    Serial.print(": ");
    Serial.println(spl[i]);
}

spl.restore();
```

### List
Получение подстрок по разделителям **без модификации исходной строки**, работает также с PROGMEM строками.
```cpp
sutils::List list(любая строка);

// установить разделитель
void setDiv(char div);

// получить размер списка
uint16_t length();

// получить индекс подстроки в списке или -1 если её нет
int16_t indexOf(const AnyText& str);

// проверить наличие подстроки в списке
bool contains(const AnyText& str);

// получить подстроку под индексом
AnyText get(uint16_t idx);
```

#### Пример
```cpp
sutil::List list(F("123;456;abc"));
Serial.print("len: ");
Serial.println(list.length());  // 3
Serial.print("index of '456':");
Serial.println(list.indexOf(F("456"))); // 1
Serial.print("index of '789':");
Serial.println(list.indexOf("789"));    // -1
```

### PrintString
```cpp
// строка, в которую можно делать print/println
sutil::PrintString prs;
prs += "как обычный String";

prs.print(10);
prs.println("hello");
prs.print(F("print!"));

Serial.println(prs);
```

### QWERTY
```cpp
// Изменить раскладку (RU в QWERTY) - String
String sutil::toQwerty(const String& ru);

// Изменить раскладку (RU в QWERTY) - char* (qw длина как ru + 1, функция добавит '\0')
char* sutil::toQwerty(const char* ru, char* qw);
```

### Base64
```cpp
// размер закодированных данных по размеру исходных
uint16_t sutil::b64::encodedLen(uint16_t len);

// будущий размер декодированных данных по строке b64 и её длине
uint16_t sutil::b64::decodedLen(const char* b64, uint16_t len);

// закодировать данные в String
void sutil::b64::encode(String* b64, uint8_t* data, uint16_t len, bool pgm = false);

// закодировать данные в char[] (библиотека не добавляет '\0' в конец)
void sutil::b64::encode(char* b64, uint8_t* data, uint16_t len, bool pgm = false);

// раскодировать данные из строки b64 в буфер data
void sutil::b64::decode(uint8_t* data, const char* b64, uint16_t len);
void sutil::b64::decode(uint8_t* data, const String& b64);
```

### Unicode
```cpp
// декодировать строку с unicode символами. зарезервировать строку на длину len. Иначе - по длине строки
String sutil::unicode::decode(const char* str, uint16_t len = 0);

// декодировать строку с unicode символами
String sutil::unicode::decode(const String& str);

// кодировать unicode символ по его коду. В массиве должно быть 5 ячеек
void sutil::unicode::encode(char* str, uint32_t c);

// кодировать unicode символ по его коду
String sutil::unicode::encode(uint32_t code);
```

### URL
```cpp
// символ должен быть urlencoded
bool sutil::url::needsEncode(char c);

// закодировать в url
void sutil::url::encode(const String& src, String& dest);
String sutil::url::encode(const String& src);

// раскодировать url
void sutil::url::decode(const String& src, String& dest);
String sutil::url::decode(const String& src);
```

### Прочие утилиты
```cpp
// Длина строки с русскими символами
uint16_t sutil::strlenRu(const char* str);

// Получить длину целого числа
uint8_t sutil::intLen(int32_t val);

// Получить длину float числа
uint8_t sutil::floatLen(double val, uint8_t dec);

// Преобразовать строку в целое число
template <typename T>
T sutil::strToInt(const char* str, uint8_t len = 0);

// Преобразовать PROGMEM строку в целое число
template <typename T>
T sutil::strToInt_P(const char* str, uint8_t len = 0);

// Преобразовать float в строку с указанием кол-ва знаков после точки
uint8_t sutil::floatToStr(double val, char* buf, uint8_t dec = 2);

// Преобразовать HEX строку в целое число. Опционально длина
uint32_t sutil::strToIntHex(const char* str, int8_t len = -1);

// Длина символа в количестве байт
uint8_t sutil::charSize(char sym);

// Конвертация числа в char* массив. Пишет от начала массива, добавляет '\0', вернёт длину строки
// для int64 макс. длина буфера - 22 символа, для int32 - 12
uint8_t sutil::uintToStr(uint32_t n, char* buf, uint8_t base = DEC);
uint8_t sutil::intToStr(int32_t n, char* buf, uint8_t base = DEC);
uint8_t sutil::uint64ToStr(uint64_t n, char* buf, uint8_t base = DEC);
uint8_t sutil::int64ToStr(int64_t n, char* buf, uint8_t base = DEC);

// конвертация из строки во float
float sutil::strToFloat(const char* s);

// конвертация из PROGEMEM строки во float
float sutil::strToFloat_P(PGM_P s);

// быстрый целочисленный логарифм 10 (длина числа в кол-ве символов)
uint8_t sutil::getLog10(uint32_t value);

// быстрое возведение 10 в степень
uint32_t sutil::getPow10(uint8_t value);
```

<a id="versions"></a>

## Версии
- v1.0

<a id="install"></a>

## Установка
- Библиотеку можно найти по названию **StringUtils** и установить через менеджер библиотек в:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Скачать библиотеку](https://github.com/GyverLibs/StringUtils/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Обновление
- Рекомендую всегда обновлять библиотеку: в новых версиях исправляются ошибки и баги, а также проводится оптимизация и добавляются новые фичи
- Через менеджер библиотек IDE: найти библиотеку как при установке и нажать "Обновить"
- Вручную: **удалить папку со старой версией**, а затем положить на её место новую. "Замену" делать нельзя: иногда в новых версиях удаляются файлы, которые останутся при замене и могут привести к ошибкам!

<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!

При сообщении о багах или некорректной работе библиотеки нужно обязательно указывать:
- Версия библиотеки
- Какой используется МК
- Версия SDK (для ESP)
- Версия Arduino IDE
- Корректно ли работают ли встроенные примеры, в которых используются функции и конструкции, приводящие к багу в вашем коде
- Какой код загружался, какая работа от него ожидалась и как он работает в реальности
- В идеале приложить минимальный код, в котором наблюдается баг. Не полотно из тысячи строк, а минимальный код