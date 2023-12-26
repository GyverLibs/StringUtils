This is an automatic translation, may be incorrect in some places. See sources and examples!

# Stringutils
A set of tools for working with lines
- Fast conversion functions
- Parsing, dividing by dividers
- Several classes-converters of data into a line and vice versa for use in other libraries
- Coding and decoding Base64, URL, Unicode, Yzukeen/QWERTYS BACK

## compatibility
Compatible with all arduino platforms (used arduino functions)

## Content
- [documentation] (#docs)
- [versions] (#varsions)
- [installation] (# Install)
- [bugs and feedback] (#fedback)

<a id="docs"> </a>

## Documentation
### Anytext
Bug class for all types of lines:
- `" Consta Char "` - lines
- `char []` - lines
- `f (" f-string ")`
- `proGMEM` - lines
- `string` - lines

Peculiarities:
- cannot change the original line, all operations are only "for reading"
- In all cases, except `Anytext (String (" line "))` ** does not create a copy of the line ** and works with the original line, i.e.The original line should be in memory during the existence of Anytext
- allows you to print and convert to any integer format, as well as compare with any other lines
- It stores the length of the line for quick calculations and comparisons.The length is considered not when creating a line, but with the first call `Length ()`

`` `CPP
// Designer
Sutil :: Anytext (String & Str);
Sutil :: Anytext (Constation String & Str);
Sutil :: Anytext (const __flashstringhelper* str, int16_t len = 0);
Sutil :: Anytext (const chaar* str, bool pgm = 0, int16_t len = 0);

// Methods
Bool pgm ();// Line from Flash memory
uint16_t Length ();// Line length
Type type ();// Type of line
const char* str ();// Get the pointer to the line.It will always return the pointer different from Nullptr!
Bool Valid ();// Line status, exists or not
Size_t Printto (Print & P);// Print in Print (from length)

// Compare with the line starting from the index
Bool Compare (Anytext S, Uint16_T from = 0);

// Compare with the line starting from the index, indicating the number of characters
Bool Comparen (Anytext S, Uint16_t Amount, Uint16_t From = 0);

// Find the position of the symbol in the line starting from the index
Int16_t Indexof (char, uint16_t from = 0);

Char charat (Uint16_T IDX);// Get a symbol by index
Char Operator [] (int IDX);// Get a symbol by index

// Bring out a string line.Will return False in case.UDECODE - Decode unicode
Bool Tostring (String & S, Bool UDecode = FALSE);

// Bring out an array.He will add '\ 0' at the end, will return the length of the line
uint16_t Tostr (char* buf, int16_t bufsize = -1);

// Get a string line.UDECODE - Decode unicode
String Tostring (Bool UDECODE = FALSE);

Bool Tobool ();// get a value as bool
int16_t toint16 ();// get a value as int 16
int32_t toint32 ();// get a value as int 32
int64_t toint64 ();// get a value as int64
Float Tofloat ();// get a value as Float

size_t hash ();// Hash lines size_t
uint32_t hash32 ();// Hash lines 32 bits

// automatically converted into
Bool
int8_t
uint8_t
int16_t
uint16_t
int32_t
uint32_t
int64_t
uint64_t
Float
Double
COST Char*
String

// automaticCranberries and compared with
COST Char*
const __flashstringhelper*
String
`` `

### Anyvalue
The additive to `Anytext` supports all other standard data types.It has a 22 byte buffer, when creating it converts the number into it:
`` `CPP
Sutil :: Anyvalue (Char Value);
Sutil :: Anyvalue (Bool Value);
Sutil :: Anyvalue (int8_t value, uint8_t base = dec);
Sutil :: Anyvalue (Uint8_t Value, Uint8_t Base = Dec);
Sutil :: Anyvalue (Int16_t Value, Uint8_t Base = Dec);
Sutil :: Anyvalue (Uint16_t Value, Uint8_t Base = Dec);
Sutil :: Anyvalue (int32_t value, uint8_t base = dec);
Sutil :: Anyvalue (Uint32_T Value, Uint8_t Base = Dec);
Sutil :: Anyvalue (int64_t value, uint8_t base = dec);
Sutil :: Anyvalue (Uint64_t Value, Uint8_t Base = Dec);
Sutil :: Anyvalue (Double Value, Uint8_t Dec = 2);

// similarly with a manual sizes of the buffer
Sutil :: Anyvaluet <Buffer size> ();
`` `

#### Example
`` `CPP
Sutil :: Anyvalue V0 ("-123456");// all lines can also be
Sutil :: Anyvalue V1 (123);
Sutil :: Anyvalue V2 (3.14);
Sutil :: Anyvalue V3 ((Uint64_T) 12345678987654321);

// conversion from among the text
v1 = 10;
v1 = 3.14;
V1 = 12345654321ULL;

Serial.println (v0);// printed in Serial
Serial.println (v1 == v2);// compared

// can be compared with any lines
Sutil :: Anytext S ("123");
String ss = "123";
Serial.println (s == "123");
Serial.println (s == f ("123"));
Serial.println (s == SS);

// Conversion in any type
Float F = V2;// f == 3.14
int i = v1;// I = 123

// Conclusion in String
String S;
v0.tostring (s);

// Conclusion in Char []
Char buf [v1.length () + 1];// +1 for '\ 0'
v1.tostr (buf);
`` `

### Parser
Separation of the line into tips according to the separator in the cycle.** Changes ** the initial line, but after completing it returns the separators to the place.

`` `CPP
sutil :: Parser p (char* str, char div = ';');

Bool Next ();// Parish the next tuning.Will return FALSE if Parsing is over
uint8_t index ();// current tuning index
const char* str ();// Get tuning
Anytext get ();// Get tuning as anytext
`` `

#### Example
`` `CPP
Char Buf [] = "123; 456; ABC";

Sutil :: Parser P (BUF);
While (p.next ()) {
    Serial.print (P.index ());
    Serial.print (":");
    Serial.println (P.Get ());
}
`` `

## SPLITER
Separation of the line into tips according to the separator in the cycle.** Changes ** the original line!After removing the object, the line is restored, or manually call `RESTORE ()`
`` `CPP
Sutil :: Splittert <Max.Settings> SPL (char* str, char div = ';');
Sutil :: Splitter Spl (Char* Str, Char Div = ';');// Auto-size (released in Heap)

VOID restore ();// Restore the line (return the dividers)
uint8_t leength ();// Number of tuning
const char* str (uint16_t IDX);// Get the tuning on the index
Anytext Get (Uint16_T IDX);// Get a setting on the index as anytext
`` `

#### Example
`` `CPP
Char Buf [] = "123; 456; ABC";

Sutil :: Splitter Spl (BUF);
for (uint8_t i = 0; i <spl.length (); i ++) {
    Serial.print (i);
    Serial.print (":");
    Serial.println (spl [i]);
}

Spl.Restore ();
`` `

## list functions
`` `CPP
// Get the number of tuning in the list
uint16_t Sutil :: list :: Length (Anytext List, Char Div = ';');

// Get the tuning index in the list
Int16_T Sutil :: list :: indexof (Anytext List, Anytext str, char div = ';');

// Checking whether the list contains the list
Bool Sutil :: List :: includes (Anytext List, Anytext Str, Char Div = ';');

// Get a setting from the list on the index
Anytext Sutil :: list :: Get (Anytext List, Uint16_T IDX, Char Div = ';');

// Parrow in an array of the specified type and size.Will return the number of recorded tuning
TEMPLATE <TYPENAME T>
uint16_t Sutil :: list :: Parse (Anytext List, T* Buf, Uint16_t Len, Char Div = ';');
`` `

#### Example
`` `CPP
Serial.println (sutil :: list :: Length ("123; 456; 333"));// 3
Serial.println (Sutil :: List :: includes ("123; 456; 333", "456"));// True
Serial.println (Sutil :: list :: indexof ("123; 456; 333", "333"));// 2
Serial.println (sutil :: list :: get ("123; 456; 333", 1));// 456

// Parish in an array
Float Arr [3];
Sutil :: list :: parse ("3.14; 2.54; 15.15"), Arr, 3);
`` `

## list class
Obtaining a set -on by dividers ** without modifying the original line ** also works with Progmem lines.
`` `CPP
Sutils :: List List (any line);

// Install the separator
VOID Setdiv (Char Div);

// get the size of the list
uint16_t Length ();

// Get the tuning index in the list or -1 if it is not
Int16_T Indexof (Anytext str);

// Check the availability of tuning in the list
Bool Includes (Anytext str);

// Get the tuning under the index
Anytext Get (Uint16_T IDX);

// Parrow in an array of the specified type and size.Will return the number of recorded tuning
TEMPLATE <TYPENAME T>
uint16_t Parse (t* buf, uint16_t len);
`` `

#### Example
`` `CPP
Sutil :: List List (F ("123; 456; 333"));
Serial.print ("len:");
Serial.println (list.length ());// 3
Serial.print ("index of '456':");
Serial.println (list.indexof (f ("456")));// 1
Serial.print ("Index of '789':");
Serial.println (List.indexof ("789"));// -1

// rewrite in an array
int arr [3];
List.parse (Arr, 3);
`` `

### Printstring
`` `CPP
// line in which you can make Print/Println
Sutil :: Printstring PRS;
PRS += "As a regular string";

PRS.Print (10);
PRS.println ("Hello");
PRS.Print (F ("Print!"));

Serial.println (PRS);
`` `

## qwerty
`` `CPP
// change the layout (ru in Qwerty) - String
String Sutil :: toqwerty (Const String & Ru);

// Change the layout (ru in QWERTY) - Char* (QW length as ru + 1, the function will add '\ 0')
Char* sutil :: toqwerty (const char* ru, char* qw);
`` `

### Base64
`` `CPP
// The size of coded data in terms of source
Uint16_T Sutil :: B64 :: Encodedlen (Uint16_t Len);

// Future size of decoded data on line b64 and its length
uint16_t Sutil :: b64 :: decodedlen (constel* b64, uint16_t len);

// encode data in string
VOID Sutil :: B64 :: Encode (String* B64, Uint8_T* DATA, UINT16_T LEN, BOL PGM = FALSE);

// encode data in char [] (the library does not add '\ 0' to the end)
VOID Sutil :: B64 :: Encode (Char* B64, Uint8_T* DATA, Uint16_T LEN, BOL PGM = FALSE);

// Express data from line B64 to DATA buffer
VOID Sutil :: b64 :: decode (uint8_t* Data, Consta Char* b64, uint16_t len);
VOID Sutil :: b64 :: decode (uint8_t* Data, Constance String & B64);
`` `

### Unicode
Decider of a line containing unicode symbols of the type `\ u0abc`.Also makes UNESCAPE symbols `\ t \ r \ n`!
`` `CPP
// Decoding the line. Remember the line for LEN length.Otherwise - along the length of the line
String Sutil :: Unicode :: Decode (Consta Char* str, uint16_t len = 0);

// Decoding the line
String Sutil :: Unicode :: Decode (Constation String & Str);

// Cod the Unicode symbol by its code.There should be 5 cells in the array
Void Sutil :: Unicode :: Encode (char* str, uint32_t c);

// Coding Unicode symbol by its code
String Sutil :: Unicode :: Encode (uint32_t code);
`` `

### url
`` `CPP
// Symbol should be urlencode
Bool Sutil :: url :: Needsencode (char c);

// encode in the URL
VOID Sutil :: url :: Encode (Const String & SRC, String & Dest);
String Sutil :: url :: Encode (Const String & SRC);

// Report the URL
VOID Sutil :: url :: decode (Const String & SRC, String & Dest);
String Sutil :: url :: decode (Const String & SRC);
`` `

### Hash
Instead of comparing the lines, you can compare the hash of these lines, which makes the program more compact, lighter and in most cases faster.The functions indicated below as “is considered a compiler” are considered a compiler, that is, ** the line does not even get into the program ** - a hash -piece is substituted instead:

`` `CPP
// is considered a compiler
Contexpr Sutil :: Size_T SH (COST Char* str);// (String Hash) Size_t size
Contexpr Sutil :: Size_t SH32 (COST Char* str);// (String Hash) Size 32 bits

// is considered in the radim
Size_t Sutil :: Hash (const char* str, int16_t len = -1);// size depends on the platform and corresponds to size_t
uint32_t Sutil :: hash32 (const char* str, int16_t len = -1);// size 32 bits

Size_t Sutil :: hash_p (pgm_p str, int16_t len = -1);// Progmem line, size size_t
uint32_t Sutil :: hash32_p (pgm_p str, int16_t len = -1);// ProgmemMEM line, pAzmer 32 bits
`` `

> On the ESP board `sh`,` hash` and `hash_p` are 32-bit by default!

The 32 -bit version of the hash has 7 collisions of 234450 English words, 16 -bit version - 170723 collisions (which is 73% - a purely statistical number of conflicts from the calculation of 16 bits - 65536)

#### Example
Determine which line was "received".Classic method with comparing rows from Progmem:

`` `CPP
Char buf [] = "some_text";

if (! strcmp_p (buf, pstr ("abcDef"))) serial.println (0);
Else if (! Strcmp_p (BUF, PSTR ("12345"))) serial.println (1);
Else if (! Strcmp_p (BUF, PSTR ("Wrong Text"))) serial.println (2);
Else if (! Strcmp_p (BUF, PSTR ("SOME Text"))) serial.println (3);
Else if (! Strcmp_p (BUF, PSTR ("HELLO"))) serial.println (4);
Else if (! Strcmp_p (BUF, PSTR ("SOME_TEXT"))) serial.PRINTLN (5);
`` `

Method with Hash Line:
`` `CPP
USing Sutil :: SH;
Using Sutil :: Hash;

Char buf [] = "some_text";

switch (hash (buf)) {
    Case SH ("ABCDEF"): serial.println (0);Break;
    Case SH ("12345"): serial.println (1);Break;
    Case SH ("Wrong Text"): serial.println (2);Break;
    Case SH ("Some Text"): Serial.println (3);Break;
    Case SH ("Hello"): serial.println (4);Break;
    Case SH ("SOME_TEXT"): Serial.Println (5);Break;
}
`` `
> One calculation of the hash takes a little more time than a comparison with a line.But the final design from the example is 2 times faster (on ESP).

> `SH (" lines ")` in this example do not fall into the program code at all - their hash is substituted instead of them

### other utilities
`` `CPP
// Line length with Russian symbols
uint16_t Sutil :: strlenru (const chaar* str);

// Get the length of the whole number
uint8_t Sutil :: intlen (int32_t val);

// Get the length of Float numbers
Uint8_t Sutil :: Floatlen (Double Val, Uint8_t Dec);

// Transform the line into a whole number
TEMPLATE <TYPENAME T>
T Sutil :: Strtoint (Consta Char* str, uint8_t len = 0);

// Transforming ProgmemMem a line into a whole number
TEMPLATE <TYPENAME T>
T Sutil :: Strtoint_p (const char* str, uint8_t len = 0);

// Transform Float into a line indicating the number of signs after the point
uint8_t Sutil :: floattostr (Double val, char* buf, uint8_t Dec = 2);

// Transform HEX line into a whole number.Optional length
Uint32_T Sutil :: Strtointhex (const char* str, int8_t len = -1);

// The length of the symbol in the number of bytes
Uint8_t Sutil :: Charsize (Char Sym);

// Conversion of the number in Char* array.Writes from the beginning of the array, adds '\ 0', will return the length of the line
// for int64 max.Boofer length - 22 characters, for Int32 - 12
uint8_t Sutil :: uinttostr (uint32_t n, char* buf, uint8_t base = dec);
uint8_t Sutil :: inttostr (int32_t n, char* buf, uint8_t base = dec);
uint8_t Sutil :: uint64tostr (uint64_t n, char* buf, uint8_t base = dec);
uint8_t Sutil :: int64tostr (int64_t n, char* buf, uint8_t base = dec);

// Convertation from a line in Float
Float Sutil :: Strtofloat (Consta Char* S);

// Convertation from Progemem Lines to Float
Float Sutil :: Strtofloat_p (pgm_p s);

// Quick integer logarithm 10 (length of the number in the number of symbols)
uint8_t Sutil :: getlog10 (uint32_t value);
uint8_t Sutil :: getlog10 (int32_t value);

// quick construction 10 to degree
uint32_t Sutil :: getpow10 (uint8_t value);
`` `

<a id="versions"> </a>

## versions
- V1.0
- v1.1.0 - optimization, features have been added, vulnerabilities are fixed
- V1.2 - Added hash functions

<a id="install"> </a>

## Installation
- The library can be found by the name ** stringutils ** and installed through the library manager in:
    - Arduino ide
    - Arduino ide v2
    - Platformio
- [download the library] (https://github.com/gyverlibs/stringutils/archive/refs/heads/main.zip) .Zip archive for manual installation:
    - unpack and put in * C: \ Program Files (X86) \ Arduino \ Libraries * (Windows X64)
    - unpack and put in * C: \ Program Files \ Arduino \ Libraries * (Windows X32)
    - unpack and put in *documents/arduino/libraries/ *
    - (Arduino id) Automatic installation from. Zip: * sketch/connect the library/add .Zip library ... * and specify downloaded archive
- Read more undertimid instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: errors and bugs are corrected in the new versions, as well as optimization and new features are added
- through the IDE library manager: find the library how to install and click "update"
- Manually: ** remove the folder with the old version **, and then put a new one in its place.“Replacement” cannot be done: sometimes in new versions, files that remain when replacing are deleted and can lead to errors!

<a id="feedback"> </a>
## bugs and feedback
Create ** Issue ** when you find the bugs, and better immediately write to the mail [alex@alexgyver.ru] (mailto: alex@alexgyver.ru)
The library is open for refinement and your ** pull Request ** 'ow!

When reporting about bugs or incorrect work of the library, it is necessary to indicate:
- The version of the library
- What is MK used
- SDK version (for ESP)
- version of Arduino ide
- whether the built -in examples work correctly, in which the functions and designs are used, leading to a bug in your code
- what code has been loaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed.Not a canvas of a thousand lines, but a minimum code