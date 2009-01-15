/**
*   \brief  absolute.h
*   \author Dave Reid
*   \brief  Header file for the absolute() implementation.
*/
#ifndef __EASL_PATHS_ABSOLUTE_H_
#define __EASL_PATHS_ABSOLUTE_H_

#include <assert.h>
#include "../../string.h"
#include "../../nextchar.h"
#include "../../writechar.h"
#include "_private.h"               // easl/ext/paths/_private.h

namespace easl
{
namespace paths
{


/**
*   \brief              Turns a relative path into an absolute path.
*   \param  dest [in]   Pointer to the buffer that will recieve the absolute file path.
*   \param  path [in]   The path to turn into an abolute path.
*   \param  base [in]   The base part of the absolute path.
*   \return             The number of T's that are copied into dest including the null terminator.
*
*   \remarks
*       The \c base parameter must be a fully qualified absolute path. If it is not possible to
*       convert the path to an absolute path, the function will return 1 and \c dest will recieve
*       an empty string.
*       \par
*       This function does not check if the resulting path is a valid or existing directory or file.
*/
template <typename T>
size_t absolute(T *dest, const T *path, const T *base)
{
    assert(path != NULL);
    assert(base != NULL);

    // We need to retrieve the sub directories from each path.
    _SubDirPair<T> path_dirs[128];
    size_t path_count;
    _split_path(path_dirs, path_count, path);

    _SubDirPair<T> base_dirs[128];
    size_t base_count;
    _split_path(base_dirs, base_count, base);


    // Now we can begin to build our list of directory pairs that will eventually
    // be stitched together.
    _SubDirPair<T> final_path_dirs[256];
    size_t final_path_count = 0;

    // We first add all of our base directories to the list. We only add it to the list
    // if the string actually has content.
    for (size_t i = 0; i < base_count; ++i)
    {
        if (base_dirs[i].end - base_dirs[i].start != 0)
        {
            final_path_dirs[final_path_count++] = base_dirs[i];
        }
    }

    // Now we loop through all of our relative path sub directories and either
    // add or remove a directory to/from the list.
    for (size_t i = 0; i < path_count; ++i)
    {
        if (_pair_is_parent_dir(path_dirs[i]))
        {
            // If we already don't have anything in the count, the paths are invalid, so we will
            // return 0.
            if (final_path_count == 0)
            {
                if (dest != NULL)
                {
                    *dest = 0;
                }

                return 1;
            }

            --final_path_count;
        }
        else
        {
            final_path_dirs[final_path_count++] = path_dirs[i];
        }
    }

    // The variable that will store our final count.
    size_t count = final_path_count;

    // Now we need to stitch together our strings.
    for (size_t i = 0; i < final_path_count; ++i)
    {
        if (dest != NULL)
        {
            size_t copy_count = _copy_pair_to_str(dest, final_path_dirs[i]);
            dest += copy_count;

            count += copy_count;

            // Now we need to write a slash to divide our sub directories. We first need to know the
            // size of the character we are writing so we can move the pointer forward. If we're writing
            // the last sub directory and it is _not_ an empty string, we don't want to add the slash.
            if ((i != final_path_count - 1) && ((final_path_dirs[i].end - final_path_dirs[i].start) != 1))
            {
                // Now write the character.
                writechar(dest, EASL_PATH_SLASH);
            }
        }
        else
        {
            count += final_path_dirs[i].end - final_path_dirs[i].start;
        }
    }

    // We need to NULL terminate the destination.
    if (dest != NULL)
    {
        *dest = 0;
    }

    return count;
}


template <typename T>
void absolute(slow_string<T> &dest, const T *path, const T *base)
{
    T *&temp = dest.c_str();

    dest = new T[absolute((T *)NULL, path, base)];
    absolute(temp, path, base);
}


string8 absolute(const char *path, const char *base)
{
    string8 dest;
    absolute(dest, path, base);

    return dest;
}
string16 absolute(const char16_t *path, const char16_t *base)
{
    string16 dest;
    absolute(dest, path, base);

    return dest;
}
string32 absolute(const char32_t *path, const char32_t *base)
{
    string32 dest;
    absolute(dest, path, base);

    return dest;
}
wstring absolute(const wchar_t *path, const wchar_t *base)
{
    wstring dest;
    absolute(dest, path, base);

    return dest;
}


}
}

#endif // __EASL_PATHS_ABSOLUTE_H_
