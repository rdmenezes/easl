/**
*   \brief  relativesize.h
*   \author Dave Reid
*   \brief  Header file for the relativesize() implementation.
*/
#ifndef __EASL_PATHS_RELATIVESIZE_H_
#define __EASL_PATHS_RELATIVESIZE_H_

#include "relative.h"

namespace easl
{
namespace paths
{

/**
*   \brief                  Retrieves the size that a buffer must be in order to store a relative path.
*   \param  path       [in] The absolute path that is to be converted to a relative path.
*   \param  base       [in] The absolute path that the \c path parameter should be made relative to.
*   \param  pathLength [in] The length in T's of the \c path string, not including the null terminator.
*   \param  baseLength [in] The length in T's of the \c path string, not including the null terminator.
*   \return                 The number of T's required to store the relative path.
*/
template <typename T>
inline size_t relativesize(const T *path, const T *base, size_t pathLength = -1, size_t baseLength = -1)
{
    return relative((T *)NULL, path, base, pathLength, baseLength);
}

}
}


#endif // __EASL_PATHS_RELATIVESIZE_H_
