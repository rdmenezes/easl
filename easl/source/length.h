/**
*   \file   length.h
*   \author Dave Reid
*   \brief  Header file for length() implementations.
*/
#ifndef __EASL_STRLEN_H_
#define __EASL_STRLEN_H_

#include <string.h>
#include <assert.h>

namespace easl
{

/**
*   \brief              Retrieves the length of a C style string.
*   \param  str [in]    The string to retrieve the length of.
*   \return             The length of the C style string.
*
*   \remarks
*       This function return the number of basic character units in the string. A
*       "basic character unit" is a 'char' for char* strings and a 'wchar_t' in
*       whcar_t* strings. The same sort of things applies for UTF strings.
*       \par
*       To retrieve the number of characters in the string insteaad of basic character
*       units, use strchrcount() instead.
*/
/*template <typename T>
inline size_t strlen(const T *str)
{
    assert(str != NULL);

    const T *temp = str;

    while (*temp != NULL)
    {
        ++temp;
    }

    return temp - str;
}
template <> inline size_t strlen(const char *str)
{
    assert(str != NULL);

    return ::strlen(str);
}
template <> inline size_t strlen(const wchar_t *str)
{
    assert(str != NULL);

    return ::wcslen(str);
}*/


template <typename T>
inline size_t length(const T *str)
{
    assert(str != NULL);

    const T *temp = str;

    while (*temp != NULL)
    {
        ++temp;
    }

    return temp - str;
}
template <> inline size_t length(const char *str)
{
    assert(str != NULL);

    return ::strlen(str);
}
template <> inline size_t length(const wchar_t *str)
{
    assert(str != NULL);

    return ::wcslen(str);
}


}

#endif // __EASL_STRLEN_H_
