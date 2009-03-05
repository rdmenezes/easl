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
*   \brief              Determines the size required for a buffer to store the specified string.
*   \param  source [in] The string to look at to determine the size of the output buffer.
*   \param  count  [in] The maximum number of U's to look at in the input string.
*   \return             The minimum size that a buffer must be in order to store the specified string.
*
*   \remarks
*       The T template parameter specifies the type of the destination buffer.
*/
template <typename T, typename U>
size_t copysize(const U *source, size_t count = -1)
{
    return copy((T *)NULL, source, (size_t)-1, count);
}


}

#endif // __EASL_COPYSIZE_H_
