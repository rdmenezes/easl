/**
*   \file   sprintf.h
*   \author Dave Reid
*   \brief  Header file for sprintf() implementations.
*/
#ifndef __EASL_SPTINTF_H_
#define __EASL_SPTINTF_H_

#include "vsprintf.h"

namespace easl
{

/**
*   \brief                  Writes formatted data to a string.
*   \param  dest     [out]  Pointer to the buffer that will recieve the formatted string.
*   \param  destSize [in]   The size of the buffer in T's.
*   \param  format   [in]   The string describing the format of the formatted string.
*   \param  args     [in]   The additional arguments to use based on the format string.
*   \return                 The number of characters written to the buffer; or -1 if an error occured.
*/
template <typename T>
inline int sprintf(T *dest, size_t destSize, const T *format, ...)
{
    va_list args;
    va_start(args, format);

    int ret = easl::vsprintf(dest, destSize, format, args);

    va_end(args);

    return ret;
}

template <size_t destSize, typename T>
inline int sprintf(T (&dest)[destSize], const T *format, ...)
{
    va_list args;
    va_start(args, format);

    int ret = easl::vsprintf(dest, destSize, format, args);

    va_end(args);

    return ret;
}

}

#endif // __EASL_SPTINTF_H_
