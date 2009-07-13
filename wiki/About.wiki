#summary About Easy-As String Library
#labels Featured

= Introduction =
Easy-As String Library (EASL) is a C++ header library for working with strings.

It's primary goal is to ease the pain with using Unicode strings in C++. It has support for UTF-8, UTF-16 and UTF-32 string encodings.


= Features =
Some of the notable features of the library include:

  * Easy to use
  * Low level C-style string manipulation functions
  * Higher level `std::string`-style string class
  * UTF encoded string manipulation routines
  * Generic API for all string encodings
  * Conversions between different encodings
  * [Extensions Extension mechanism] for providing some not-so-common string routines.

= Non-Features =
Some of the features EASL does not do. If your program needs any of the functionality listed below, EASL is not useful:

  * Non-Unicode string manipulation
      * No support for UCS-2, etc. Only supports UTF-8, UTF-16 and UTF-32. Conversions between UCS and UTF encodings may be implemented in the future.

EASL does not work with any string encoding other than UTF-8, UTF-16 and UTF-32. So, for example, if you need to convert a UTF-16 string to a UCS-2 string, you will need to perform the conversion either manually or via a separate library such as IBM's ICU. A UCS/UTF converter may be implemented in the future.

= Who is EASL For? =
EASL is useful for anyone who needs Unicode support in their applications. Applications can still use EASL for non Unicode functionality. For example, if an application only uses ASCII strings, they can still use EASL for it's miscellaneous string manipulation routines and it's simple interface. UTF-8 strings are fully compatible with ASCII strings, so ASCII-only applications can still make use of EASL.


= Differences with IBM's ICU =
[http://www.icu-project.org/ ICU] is a much larger project with a much broader scope than EASL. EASL cares only for UTF-8, UTF-16 and UTF-32 formats. This restricts EASL from only having to worry about the Unicode character set. On the other hand, ICU supports pretty much every character set you can think of (though, they do recommend using Unicode as much as possible). This makes ICU much more heavy-weight that EASL.

ICU also has much better localization support. Localization will slowly be improved in EASL in the future.

ICU is probably a "better" solution at the moment, but this will change as EASL becomes much more stable and adds additional features; in particular better localization.

If you're looking for an easy to use API, then EASL is definitely the way to go. Much of the library is based on templates, so there is only a single generic API for every type of character encoding. For example, converting a UTF-16 string to a UTF-8 string in EASL can be done in a single line:

{{{
easl::string utf8_string(some_utf16_string);
}}}

= Examples =
Below are some simple examples demonstrating the use of EASL. These examples are taken directly from the [Examples] page.

== Example 1: Simple Operations ==
This example demonstrates how to use traditional C-like string manipulation routines for strings.

Code:
{{{
#include <easl/easl.h>
#include <iostream>

int main()
{
    char str1[1024];
    easl::copy(str1, "Hello, World!");

    wchar_t *str2 = new wchar_t[1024];
    easl::copy(str2, L"Hello!", 1024);

    std::cout << "String 1 length:  " << easl::length(str1) << std::endl;
    std::cout << "String 1 content: " << str1 << std::endl << std::endl;

    std::wcout << L"String 2 length:  " << easl::length(str2) << std::endl;
    std::wcout << L"String 2 content: " << str2 << std::endl;

    delete [] str2;

    return 0;
}
}}}

Output
{{{
String 1 length:  13
String 1 content: Hello, World!

String 2 length:  6
String 2 content: Hello!
}}}

Here the generic API is put to use. We can see that the same function is called for both types of strings. So no more `strcpy()`/`wcscpy()` mixing and matching for C-style strings.

We've created one string with a static array and another with a dynamic array. We use the same technique as the standard C++ library by not requiring us to specify the size of the destination buffer for the static array. The size must be explicitly specified for the dynamic array, however.


== Example 5: String Class ==
This simple example demonstrates the use of EASL's `std::string`-like string class.

Code:
{{{
#include <easl/easl.h>
#include <iostream>

int main()
{
    easl::string some_utf8_string("Hello");
    easl::wstring some_wchar_string(L", World!");

    // Append our wchar string to our UTF-8 string.
    some_utf8_string += some_wchar_string;
    
    // Create a UTF-16 and UTF-32 string from our UTF-8 string.
    // Converting between different encodings takes only a single line.
    easl::string16 some_utf16_string(some_utf8_string);
    easl::string32 some_utf32_string(some_utf16_string);
    

    // Output some results.
    std::cout << "some_utf8_string:  " << some_utf8_string << std::endl;
    std::cout << "some_utf16_string: " << some_utf16_string << std::endl;
    std::cout << "some_utf32_string: " << some_utf32_string << std::endl;

    return 0;
}
}}}

Output:
{{{
some_utf8_string:  Hello, World!
some_utf16_string: Hello, World!
some_utf32_string: Hello, World!
}}}

This example demonstrates how easy it is to work with different string encodings when using EASL's string class(es). Here we first declare two strings of different encodings. The second string, which is a `wchar_t *` string internally is appended to a `char *` (UTF-8) string.

The string is then converted to a UTF-16 string, which in turn is converted to a UTF-32 string. The results are then printed to standard output to prove that it works. We can also see how `operator <<` has been overloaded so that our custom strings can be easily printed to standard output and any other `std::basic_ostream`.

In addition, it can be seen that the string classes are quite similar to `std::string`'s (but not exactly the same). The `easl::string` and `easl::wstring` classes use a `char *` and `wchar_t *` string internally, just like `std::string` and `std::wstring`. The UTF-16 string is `easl::string16` and the UTF-32 string is `easl::string32`. A `easl::string8` class is also available which is simply an alias for `easl::string` to match with `easl::string16` and `easl::string32`.