#summary EASL usage examples

= Introduction =
This page will show some examples of how to use EASL.


= Example 1: Simple Operations =
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


= Example 2: Simple Conversion =
This example demonstrates EASL's ability to convert between different types of strings. In this example, we do a simple conversion between an archaic ANSI `char *` string and a `wchar_t *` string. This type of conversion is convienient if some system function expects the string as a `wchar_t *`.

Code:
{{{
#include <easl/easl.h>
#include <iostream>

int main()
{
    const char *source = "Hello, World!";

    size_t size = easl::copysize<wchar_t>(source);
    wchar_t *dest = new wchar_t[size];
    easl::copy(dest, source);

    std::wcout << L"Converted String: " << dest << std::endl;

    delete [] dest;

    return 0;
}
}}}

Output:
{{{
Converted String: Hello, World!
}}}

Here we are converting a `char *` string ("Hello, World!") to a `wchar_t *` string. We first need to determine how many `wchar_t`'s we need to allocate in order to store the converted string with a null terminator using `easl::copysize()`. This is a function template that needs the type we are converting to to be specified explicitly.

Once the memory for the new string has been allocated, we can perform the actual conversion with `easl::copy()`.

As you can see, copying a string with `easl::copy()` will implicitly convert the string to the appropriate format. Also, you can use `easl::copysize()` to determine the size required for a buffer for a regular string copy.


= Example 3: Simple UTF Operations =
This example demonstrates some simple operations for working with UTF encoded strings. It demonstrates some useful operations for working with UTF encoded strings such as iterating through each character in the string.

Code:
{{{
#include <easl/easl.h>
#include <iostream>

int main()
{
    // UTF-8 string. Compatible with ASCII.
    const char *str = "I'm a UTF-8 string!";

    // Iterate through each character.
    const char *temp = str;
    easl::char32_t ch;
    while ((ch = easl::nextchar(temp)) != NULL)
    {
        std::cout << static_cast<char>(ch);
    }

    std::cout << std::endl;

    std::cout << "Number of char's in str1: " << 
        easl::length(str) << std::endl;

    std::cout << "Number of characters in str1: " << 
        easl::charcount(str) << std::endl << std::endl;

    return 0;
}
}}}

Output:
{{{
I'm a UTF-8 string!
Number of char's in str1: 19
Number of characters in str1: 19
}}}

Here we take a UTF-8 encoded string and perform some operations on it. The first thing we do is iterate through each character in the string with `easl::strnextchar()`. We need to create a temporary pointer for the iterator so that we can reference the original string reliably.

We then demonstrate how the number of basic character units can be retrieved as well as the number of characters. `easl::length()` will always return the number of character units, and not necessarily the number of characters. `easl::charcount()` will always return the number of characters. In this example, a "basic character unit" is a `char`; for a `char16_t *` string, this would be a `char16_t`, etc.

The beauty of this example is that the original string can be changed to any other UTF encoding (UTF-16 or UTF-32) and the program will still work without any modifications to the rest of the code.


= Example 4: Simple UTF Conversion =
This example will show how easy it is to convert a string between different UTF character encodings and then back to a `wchar_t *` string.

Code:
{{{
#include <easl/easl.h>
#include <iostream>

int main()
{
    // UTF-8 string. Compatible with ASCII.
    const char *source = "Hello, World!";

    // Convert to UTF-16
    easl::char16_t *dest = 
        new easl::char16_t[easl::copysize<easl::char16_t>(source)];
    easl::copy(dest, source);

    // Now convert back to wchar_t.
    wchar_t *final_dest = new wchar_t[easl::copysize<wchar_t>(dest)];
    easl::copy(final_dest, dest);

    std::wcout << L"String: " << final_dest << std::endl;

    delete [] dest;
    delete [] final_dest;

    return 0;
}
}}}

Output
{{{
String: Hello, World!
}}}

This example first converts a UTF-8 encoded string to a UTF-16 string. It then converts the UTF-16 string to a traditional `wchar_t` string. We can see that the process for converting between UTF encodings is the same as converting between `char *` and `wchar_t *` strings.


= Example 5: String Class =
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