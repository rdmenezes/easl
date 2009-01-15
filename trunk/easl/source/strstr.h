/**
*   \file   strstr.h
*   \author Dave Reid
*   \brief  Header file for strstr() implementations.
*/
#ifndef __EASL_STRSTR_H_
#define __EASL_STRSTR_H_

#include <string.h>
#include "strchr.h"
#include "strcmp.h"
#include "getchar.h"

namespace easl
{

/**
*   \brief              Finds a string from within another string.
*   \param  str1 [in]   The string to be scanned.
*   \param  str2 [in]   The string to look for inside \c str1.
*   \return             A pointer to the first occurance of \c str2; or NULL if the string is not found.
*/
template <typename T>
inline const T * strstr(const T *str1, const T *str2)
{
    assert(str1 != NULL);
    assert(str2 != NULL);

    if (*str2 == 0)
    {
        return str1;
    }

    // Grab the length of the second string.
    //size_t str2_length = easl::strlen(str2);
    size_t str2_length = easl::length(str2);

    // Grab the first character of our second string.
    uchar32_t first_char = getchar(str2, 0);

    for ( ; (str1 = easl::strchr(str1, first_char)) != NULL; nextchar(str1))
    {
        if (easl::strcmp(str1, str2, str2_length) == 0)
        {
            return str1;
        }
    }

    return NULL;
}
template <> inline const char * strstr(const char *str1, const char *str2)
{
    // This function should work all the time.
    return ::strstr(str1, str2);
}
template <> inline const wchar_t * strstr(const wchar_t *str1, const wchar_t *str2)
{
    return ::wcsstr(str1, str2);
}


template <typename T>
inline T * strstr(T *str1, const T *str2)
{
    return (T *)strstr((const T *)str1, str2);
}
template <> inline char * strstr(char *str1, const char *str2)
{
    // This function should work all the time.
    return ::strstr(str1, str2);
}
template <> inline wchar_t * strstr(wchar_t *str1, const wchar_t *str2)
{
    return ::wcsstr(str1, str2);
}

}

#endif // __EASL_STRSTR_H_
