/**
*   \file   appendsize.h
*   \author Dave Reid
*   \brief  Header file for appendsize() implementations.
*/
#ifndef __EASL_APPENDSIZE_H_
#define __EASL_APPENDSIZE_H_

#include "append.h"

namespace easl
{

/**
*   \brief              Determines the size required for a buffer to store the specified string.
*   \param  str1   [in] The first string to consider for the concatination.
*   \param  str2   [in] The second string to consider for the concatination.
*   \param  count  [in] The maximum number of T's to look at in the second string.
*   \return             The minimum size that a buffer must be in order to store the concatination of the two strings.
*/
template <typename T>
size_t appendsize(const T *str1, const T *str2, size_t count = -1)
{
    return appendsize((T *)str1, str2, (size_t)-1, count);
}

}

#endif // __EASL_APPENDSIZE_H_
