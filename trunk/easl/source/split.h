/**
*   \file   split.h
*   \author Dave Reid
*   \brief  Header file for split() implementations.
*/
#ifndef __EASL_SPLIT_H_
#define __EASL_SPLIT_H_

#include <vector>
#include "findfirst.h"
#include "reference_string.h"

namespace easl
{

/**
*   \brief                         Splits a string and places each part in a list.
*   \param  str              [in]  The string to split.
*   \param  delimiter        [in]  The string that is used to determine where the string should be split.
*   \param  list             [out] A reference to the list that will recieve the different pieces.
*   \param  includeDelimiter [in]  Determines if the delimiter should be included in the returned pieces.
*   \param  strLength        [in]  The length in T's of the string, not including the null terminator.
*   \param  delLength        [in]  The length in T's of the delimiter string, not including the null terminator.
*
*   \remarks
*       Empty strings are never added to the list.
*/
template <typename T>
void split(T *str, const T *delimiter, std::vector<reference_string<T> > &list, bool includeDelimiter = true,
           size_t strLength = (size_t)-1, size_t delLength = (size_t)-1)
{
    assert(str != NULL);

	// The current piece.
    reference_string<T> cur_string;

    if (delLength == (size_t)-1)
    {
        delLength = length(delimiter);
    }

    T *temp = str;

    // Now we need to keep searching for our delimiter until we don't find any more.
    while (strLength > 0 && (temp = findfirst(str, delimiter, strLength, delLength)) != NULL)
    {
        cur_string.start = str;

        // If we're not including the delimiter, the end will simply be equal to 'temp'. Otherwise, it
        // will be equal to 'temp' + length of delimiter.
        if (!includeDelimiter)
        {
            cur_string.end = temp;
        }
        else
        {
            cur_string.end = temp + delLength;
        }

        if (length(cur_string) > 0)
        {
            list.push_back(cur_string);
        }

        strLength -= (temp - str) + delLength;

        // We need to move past the delimiter that we just found.
        str = temp + delLength;
    }

    // We need to make sure that we have our very last part.
    if (strLength > 0)
    {
        cur_string.start = str;
        cur_string.end = str;

        uchar32_t ch;
        while (strLength > 0 && (ch = nextchar(cur_string.end)) != NULL)
        {
            strLength -= charwidth<T>(ch);
        };

        if (length(cur_string) > 0)
        {
            list.push_back(cur_string);
        }
    }
}

}

#endif // __EASL_SPLIT_H_
