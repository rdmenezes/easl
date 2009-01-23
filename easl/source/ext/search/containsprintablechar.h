/**
*   \file   containsprintablechar.h
*   \author Dave Reid
*   \brief  Header file for the containsprintablechar() implementation.
*/
#ifndef __EASL_SEARCH_CONTAINSPRINTABLECHAR
#define __EASL_SEARCH_CONTAINSPRINTABLECHAR

#include "nextchar.h"

namespace easl
{
namespace search
{

/**
*   \brief                 Determines if the specified string contains a printable character.
*   \param  str       [in] The string to check for printable characters.
*   \param  strLength [in] The length in T's of the string.
*   \return                True if the string contains at least one printable character; false otherwise.
*
*   \remarks
*       A printable character is a character that has some sort of visual appearance. A printable
*       character is _not_ a tab, space, line-feed, etc. The first 32 ASCII values are not
*       considered printable characters.
*/
template <typename T>
bool containsprintablechar(const T *str, size_t strLength = -1)
{
    uchar32_t ch;
    while (strLength > 0 && (ch = nextchar(str)) != NULL)
    {
        if (ch > ' ')
        {
            return true;
        }
    }

    return false;
}

}
}

#endif // __EASL_SEARCH_CONTAINSPRINTABLECHAR
