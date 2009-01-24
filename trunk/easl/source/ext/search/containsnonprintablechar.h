/**
*   \file   containsnonprintablechar.h
*   \author Dave Reid
*   \brief  Header file for the containsnonprintablechar() implementation.
*/
#ifndef __EASL_SEARCH_CONTAINSNONPRINTABLECHAR
#define __EASL_SEARCH_CONTAINSNONPRINTABLECHAR

#include "nextchar.h"
#include "length.h"

namespace easl
{
namespace search
{

/**
*   \brief                 Determines if the specified string contains a non-printable character.
*   \param  str       [in] The string to check for printable characters.
*   \param  strLength [in] The length in T's of the string.
*   \return                True if the string contains a non-printable character; false otherwise.
*
*   \remarks
*       A printable character is a character that has some sort of visual appearance. A printable
*       character is _not_ a tab, space, line-feed, etc. The first 32 ASCII values are not
*       considered printable characters.
*/
template <typename T>
bool containsnonprintablechar(const T *str, size_t strLength = -1)
{
    uchar32_t ch;
    while (strLength > 0 && (ch = nextchar(str)) != NULL)
    {
        if (ch <= ' ')
        {
            return true;
        }
    }

    return false;
}

template <typename T>
bool containsnonprintablechar(const reference_string<T> &str)
{
    return containsnonprintablechar(str.start, length(str));
}

}
}

#endif // __EASL_SEARCH_CONTAINSNONPRINTABLECHAR
