/**
*   \file   strcpy.h
*   \author Dave Reid
*   \brief  Header file for strcpy() implementations.
*/
#ifndef __EASL_STRCPY_H_
#define __EASL_STRCPY_H_

#include <string.h>
#include <assert.h>
#include <errno.h>

namespace easl
{

/**
*   \brief                  Copies one string to another.
*   \param  dest     [out]  The destination string.
*   \param  src      [in]   The source string.
*   \param  destSize [in]   The size in T's of the buffer pointed to by \c dest.
*   \return                 Zero is successful; an error otherwise.
*/
template <typename T>
inline errno_t strcpy(T *dest, const T *src, size_t destSize)
{
    if (dest == NULL || src == NULL || destSize == 0)
    {
        return EINVAL;
    }

    // Now we loop through and copy character by character.
    while (destSize - 1 > 0 && *src != NULL)
    {
        *dest = *src;

        ++dest;
        ++src;
        --destSize;
    }

    // Ensure that we have a null terminator.
    *dest = NULL;

    return 0;
}
inline errno_t strcpy(char *dest, const char *src, size_t destSize)
{
#if ((COMPILER & COMPILER_VC) && COMPILER >= COMPILER_VC80)
    return ::strncpy_s(dest, destSize, src, destSize);
#else
	::strncpy(dest, src, destSize);
    return 0;
#endif
}
inline errno_t strcpy(wchar_t *dest, const wchar_t *src, size_t destSize)
{
#if ((COMPILER & COMPILER_VC) && COMPILER >= COMPILER_VC80)
    return ::wcsncpy_s(dest, destSize, src, destSize);
#else
	::wcsncpy(dest, src, destSize);
    return 0;
#endif
}

template <size_t destSize, typename T>
inline errno_t strcpy(T (&dest)[destSize], const T *src)
{
    return easl::strcpy(dest, src, destSize);
}

}

#endif // __EASL_STRCPY_H_
