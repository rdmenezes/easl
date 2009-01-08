/**
*   \file   strpbrk.h
*   \author Dave Reid
*   \brief  Header file for strpbrk() implementations.
*/
#ifndef __EASL_STRPBRK_H_
#define __EASL_STRPBRK_H_

#include <string.h>

namespace easl
{

/**
*   \brief                  Retrieves the first occurance in a string of any character contained in another string.
*   \param  str [in]        Null terminated string to search.
*   \param  charSet [in]    The set of characters to look for in \c str.
*   \return                 A pointer in \c str of the first occurance of any character in \c charSet; or NULL if no characters are found.
*/
template <typename T>
inline T * strpbrk(T *str, const T *charSet)
{
    return NULL;
}
#ifdef EASL_ONLY_ASCII
template <> inline char * strpbrk(char *str, const char *charSet)
{
    return ::strpbrk(str, charSet);
}
#endif
template <> inline wchar_t * strpbrk(wchar_t *str, const wchar_t *charSet)
{
    return ::wcspbrk(str, charSet);
}

template <typename T>
inline const T * strpbrk(const T *str, const T *charSet)
{
    return NULL;
}
#ifdef EASL_ONLY_ASCII
template <> inline const char * strpbrk(const char *str, const char *charSet)
{
    return ::strpbrk(str, charSet);
}
#endif
template <> inline const wchar_t * strpbrk(const wchar_t *str, const wchar_t *charSet)
{
    return ::wcspbrk(str, charSet);
}

}

#endif // __EASL_STRPBRK_H_
