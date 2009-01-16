/**
*   \file   append.h
*   \author Dave Reid
*   \brief  Header file for append() implementations.
*/
#ifndef __EASL_APPEND_H_
#define __EASL_APPEND_H_

#if ((COMPILER & COMPILER_VC) && COMPILER >= COMPILER_VC80)
#include <string.h>
#endif

#include <assert.h>
#include "reference_string.h"

namespace easl
{

/**
*   \brief                  Appends a string onto another string.
*   \param  dest     [out]  The string that will be appended with the other string.
*   \param  src      [in]   The string to append to the destination string.
*   \param  destSize [in]   The size in T's of the buffer pointed to by \c dest.
*   \param  count    [in]   The maximum number of U's to append to the destination string from the source string.
*   \return                 The number of T's at a minimum that are required to store the full catenated string plus the NULL terminator.
*
*   \remarks
*       The destination buffer must be NULL terminated to indicate the end of the string.
*       \par
*       If an error occurs, 0 is returned.
*/
template <typename T>
inline size_t append(T *dest, const T *src, size_t destSize = -1, size_t count = -1)
{
    if (src == NULL || destSize == 0)
    {
        return 0;
    }

    // Our temporary size so that we can calculate the number of T's we need for the return value.
    size_t append_size = 0;

    // We need to get to the end of the destination string.
    while (destSize > 1 && *dest != NULL)
    {
        ++dest;
        ++append_size;
        --destSize;
        --count;
    };

    // Now simply copy the string over.
    if (dest != NULL)
    {
        append_size += easl::copy(dest, src, destSize, count);
    }
    else
    {
        append_size += easl::copysize<T>(src, count);
    }

    return append_size;
}
#if ((COMPILER & COMPILER_VC) && COMPILER >= COMPILER_VC80)
#ifdef EASL_ONLY_ASCII
template <> inline size_t append(char *dest, const char *src, size_t destSize, size_t count)
{
    if (dest != NULL)
    {
        ::strncat_s(dest, destSize, src, count);
    }

    return easl::length(dest) + count + 1;
}
#endif  // EASL_ONLY_ASCII
template <> inline size_t append(wchar_t *dest, const wchar_t *src, size_t destSize, size_t count)
{
    if (dest != NULL)
    {
        ::wcsncat_s(dest, destSize, src, count);
    }

    return easl::length(dest) + count + 1;
}
#endif


template <typename T>
inline size_t append(T *dest, const reference_string<T> &source, size_t destSize)
{
    return append(dest, source.start, dest_size, source.end - source.start);
}


}

#endif // __EASL_APPEND_H_
