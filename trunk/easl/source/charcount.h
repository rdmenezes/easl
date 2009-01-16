/**
*   \file   charcount.h
*   \author Dave Reid
*   \brief  Header file for chrcount() implementations.
*/
#ifndef __EASL_CHARCOUNT_H_
#define __EASL_CHARCOUNT_H_

#include "nextchar.h"
#include "length.h"

namespace easl
{

/**
*   \brief              Retrieves the number of characters in the given string.
*   \param  str [in]    The string to check.
*   \return             The number of characters that make up the string; or 0 if an error occurs.
*
*   \remarks
*       The input string must be null terminated. The null terminator should be the same size
*       as that of the strings internal character.
*       \par
*       This function does not return the number of bytes in the string. Rather, it calculates
*       the number of characters in the string, not including the null terminator.
*/
template <typename T>
size_t charcount(const T *str)
{
#ifdef EASL_ONLY_ASCII
    return easl::length(str);
#else
    size_t count = 0;
    while (easl::nextchar(str) != NULL)
    {
        ++count;
    }

    return count;
#endif
}

template <typename T>
size_t charcount(const reference_string<T> &str)
{
#ifdef EASL_ONLY_ASCII
    return easl::length(str);
#else
    size_t count = 0;

    T *temp = str.start;
    while (temp < str.end)
    {
        ++count;

        nextchar(temp);
    }

    return count;
#endif
}

}

#endif // __EASL_CHARCOUNT_H_
