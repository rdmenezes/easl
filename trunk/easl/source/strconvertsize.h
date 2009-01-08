/**
*   \file   strconvertsize.h
*   \author Dave Reid
*   \brief  Header file for converting between different types of strings.
*/
#ifndef __EASL_STRCONVERTSIZE_H_
#define __EASL_STRCONVERTSIZE_H_

#include "strconvert.h"
#include "strlen.h"

namespace easl
{

/**
*   \brief              Determines the size that a destination buffer must be to convert the specified string.
*   \param  str [in]    The string to look at to determine the size of the output buffer.
*   \return             The number of T's that must be allocated in order to store a conversion of the input string including the NULL terminator.
*/
template <typename T, typename U>
size_t strconvertsize(const U *str)
{
    // If we make it here, T and U are the same type.
    return easl::strlen(str) + 1;
}

template <> size_t strconvertsize<char>(const char16_t *str)
{
    return strconvert((char *)NULL, str);
}
template <> size_t strconvertsize<char>(const char32_t *str)
{
    return strconvert((char *)NULL, str);
}

template <> size_t strconvertsize<char16_t>(const char *str)
{
    return strconvert((char16_t *)NULL, str);
}
template <> size_t strconvertsize<char16_t>(const char32_t *str)
{
    return strconvert((char16_t *)NULL, str);
}

template <> size_t strconvertsize<char32_t>(const char *str)
{
    return strconvert((char32_t *)NULL, str);
}
template <> size_t strconvertsize<char32_t>(const char16_t *str)
{
    return strconvert((char32_t *)NULL, str);
}

template <> size_t strconvertsize<wchar_t>(const char *str)
{
    return strconvert((wchar_t *)NULL, str);
}
template <> size_t strconvertsize<wchar_t>(const char16_t *str)
{
    return strconvert((wchar_t *)NULL, str);
}
template <> size_t strconvertsize<wchar_t>(const char32_t *str)
{
    return strconvert((wchar_t *)NULL, str);
}

template <typename T>
size_t strconvertsize(const wchar_t *str)
{
    if (sizeof(wchar_t) == 2)
    {
        return strconvertsize<T>((const char16_t *)str);
    }
    else if (sizeof(wchar_t) == 4)
    {
        return strconvertsize<T>((const char32_t *)str);
    }

    return strconvertsize<T>((const char *)str);
}

}

#endif // __EASL_STRCONVERTSIZE_H_
