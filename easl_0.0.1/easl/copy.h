/**
*   \file   copy.h
*   \author Dave Reid
*   \brief  Header file for copy() implementations.
*/
#ifndef __EASL_COPY_H_
#define __EASL_COPY_H_

#include "setup.h"
#include "nextchar.h"
#include "writechar.h"

#if ((COMPILER & COMPILER_VC) && COMPILER >= COMPILER_VC80)
#include <string.h>
#endif

namespace easl
{

/**
*   \brief                  Copies a string over to another string.
*   \param  dest     [out]  The destination buffer.
*   \param  source   [in]   The source string.
*   \param  destSize [in]   The size of the destination buffer in T's.
*   \param  count    [in]   The maximum number of T's to copy from the source.
*   \return                 The number of T's that are copied to the destination.
*
*   \remarks
*       If \c dest is NULL, the function will return the number of T's required to store
*       the source string based on the other parameters.
*       \par
*       This function does an implicit conversion between the strings. For example, if
*       \c dest is a char string and \c source is a char16_t string, the source will be
*       converted from a UTF-16 char16_t string to a UTF-8 char string.
*       \par
*       The \c destSize parameter specifies the size in T's of the destination buffer.
*       If this is equal to -1, the function will do an unsafe copy and should in theory
*       be slightly quicker. If \c destSize is a value larger than the actual size of
*       the destination buffer, the results are undefined. This parameter can not be NULL.
*       \par
*       The \count parameter specifies the number of T's to copy over from the source
*       string. Note that this is _not_ the number of characters to copy over, but rather
*       the number of T's. Use the parameter to only copy over a subset of the source
*       string. If \c count is larger or equal to the number of T's in the source string,
*       the entire source string is copied over. This parameter should include the NULL
*       terminator.
*       \par
*       The resulting string is always null terminated.
*       \par
*       If the copy fails for whatever reason, the function will return 0. The function
*       will fail if \c source is NULL.
*/
template <typename T>
size_t copy(T *dest, const T *source, size_t destSize = -1, size_t count = -1)
{
    if (source == NULL || (dest == NULL && destSize == 0))
    {
        return 0;
    }

    // We need to copy the original count so can return a good value.
    size_t temp_count = count;

    // Now we loop through and copy character by character.
    while (destSize > 1 && count > 0 && *source != NULL)
    {
        *dest = *source;

        ++dest;
        ++source;
        --destSize;
        --count;
    }

    // Ensure that we have a null terminator.
    *dest = NULL;

    return temp_count + 1;
}
#if ((COMPILER & COMPILER_VC) && COMPILER >= COMPILER_VC80)
template <> size_t copy(char *dest, const char *source, size_t destSize, size_t count)
{
    if (dest != NULL)
    {
        ::strncpy_s(dest, destSize, source, count);
    }

    return count + 1;
}
template <> size_t copy(wchar_t *dest, const wchar_t *source, size_t destSize, size_t count)
{
    if (dest != NULL)
    {
        ::wcsncpy_s(dest, destSize, source, count);
    }

    return count + 1;
}
#endif

/**
*   \brief                  Copies a string over to another string.
*   \param  dest     [out]  The destination buffer.
*   \param  source   [in]   The source string.
*   \param  destSize [in]   The size of the destination buffer in T's.
*   \param  count    [in]   The maximum number of U's to copy from the source.
*   \return                 The number of T's that are copied to the destination.
*
*   \remarks
*       If \c dest is NULL, the function will return the number of T's required to store
*       the source string based on the other parameters.
*       \par
*       This function does an implicit conversion between the strings. For example, if
*       \c dest is a char string and \c source is a char16_t string, the source will be
*       converted from a UTF-16 char16_t string to a UTF-8 char string.
*       \par
*       The \c destSize parameter specifies the size in T's of the destination buffer.
*       If this is equal to -1, the function will do an unsafe copy and should in theory
*       be slightly quicker. If \c destSize is a value larger than the actual size of
*       the destination buffer, the results are undefined. This parameter can not be NULL.
*       \par
*       The \count parameter specifies the number of U's to copy over from the source
*       string. Note that this is _not_ the number of characters to copy over, but rather
*       the number of U's. Use the parameter to only copy over a subset of the source
*       string. If \c count is larger or equal to the number of T's in the source string,
*       the entire source string is copied over. This parameter should include the NULL
*       terminator.
*       \par
*       The resulting string is always null terminated.
*       \par
*       If the copy fails for whatever reason, the function will return 0. The function
*       will fail if \c source is NULL.
*/
template <typename T, typename U>
size_t copy(T *dest, const U *source, size_t destSize = -1, size_t count = -1)
{
    if (source == NULL || destSize == 0)
    {
        return 0;
    }

    // Variable to store the return value. We initialse to 1 for the null terminator.
    size_t copy_size = 1;

    // Now we need to loop through each character in the source and copy over the character.
    uchar32_t ch;
    while (destSize > 1 && count > 0 && (ch = nextchar(source)) != NULL)
    {
        // Variable that stores the width of the current character.
        size_t char_width;

        if (dest != NULL)
        {
            // Write the character.
            char_width = writechar(dest, ch);
        }
        else
        {
            // Our destination is NULL, so all we need to do is grab the width of the character.
            // If this returns 0, we can safely assume that writechar() will write the replacement
            // character.
            char_width = charwidth<T>(ch);
            if (char_width == 0)
            {
                char_width = charwidth<T>(UNI_REPLACEMENT_CHAR);
            }
        }

        // Now increment our copy size.
        copy_size += char_width;

        --destSize;
        --count;
    }

    // NULL terminate the destination.
    if (dest != NULL)
    {
        *dest = NULL;
    }

    return copy_size;
}

}

#endif // __EASL_COPY_H_
