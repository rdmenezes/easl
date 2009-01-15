/**
*   \brief  isrelative.h
*   \author Dave Reid
*   \brief  Header file for the isrelative() implementation.
*/
#ifndef __EASL_PATHS_ISRELATIVE_H_
#define __EASL_PATHS_ISRELATIVE_H_

#include "isabsolute.h"

namespace easl
{
namespace paths
{

/**
*   \brief              Determines if a given path is relative.
*   \param  path [in]   The path to check for relativeness.
*   \return             True if the path is relative; false otherwise.
*
*   \remarks
*       This function simply returns !isabsolute().
*/
template <typename T>
bool isrelative(const T *path)
{
    return !isabsolute();
}

}
}

#endif // __EASL_PATHS_ISRELATIVE_H_