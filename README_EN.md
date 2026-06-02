This is an automatic translation and may be incorrect in some places. See the source README and examples for authoritative information.

[![latest](https://img.shields.io/github/v/release/GyverLibs/StringUtils.svg?color=brightgreen)](https://github.com/GyverLibs/StringUtils/releases/latest/download/StringUtils.zip)
[![PIO](https://badges.registry.platformio.org/packages/gyverlibs/library/StringUtils.svg)](https://registry.platformio.org/libraries/gyverlibs/StringUtils)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD%24%E2%82%AC%20%D0%9F%D0%BE%D0%B4%D0%B4%D0%B5%D1%80%D0%B6%D0%B0%D1%82%D1%8C-%D0%B0%D0%B2%D1%82%D0%BE%D1%80%D0%B0-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/StringUtils?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# StringUtils
A set of tools for working with lines
- Quick conversion functions
- Parsing, dividing
- Multiple classes-converters of data to string and back for use in other libraries
- Encoding and decoding base64, url, unicode, ytsuken/qwerty layouts

### Compatibility
Compatible with all Arduino platforms (Arduino features are used)

## Contents
- [Documentation.](#docs)
- [Versions](#versions)
- [Installation](#install)
- [Bugs and feedback](#feedback)

<a id="docs"></a>

## Documentation.
### `Text`
Class wrapper for all types of lines. Can be created in a constructor from:
- `"const char"`- lines
- `char[]`- lines
- `F("f-строки")`
- `PROGMEM`- lines
- `String`- lines

Features:
- Stores the type and length of the line
- Allows you to **print **, **convert ** to any integer format and **compare ** with variables of all standard types, as well as compare with any other strings
- Output in substrings in different ways, search and separation
- **Can't change the source line**, all operations are read-only
- **Does not create a copy of the line** and works with the original line, i.e. the original line must be in memory for the duration of the Text* existence.
- If created from a String string, the original String string should not change during the operation of the Text instance.

```cpp
// =============
Text(String& str);
Text(const String& str);
Text(const uint8_t* str, uint16_t len);
Text(const char* str, int16_t len = 0, bool pgm = 0);
Text(const __FlashStringHelper* str, int16_t len = 0);

// =============
operator bool();                    // The status of the line, whether it exists or not
bool valid();                       // The status of the line, whether it exists or not
bool pgm();                         // A line from Flash memory
uint16_t length();                  // Line length
uint16_t readLen();                 // Calculate and return line length (const)
void calcLen();                     // Calculate and remember the length of the line (non-const)
Type type();                        // Line type
const uint8_t* bytes();             // Get a pointer to the line. Always return a pointer other than the nullptr!
const char* str();                  // Get a pointer to the line. Always return the non-zero pointer.
const char* end();                  // end pointer. Always return the non-zero pointer.
bool terminated();                  // line is valid and ends \0

// ======== UNICODE ========
// Line length with unicode symbols
uint16_t lengthUnicode();

// get the position of the Unicode symbol in a string if it contains Unicode
uint16_t posToUnicode(uint16_t pos);

// get the real position of the symbol in the line if it contains Unicode
uint16_t unicodeToPos(uint16_t pos);

// ===============
size_t hash();              // hash string size t
uint32_t hash32();          // hash string 32 bits

// ======== PRINT ========
size_t printTo(Print& p);   // Print in Print (taking into account length)

// ===================
// Compared to all types of lines by ==

// Compare with a line starting with an index
bool compare(Text s, uint16_t from = 0);

// Compare with a string starting with an index, indicating the number of characters
bool compareN(Text s, uint16_t amount, uint16_t from = 0);

// Find the position of the symbol in the line, starting with the index
int16_t indexOf(char sym, uint16_t from = 0);

// Find the line position in the line
int16_t indexOf(Text txt, uint16_t from = 0);

// Find the line position in the line
int16_t indexOfUnicode(Text txt, uint16_t from = 0);

// Find the position of the symbol in the line from the end
int16_t lastIndexOf(char sym);

// Find the line position in the line from the end
int16_t lastIndexOf(Text txt);

// Find the line position in the line from the end
int16_t lastIndexOfUnicode(Text txt);

// Find the symbol and get a pointer for the first entry
const char* find(char sym, uint16_t from = 0);

// line-up
bool startsWith(const Text& txt);
bool startsWith(char c);

// line
bool endsWith(const Text& txt);
bool endsWith(char c);

// =======Separation and Parsing =======
// returns a new line with deleted space characters from the beginning and end
Text trim();

// Calculate the number of substrings separated by the symbol (number of separators +1)
uint16_t count(char sym);

// Calculate the number of substrings separated by rows (number of separators +1)
uint16_t count(Text txt);

// Divide by symbol-separator in an array of any type
uint16_t split(T* arr, uint16_t len, char div);
uint16_t split(T** arr, uint16_t len, char div);

// Divide by row-separator into an array of any type
uint16_t split(T* arr, uint16_t len, Text div);
uint16_t split(T** arr, uint16_t len, Text div);

// Get a substring from the list by index and divider
Text getSub(uint16_t idx, char div);
Text getSub(uint16_t idx, Text div);

// Find the substring index by separator. Return -1 if not found
int findSub(Text sub, char div);
int findSub(Text sub, Text div);

// Select a substring (beginning, end not including). Negative indices work from the end of the line.
Text substring(int16_t start, int16_t end = 0);

// select a substring with the content of Unicode (beginning, end not including). Negative indices work from the end of the line.
Text substringUnicode(int16_t start, int16_t end = 0);

// Get an index symbol. Negatives allowed
char charAt(int idx);
char operator[](int idx);

// ============ SROWS ========
// receive a const char* copy (Cstr is converted to const char*). Always valid and terminated. If the text is from PGM or not terminated, a temporary copy will be created.
Cstr c_str();

// Get like a String string
String toString(bool decodeUnicode = false);

// Put a line in String. It'll bring back false when you fail.
bool toString(String& s, bool decodeUnicode = false);

// Add to the String line. It'll bring back false when you fail.
bool addString(String& s, bool decodeUnicode = false);

// Get the char array. Returns the length of the line. terminate - complete the line with zero
uint16_t toStr(char* buf, int16_t bufsize = -1, bool terminate = true);

// Get as a string, decode unicode
String decodeUnicode();

// Get as a string, decode urlencode
String decodeUrl();

// ============ B64 =======
// data size (byte) if they are b64
size_t sizeB64();

// variable out of b64
bool decodeB64(void* var, size_t size);

// ============ Numbers =========
bool toBool();              // get valued like a bool
int toInt();                // get valued as int
int16_t toInt16();          // get valued as int16
int32_t toInt32();          // get valued as int32
int64_t toInt64();          // get value as int64
uint32_t toInt32HEX();      // Get the value as uint 32 from the HEX string
float toFloat();            // float

// It is also automatically converted and compared to
char + unsigned
short + unsigned
int + unsigned
long + unsigned
long long + unsigned
bool
float
double
String

// manually
const char* _str;           // pointer
uint16_t _len;              // length
```

#### Integer conversion
`Text`can be converted to all standard integer types manually and automatically (implicit and explicit):

```cpp
Text t("1234");

int v = t;
(int)t;
t.toInt();
```

#### Comparison
`Text`can be compared with all standard types (automatically converted):

```cpp
Text t("1234");

t == 1234;
t >= 1234;
t == "1234";
t == F("1234");
```

#### Conversion to bool
Bringing the Text to`bool`Checks **validity** strings (correct or not), these are constructions of the form:

```cpp
Text t;

(bool)t;
!t;
!!t;
if (t);
```

For conversion into`bool`How ** value ** should be used to convert`toBool()`Or better yet, a comparison (taking into account the validity):

```cpp
Text t;

t.toBool();
if (t == true);
if (t == false);
```

If the line is invalid, a comparison with`bool`return`false`no matter what.

#### Conclusion
`Text`can be introduced`String`-line:

```cpp
t.toString();      // return String
String s = t;      // sign up
t.toString(s);     // sign up
t.addString(s);    // add to
```

You can put it in a line with decoding:

```cpp
String s = t.decodeUnicode();   // Decode UCN characters (form \uxxx\uxxx)
String s = t.decodeUrl();       // Decode urlencoded characters (of %AB)
```

#### Exit to the array
`Text`can be massed`char`(You can specify the size of the buffer and stop the line):

```cpp
char buf[20];
t.toStr(buf);
```

#### Conclusion as a C-line
Often you have to pass it on.`Text`a line in a function that takes`const char*`. Method`str()`return the pointer to the beginning of the line, but when`Text`created
- From PROGMEM lines
- From a regular line with a size smaller than the original string (e.g. any JSON element when parsing the GSON library)

Such a line will not be correct because it has an inappropriate length or memory area. For correct transmission, use the method`c_str()`: it will create a temporary correct copy of the string when needed (PGM or incomplete length). For example,`foo(text.c_str())`. Note: This pointer is **temporary** - you can't transfer a line to functions that remember a pointer to a line instead of copying it yourself! Examples:`WiFi.begin(text.c_str())`The function can copy the line itself. For the library.`PubSubClient` - `client.setServer(text.c_str())`You can not, you need to output a buffer or String string to char and leave to exist in the client definition area. How to understand this: study the source!

#### Creation`String`
`Text`stores a pointer to a line, so the line must exist in memory for the duration of its existence`Text`:

```cpp
// 1. you can't do that.
Text t1(String("123"));  // The line will be uploaded from memory!
// t1. program

// 2. you can't do that.
String s;
Text t1(s);
s += String("123");     // The line address will change!
// t1. program

// 3. That's what you can do.
void foo(const Text& text) {
    // String exists here.
    // text...
}
foo(String("123"));
```

#### Substrings

```cpp
Text list("abc/123/def");
Serial.println(list.getSub(0, '/')); // abc
Serial.println(list.getSub(2, '/')); // def

Serial.println(list.substring(4, 6));   // 123
```

#### Parsing into the array

```cpp
// parsing
Text arr[3];
list.split(arr, 3, '/');
Serial.println(arr[0]);
Serial.println(arr[1]);
Serial.println(arr[2]);

// parsing
int arr2[3];    // float, long...
list.split(arr2, 3, '/');
Serial.println(arr2[0]);
Serial.println(arr2[1]);
Serial.println(arr2[2]);
```

#### Parsing protocols
Built-in splitter and hash functions make it very easy and efficient to disassemble various text protocols. For example, a package`key=value`where`key`It can refer to a variable in the code. The packet can be divided, the key can be hashed and polled via switch to assign a variable:

```cpp
Text txt("key1=1234");
int val = txt.getSub(1, '=');   // int

switch (txt.getSub(0, '=').hash()) {    // hash
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

protocol`name/index/value`where`name`- text key,`index`- serial number:

```cpp
Text txt("key/3/1234");

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

### `Value`
Supplement to`Text`It supports all other standard data types. Has a buffer of 22 bytes, when creating converts the number into it:
```cpp
Value(bool value);
Value(char + unsigned value, uint8_t base = DEC);
Value(short + unsigned value, uint8_t base = DEC);
Value(int + unsigned value, uint8_t base = DEC);
Value(long + unsigned value, uint8_t base = DEC);
Value(long long + unsigned value, uint8_t base = DEC);
Value(double value, uint8_t dec = 2);
```

#### Example
```cpp
Value v0("-123456");   // all lines are also possible
Value v1(123);
Value v2(3.14);
Value v3((uint64_t)12345678987654321);

// convertible
v1 = 10;
v1 = 3.14;
v1 = 12345654321ull;

Serial.println(v0);         // printable
Serial.println(v1 == v2);   // compare

// compared to any string
Text s("123");
String ss = "123";
Serial.println(s == "123");
Serial.println(s == F("123"));
Serial.println(s == ss);

// convertible
float f = v2;   // f == 3.14
int i = v1;     // i = 123

// output
String S;
v0.toString(s);

// output in char[]
char buf[v1.length() + 1];  // +1 for '\0'
v1.toStr(buf);
```

> `Text`Automatically compare and convert to all types except`bool`. Use it.`toBool()`. The conversion to bool shows the existence of a string

#### Use in libraries
On the base.`Text`The following libraries have been built:
- [GSON](https://github.com/GyverLibs/GSON)
- [GyverHub](https://github.com/GyverLibs/GyverHub)
- [Settings](https://github.com/GyverLibs/Settings)
- [Pairs](https://github.com/GyverLibs/Pairs)
- [FastBot2](https://github.com/GyverLibs/FastBot2)
- [GyverHTTP](https://github.com/GyverLibs/GyverHTTP)
- [GyverDB](https://github.com/GyverLibs/GyverDB)

##### Transfer of text to function
- Lines of any type
- Without allocations, which is extremely critical when assembling String
- Without creating a dozen overloaded functions

For example, you need a function that accepts a string in any form. In the Arduino vanilla framework, you can do this:
```cpp
void setText(const String& str) {
    // and, for example, add to a line
    s += str;
}
```

This function can take any lines:
```cpp
setText("const literal");
setText(F("F-string"));

char str[] = "buffer string";
setText(str);

String s = "Arduino String";
setText(s);
```But this line will *duplicate* in the constructor.`String`The worst part is the dynamic memory! Thus, if you add to the conditionally global String in this definition area, reallocation and fragmentation of memory will occur.`Text`This completely avoids this:

```cpp
void setText(const Text& str) {
    // and, for example, add to a line
    str.addString(s);
}
```Now this feature can also accept strings in any format, but does not create copies of them, and for example, adding to a line becomes fast and safe.

##### Conclusion
Text is also convenient for output, for example in a class that stores a buffer and fills it with data and knows its length:

```cpp
class MyClass {
    public:
    Text get() {
        return Text(buffer, len);
    }

    private:
    char buffer[20];
    int len;
};

MyClass s;
Serial.println(s.get());
```

Inheritance option:
```cpp
class MyClass : public Text {
    public:
    void foo() {
        Text::_str = buffer;
        Text::_len = somelen;
    }

    private:
    char buffer[20];
};

MyClass s;
Serial.println(s);
```

If not`Text`use`Value`The function can also receive any numerical data.

### `TextList`
Separator`Text`listing`Text`substrate.

#### Static
```cpp
TextListT<int16_t cap>(Text list, char div);
TextListT<int16_t cap>(Text list, Text div);

// number
uint16_t length();

// buffer
uint16_t capacity();

// substrate
const Text& get(uint16_t idx);
const Text& operator[](int idx);
```

#### Dynamic.
```cpp
TextList(Text list, char div);
TextList(Text list, Text div);
// number
uint16_t length();

// substrate
const Text& get(uint16_t idx);
const Text& operator[](int idx);
```

### `TextParser`
"Flow" divider`Text`substrings to work in a cycle

```cpp
TextParser(const Text& txt, char div);
TextParser(const Text& txt, const Text& div);

// scratch. It will return true if the substrate is found.
bool parse();

// substring index (always > 0)
int index();

// substrate
const Text& get();
```

Example:
```cpp
// for
for (TextParser p("123;456", ';'); p.parse();) {
    Serial.println(p);
}

// while
TextParser p("123;456", ';');
while (p.parse()) {
    Serial.println(p);
}
```

An example with nested substrings with different separators:
```cpp
Text t("123;456\nabc;def;ghk\n333;444");

for (TextParser row(t, '\n'); row.parse();) {
    for (TextParser col(row, ';'); col.parse();) {
        Serial.print(col);
        Serial.print(',');
    }
    Serial.println();
}

// conclusion:
// 123,456,
// abc,def,ghk,
// 333,444,
```

### `StringExt`/`StringStatic`
Static thong builder based on Text, replacement[mString](https://github.com/GyverLibs/mString)
```cpp
template <uint16_t cap> StringStatic;
StringExt(char* buf, uint16_t capacity, uint16_t len = 0);

// cleanse
void clear();

// zero out
void terminate();

// Automatically terminate with zero (silent false)
void terminateAuto(bool ter);

// prune
void trunc(uint16_t from);

// add
bool concat(const Value& txt);
bool concat(const char* str, uint16_t len);
bool concat(double val, uint8_t dec);

// appropriate
bool assign(const Value& txt);
bool assign(const char* str, uint16_t len);
bool assign(double val, uint8_t dec);

// You can equate and add any type with =, +=, +
```

Example:
```cpp
StringStatic<50> s;

// char str[20];
// StringExt s(str, 20);

s = F("abc");
s += "def";
s += 12345;
s += 'a';
Serial.println(s);
Serial.println(s.length());
s.clear();

s = s + 123 + "abc" + F("FSTR") + 3.14;
Serial.println(s);
```

You can add an existing line:
```cpp
char str[20] = "hello"; // len 5
StringExt s(str, 20, 5);
s += F(" world!");
Serial.println(s);
```

### Outdated parsers
<details>
<summary>Deploy</summary >>

### `su::Parser`
Dividing a line into substrings by separator in a cycle. **Changes the original string, but returns the separators to place after completion.

```cpp
su::Parser p(String& str, char div = ';');
su::Parser p(const char* str, char div = ';');

bool next();        // to scrape the next line. It will return false if the parsing is finished.
uint8_t index();    // index
const char* str();  // substrate
Text get();      // substrate
```

#### Example
```cpp
char buf[] = "123;456;abc";

su::Parser p(buf);
while (p.next()) {
    Serial.print(p.index());
    Serial.print(": ");
    Serial.println(p.get());
}
```

### `Splitter`
Dividing a line into substrings by separator in a cycle. **Changes the original line! After removing the object, the string is restored, or manually call`restore()`
```cpp
SplitterT<макс. подстрок> spl(String& str, char div = ';');
SplitterT<макс. подстрок> spl(const char* str, char div = ';');

Splitter spl(String& str, char div = ';');       // Auto-size (highlighted in heap)
Splitter spl(const char* str, char div = ';');   // Auto-size (highlighted in heap)

void setDiv(char div);          // divider
void restore();                 // Restore the line (return the dividers)
uint8_t length();               // lineage
const char* str(uint16_t idx);  // index
Text get(uint16_t idx);      // Get an index line like Text
```

#### Example
```cpp
char buf[] = "123;456;abc";

Splitter spl(buf);
for (uint8_t i = 0; i < spl.length(); i++) {
    Serial.print(i);
    Serial.print(": ");
    Serial.println(spl[i]);
}

spl.restore();
```

### `su::list`function
```cpp
// Get the number of substrates in the list
uint16_t su::list::length(Text list, char div = ';');

// Get the substring index in the list
int16_t su::list::indexOf(Text list, Text str, char div = ';');

// Check if the list contains a substring
bool su::list::includes(Text list, Text str, char div = ';');

// Get a substrate from the index list
Text su::list::get(Text list, uint16_t idx, char div = ';');

// dissect into an array of the specified type and size. Return the number of recorded substrings
template <typename T>
uint16_t su::list::parse(Text list, T* buf, uint16_t len, char div = ';');
```

#### Example
```cpp
Serial.println(su::list::length("123;456;333"));             // 3
Serial.println(su::list::includes("123;456;333", "456"));    // true
Serial.println(su::list::indexOf("123;456;333", "333"));     // 2
Serial.println(su::list::get("123;456;333", 1));             // 456

// massif
float arr[3];
su::list::parse(F("3.14;2.54;15.15"), arr, 3);
```

### `su::List`class
Obtaining substrings by separators ** without modifying the source line ** also works with PROGMEM strings.
```cpp
List(Text);

// divider
void setDiv(char div);

// size
uint16_t length();

// get a substring index in the list or -1 if it is not
int16_t indexOf(Text str);

// check if there is a substrate in the list
bool includes(Text str);

// substrate
Text get(uint16_t idx);

// dissect into an array of the specified type and size. Return the number of recorded substrings
template <typename T>
uint16_t parse(T* buf, uint16_t len);
```

#### Example
```cpp
su::List list(F("123;456;333"));
Serial.print("len: ");
Serial.println(list.length());  // 3
Serial.print("index 2: ");
Serial.println(list[2]);  // 333
Serial.print("index of '456':");
Serial.println(list.indexOf(F("456"))); // 1
Serial.print("index of '789':");
Serial.println(list.indexOf("789"));    // -1

// massif
int arr[3];
list.parse(arr, 3);
```
</details>

### su::PrintString
```cpp
// line in which you can print/println
su::PrintString prs;
prs += "как обычный String";

prs.print(10);
prs.println("hello");
prs.print(F("print!"));

Serial.println(prs);
```

### QWERTY
```cpp
// Change the layout (RU in QWERTY) - String
String su::toQwerty(const String& ru);

// Change layout (RU in QWERTY) - char* (qw length as ru + 1, the function will add '\0')
char* su::toQwerty(const char* ru, char* qw);
```

### Base64
```cpp
// code in b64
char encodeByte(uint8_t n);

// size of encoded data by size of initial
size_t encodedLen(size_t len);

// encode in char[encodedLen()] (does not add '\0' to the end)
size_t encode(char* b64, const void* data, size_t len, bool pgm = false);

// string
size_t encode(String* b64, const void* data, size_t len, bool pgm = false);
size_t encode(String& b64, const void* data, size_t len, bool pgm = false);


// decode from b64
uint8_t decodeChar(char b);

// future size of decoded data on line b64 and its length
size_t decodedLen(const void* b64);
size_t decodedLen(const void* b64, size_t len);

// decode data from len-length line b64 into data buffer
size_t decode(void* data, const void* b64);
size_t decode(void* data, const void* b64, size_t len);

// Decode data from line b64 to data buffer
size_t decode(void* data, const String& b64);

// decode the data from the line b64 length len into itself
size_t decode(void* b64);
size_t decode(void* b64, size_t len);
```

### Unicode
A line decoder containing unicode characters of the form`\u0abc`. It also makes unescape symbols`\t\r\n`!
```cpp
// decode a string with unicode characters to itself (does not add '\0' to the end)
size_t decodeSelf(char* str);
size_t decodeSelf(char* str, size_t len);

// decode a string with unicode characters
String decode(const char* str);
String decode(const char* str, size_t len);

// decode a string with unicode characters
String decode(const String& str);

// encode the unicode symbol by its code. There should be 5 cells in the array.
uint8_t encode(char* str, uint32_t c, bool terminate = true);

// code the unicode symbol by its code
String encode(uint32_t code);
```

### URL
```cpp
// The symbol must be urlencoded
bool needsEncode(char c);

// urlencoded
size_t encodedLen(const char* str);
size_t encodedLen(const char* str, size_t len);

// encode in char[encodedLen()] (does not add '\0' to the end)
size_t encode(char* url, const char* str);
size_t encode(char* url, const char* str, size_t len);

// string
void encode(String* url, const char* str);
void encode(String* url, const char* str, size_t len);
void encode(String& url, const char* str);
void encode(String& url, const char* str, size_t len);

String encode(const char* str);
String encode(const char* str, size_t len);

String encode(const String& str);

// line length
size_t decodedLen(const char* url);
size_t decodedLen(const char* url, size_t len);

// decode url (does not add '\0' to the end)
size_t decode(char* str, const char* url);
size_t decode(char* str, const char* url, size_t len);

// string
size_t decode(String* str, const char* url);
size_t decode(String* str, const char* url, size_t len);
size_t decode(String& str, const char* url);
size_t decode(String& str, const char* url, size_t len);

String decode(const char* url);
String decode(const char* url, size_t len);
String decode(const String& url);

// decode the url into itself
size_t decodeSelf(char* url);
size_t decodeSelf(char* url, size_t len);
```

### Length
```cpp
// StringLength line length, executed at the compilation stage
constexpr size_t SL(const char* str);
constexpr size_t operator"" _SL;  // C++ 11
```

Examples
```cpp
int L1 = SL("text");
int L2 = "text"_SL;
```

### Hash
Instead of comparing strings, you can compare the hash of these strings, which makes the program smaller, lighter, and in most cases faster. The functions listed below as "considered a compiler" are considered a compiler, that is, ** the string does not even fall into the program code** - a hash number is substituted instead:

```cpp
// compiler
constexpr size_t SH(const char* str);           // (String Hash) size t
constexpr size_t SH32(const char* str);             // (String Hash) size 32 bits

constexpr size_t operator"" _h;                     // C++ 11
constexpr size_t operator"" _h32;                   // C++ 11

// rent-time
size_t su::hash(const char* str, int16_t len = -1);     // The size depends on the platform and corresponds to size t.
uint32_t su::hash32(const char* str, int16_t len = -1); // 32-bit size

size_t su::hash_P(PGM_P str, int16_t len = -1);         // PROGMEM string, size t
uint32_t su::hash32_P(PGM_P str, int16_t len = -1);     // Progm string, size 32 bits
```

New macro (starting with v1.4.30 library)
```cpp
H(key);  // is equivalent to SH ("key") and "key" h
```

> On ESP boards`_SH`, `SH`, `hash`and`hash_P`The default is 32-bit!

According to the test, the 32-bit version of the hash has 7 collisions out of 234,450 English words, the 16-bit version - 170723 collisions (that is 73% - a purely statistical number of collisions from the calculation of 16 bits - 65,536 values)

#### Example
Determine which line was "received". The classic way to compare lines from PROGMEM:

```cpp
char buf[] = "some_text";

if (!strcmp_P(buf, PSTR("abcdef"))) Serial.println(0);
else if (!strcmp_P(buf, PSTR("12345"))) Serial.println(1);
else if (!strcmp_P(buf, PSTR("wrong text"))) Serial.println(2);
else if (!strcmp_P(buf, PSTR("some text"))) Serial.println(3);
else if (!strcmp_P(buf, PSTR("hello"))) Serial.println(4);
else if (!strcmp_P(buf, PSTR("some_text"))) Serial.println(5);
```

The line hash method:
```cpp
char buf[] = "some_text";

switch (hash(buf)) {
    case SH("abcdef"):      Serial.println(0); break;
    case SH("12345"):       Serial.println(1); break;
    case SH("wrong text"):  Serial.println(2); break;
    case "some text"_h:     Serial.println(3); break;
    case "hello"_h:         Serial.println(4); break;
    case H(some_text):      Serial.println(5); break;
}
```
> One hash calculation takes a little longer than a comparison with a string. But the final design from the example is performed 2 times faster (on ESP).

> `SH("строки")`and`"строки"_h`in this example, do not fall into the code of the program at all - instead of them, their hash is substituted

### Other utility
```cpp
// Line length with Russian symbols
uint16_t su::strlenRu(const char* str);

// Get the length of a whole number
uint8_t su::intLen(int32_t val);

// Get the length of the float number
uint8_t su::floatLen(double val, uint8_t dec);

// Convert a string to an integer
template <typename T>
T su::strToInt(const char* str, uint8_t len = 0);

// Convert PROGMEM string to an integer
template <typename T>
T su::strToInt_P(const char* str, uint8_t len = 0);

// Convert float to a line with the number of characters after the point
uint8_t su::floatToStr(double val, char* buf, uint8_t dec = 2);

// Convert the HEX string to an integer. Optionally length
uint32_t su::strToIntHex(const char* str, int8_t len = -1);

// The length of the symbol in the number of bytes
uint8_t su::charSize(char sym);

// Convert number to char* array. Writes from the beginning of the array, adds '\0', returns the length of the line
// For int64 max. buffer length - 22 characters, for int32 - 12
uint8_t su::uintToStr(uint32_t n, char* buf, uint8_t base = DEC);
uint8_t su::intToStr(int32_t n, char* buf, uint8_t base = DEC);
uint8_t su::uint64ToStr(uint64_t n, char* buf, uint8_t base = DEC);
uint8_t su::int64ToStr(int64_t n, char* buf, uint8_t base = DEC);

// floating
float su::strToFloat(const char* s);

// Convert from PROGEMEM string to float
float su::strToFloat_P(PGM_P s);

// fast integer logarithm 10 (length of number in number of characters)
uint8_t su::getLog10(uint32_t value);
uint8_t su::getLog10(int32_t value);

// elevation
uint32_t su::getPow10(uint8_t value);
```

<a id="versions"></a>

## Versions
- v1.0
- v1.1.0 - Optimization, added features, fixed vulnerabilities
- v1.2 - hash functions added
- v1.2.x - minor corrections and improvements
- v1.3 - Const optimization, features added, comparisons
- v1.3.1 - Substring added
- v1.3.2 Supports ESP8266 versions 2. xx
- v1.3.5 - uintToStr: HEX is now lowercase as C functions
- v1.3.6 - toInt32HEX(), count(), split() and getSub() are added to Text. Added TextList parser
- v1.3.7 - Warnings fixed on AVR
- v1.4.0 - AnyText is renamed Text, sutil is renamed su, features are added to Text, conversion result and substring are given to C and JS function n
- 1.4.1 
  - optimized for split
  - You can split into an array of pointers into variables.
  - TextParser added
  - StringExt and StringStatic builders added
- 1.4.3 - Comparison optimization, added constexpr line length measurement
- 1.4.7 - Fixed split bug for esp32
- 1.4.9 - Optimization, short hash functions added
- 1.4.10 - DecodeUrl and decodeUnicode are added to Text
- 1.4.12 - Unicode tools (substring, indexOf) are added to Text
- 1.4.15 - minor improvements, partially "ubran" su prefix, the text added correct output in si-strings with a temporary buffer

<a id="install"></a>

## Installation
- The library can be found by the name **StringUtils** and installed through the library manager in:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Download the library](https://github.com/GyverLibs/StringUtils/archive/refs/heads/main.zip).zip archive for manual installation:
    - Unpack and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Unpack and put in *C:\Program Files\Arduino\libraries* (Windows x32)
    - Unpack and put in *Documents/Arduino/libraries/ *
    - (Arduino IDE) Automatic installation from .zip: *Sketch/Connect library/Add .ZIP library...* and specify downloaded archive
- Read more detailed instructions for installing libraries[here](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: new versions fix errors and bugs, as well as optimize and add new features.
- Through the library manager IDE: find the library as when installing and click "Update"
- Manually: **Delete the folder with the old version** and then put the new one in its place. “Replacement” can not be done: sometimes new versions delete files that will remain when replaced and can lead to errors!

<a id="feedback"></a>
## Bugs and feedback
If you find bugs, create **Issue**, or better write to the mail immediately.[alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
The library is open for revision and your **Pull Requests*!

When reporting bugs or incorrect work of the library, it is necessary to specify:
- Library version
- What is used by the IC
- SDK version (for ESP)
- Arduino IDE version
- Are embedded examples that use features and designs that cause bugs in your code working correctly?
- What code was downloaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed. Not a canvas of a thousand lines, but a minimum code.
