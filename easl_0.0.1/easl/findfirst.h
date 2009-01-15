/**
*   \file   findfirstchar.h
*   \author Dave Reid
*   \brief  Header file for findfirstchar() implementations.
*/
#ifndef __EASL_FINDFIRSTCHAR_H_
#define __EASL_FINDFIRSTCHAR_H_

#include <string.h>
#include "nextchar.h"

namespace easl
{

/**
*   \brief                  Finds the first occurance of a character inside a string.
*   \param  str       [in]  The string to look for the character in.
*   \param  character [in]  The character to look for.
*   \return                 If the character is found, returns a pointer to that character. Otherwise, a NULL pointer is returned.
*/
template <typename T>
inline const T * findfirst(const T *str, uchar32_t character)
{
    const T *tmp = str;

    uchar32_t ch;
    while ((ch = easl::nextchar(tmp)) != character && ch != NULL)
    {
        str = tmp;
    }

    return (ch == character) ? str : NULL;
}
#ifdef EASL_ONLY_ASCII
template <> inline const char * findfirst(const char *str, uchar32_t character)
{
    return ::strchr(str, character);
}
#endif
template <> inline const wchar_t * findfirst(const wchar_t *str, uchar32_t character)
{
    return ::wcschr(str, (wchar_t)character);
}


template <typename T>
inline T * findfirst(T *str, uchar32_t character)
{
    return (T *)findfirst((const T *)str, character);
}
#ifdef EASL_ONLY_ASCII
template <> inline char * findfirst(char *str, uchar32_t character)
{
    return ::strchr(str, character);
}
#endif
template <> inline wchar_t * findfirst(wchar_t *str, uchar32_t character)
{
    return ::wcschr(str, (wchar_t)character);
}


/**
*   \brief              Finds a string from within another string.
*   \param  str1 [in]   The string to be scanned.
*   \param  str2 [in]   The string to look for inside \c str1.
*   \return             A pointer to the first occurance of \c str2; or NULL if the string is not found.
*/
template <typename T>
inline const T * findfirst(const T *str1, const T *str2)
{
    assert(str1 != NULL);
    assert(str2 != NULL);

    if (*str2 == 0)
    {
        return str1;
    }

    // Grab the length of the second string.
    size_t str2_length = easl::length(str2);

    // Grab the first character of our second string.
    uchar32_t first_char = getchar(str2, 0);

    for ( ; (str1 = easl::findfirst(str1, first_char)) != NULL; nextchar(str1))
    {
        if (easl::compare(str1, str2, str2_length) == 0)
        {
            return str1;
        }
    }

    return NULL;
}
template <> inline const char * findfirst(const char *str1, const char *str2)
{
    // This function should work all the time.
    return ::strstr(str1, str2);
}
template <> inline const wchar_t * findfirst(const wchar_t *str1, const wchar_t *str2)
{
    return ::wcsstr(str1, str2);
}


template <typename T>
inline T * findfirst(T *str1, const T *str2)
{
    return (T *)findfirst((const T *)str1, str2);
}
template <> inline char * findfirst(char *str1, const char *str2)
{
    // This function should work all the time.
    return ::strstr(str1, str2);
}
template <> inline wchar_t * findfirst(wchar_t *str1, const wchar_t *str2)
{
    return ::wcsstr(str1, str2);
}


}

#endif // __EASL_FINDFIRSTCHAR_H_
