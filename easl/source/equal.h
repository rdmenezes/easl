/**
*   \file   equal.h
*   \author Dave Reid
*   \brief  Header file for equal() implementations.
*/
#ifndef __EASL_EQUAL_H_
#define __EASL_EQUAL_H_

#include "types.h"
#include "nextchar.h"
#include "length.h"

namespace easl
{

/**
*   \brief                      Determines if two strings are equal.
*   \param  str1          [in]  The first string to compare.
*   \param  str2          [in]  The second string to compare.
*   \param  caseSensitive [in]  Specifies whether or not the comparison is case sensitive.
*   \return                     True if the two strings are equal; false otherwise.
*
*   \remarks
*       Both strings must be NULL terminated. By default, the comparison is case sensitive.
*/
template <typename T, typename U>
bool equal(const T *str1, const U *str2, bool caseSensitive = true)
{
    if (str1 == NULL || str2 == NULL)
    {
        return false;
    }

    // We need to grab the first characters first.
    uchar32_t ch1 = nextchar(str1);
    uchar32_t ch2 = nextchar(str2);

    if (caseSensitive)
    {
        while (ch1 != NULL && ch2 != NULL)
        {
            if (ch1 != ch2)
            {
                return false;
            }

            ch1 = nextchar(str1);
            ch2 = nextchar(str2);
        }
    }
    else
    {
        while (ch1 != NULL && ch2 != NULL)
        {
            if (tolower(ch1) != tolower(ch2))
		    {
			    return false;
		    }

            ch1 = nextchar(str1);
            ch2 = nextchar(str2);
        }
    }

    // If both strings aren't at their null terminators, they must be different.
    if (ch1 != 0 || ch2 != 0)
    {
	    return false;
    }

	return true;
}


// Optimized case.
template <typename T>
bool equal(const reference_string<T> &str1, const reference_string<T> &str2, bool caseSensitive = true)
{
    // If the lengths are different, it's impossible for the strings to be the same. Calculating
    // the lengths for reference strings is a fast operation, so this check is worth-while. It
    // also simplifies the next part.
    size_t len = length(str1);
    if (len != length(str2))
    {
        return false;
    }

    // If the internal pointers are the same, the strings are also the same.
    if (str1.start == str2.start && str1.end == str2.end)
    {
        return true;
    }

    while (--len >= 0)
    {
        if (caseSensitive)
        {
            if (str1.start[len] != str2.start[len])
            {
                return false;
            }
        }
        else
        {
            if (tolower(str1.start[len]) != tolower(str2.start[len]))
            {
                return false;
            }
        }
    }

    return true;
}


template <typename T, typename U>
bool equal(const reference_string<T> &str1, const reference_string<U> &str2, bool caseSensitive = true)
{
    // TODO: We'll have to do it manually without the first function.

    return true;
}


}

#endif // __EASL_EQUAL_H_
