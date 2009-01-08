/**
*   \file   strgetchar.h
*   \author Dave Reid
*   \brief  Header file for strgetchar() implementations.
*/
#ifndef __EASL_STRGETCHAR_H_
#define __EASL_STRGETCHAR_H_

#include "strnextchar.h"

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
uchar32_t strgetchar(const T *str, size_t index)
{
    return (uchar32_t)str[index];
}
template <> uchar32_t strgetchar(const char *str, size_t index)
{
    return (uchar32_t)(unsigned char)str[index];
}
template <> uchar32_t strgetchar(const uchar16_t *str, size_t index)
{
    return (uchar32_t)(uchar16_t)str[index];
}
#else
template <typename T>
uchar32_t strgetchar(const T *str, size_t index)
{
    const T *temp = str;

    uchar32_t ch;
    while ((ch = easl::strnextchar(temp)) != NULL)
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
template <> uchar32_t strgetchar(const char32_t *str, size_t index)
{
    return static_cast<uchar32_t>(str[index]);
}
#endif

template <> uchar32_t strgetchar(const wchar_t *str, size_t index)
{
    if (sizeof(wchar_t) == 2)
    {
        return strgetchar((const char16_t *)str, index);
    }
    else if (sizeof(wchar_t) == 4)
    {
        return strgetchar((const char32_t *)str, index);
    }

    return strgetchar((const char *)str, index);
}

}

#endif // __EASL_STRGETCHAR_H_
