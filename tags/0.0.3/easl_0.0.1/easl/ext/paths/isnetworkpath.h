/**
*   \brief  isnetworkpath.h
*   \author Dave Reid
*   \brief  Header file for the isnetworkpath() implementation.
*/
#ifndef __EASL_PATHS_ISNETWORKPATH_H_
#define __EASL_PATHS_ISNETWORKPATH_H_

#include "../../nextchar.h"

namespace easl
{
namespace paths
{

/**
*   \brief              Checks to see if the specified string is a network path.
*   \param  path [in]   The path to check.
*   \return             True if the path is a network path; false otherwise.
*/
template <typename T>
bool isnetworkpath(const T *path)
{
#if (PLATFORM == PLATFORM_WINDOWS)
    // We need to look at the first character. If it is a slash, we need to look at the next one.
    uchar32_t ch = nextchar(path);
    if (ch == '\\' || ch == '/')
    {
        // Grab the next character. If it is also a back slash, we have a network path.
        ch = nextchar(path);
        if (ch == '\\' || ch == '/')
        {
            return true;
        }
    }
#else
#pragma message ("Warning: isnetworkpath() is only supported on Windows for the moment.")
#endif

    return false;
}

}
}

#endif // __EASL_PATHS_ISNETWORKPATH_H_
