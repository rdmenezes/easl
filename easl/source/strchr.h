/**
*   \file   strchr.h
*   \author Dave Reid
*   \brief  Header file for strchr() implementations.
*/
#ifndef __EASL_STRCHR_H_
#define __EASL_STRCHR_H_

#include <string.h>
#include "types.h"

namespace easl
{

/**
*   \brief                  Finds the first occurance of a character inside a string.
*   \param  str       [in]  The string to look for the character in.
*   \param  character [in]  The character to look for.
*   \return                 If the character is found, returns a pointer to that character. Otherwise, a NULL pointer is returned.
*/
template <typename T>
inline T * strchr(T *str, uchar32_t character)
{
    T *tmp = str;

    uchar32_t ch;
    while ((ch = easl::strnextchar(str)) != character && ch != NULL)
    {
        str = tmp;
    }

    return str;
}
#ifdef EASL_ONLY_ASCII
template <> inline char * strchr(char *str, uchar32_t character)
{
    return ::strchr(str, character);
}
#endif
template <> inline wchar_t * strchr(wchar_t *str, uchar32_t character)
{
    return ::wcschr(str, (wchar_t)character);
}

template <typename T>
inline const T * strchr(const T *str, uchar32_t character)
{
    const T *tmp = str;

    uchar32_t ch;
    while ((ch = easl::strnextchar(tmp)) != character && ch != NULL)
    {
        str = tmp;
    }

    return str;
}
#ifdef EASL_ONLY_ASCII
template <> inline const char * strchr(const char *str, uchar32_t character)
{
    return ::strchr(str, character);
}
#endif
template <> inline const wchar_t * strchr(const wchar_t *str, uchar32_t character)
{
    return ::wcschr(str, (wchar_t)character);
}

}

#endif // __EASL_STRCHR_H_
