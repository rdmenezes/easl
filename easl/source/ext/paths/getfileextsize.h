/**
*   \brief  getfileextsize.h
*   \author Dave Reid
*   \brief  Header file for the getfileextsize() implementation.
*/
#ifndef __EASL_PATHS_GETFILEEXTSIZE_H_
#define __EASL_PATHS_GETFILEEXTSIZE_H_

#include "getfileext.h"

namespace easl
{
namespace paths
{

/**
*   \brief                 Retrieves the number of T's required to store the file extension specified by the input string.
*   \param  str       [in] The string whose extension should be checked.
*   \param  strLength [in] The length in T's of the string, not including the null terminator.
*   \return                The number of T's that must be allocated in order to store the extension of \c src.
*/
template <typename T>
size_t getfileextsize(const T *str, size_t strLength = -1)
{
    return getfileext((T *)NULL, str, strLength);
}

}
}

#endif // __EASL_PATHS_GETFILEEXTSIZE_H_
