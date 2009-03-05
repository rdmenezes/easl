/**
*   \file   erase.h
*   \author Dave Reid
*   \brief  Header file for erase() implementations.
*/
#ifndef __EASL_ERASE_H_
#define __EASL_ERASE_H_

#include "nextchar.h"
#include "length.h"

namespace easl
{

/**
*   \brief                      Removes a section of a given string.
*   \param  str       [in, out] The string whose section is to be removed.
*   \param  start     [in]      The index of the first character to remove from the string.
*   \param  count     [in]      The number of characters to remove from the string.
*   \param  strLength [in]      The length in T's of the input string.
*
*   \remarks
*       The character at position \c start is also removed.
*/
template <typename T>
inline void erase(T *str, size_t start, size_t count, size_t strLength = -1)
{
    // This assertion checks that we aren't trying to erase past the end of the string.
    // The function won't crash in release builds, but it's good to have this here to
    // ensure correctness.
    assert(start + count <= ((strLength != -1) ? strLength : length(str)));

    // Stores the length of the string before the index of the last character to remove.
    size_t temp_length = start + count;

    // We need to move our input string to the first character that we're removing.
    while (start > 0)
    {
        nextchar(str);
        --start;
    }

    // Now we need to get to the last character that we're removing.
    T *end = str;
    while (count > 0)
    {
        nextchar(end);
        --count;
    }

    // Now we need to write each character after 'end' to the string.
    uchar32_t ch;
    while (strLength - temp_length > 0 && (ch = nextchar(end)) != '\0')
    {
        writechar(str, ch);
    }

    // Null terminate.
    *str = '\0';
}

template <typename T>
inline void erase(reference_string<T> &str, size_t start, size_t count)
{
    return erase(str.start, start, count, length(str));
}

}

#endif // __EASL_ERASE_H_
