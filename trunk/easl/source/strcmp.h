/**
*   \file   strcmp.h
*   \author Dave Reid
*   \brief  Header file for strcmp() implementations.
*/
#ifndef __EASL_STRCMP_H_
#define __EASL_STRCMP_H_

#include <string.h>
#include <assert.h>
#include "nextchar.h"

namespace easl
{

/**
*   \brief              Compares two strings for equality.
*   \param  str1  [in]  The first string to compare.
*   \param  str2  [in]  The second string to compare.
*   \param  count [in]  The number of characters to compare in the second string (\c str2).
*   \return             0 if the two strings are equal; -1 if \c str1 is lower than \c str2; +1 otherwise.
*/
// NOTE: Not sure about the implementation of strcmp(). Do we do the comparisons
// T by T, or by character by character?
template <typename T, typename U>
inline int strcmp(const T *str1, const U *str2, size_t count = -1)
{
    assert(str1 != NULL);
    assert(str2 != NULL);

    int ret = 0;

    while (count > 0)
    {
        uchar32_t ch1 = nextchar(str1);
        uchar32_t ch2 = nextchar(str2);

        ret = ch1 - ch2;

        if (ret != 0 || ch2 == NULL)
        {
            break;
        }

        count -= getcharwidth<U>(ch2);
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
#ifdef EASL_ONLY_ASCII
template <> inline int strcmp(const char *str1, const char *str2, size_t count)
{
    assert(str2 != NULL);
    assert(str2 != NULL);

    // NOTE: If the comparison is character by character and not byte by byte, we can't
    // use this method unless we're doing ASCII only.
    if (count != -1)
    {
        return ::strncmp(str1, str2, count);
    }

    return ::strcmp(str1, str2);
}
#endif
template <> inline int strcmp(const wchar_t *str1, const wchar_t *str2, size_t count)
{
    assert(str2 != NULL);
    assert(str2 != NULL);

    if (count != -1)
    {
        return ::wcsncmp(str1, str2, count);
    }

    return ::wcscmp(str1, str2);
}

}

#endif // __EASL_STRCMP_H_
