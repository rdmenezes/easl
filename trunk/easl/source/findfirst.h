/**
*   \file   findfirst.h
*   \author Dave Reid
*   \brief  Header file for findfirst() implementations.
*/
#ifndef __EASL_FINDFIRST_H_
#define __EASL_FINDFIRST_H_

#include <string.h>
#include "getchar.h"
#include "compare.h"
#include "charwidth.h"
#include "nextchar.h"
#include "length.h"

namespace easl
{

/**
*   \brief                 Finds the first occurance of a character inside a string.
*   \param  str       [in] The string to look for the character in.
*   \param  character [in] The character to look for.
*   \param  strLength [in] The length in T's of the string, not including the null terminator.
*   \return                If the character is found, returns a pointer to that character. Otherwise, a NULL pointer is returned.
*/
template <typename T>
T * findfirst(T *str, uchar32_t character, size_t strLength = -1)
{
    T *temp = str;

    uchar32_t ch;
    while (strLength > 0 && (ch = nextchar(temp)) != NULL)
    {
        if (ch == character)
        {
            return str;
        }

        strLength -= temp - str;
        str = temp;
    }

    return NULL;
}

// Optimized cases.
#ifdef EASL_ONLY_ASCII
inline const char * findfirst(const char *str, uchar32_t character)
{
    return ::strchr(str, character);
}
inline char * findfirst(char *str, uchar32_t character)
{
    return ::strchr(str, character);
}
#endif
inline const wchar_t * findfirst(const wchar_t *str, uchar32_t character)
{
    return ::wcschr(str, (wchar_t)character);
}
inline wchar_t * findfirst(wchar_t *str, uchar32_t character)
{
    return ::wcschr(str, (wchar_t)character);
}

template <typename T>
T * findfirst(const reference_string<T> &str, uchar32_t character)
{
    return findfirst(str.start, character, length(str));
}


/**
*   \brief                  Finds a string from within another string.
*   \param  str1       [in] The string to be scanned.
*   \param  str2       [in] The string to look for inside \c str1.
*   \param  str1Length [in] The length in T's of the first string, not including the null terminator.
*   \param  str2Length [in] The length in T's of the first string, not including the null terminator.
*   \return                 A pointer to the first occurance of \c str2; or NULL if the string is not found.
*/
template <typename T>
const T * findfirst(const T *str1, const T *str2, size_t str1Length = -1, size_t str2Length = -1)
{
    assert(str1 != NULL);
    assert(str2 != NULL);

    if (str2Length == 0 || *str2 == 0)
    {
        return str1;
    }

    // We need to know the length of the second string.
    if (str2Length == -1)
    {
        str2Length = length(str2);
    }

    // Grab the first character of our second string.
    uchar32_t first_char = easl::getchar(str2, 0);

    for ( ; (str1 = findfirst(str1, first_char, str1Length)) != NULL; nextchar(str1))
    {
        if (compare(str1, str2, str2Length) == 0)
        {
            return str1;
        }
    }

    return NULL;
}

// Optimized case.
inline const char * findfirst(const char *str1, const char *str2)
{
    // This function should work all the time.
    return ::strstr(str1, str2);
}
inline const wchar_t * findfirst(const wchar_t *str1, const wchar_t *str2)
{
    return ::wcsstr(str1, str2);
}


template <typename T>
T * findfirst(T *str1, const T *str2, size_t str1Length = -1, size_t str2Length = -1)
{
    return (T *)findfirst((const T *)str1, str2, str1Length, str2Length);
}

// Optimized case
inline char * findfirst(char *str1, const char *str2)
{
    // This function should work all the time.
    return ::strstr(str1, str2);
}
inline wchar_t * findfirst(wchar_t *str1, const wchar_t *str2)
{
    return ::wcsstr(str1, str2);
}


template <typename T>
T * findfirst(const reference_string<T> &str1, const reference_string<T> &str2)
{
    return findfirst(str1.start, str2.start, length(str1), length(str2));
}


}

#endif // __EASL_FINDFIRST_H_
