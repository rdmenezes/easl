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

template <typename T>
size_t relativesize(const T *path, const T *base)
{
    return relative((T *)NULL, path, base);
}

}
}


#endif // __EASL_PATHS_RELATIVESIZE_H_
