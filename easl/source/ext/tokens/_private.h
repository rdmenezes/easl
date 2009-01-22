/**
*   \file   _private.h
*   \author Dave Reid
*   \brief  Private header file for the tokens extension.
*/
#ifndef __EASL_TOKENS_PRIVATE_H_
#define __EASL_TOKENS_PRIVATE_H_

#include "../../findfirst.h"
#include "../../equal.h"

namespace easl
{

/**
*   \brief                   Checks if a string starts with a token in the specified token group.
*   \param  str    [in, out] The string to check.
*   \param  groups [in]      The space seperated list of token groups.
*   \return                  True if a token group is found; false otherwise.
*
*   \remarks
*       If a token group is not found, \c str is not modified. Otherwise, \c str is moved to the
*       character after the last character in the token group.
*/
template <typename T>
inline bool _check_token_group(T *&str, T *groups)
{
    // TODO: This can be made more efficient. We should make it more specific and remove the easl::findfirst() calls.

    T *temp = groups;

    uchar32_t ch;
    while ((ch = nextchar(temp)) != NULL)
    {
        // If we have a space, we can now check if this token is at the start of the string.
        if (ch == ' ')
        {
            size_t char_size = temp - groups - 1;

            if (easl::findfirst(str, groups, char_size, char_size) == str)
            {
                str += char_size;
                return true;
            }

            groups = temp;
        }
    }

    // We need to do one final check.
    size_t char_size = temp - groups;
    if (easl::findfirst(str, groups, char_size, char_size) == str)
    {
        str += char_size;
        return true;
    }

    return false;
}


/**
*   \brief                        Checks to see if a string starts with one of the strings in the specified starting ignore block string.
*   \param  str         [in, out] The string to check.
*   \param  blockStarts [in]      The string containing the ignore block starting strings.
*   \param  start       [in]      Reference to the reference string that will recieve the starting ignore block string.
*   \return                       True if a starting ignore block string is found; false otherwise.
*
*   \remarks
*       When this function returns true, the input string is moved to the end of the starting ignore block.
*/
template <typename T>
inline bool _check_ignore_block_start(T *&str, T *blockStarts, reference_string<T> &start)
{
    // The original input value.
    T *temp = str;
    if (_check_token_group(temp, blockStarts))
    {
        // We found a starting block.
        start.start = str;
        start.end = temp;
        str = temp;
        return true;
    }

    return false;
}

/**
*   \brief              Retrieves the block at the specified index.
*   \param  block [out] Reference to the reference string that will recieve the 
*/
template <typename T>
inline void _get_block(reference_string<T> &block, size_t index, T *blocks)
{
    size_t temp_index = 0;

    T *temp = blocks;

    uchar32_t ch = '\0';
    do
    {
        // If our indexes are the same, we want to set the start of the block. If the
        // temporary index is larger, it means that we have to set the ending block
        // and return.
        if (ch == ' ' || ch == '\0')
        {
            if (ch == ' ')
            {
                ++temp_index;
            }

            blocks = temp;

            if (temp_index == index)
            {
                block.start = blocks;
            }
            else if (temp_index > index)
            {
                block.end = blocks - 1;
                return;
            }
        }

    } while ((ch = nextchar(temp)) != NULL);

    if (block.start != NULL)
    {
        block.end = temp;
    }
}

/**
*   \brief                        Checks to see if a matching ending block string is at the start of the specified string.
*   \param  str         [in, out] The string to check.
*   \param  start       [in]      The starting block string.
*   \param  blockStarts [in]      The list of starting block strings.
*   \param  blockEnds   [in]      The list of ending block strings.
*   \return                       True if a match is found; false otherwise.
*
*   \remarks
*       When this function returns true, the input string is moved to the end of the ending ignore block.
*/
template <typename T>
inline bool _check_ignore_block_end(T *&str, const reference_string<T> &start, T *blockStarts, T *blockEnds)
{
    // Now we need to scan through our starts and get the next index of the string
    // that is equal to 'start'. It's important that the index starts at -1.
    size_t index = 0;

    uchar32_t ch = '\0';

    do
    {
        if (ch == ' ' || ch == '\0')
        {
            if (equal(blockStarts, start.start, true, length(start), length(start)))
            {
                // Now we need to check the corresponding closing block.
                reference_string<T> end = {NULL, NULL};
                _get_block(end, index, blockEnds);
                if (equal(str, end.start, true, length(end), length(end)))
                {
                    str += length(end);
                    return true;
                }
            }

            ++index;
        }

    } while ((ch = nextchar(blockStarts)) != NULL);


    return false;
}



}

#endif // __EASL_TOKENS_PRIVATE_H_
