/**
*   \file   strstr.h
*   \author Dave Reid
*   \brief  Header file for strstr() implementations.
*/
#ifndef __EASL_STRSTR_H_
#define __EASL_STRSTR_H_

#include <string.h>

namespace easl
{

/**
*   \brief              Finds a string from within another string.
*   \param  str1 [in]   The string to be scanned.
*   \param  str2 [in]   The string to look for inside \c str1.
*   \return             A pointer to the first occurance of \c str2; or NULL if the string is not found.
*/
template <typename T>
inline T * strstr(T *str1, const T *str2)
{
    return NULL;
}
#ifdef EASL_ONLY_ASCII
template <> inline char * strstr(char *str1, const char *str2)
{
    return ::strstr(str1, str2);
}
#endif
template <> inline wchar_t * strstr(wchar_t *str1, const wchar_t *str2)
{
    return ::wcsstr(str1, str2);
}

template <typename T>
inline const T * strstr(const T *str1, const T *str2)
{
    return NULL;
}
#ifdef EASL_ONLY_ASCII
template <> inline const char * strstr(const char *str1, const char *str2)
{
    return ::strstr(str1, str2);
}
#endif
template <> inline const wchar_t * strstr(const wchar_t *str1, const wchar_t *str2)
{
    return ::wcsstr(str1, str2);
}

}

#endif // __EASL_STRSTR_H_
