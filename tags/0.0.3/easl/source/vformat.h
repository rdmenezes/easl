/**
*   \file   vformat.h
*   \author Dave Reid
*   \brief  Header file for vformat() implementations.
*/
#ifndef __EASL_VFORMAT_H_
#define __EASL_VFORMAT_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <assert.h>
#include "copy.h"

namespace easl
{

/**
*   \brief                 Writes formatted data to a string from a pointer to a list of arguments.
*   \param  dest     [out] Pointer to the buffer that will recieve the formatted string.
*   \param  destSize [in]  The size of the buffer in T's.
*   \param  format   [in]  The string describing the format of the formatted string.
*   \param  args     [in]  The pointer to the arguments to use for the formatted string.
*   \return                The number of characters that are written to the string.
*
*   \remarks
*       If \c dest is NULL, the function will return the number of T's required to store the
*       formatted string including the null terminator.
*       \par
*       Internally this function uses vsprintf() which forces the \c format parameter to be
*       NULL terminated. This may change in the future.
*       \par
*       If an error occurs, 0 is returned.
*/
template <typename T>
inline int vformat(T *dest, size_t destSize, const T *format, va_list args)
{
    size_t format_size = easl::copysize<wchar_t>(format);
    wchar_t *tmp_format = new wchar_t[format_size];
    easl::copy(tmp_format, format, format_size);

    int result;

    // We will convert to a wchar_t * string and call that implementation. This is slow,
    // but it's the simplest way to do it.
    if (dest != NULL)
    {
        wchar_t *tmp_dest = new wchar_t[destSize];
        easl::copy(tmp_dest, dest, destSize);

        result = easl::vformat(tmp_dest, destSize, tmp_format, args);

        // Convert our string back.
        easl::copy(dest, tmp_dest);

        delete [] tmp_dest;
    }
    else
    {
        result = easl::vformat((wchar_t *)NULL, 0, tmp_format, args);
    }

    delete [] tmp_format;

    return result;
}
#ifdef EASL_ONLY_ASCII
template <> inline int vformat(char *dest, size_t destSize, const char *format, va_list args)
{
#if ((COMPILER & COMPILER_VC) && COMPILER >= COMPILER_VC80)
    if (dest != NULL)
    {
        return ::vsprintf_s(dest, destSize, format, args);
    }

    return ::_vscprintf(format, args);
#else
    if (dest != NULL)
    {
        return ::vsprintf(dest, format, args);
    }

    // GCC doesn't support vscprintf, so we need to find the size manually. We will use the C99 way of doing it.

    // Our temporary pointer.
    char temp[2];
    return vsnprintf(temp, 2, format, args) + 1;
#endif
}
#endif
template <> inline int vformat(wchar_t *dest, size_t destSize, const wchar_t *format, va_list args)
{
#if ((COMPILER & COMPILER_VC) && COMPILER >= COMPILER_VC80)
    if (dest != NULL)
    {
        return ::vswprintf_s(dest, destSize, format, args);
    }

    return ::_vscwprintf(format, args);
#else
    if (dest != NULL)
    {
        return ::vswprintf(dest, format, args);
    }

    // GCC doesn't support vscprintf, so we need to find the size manually. We will use the C99 way of doing it.

    // Our temporary pointer.
    wchar_t temp[2];
    return _vsnwprintf(temp, 2, format, args) + 1;
#endif
}

template <size_t destSize, typename T>
inline int vformat(T (&dest)[destSize], const T *format, va_list args)
{
    return easl::vformat(dest, destSize, format, args);
}

}

#endif // __EASL_VFORMAT_H_
