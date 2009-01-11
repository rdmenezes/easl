/**
*   \file   vsprintf.h
*   \author Dave Reid
*   \brief  Header file for vsprintf() implementations.
*/
#ifndef __EASL_VSPRINTF_H_
#define __EASL_VSPRINTF_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <assert.h>
#include "convert.h"

namespace easl
{

/**
*   \brief                  Writes formatted data to a string from a pointer to a list of arguments.
*   \param  dest     [out]  Pointer to the buffer that will recieve the formatted string.
*   \param  destSize [in]   The size of the buffer in T's.
*   \param  format   [in]   The string describing the format of the formatted string.
*   \param  args     [in]   The pointer to the arguments to use for the formatted string.
*   \return                 The number of characters that are written to the string; or -1 if an error occured.
*/
template <typename T>
inline int vsprintf(T *dest, size_t destSize, const T *format, va_list args)
{
    // We will convert to a wchar_t * string and call that implementation. This is slow,
    // but it's the simplest way to do it.
    wchar_t *tmp_dest = new wchar_t[destSize];
    easl::convert(tmp_dest, dest);

    wchar_t *tmp_format = new wchar_t[easl::convertsize<wchar_t>(format)];
    easl::convert(tmp_format, format);

    int result = easl::vsprintf(tmp_dest, destSize, tmp_format, args);

    // Convert our string back.
    easl::convert(dest, tmp_dest);

    delete [] tmp_dest;
    delete [] tmp_format;

    return result;
}
#ifdef EASL_ONLY_ASCII
template <> inline int vsprintf(char *dest, size_t destSize, const char *format, va_list args)
{
#if ((COMPILER & COMPILER_VC) && COMPILER >= COMPILER_VC80)
    return ::vsprintf_s(dest, destSize, format, args);
#else
    return ::vsprintf(dest, format, args);
#endif
}
#endif
template <> inline int vsprintf(wchar_t *dest, size_t destSize, const wchar_t *format, va_list args)
{
#if ((COMPILER & COMPILER_VC) && COMPILER >= COMPILER_VC80)
    return ::vswprintf_s(dest, destSize, format, args);
#else
    return ::vswprintf(dest, format, args);
#endif
}

template <size_t destSize, typename T>
inline int vsprintf(T (&dest)[destSize], const T *format, va_list args)
{
    return easl::vsprintf(dest, destSize, format, args);
}

}

#endif // __EASL_VSPRINTF_H_
