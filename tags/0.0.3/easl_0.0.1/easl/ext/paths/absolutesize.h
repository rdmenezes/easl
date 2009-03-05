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

template <typename T>
size_t absolutesize(const T *path, const T *base)
{
    return absolute((T *)NULL, path, base);
}

}
}

#endif // __EASL_PATHS_ABSOLUTESIZE_H_
