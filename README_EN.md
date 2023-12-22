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

In all cases, except for `Anytext (String (" line "))` does not create a copy of the line and works with the original line, i.e.The original line should be in memory during the existence of Anytext.Allows you to print and convert to any format.

`` `CPP
// Designer
Sutil :: Anytext (String & Str);
Sutil :: Anytext (Constation String & Str);
Sutil :: Anytext (const __flashstringhelper* str, int16_t _len = -1);
Sutil :: Anytext (const chaar* str, bool pgm = 0, int16_t _len = -1);

// Methods
// Line from Flash memory
Bool pgm ();

// Line length
uint16_t Length ();

// Type of line
Type type ();

// Get the pointer to the line
const char* str ();

// Line status
Bool Valid ();

// Print in Print (from length)
Size_t Printto (Print & P);

// ========================== search ==========================================================
// compare with the line
Bool Compare (Constext & S, Uint16_T from = 0);

// Compare with the line indicating the number of characters
Bool Comparen (Constext & S, Uint16_T AMOUNT, UINT16_T FROM = 0);

// Find the position of the symbol in the line
Int16_t Indexof (char, uint16_t from = 0);

// Get a symbol by index
Char charat (Uint16_T IDX);

// Get a symbol by index
Char Operator [] (int IDX);

// Get like string string
String Tostring ();

// Bring out a string line.Will return FALSE in case of failure
Bool Tostring (String & S);

// Bring out an array.He will add '\ 0' at the end!
VOID TOSTR (Char* BUF);

// get a value as bool
Bool Tobool ();

// get a value as int 16
int16_t toint16 ();

// get a value as int 32
int32_t toint32 ();

// get a value as int64
int64_t toint64 ();

// get a value as Float
Float Tofloat ();

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
`` `

### Anyvalue
The additive to `Anytext` supports all other standard data types.It has a 22 byte buffer, when creating it converts the number into it:
`` `CPP
Sutil :: Anyvalue (Consta Char & Value);
Sutil :: Anyvalue (Const Bool & Value);
Sutil :: Anyvalue (Constus Int8_t & Value, Uint8_t Base = Dec);
Sutil :: Anyvalue (Consta Uint8_t & Value, Uint8_t Base = Dec);
Sutil :: Anyvalue (Const16_t & Value, Uint8_t Base = Dec);
Sutil :: Anyvalue (Consta Uint16_t & Value, Uint8_t Base = Dec);
Sutil :: Anyvalue (Consta Int32_T & Value, Uint8_t Base = Dec);
Sutil :: Anyvalue (Consta Uint32_t & Value, Uint8_t Base = Dec);
Sutil :: Anyvalue (Consta Int64_t & Value, Uint8_t Base = Dec);
Sutil :: ANYVALUE (COST UINT64_T & VALUE, UINT8_T BASE =dec);
Sutil :: Anyvalue (const Double & Value, Uint8_t Dec = 2);

// similarly with a manual sizes of the buffer
Sutil :: Anyvaluet <Buffer size> ();
`` `

#### Example
`` `CPP
Sutil :: Anyvalue V0 ("-123456");// all lines can also be
Sutil :: Anyvalue V1 (123);
Sutil :: Anyvalue V2 (3.14);
Sutil :: Anyvalue V3 ((Uint64_T) 12345678987654321);

Serial.println (v0);
Serial.println (v1 == v2);

// CAST
Float F = V2;// f == 3.14
`` `

### Parser
Separation of the line into tips according to the separator in the cycle.** Changes ** the initial line, but after completing it returns the separators to the place.

`` `CPP
sutil :: Parser p (char* str, char div = ';');

// Parish the next tuning.Will return FALSE if Parsing is over
Bool Next ();

// current tuning index
uint8_t index ();

// Get tuning
const char* str ();

// Get tuning as anytext
Anytext get ();
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

// Restore the line (return the dividers)
VOID restore ();

// Number of tuning
uint8_t leength ();

// Get the tuning on the index
const char* str (uint16_t IDX);

// Get a setting on the index as anytext
Anytext Get (Uint16_T IDX);
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
Uint16_T Sutil :: List :: Length (Cost Anytext & List, Char Div = ';');

// Get the tuning index in the list
Int16_T Sutil :: list :: indexof (Cost Anytext & List, const ANYTEXT & STR, Char Div = ';');

// Checking whether the list contains the list
Bool Sutil :: List :: Contains (ConstaText & List, Cost Anytext & Str, Char Div = ';');

// Get a setting from the list on the index
Anytext Sutil :: List :: Get (Constext & List, Uint16_T IDX, Char Div = ';');

// Parrow in an array of the specified type and size.Will return the number of recorded tuning
TEMPLATE <TYPENAME T>
Uint16_T Sutil :: List :: Parse (Constext & List, T* Buf, Uint16_t Len, Char Div = ';');
`` `

#### Example
`` `CPP
Serial.println (sutil :: list :: Length ("123; 456; 333"));// 3
Serial.println (sutil :: list :: containe ("123; 456; 333", "456"));// True
Serial.println (Sutil :: list :: indexof ("123; 456; 333", "333"));// 2
Serial.println (sutil :: list :: get ("123; 456; 333", 1));// 456

// rewrite in an array
Float Arr [3];
Sutil :: list :: parse ("3.14; 2.54; 15.15"), Arr, 3);
`` `

## list class
Obtaining a settings by dividers ** without modifying the original line ** also works with Progmem lines.
`` `CPP
Sutils :: List List (any line);

// Install the separator
VOID Setdiv (Char Div);

// get the size of the list
uint16_t Length ();

// Get the tuning index in the list or -1 if it is not
Int16_T Indexof (Constext & Str);

// Check the availability of tuning in the list
Bool Contains (Constext & Str);

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
PRS += "Howregular string ";

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

### other utilities
`` `CPP
// Line length with Russian symbols
uint16_t Sutil :: strlenru (const chaar* str);

// Get the length of the whole number
uint8_t Sutil :: intlen (int32_t val);

// Get the length of Float numbers
Uint8_t Sutil :: Floatlen (Double Val, Uint8_t Dec);

// Transfer the line into a whole number
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

<a id="install"> </a>

## Installation
- The library can be found by the name ** stringutils ** and installed through the library manager in:
    - Arduino ide
    - Arduino ide v2
    - Platformio
- [download library] (https://github.com/gyverlibs/stringutils/archive/refs/heads/main.zip) .z).IP archive for manual installation:
    - unpack and put in * C: \ Program Files (X86) \ Arduino \ Libraries * (Windows X64)
    - unpack and put in * C: \ Program Files \ Arduino \ Libraries * (Windows X32)
    - unpack and put in *documents/arduino/libraries/ *
    - (Arduino id) Automatic installation from. Zip: * sketch/connect the library/add .Zip library ... * and specify downloaded archive
- Read more detailed instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%BD%D0%BE%BE%BE%BED0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
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