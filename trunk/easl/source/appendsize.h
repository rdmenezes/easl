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
*   \brief                  Determines the size required for a buffer to store the specified string.
*   \param  str1       [in] The first string to consider for the concatination.
*   \param  str2       [in] The second string to consider for the concatination.
*   \param  str1Length [in] The length in T's of the first string, not including the null terminator.
*   \param  str2Length [in] The length in T's of the first string, not including the null terminator.
*   \return                 The minimum size that a buffer must be in order to store the concatination of the two strings.
*
*   \remarks
*       If is safe for \c str1Length and/or \c str2Length to be set to -1, in which case the
*       function will assume that the appropriate string is null terminated.
*/
template <typename T>
inline size_t appendsize(const T *str1, const T *str2, size_t str1Length = -1, size_t str2Length = -1)
{
    return append((T *)str1, str2, (size_t)-1, str1Length, str2Length);
}

}

#endif // __EASL_APPENDSIZE_H_
