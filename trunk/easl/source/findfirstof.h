/**
*   \file   findfirstof.h
*   \author Dave Reid
*   \brief  Header file for findfirstof() implementations.
*/
#ifndef __EASL_FINDFIRSTOF_H_
#define __EASL_FINDFIRSTOF_H_

#include <string.h>
#include "findfirst.h"

namespace easl
{

/**
*   \brief                 Retrieves the first occurance of the specified character in a string.
*   \param  str       [in] The string to search.
*   \param  character [in] The character to look for.
*   \param  strLength [in] The length in T's of the string, not including the null terminator.
*   \return                A pointer in \c str to the fist occurance of the character.
*
*   \remarks
*       This function is functionally equivalent to findfirst(const T *, uchar32_t, size_t).
*/
template <typename T>
inline T * findfirstof(T *str, uchar32_t character, size_t strLength = -1)
{
    return easl::findfirst(str, character, strLength);
}


/**
*   \brief                 Retrieves the first occurance in a string of any character contained in another string.
*   \param  str       [in] Null terminated string to search.
*   \param  charSet   [in] The set of characters to look for in \c str.
*   \param  strLength [in] The length in T's of the string, not including the null terminator.
*   \param  setLength [in] The length in T's of the character set string, not including the null terminator.
*   \return                A pointer in \c str to the first occurance of any character in \c charSet; or NULL if no characters are found.
*/
template <typename T>
const T * findfirstof(const T *str, const T *charSet, size_t strLength = -1, size_t setLength = -1)
{
    const T *temp = str;

    uchar32_t ch;
    while (strLength > 0 && (ch = nextchar(temp)) != NULL)
    {
        // Now we need to search through the character set and check if the character
        // is equal to any of those.
        const T *temp_set = charSet;

        // Stores the start of the next character.
        const T *start_of_ch2 = temp_set;

        size_t temp_length = setLength;
        uchar32_t ch2;
        while (temp_length > 0 && (ch2 = nextchar(temp_set)) != NULL)
        {
            if (ch2 == ch)
            {
                return str;
            }

            temp_length -= temp - str;
            start_of_ch2 = temp_set;
        }

        strLength -= temp - str;
        str = temp;
    }

    return NULL;
}

// Optimized cases.
#ifdef EASL_ONLY_ASCII
inline const char * findfirstof(const char *str, const char *charSet)
{
    return ::strpbrk(str, charSet);
}
#endif
inline const wchar_t * findfirstof(const wchar_t *str, const wchar_t *charSet)
{
    return ::wcspbrk(str, charSet);
}

template <typename T>
T * findfirstof(T *str, const T *charSet, size_t strLength = -1, size_t setLength = -1)
{
    return (T *)findfirstof((const T *)str, charSet, strLength, setLength);
}

// Optimized cases.
#ifdef EASL_ONLY_ASCII
inline char * findfirstof(char *str, const char *charSet)
{
    return ::strpbrk(str, charSet);
}
#endif
inline wchar_t * findfirstof(wchar_t *str, const wchar_t *charSet)
{
    return ::wcspbrk(str, charSet);
}

}

#endif // __EASL_FINDFIRSTOF_H_
