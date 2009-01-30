/**
*   \brief  absolutesize.h
*   \author Dave Reid
*   \brief  Header file for the absolutesize() implementation.
*/
#ifndef __EASL_PATHS_ABSOLUTESIZE_H_
#define __EASL_PATHS_ABSOLUTESIZE_H_

#include "absolute.h"

namespace easl
{
namespace paths
{

/**
*   \brief                  Retrieves the size that a buffer must be in order to store an absolute path.
*   \param  path       [in] The relative path that is to be converted to an absolute path.
*   \param  base       [in] The absolute path that the \c path parameter should be made absolute to.
*   \param  pathLength [in] The length in T's of the \c path string, not including the null terminator.
*   \param  baseLength [in] The length in T's of the \c path string, not including the null terminator.
*   \return                 The number of T's required to store the absolute path.
*/
template <typename T>
inline size_t absolutesize(const T *path, const T *base, size_t pathLength = -1, size_t baseLength = -1)
{
    return absolute((T *)NULL, path, base, pathLength, baseLength);
}

}
}

#endif // __EASL_PATHS_ABSOLUTESIZE_H_
