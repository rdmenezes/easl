/**
*   \file   copysize.h
*   \author Dave Reid
*   \brief  Header file for copysize() implementations.
*/
#ifndef __EASL_COPYSIZE_H_
#define __EASL_COPYSIZE_H_

#include "copy.h"

namespace easl
{

/**
*   \brief                    Determines the size required for a buffer to store the specified string.
*   \param  source       [in] The string to look at to determine the size of the output buffer.
*   \param  sourceLength [in] The length of the source strings in U's, not including the null terminator
*   \return                   The minimum size that a buffer must be in order to store the specified string.
*
*   \remarks
*       The T template parameter specifies the type of the destination buffer.
*/
template <typename T, typename U>
size_t copysize(const U *source, size_t sourceLength = -1)
{
    return copy((T *)NULL, source, (size_t)-1, sourceLength);
}

template <typename T, typename U>
size_t copysize(const reference_string<U> &source)
{
    return copy((T *)NULL, source, (size_t)-1);
}


}

#endif // __EASL_COPYSIZE_H_
