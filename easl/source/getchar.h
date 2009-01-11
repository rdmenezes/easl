/**
*   \file   getchar.h
*   \author Dave Reid
*   \brief  Header file for getchar() implementations.
*/
#ifndef __EASL_GETCHAR_H_
#define __EASL_GETCHAR_H_

#include "nextchar.h"

namespace easl
{

/**
*   \brief              Retrieves a character from the specified string.
*   \param  str   [in]  The string to retrieve the character from.
*   \param  index [in]  The zero based index of the character to retrieve.
*   \return             The character at the location specified by the index.
*
*   \remarks
*       If the index is not valid, results are undefined.
*/
#ifdef EASL_ONLY_ASCII
template <typename T>
uchar32_t getchar(const T *str, size_t index)
{
    return (uchar32_t)str[index];
}
template <> uchar32_t getchar(const char *str, size_t index)
{
    return (uchar32_t)(unsigned char)str[index];
}
template <> uchar32_t getchar(const uchar16_t *str, size_t index)
{
    return (uchar32_t)(uchar16_t)str[index];
}
#else
template <typename T>
uchar32_t getchar(const T *str, size_t index)
{
    const T *temp = str;

    uchar32_t ch;
    while ((ch = easl::nextchar(temp)) != NULL)
    {
        size_t cur_pos = temp - str;

        if (cur_pos == index + 1)
        {
            return ch;
        }
        else if (cur_pos > index + 1)
        {
            return 0;
        }
    }

    return 0;
}
template <> uchar32_t getchar(const char32_t *str, size_t index)
{
    return static_cast<uchar32_t>(str[index]);
}
#endif

template <> uchar32_t getchar(const wchar_t *str, size_t index)
{
    if (sizeof(wchar_t) == 2)
    {
        return getchar((const char16_t *)str, index);
    }
    else if (sizeof(wchar_t) == 4)
    {
        return getchar((const char32_t *)str, index);
    }

    return getchar((const char *)str, index);
}

}

#endif // __EASL_GETCHAR_H_
