/**
*   \file   append.h
*   \author Dave Reid
*   \brief  Header file for append() implementations.
*/
#ifndef __EASL_APPEND_H_
#define __EASL_APPEND_H_

#include <assert.h>
#include "copy.h"

namespace easl
{

/**
*   \brief                     Appends a string onto another string.
*   \param  dest         [out] The string that will be appended with the other string.
*   \param  source       [in]  The string to append to the destination string.
*   \param  destSize     [in]  The size in T's of the buffer pointed to by \c dest.
*   \param  destLength   [in]  The number of T's making up the existing string in \c dest, not including the null terminator.
*   \param  sourceLength [in]  The number of T's making up the source string, not including the null terminator.
*   \return                    The number of T's at a minimum that are required to store the full catenated string plus the null terminator.
*
*   \remarks
*       The destination buffer must be NULL terminated to indicate the end of the string.
*       \par
*       If an error occurs, 0 is returned.
*/
template <typename T>
size_t append(T *dest, const T *source, size_t destSize = -1, size_t destLength = -1, size_t sourceLength = -1)
{
    if (source == NULL || destSize == 0)
    {
        return 0;
    }

    // Our temporary size so that we can calculate the number of T's we need for the return value.
    size_t append_size = 0;

    // We need to get to the end of the destination string.
    while (destSize > 1 && destLength > 0 && *dest != '\0')
    {
        ++dest;
        ++append_size;
        --destSize;
        --destLength;
    };

    // Now simply copy the string over.
    if (dest != NULL)
    {
        append_size += easl::copy(dest, source, destSize, sourceLength);
    }
    else
    {
        append_size += easl::copysize<T>(source, sourceLength);
    }

    return append_size;
}

/**
*   \brief                     Appends a string onto another string.
*   \param  dest         [out] The string that will be appended with the other string.
*   \param  source       [in]  The string to append to the destination string.
*   \param  destSize     [in]  The size in T's of the buffer pointed to by \c dest.
*   \param  destLength   [in]  The number of T's making up the existing string in \c dest, not including the null terminator.
*   \return                    The number of T's at a minimum that are required to store the full catenated string plus the null terminator.
*
*   \remarks
*       The destination buffer must be NULL terminated to indicate the end of the string.
*       \par
*       If an error occurs, 0 is returned.
*/
template <typename T>
inline size_t append(T *dest, const reference_string<T> &source, size_t destSize = -1, size_t destLength = -1)
{
    return append(dest, source.start, destSize, destLength, length(source));
}


}

#endif // __EASL_APPEND_H_
