#summary Technical Information

= Introduction =
This page will discuss some technical issues with EASL that developers should be aware of.

= `char *` strings =
A `char *` type string is always interpreted as a UTF-8 encoded string. Programs using ASCII-only strings should be able to make use of EASL without any real hassles. On the other hand, calling the `str*()` functions with `char *` strings manipulated with EASL is not very well defined for the time being.

= wchar_t =
The `wchar_t *` style strings aren't sufficient for reliable cross platform Unicode support. On Windows, a `wchar_t` is two bytes, whereas (I think) Linux assumes it to be four bytes.

The way it works in EASL is that the size of a `wchar_t` will determine how EASL assumes a `wchar_t *` string is encoded. When `wchar_t` is 8 bits (not likely), it is assumed that the string is encoded as UTF-8. When `wchar_t` is 16 bits, UTF-16 is assumed. When it is 32 bits, UTF-32 is assumed.

According to [http://www.metagraphics.com/index.htm?page=pubs/mgct_language-portable-code.htm this article], these assumptions should work reliably on Windows platforms. I'm assuming the same can be said for other operating systems.

The table below outlines the assumptions EASL makes on the encoding used by a `wchar_t *` string based on the size of a `wchar_t`.

|| *`sizeof(wchar_t)`* || *UTF Encoding* ||
|| 1                   ||     UTF-8      ||
|| 2                   ||     UTF-16     ||
|| 4                   ||     UTF-32     ||


= Null Termination =
The low level string routines use null termination for determining the position of the end of a string. The null terminator should be the same size as a basic character unit. A "basic character unit" is a `wchar_t` in a `wchar_t *` string, a `char32_t` in a `char32_t *` string, etc.


= Locale =
EASL uses the locale defined by the C standard function `setlocal()` for locale dependent operations. Currently there is only limited support for locales other than English. At the moment EASL doesn't provide it's own `setlocale()` function, but it's likely to have much better support for locales in the future.