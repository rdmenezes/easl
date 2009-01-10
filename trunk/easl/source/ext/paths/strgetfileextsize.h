/**
*   \brief  strgetfileextsize.h
*   \author Dave Reid
*   \brief  Header file for the strgetfileextsize() implementation.
*/
#ifndef __EASL_PATH_STRGETFILEEXTSIZE_H_
#define __EASL_PATH_STRGETFILEEXTSIZE_H_

#include "strgetfileext.h"

namespace easl
{
namespace paths
{

/**
*   \brief              Retrieves the number of T's required to store the file extension specified by the input string.
*   \param  str [in]    The string whose extension should be checked.
*   \return             The number of T's that must be allocated in order to store the extension of \c src.
*/
template <typename T>
size_t strgetfileextsize(const T *str)
{
    return strgetfileext(str, (T *)NULL);
}

}
}

#endif // __EASL_PATH_STRGETFILEEXTSIZE_H_
