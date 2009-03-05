/**
*   \file   replace.h
*   \author Dave Reid
*   \brief  Header file for replace() implementations.
*/
#ifndef __EASL_REPLACE_H_
#define __EASL_REPLACE_H_

#include "nextchar.h"
#include "writechar.h"

namespace easl
{

/**
*   \brief                      Replaces each occurance of a character in a string with another character.
*   \param  str       [in, out] The string that should have it's characters replaced.
*   \param  oldChar   [in]      The character that should be replaced.
*   \param  newChar   [in]      The character that \c oldChar should be replaced with.
*   \param  strSize   [in]      The size in T's of the buffer pointed to by \c str.
*   \param  strLength [in]      The length in T's of the input string, not including the null terminator.
*   \return                     The number of T's that must be allocated in the strings buffer in order to store the modified string.
*
*   \remarks
*       If \c strSize is equal to -1, the function will assume that the buffer is large enough to
*       store the modified string.
*/
template <typename T>
size_t replace(T *str, uchar32_t oldChar, uchar32_t newChar, size_t strSize = -1, size_t strLength = -1)
{
    assert(str != NULL);

    size_t old_width = charwidth<T>(oldChar);
    size_t new_width = charwidth<T>(newChar);

    bool null_terminated = false;
    if (old_width != new_width && strLength == (size_t)-1)
    {
        strLength = length(str);
        null_terminated = true;
    }

    T *temp = str;

    size_t count = 1;

    // We need to loop through each character and replace it.
    uchar32_t ch;
    while (strSize > 1 && strLength > 0 && (ch = nextchar(temp)) != '\0')
    {
        if (ch == oldChar)
        {
            // If the characters have the same width, we can just do a direct replace.
            if (old_width == new_width)
            {
                writechar(str, newChar);
            }
            else
            {
                // The width's are different which means we will need to move some memory around.
                // We need to move the content after our original character to their appropriate
                // position before writing the character. We first need to determine the offset
                // which will determine where to move the content to.
                int offset = ((int)(new_width - old_width));

                assert((size_t)offset < (strSize - 1));
                _movestr(temp + offset, temp, (null_terminated) ? strLength + 1 : strLength);

                writechar(str, newChar);

                // Modify our count.
                count += offset;
            }
        }

        str = temp;

        size_t ch_width = charwidth<T>(ch);
        strLength -= ch_width;
        strSize -= ch_width;
        count += ch_width;
    }

    return count;
}


}

#endif // __EASL_REPLACE_H_
