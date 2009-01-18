/**
*   \file   format.h
*   \author Dave Reid
*   \brief  Header file for format() implementations.
*/
#ifndef __EASL_FORMAT_H_
#define __EASL_FORMAT_H_

#include "vformat.h"

namespace easl
{

/**
*   \brief                 Writes formatted data to a string.
*   \param  dest     [out] Pointer to the buffer that will recieve the formatted string.
*   \param  destSize [in]  The size of the buffer in T's.
*   \param  format   [in]  The string describing the format of the formatted string.
*   \param  args     [in]  The additional arguments to use based on the format string.
*   \return                The number of characters written to the buffer.
*
*   \remarks
*       The \c format string should be in the same format as that in the sprintf() function.
*       \par
*       If \c dest is NULL, the function will return the number of T's required to store the
*       formatted string including the null terminator.
*       \par
*       Internally this function uses sprintf() which forces the \c format parameter to be
*       NULL terminated. This may change in the future.
*       \par
*       If an error occurs, 0 is returned.
*/
template <typename T>
inline int format(T *dest, size_t destSize, const T *format, ...)
{
    va_list args;
    va_start(args, format);

    int ret = easl::vformat(dest, destSize, format, args);

    va_end(args);

    return ret;
}

template <size_t destSize, typename T>
inline int format(T (&dest)[destSize], const T *format, ...)
{
    va_list args;
    va_start(args, format);

    int ret = easl::vformat(dest, destSize, format, args);

    va_end(args);

    return ret;
}

}

#endif // __EASL_FORMAT_H_
