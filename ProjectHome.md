Easy-As String Library is a C++ header library for working with Unicode strings. It's main goal is to simplify the use of different Unicode string encodings in C++.

The library provides support for Unicode strings in UTF-8, UTF-16 and UTF-32 formats. It also supports `wchar_t *` strings with a few assumptions (see [Technical Information](TechnicalInfo.md)).

Both a low level and higher level API is provided. That is, generic implementations of the `strcpy()`, `strlen()`, etc type functions are provided as well as a `std::string`-like string class.

# Features #
Some of the notable features of the library include:

  * Low level C-style string manipulation functions
  * Higher level `std::string`-style string class
  * UTF encoded string manipulation routines
  * Generic API for all string encodings
  * Conversions between different encodings
  * [Extension mechanism](Extensions.md) for providing some not-so-common string routines.

# Non-Features #
Some of the features EASL does not do. If your program needs any of the functionality listed below, EASL is not useful:

  * Non-Unicode string manipulation
    * No support for UCS-2, etc. Only supports UTF-8, UTF-16 and UTF-32. Conversions between UCS and UTF encodings may be implemented in the future.

EASL does not work with any string encoding other than UTF-8, UTF-16 and UTF-32. So, for example, if you need to convert a UTF-16 string to a UCS-2 string, you will need to perform the conversion either manually or via a separate library such as IBM's ICU. A UCS/UTF converter may be implemented in the future.

# Who is EASL For? #
EASL is useful for anyone who needs Unicode support in their applications. Applications can still use EASL for non Unicode functionality. For example, if an application only uses ASCII strings, they can still use EASL for it's miscellaneous string manipulation routines and it's simple interface. UTF-8 strings are fully compatible with ASCII strings, so ASCII-only applications can still make use of EASL.