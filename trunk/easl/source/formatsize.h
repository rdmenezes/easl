/**
*   \file   formatsize.h
*   \author Dave Reid
*   \brief  Header file for formatsize() implementations.
*/
#ifndef __EASL_FORMATSIZE_H_
#define __EASL_FORMATSIZE_H_

#include "vformat.h"

namespace easl
{

/**
*   \brief              Determines the required size of a buffer to store the specified formatted string.
*   \param  format [in] The format string to use for formatting the data.
*   \return             The number of T's required to store the formatted string based on the format string and the input parameters.
*/
template <typename T>
size_t formatsize(const T *format, ...)
{
    va_list args;
    va_start(args, format);

    int ret = easl::vformat((T *)NULL, 0, format, args);

    va_end(args);

    return ret;
}

}

#endif // __EASL_FORMATSIZE_H_
