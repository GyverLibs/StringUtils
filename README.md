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
Класс-обёртка для всех типов строк. Может быть создана в конструкторе из:
- `"const char"` - строки
- `char[]` - строки
- `F("f-строки")`
- `PROGMEM` - строки
- `String` - строки

Особенности:
- Хранит тип и длину строки, все операции доступны и оптимизированы для всех типов строк
- Позволяет **печататься**, **конвертироваться** в любой целочисленный формат и **сравниваться** с переменными всех стандартных типов, а также сравниваться с любыми другими строками
- **Не может изменять исходную строку**, все операции только "для чтения"
- **Не создаёт копию строки** и работает с оригинальной строкой, т.е. *оригинальная строка должна быть в памяти на время существования AnyText*
- Если создана из String строки, то оригинальная String строка не должна меняться в процессе работы экземпляра AnyText

```cpp
// ====== КОНСТРУКТОР ======
sutil::AnyText(String& str);
sutil::AnyText(const String& str);
sutil::AnyText(const __FlashStringHelper* str, int16_t len = 0);
sutil::AnyText(const char* str, int16_t len = 0, bool pgm = 0);

// ======== СИСТЕМА ========
bool valid();           // Статус строки, существует или нет
bool pgm();             // Строка из Flash памяти
uint16_t length();      // Длина строки
uint16_t readLen();     // посчитать и вернуть длину строки (const)
void calcLen();         // пересчитать и запомнить длину строки (non-const)
Type type();            // Тип строки
const char* str();      // Получить указатель на строку. Всегда вернёт ненулевой указатель
const char* end();      // указатель на конец строки. Всегда вернёт ненулевой указатель
bool terminated();      // строка валидна и оканчивается \0

// ======== ХЭШ ========
size_t hash();              // хэш строки size_t
uint32_t hash32();          // хэш строки 32 бит

// ======== PRINT ========
size_t printTo(Print& p);   // Напечатать в Print (c учётом длины)

// ======== СРАВНЕНИЕ И ПОИСК ========
// сравнивается со всеми типами строк через ==

// Сравнить со строкой, начиная с индекса
bool compare(AnyText s, uint16_t from = 0);

// Сравнить со строкой, начиная с индекса, с указанием количества символов
bool compareN(AnyText s, uint16_t amount, uint16_t from = 0);

// Найти позицию символа в строке, начиная с индекса
int16_t indexOf(char sym, uint16_t from = 0);

// найти символ и получить указатель на первое вхождение
const char* find(char sym, uint16_t from = 0);

// ======== РАЗДЕЛЕНИЕ И ПАРСИНГ ========
// вернёт новую строку с убранными пробельными символами с начала и конца
AnyText trim();

// Посчитать количество подстрок, разделённых символом (количество символов +1)
uint16_t count(char sym, uint16_t from = 0);

// Разделить по символу-разделителю в массив
uint16_t split(AnyText* arr, uint16_t len, char div);

// Получить подстроку из списка по индексу
AnyText getSub(uint16_t idx, char div);

// выделить подстроку (начало, конец). Отрицательные индексы работают с конца строки
AnyText substring(int16_t start, int16_t end = 0);

// Получить символ по индексу
char charAt(uint16_t idx);

// ======== ВЫВОД. СТРОКИ ========
// Получить как String строку
String toString(bool decodeUnicode = false);

// Вывести в String строку. Вернёт false при неудаче
bool toString(String& s, bool decodeUnicode = false);

// Добавить к String строке. Вернёт false при неудаче
bool addString(String& s, bool decodeUnicode = false);

// Вывести в char массив. Вернёт длину строки. terminate - завершить строку нулём
uint16_t toStr(char* buf, int16_t bufsize = -1, bool terminate = true);

// ======== ВЫВОД. B64 ========
// размер данных (байт), если они b64
size_t sizeB64();

// вывести в переменную из b64
bool decodeB64(void* var, size_t size);

// ======== ВЫВОД. ЧИСЛА ========
bool toBool();              // получить значение как bool
int16_t toInt16();          // получить значение как int16
int32_t toInt32();          // получить значение как int32
int64_t toInt64();          // получить значение как int64
uint32_t toInt32HEX();      // получить значение как uint 32 из HEX строки
float toFloat();            // получить значение как float

// также автоматически конвертируется и сравнивается с
bool
char + unsigned
short + unsigned
int + unsigned
long + unsigned
long long + unsigned
float
double
String

// для ручного управления строкой
const char* _str;           // указатель на строку
uint16_t _len;              // длина
```

#### Пример
```cpp
// конструктор
sutil::AnyText v0("-123456");
sutil::AnyText v1 = "-123456";
v1 = F("-123456");
String s("abcd");
sutil::AnyText v2(s);
v2 = s;

// сравнение
v2 == v1;
v2 == F("text");
v1 == -123456;

// авто конвертация
int v = v0;
String s2 = v2;

// вывод в массив
char buf[20];
v1.toStr(buf);

// парсинг и разделение
sutil::AnyText list("abc/123/def");
Serial.println(list.getSub(0, '/')); // abc
Serial.println(list.getSub(2, '/')); // def

Serial.println(list.substring(4, 6));   // 123

sutil::AnyText arr[3];
list.split(arr, 3, '/');
Serial.println(arr[0]);
Serial.println(arr[1]);
Serial.println(arr[2]);

// так делать НЕЛЬЗЯ
AnyText t1(String("123"));  // строка будет выгружена из памяти!
// t1.... программа сломается

String s;
AnyText t1(s);
s += String("123");     // адрес строки изменится!
// t1.... программа сломается

// в то же время вот так - можно
void foo(const AnyText& text) {
    // String существует тут
    print(text);
}
foo(String("123"));
```

Встроенный разделитель и хэш-функции позволяют очень просто и эффективно разбирать различные текстовые протоколы. Например пакет вида `key=value`, где `key` может отсылать к переменной в коде. Пакет можно разделить, ключ хешировать и опросить через switch для присвоения н ужной переменной:
```cpp
AnyText txt("key1=1234");
int val = txt.getSub(1, '=');   // значение в int

switch (txt.getSub(0, '=').hash()) {    // хэш ключа
    case SH("key1"):
        var1 = val;
        break;
    case SH("key2"):
        var2 = val;
        break;
    case SH("key3"):
        var2 = val;
        break;
}
```

или протокол вида `name/index/value`, где `name` - текстовый ключ, `index` - порядковый номер:
```cpp
AnyText txt("key/3/1234");

int val = txt.getSub(2, '/');

switch (txt.getSub(0, '/').hash()) {
    case SH("key"):
        switch(txt.getSub(1, '/').toInt16()) {
            case 0: break;
            case 1: break;
            case 2: break;
            //.....
        }
        break;
    case SH("keykey"):
        //...
        break;
    case SH("anotherKey"):
        //...
        break;
}
```

### AnyValue
Добавка к `AnyText`, поддерживает все остальные стандартные типы данных. Имеет буфер 22 байта, при создании конвертирует число в него:
```cpp
sutil::AnyValue(bool value);
sutil::AnyValue(char + unsigned value, uint8_t base = DEC);
sutil::AnyValue(short + unsigned value, uint8_t base = DEC);
sutil::AnyValue(int + unsigned value, uint8_t base = DEC);
sutil::AnyValue(long + unsigned value, uint8_t base = DEC);
sutil::AnyValue(long long + unsigned value, uint8_t base = DEC);
sutil::AnyValue(double value, uint8_t dec = 2);

// аналогично с ручным размером буфера
sutil::AnyValueT<размер буфера>();
```

#### Пример
```cpp
sutil::AnyValue v0("-123456");   // все строки также можно
sutil::AnyValue v1(123);
sutil::AnyValue v2(3.14);
sutil::AnyValue v3((uint64_t)12345678987654321);

// конвертируется из числа в текст
v1 = 10;
v1 = 3.14;
v1 = 12345654321ull;

Serial.println(v0);         // печатается в Serial
Serial.println(v1 == v2);   // сравнивается

// сравнивается с любыми строками
sutil::AnyText s("123");
String ss = "123";
Serial.println(s == "123");
Serial.println(s == F("123"));
Serial.println(s == ss);

// конвертируется в любой тип
float f = v2;   // f == 3.14
int i = v1;     // i = 123

// выводится в String
String S;
v0.toString(s);

// выводится в char[]
char buf[v1.length() + 1];  // +1 для '\0'
v1.toStr(buf);
```

#### Использование в библиотеках
Передача текста в функцию:
- Строки любого типа
- Без аллокаций, что чрезвычайно критично при сборке String
- Без создания десятка перегруженных функций

Например нужна функция, принимающая строку в любом виде. В ванильном фреймворке Arduino можно сделать так:
```cpp
void setText(const String& str) {
    // и например прибавить к строке
    s += str;
}
```

Такая функция сможет принимать любые строки:
```cpp
setText("const literal");
setText(F("F-string"));

char str[] = "buffer string";
setText(str);

String s = "Arduino String";
setText(s);
```
Но эта строка будет *продублирована* в конструкторе `String`, и самое страшное - в динамической памяти! Таким образом при прибавлении к условно-глобальной String в этой области определения случится переаллокация и фрагментирование памяти. `AnyText` позволяет полностью этого избежать:

```cpp
void setText(const AnyText& str) {
    // и например прибавить к строке
    str.addString(s);
}
```
Теперь эта функция так же умеет принимать строки в любом формате, но **не создаёт их копии**, и например прибавление к строке становится быстрым и безопасным.

Также AnyText удобен для вывода, например в классе, который хранит буфер и сам наполняет его данными и знает их длину:

```cpp
class MyClass {
    public:
    sutil::AnyText get() {
        return sutil::AnyText(buffer, len);
    }

    private:
    char buffer[20];
    int len;
};

MyClass s;
Serial.println(s.get());
```

Вариант с наследованием:
```cpp
class MyClass : public sutil::AnyText {
    public:
    void foo() {
        sutil::AnyText::_str = buffer;
        sutil::AnyText::_len = somelen;
    }

    private:
    char buffer[20];
};

MyClass s;
Serial.println(s);
```

Если вместо `AnyText` использовать `AnyValue` - функция сможет принимать также любые численные данные.

### AnyTextList
Разделитель `AnyText` списков на `AnyText` подстроки.

#### Статический
```cpp
AnyTextListT<int16_t cap>(const AnyText& list, char div);

// количество построк
uint16_t length();

// размер буфера
uint16_t capacity();

// получить подстроку под индексом
const AnyText& get(uint16_t idx);
const AnyText& operator[](int idx);
```

#### Динамический
```cpp
AnyTextList(const AnyText& list, char div);
// количество построк
uint16_t length();

// получить подстроку под индексом
const AnyText get(uint16_t idx);
const AnyText operator[](int idx);
```

### Parser
Разделение строки на подстроки по разделителю в цикле. **Изменяет** исходную строку, но после завершения возвращает разделители на место.

```cpp
sutil::Parser p(String& str, char div = ';');
sutil::Parser p(const char* str, char div = ';');

bool next();        // парсить следующую подстроку. Вернёт false, если парсинг закончен
uint8_t index();    // индекс текущей подстроки
const char* str();  // получить подстроку
AnyText get();      // получить подстроку как AnyText
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
sutil::SplitterT<макс. подстрок> spl(String& str, char div = ';');
sutil::SplitterT<макс. подстрок> spl(const char* str, char div = ';');

sutil::Splitter spl(String& str, char div = ';');       // авто-размер (выделяется в heap)
sutil::Splitter spl(const char* str, char div = ';');   // авто-размер (выделяется в heap)

void setDiv(char div);          // установить разделитель
void restore();                 // восстановить строку (вернуть разделители)
uint8_t length();               // количество подстрок
const char* str(uint16_t idx);  // получить подстроку по индексу
AnyText get(uint16_t idx);      // получить подстроку по индексу как AnyText
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

### list функции
```cpp
// Получить количество подстрок в списке
uint16_t sutil::list::length(AnyText list, char div = ';');

// Получить индекс подстроки в списке
int16_t sutil::list::indexOf(AnyText list, AnyText str, char div = ';');

// Проверка содержит ли список подстроку
bool sutil::list::includes(AnyText list, AnyText str, char div = ';');

// Получить подстроку из списка по индексу
AnyText sutil::list::get(AnyText list, uint16_t idx, char div = ';');

// распарсить в массив указанного типа и размера. Вернёт количество записанных подстрок
template <typename T>
uint16_t sutil::list::parse(AnyText list, T* buf, uint16_t len, char div = ';');
```

#### Пример
```cpp
Serial.println(sutil::list::length("123;456;333"));             // 3
Serial.println(sutil::list::includes("123;456;333", "456"));    // true
Serial.println(sutil::list::indexOf("123;456;333", "333"));     // 2
Serial.println(sutil::list::get("123;456;333", 1));             // 456

// распарсить в массив
float arr[3];
sutil::list::parse(F("3.14;2.54;15.15"), arr, 3);
```

### List класс
Получение подстрок по разделителям **без модификации исходной строки**, работает также с PROGMEM строками.
```cpp
List(AnyText);

// установить разделитель
void setDiv(char div);

// получить размер списка
uint16_t length();

// получить индекс подстроки в списке или -1 если её нет
int16_t indexOf(AnyText str);

// проверить наличие подстроки в списке
bool includes(AnyText str);

// получить подстроку под индексом
AnyText get(uint16_t idx);

// распарсить в массив указанного типа и размера. Вернёт количество записанных подстрок
template <typename T>
uint16_t parse(T* buf, uint16_t len);
```

#### Пример
```cpp
sutil::List list(F("123;456;333"));
Serial.print("len: ");
Serial.println(list.length());  // 3
Serial.print("index 2: ");
Serial.println(list[2]);  // 333
Serial.print("index of '456':");
Serial.println(list.indexOf(F("456"))); // 1
Serial.print("index of '789':");
Serial.println(list.indexOf("789"));    // -1

// переписать в массив
int arr[3];
list.parse(arr, 3);
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
size_t sutil::b64::encodedLen(size_t len);

// будущий размер декодированных данных по строке b64 и её длине
size_t sutil::b64::decodedLen(const char* b64, size_t len);

// закодировать данные в String
void sutil::b64::encode(String* b64, uint8_t* data, size_t len, bool pgm = false);

// закодировать данные в char[] (библиотека не добавляет '\0' в конец)
void sutil::b64::encode(char* b64, uint8_t* data, size_t len, bool pgm = false);

// раскодировать данные из строки b64 в буфер data
void sutil::b64::decode(uint8_t* data, const char* b64, size_t len);
void sutil::b64::decode(uint8_t* data, const String& b64);
```

### Unicode
Декодер строки, содержащей unicode символы вида `\u0abc`. Также делает unescape символов `\t\r\n`!
```cpp
// декодировать строку.Зарезервировать строку на длину len. Иначе - по длине строки
String sutil::unicode::decode(const char* str, uint16_t len = 0);

// декодировать строку
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
void sutil::url::encode(const char* src, uint16_t len, String& dest);
void sutil::url::encode(const String& src, String& dest);
String sutil::url::encode(const String& src);

// раскодировать url
void sutil::url::decode(const char* src, uint16_t len, String& dest);
void sutil::url::decode(const String& src, String& dest);
String sutil::url::decode(const String& src);
```

### Hash
Вместо сравнения строк можно сравнивать хэш этих строк, что делает программу компактнее, легче и в большинстве случаев быстрее. Функции, указанные ниже как "считается компилятором" - считаются компилятором, то есть **строка даже не попадает в код программы** - вместо неё подставляется хэш-число:

```cpp
// считается компилятором
constexpr sutil::size_t SH(const char* str);               // (String Hash) размер size_t
constexpr sutil::size_t SH32(const char* str);             // (String Hash) размер 32 бит

// считается в рантайме
size_t sutil::hash(const char* str, int16_t len = -1);     // Размер зависит от платформы и соответствует size_t
uint32_t sutil::hash32(const char* str, int16_t len = -1); // Размер 32 бит

size_t sutil::hash_P(PGM_P str, int16_t len = -1);         // PROGMEM строка, размер size_t
uint32_t sutil::hash32_P(PGM_P str, int16_t len = -1);     // PROGMEM строка, размер 32 бит
```

> На ESP-платах `SH`, `hash` и `hash_P` по умолчанию являются 32-битными!

По проведённому тесту 32-битная версия хэша имеет 7 коллизий из 234450 английских слов, 16-битная версия - 170723 коллизий (что есть 73% - чисто статистическое количество коллизий из расчёта 16 бит - 65536 значений)

#### Пример
Определить, какая строка была "получена". Классический способ со сравнением строк из PROGMEM:

```cpp
char buf[] = "some_text";

if (!strcmp_P(buf, PSTR("abcdef"))) Serial.println(0);
else if (!strcmp_P(buf, PSTR("12345"))) Serial.println(1);
else if (!strcmp_P(buf, PSTR("wrong text"))) Serial.println(2);
else if (!strcmp_P(buf, PSTR("some text"))) Serial.println(3);
else if (!strcmp_P(buf, PSTR("hello"))) Serial.println(4);
else if (!strcmp_P(buf, PSTR("some_text"))) Serial.println(5);
```

Способ с хэшем строки:
```cpp
using sutil::SH;
using sutil::hash;

char buf[] = "some_text";

switch (hash(buf)) {
    case SH("abcdef"):      Serial.println(0); break;
    case SH("12345"):       Serial.println(1); break;
    case SH("wrong text"):  Serial.println(2); break;
    case SH("some text"):   Serial.println(3); break;
    case SH("hello"):       Serial.println(4); break;
    case SH("some_text"):   Serial.println(5); break;
}
```
> Один расчёт хэша занимает чуть большее время, чем сравнение со строкой. Но итоговая конструкция из примера выполняется в 2 раза быстрее (на ESP).

> `SH("строки")` в данном примере вообще не попадают в код программы - вместо них подставляется их хэш

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
uint8_t sutil::getLog10(int32_t value);

// быстрое возведение 10 в степень
uint32_t sutil::getPow10(uint8_t value);
```

<a id="versions"></a>

## Версии
- v1.0
- v1.1.0 - оптимизация, добавлены фичи, исправлены уязвимости
- v1.2 - добавлены хэш-функции
- v1.2.x - мелкие исправления и улучшения
- v1.3 - оптимизация const, добавлены фичи, сравнения
- v1.3.1 - добавлен substring
- v1.3.2 - поддержка ESP8266 версий 2.x
- v1.3.5 - uintToStr: HEX теперь в нижнем регистре как у си-функций
- v1.3.6 - в AnyText добавлены toInt32HEX(), count(), split() и getSub(). Добавлен парсер AnyTextList
- v1.3.7 - исправлены варнинги на AVR

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