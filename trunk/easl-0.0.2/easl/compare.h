/**
*   \file   compare.h
*   \author Dave Reid
*   \brief  Header file for compare() implementations.
*/
#ifndef __EASL_STRCMP_H_
#define __EASL_STRCMP_H_

#include <string.h>
#include <assert.h>
#include "nextchar.h"
#include "length.h"
#include "reference_string.h"

namespace easl
{

/**
*   \brief                  Compares two strings for equality.
*   \param  str1       [in] The first string to compare.
*   \param  str2       [in] The second string to compare.
*   \param  str1Length [in] The length in T's of the first string.
*   \param  str2Length [in] The length in T's of the second string.
*   \return                 0 if the two strings are equal; -1 if \c str1 is lower than \c str2; +1 otherwise.
*
*   \remarks
*       The comparison is case sensitive. "Hello" does not equal "HELLO".
*       \par
*       If the length of a string is unknown, but it is NULL terminated, set the length to -1.
*/
template <typename T, typename U>
inline int compare(const T *str1, const U *str2, size_t str1Length = -1, size_t str2Length = -1)
{
    assert(str1 != NULL);
    assert(str2 != NULL);

    int ret = 0;

    while (str1Length > 0 && str2Length > 0)
    {
        uchar32_t ch1 = nextchar(str1);
        uchar32_t ch2 = nextchar(str2);

        ret = ch1 - ch2;

        if (ret != 0 || ch2 == NULL)
        {
            break;
        }

        size_t width = charwidth<T>(ch2);
        str1Length -= width;
        str2Length -= width;
    }

    if (ret < 0)
    {
        return -1;
    }
    else if (ret > 0)
    {
        return 1;
    }

    return 0;
}

template <typename T, typename U>
inline int compare(const reference_string<T> &str1, const reference_string<U> &str2)
{
    return compare(str1.start, str2.start, length(str1), length(str2));
}


}

#endif // __EASL_STRCMP_H_
