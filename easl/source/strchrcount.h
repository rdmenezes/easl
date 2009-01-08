/**
*   \file   strchrcount.h
*   \author Dave Reid
*   \brief  Header file for strchrcount() implementations.
*/
#ifndef __EASL_CHARACTER_H_
#define __EASL_CHARACTER_H_

#include "strnextchar.h"

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
size_t strchrcount(const T *str)
{
#ifdef EASL_ONLY_ASCII
    return easl::strlen(str);
#else
    size_t count = 0;
    uchar32_t ch;
    while ((ch = easl::strnextchar(str)) != NULL)
    {
        ++count;
    }

    return count;
#endif
}

}

#endif // __EASL_CHARACTER_H_
