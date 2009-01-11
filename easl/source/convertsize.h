/**
*   \file   convertsize.h
*   \author Dave Reid
*   \brief  Header file for convertsize() implementations.
*/
#ifndef __EASL_STRCONVERTSIZE_H_
#define __EASL_STRCONVERTSIZE_H_

#include "convert.h"
#include "strlen.h"

namespace easl
{

/**
*   \brief              Determines the size that a destination buffer must be to convert the specified string.
*   \param  str [in]    The string to look at to determine the size of the output buffer.
*   \return             The number of T's that must be allocated in order to store a conversion of the input string including the NULL terminator.
*/
template <typename T, typename U>
size_t convertsize(const U *str)
{
    // If we make it here, T and U are the same type.
    return easl::strlen(str) + 1;
}

template <> size_t convertsize<char>(const char16_t *str)
{
    return convert((char *)NULL, str);
}
template <> size_t convertsize<char>(const char32_t *str)
{
    return convert((char *)NULL, str);
}

template <> size_t convertsize<char16_t>(const char *str)
{
    return convert((char16_t *)NULL, str);
}
template <> size_t convertsize<char16_t>(const char32_t *str)
{
    return convert((char16_t *)NULL, str);
}

template <> size_t convertsize<char32_t>(const char *str)
{
    return convert((char32_t *)NULL, str);
}
template <> size_t convertsize<char32_t>(const char16_t *str)
{
    return convert((char32_t *)NULL, str);
}

template <> size_t convertsize<wchar_t>(const char *str)
{
    return convert((wchar_t *)NULL, str);
}
template <> size_t convertsize<wchar_t>(const char16_t *str)
{
    return convert((wchar_t *)NULL, str);
}
template <> size_t convertsize<wchar_t>(const char32_t *str)
{
    return convert((wchar_t *)NULL, str);
}

template <typename T>
size_t convertsize(const wchar_t *str)
{
    if (sizeof(wchar_t) == 2)
    {
        return convertsize<T>((const char16_t *)str);
    }
    else if (sizeof(wchar_t) == 4)
    {
        return convertsize<T>((const char32_t *)str);
    }

    return convertsize<T>((const char *)str);
}

}

#endif // __EASL_STRCONVERTSIZE_H_
