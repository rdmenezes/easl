/**
*   \file   findfirstof.h
*   \author Dave Reid
*   \brief  Header file for findfirstof() implementations.
*/
#ifndef __EASL_STRPBRK_H_
#define __EASL_STRPBRK_H_

#include <string.h>
#include "findfirst.h"

namespace easl
{

/**
*   \brief                  Retrieves the first occurance of the specified character in a string.
*   \param  str [in]        The string to search.
*   \param  character [in]  The character to look for.
*   \return                 A pointer in \c str to the fist occurance of the character.
*
*   \remarks
*       This function is functionally equivalent to findfirst(const T *, uchar32_t).
*/
template <typename T>
inline const T * findfirstof(const T *str, uchar32_t character)
{
    return easl::findfirst(str, character);
}

template <typename T>
inline T * findfirstof(T *str, uchar32_t character)
{
    return easl::findfirst(str, character);
}


/**
*   \brief                  Retrieves the first occurance in a string of any character contained in another string.
*   \param  str [in]        Null terminated string to search.
*   \param  charSet [in]    The set of characters to look for in \c str.
*   \return                 A pointer in \c str to the first occurance of any character in \c charSet; or NULL if no characters are found.
*/
template <typename T>
inline const T * findfirstof(const T *str, const T *charSet)
{
    const T *temp = str;

    uchar32_t ch;
    while ((ch = nextchar(temp)) != NULL)
    {
        // Now we need to search through the character set and check if the character
        // is equal to any of those.
        const T *temp_set = charSet;
        uchar32_t ch2;
        while ((ch2 = nextchar(temp_set)) != NULL)
        {
            if (ch2 == ch)
            {
                return str;
            }
        }

        str = temp;
    }

    return NULL;
}
#ifdef EASL_ONLY_ASCII
template <> inline const char * findfirstof(const char *str, const char *charSet)
{
    return ::strpbrk(str, charSet);
}
#endif
template <> inline const wchar_t * findfirstof(const wchar_t *str, const wchar_t *charSet)
{
    return ::wcspbrk(str, charSet);
}

template <typename T>
inline T * findfirstof(T *str, const T *charSet)
{
    return (T *)findfirstof((const T *)str, charSet);
}
#ifdef EASL_ONLY_ASCII
template <> inline char * findfirstof(char *str, const char *charSet)
{
    return ::strpbrk(str, charSet);
}
#endif
template <> inline wchar_t * findfirstof(wchar_t *str, const wchar_t *charSet)
{
    return ::wcspbrk(str, charSet);
}

}

#endif // __EASL_STRPBRK_H_
