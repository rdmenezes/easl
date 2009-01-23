/**
*   \file   equal.h
*   \author Dave Reid
*   \brief  Header file for equal() implementations.
*/
#ifndef __EASL_EQUAL_H_
#define __EASL_EQUAL_H_

#include "types.h"
#include "charcount.h"

namespace easl
{

/**
*   \brief                     Determines if two strings are equal.
*   \param  str1          [in] The first string to compare.
*   \param  str2          [in] The second string to compare.
*   \param  caseSensitive [in] Specifies whether or not the comparison is case sensitive.
*   \param  str1Length    [in] The length in T's of the first string, not including the null terminator.
*   \param  str2Length    [in] The length in U's of the second string, not including the null terminator.
*   \return                    True if the two strings are equal; false otherwise.
*
*   \remarks
*       Both strings must be NULL terminated. By default, the comparison is case sensitive.
*/
// TODO: Should implement a T-only optimization. We can remove the charcount() checks at the very least.
template <typename T, typename U>
inline bool equal(const T *str1, const U *str2, bool caseSensitive = true, size_t str1Length = -1, size_t str2Length = -1)
{
    if (str1 == NULL || str2 == NULL)
    {
        return false;
    }

    // If the number of characters in the strings are different, they are not equal.
    // NOTE: This should probably be optimised out.
    if (charcount(str1, str1Length) != charcount(str2, str2Length))
    {
        return false;
    }

    // We need to grab the first characters first.
    uchar32_t ch1 = nextchar(str1);
    uchar32_t ch2 = nextchar(str2);

    if (caseSensitive)
    {
        while ((str1Length > 0 && ch1 != NULL) && (str2Length > 0 && ch2 != NULL))
        {
            if (ch1 != ch2)
            {
                return false;
            }

            ch1 = nextchar(str1);
            ch2 = nextchar(str2);

            str1Length -= charwidth<T>(ch1);
            str2Length -= charwidth<U>(ch2);
        }
    }
    else
    {
        while ((str1Length > 0 && ch1 != NULL) && (str2Length > 0 && ch2 != NULL))
        {
            if (tolower(ch1) != tolower(ch2))
		    {
			    return false;
		    }

            ch1 = nextchar(str1);
            ch2 = nextchar(str2);

            str1Length -= charwidth<T>(ch1);
            str2Length -= charwidth<U>(ch2);
        }
    }

	return true;
}


// Optimized case. The function can be simplified and should be more efficient when both
// strings are the same type.
template <typename T>
inline bool equal(const reference_string<T> &str1, const reference_string<T> &str2, bool caseSensitive = true)
{
    // If the lengths are different, it's impossible for the strings to be the same. Calculating
    // the lengths for reference strings is a fast operation, so this check is worth while. It
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

    if (caseSensitive)
    {
        while (--len >= 0)
        {
            if (str1.start[len] != str2.start[len])
            {
                return false;
            }
        }
    }
    else
    {
        while (--len >= 0)
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
inline bool equal(const reference_string<T> &str1, const reference_string<U> &str2, bool caseSensitive = true)
{
    return equal(str1.start, str2.start, caseSensitive, length(str1), length(str2));
}


}

#endif // __EASL_EQUAL_H_
