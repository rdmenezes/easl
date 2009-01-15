/**
*   \file   strcat.h
*   \author Dave Reid
*   \brief  Header file for strcat() implementations.
*/
#ifndef __EASL_STRCAT_H_
#define __EASL_STRCAT_H_

#include <string.h>
#include <assert.h>
#include <errno.h>

namespace easl
{

/**
*   \brief                  Appends a string onto another string.
*   \param  dest     [out]  The string that will be appended with the other string.
*   \param  src      [in]   The string to append to the destination string.
*   \param  destSize [in]   The size in T's of the buffer pointed to by \c dest.
*   \return                 Zero is successful; an error on failure.
*
*   \remarks
*       The destination buffer must be NULL terminated to indicate the end of the string.
*/
template <typename T>
inline errno_t strcat(T *dest, const T *src, size_t destSize)
{
    if (dest == NULL || src == NULL || destSize == 0)
    {
        return EINVAL;
    }

    // We need to get to the end of the destination string.
    while (*dest != NULL)
    {
        ++dest;
        --destSize;
    };

    // Now simply copy the string over.
    //easl::strcpy(dest, src, destSize);
    easl::copy(dest, src, destSize);

    return 0;
}
#ifdef EASL_ONLY_ASCII
template <> inline errno_t strcat(char *dest, const char *src, size_t destSize)
{
#if ((COMPILER & COMPILER_VC) && COMPILER >= COMPILER_VC80)
    return ::strncat_s(dest, destSize, src, destSize);
#else
	::strncat(dest, src, destSize);
    return 0;
#endif
}
#endif
template <> inline errno_t strcat(wchar_t *dest, const wchar_t *src, size_t destSize)
{
#if ((COMPILER & COMPILER_VC) && COMPILER >= COMPILER_VC80)
    return ::wcsncat_s(dest, destSize, src, destSize);
#else
	::wcsncat(dest, src, destSize);
    return 0;
#endif
}

template <size_t destSize, typename T> 
inline errno_t strcat(char (&dest)[destSize], const T *src)
{
    return easl::strcat(dest, src, destSize);
}

}

#endif // __EASL_STRCAT_H_
