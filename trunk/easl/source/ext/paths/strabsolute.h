/**
*   \brief  strabsolute.h
*   \author Dave Reid
*   \brief  Header file for the strabsolute() implementation.
*/
#ifndef __EASL_STRABSOLUTE_H_
#define __EASL_STRABSOLUTE_H_

#include "../../strnextchar.h"
#include "../../_private.h"

// Defines the directory slash for platforms.
#if (PLATFORM == PLATFORM_WINDOWS)
#define EASL_PATH_SLASH     '\\'
#else
#define EASL_PATH_SLASH     '/'
#endif

namespace easl
{
namespace paths
{

template <typename T>
struct _SubDirPair
{
    const T *start;
    const T *end;
};

template <typename T>
void _split_path(_SubDirPair<T> (&dest)[128], size_t &count, const T *path)
{
    // Our current sub directory. There is always at least one sub directory.
    _SubDirPair<T> current_pair;
    current_pair.start = path;

    // We need to ensure that our counter is at 0.
    count = 0;

    const T *temp = path;
    uchar32_t ch;
    while ((ch = easl::strnextchar(temp)) != NULL)
    {
        if (ch == '\\' || ch == '/')
        {
            current_pair.end = path;
            dest[count++] = current_pair;

            // We need to return straight away if we have too many sub directories.
            if (count == 128)
            {
                return;
            }

            // Now we can start our new pair.
            current_pair.start = temp;
        }
        
        path = temp;
    }

    // We need to add our last pair to the list.
    current_pair.end = temp;
    dest[count++] = current_pair;
}

/**
*   \brief              Determines if the specified string pair is equal to ".."
*   \param  dir [in]    The directory pair to check.
*   \return             True if the string is equal to ".."
*/
template <typename T>
bool _pair_is_parent_dir(const _SubDirPair<T> &dir)
{
    const T *temp = dir.start;
    uchar32_t ch1 = easl::strnextchar(temp);
    uchar32_t ch2 = easl::strnextchar(temp);
    if (temp == dir.end && ch1 == '.' && ch2 == '.')
    {
        return true;
    }

    return false;
}

/**
*   \brief              Copies a directory pair to a string.
*   \param  dest [out]  Pointer to the buffer that the string pair will be copied to.
*   \param  pair [in]   The pair that will be copied to the destination buffer.
*   \return             The number of T's that are copied over.
*/
template <typename T>
size_t _copy_pair_to_str(T *dest, const _SubDirPair<T> &pair)
{
    // Stores the number of T's that we're copying over.
    size_t count = 0;

    if (pair.start != pair.end)
    {
        const T *temp = pair.start;
        while (temp != pair.end)
        {
            // Grab the next character.
            uchar32_t ch = easl::strnextchar(temp);

            // Grab the size of the character so we can modify the final count.
            size_t char_size = easl::get_char_size<T>(ch);
            count += char_size;

            // Now write the character.
            write_char(dest, ch, char_size);

            dest += char_size;
        }
    }

    return count;
}


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
*       This function does not check if the resulting path is a valid directory.
*/
template <typename T>
size_t strabsolute(T *dest, const T *path, const T *base)
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
                return 0;
            }

            --final_path_count;
        }
        else
        {
            if (path_dirs[i].end - path_dirs[i].start != 0)
            {
                final_path_dirs[final_path_count++] = path_dirs[i];
            }
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
            if ((i != final_path_count - 1) && ((final_path_dirs[i].end - final_path_dirs[i].start) != 1) &&
                (final_path_dirs[i].end - final_path_dirs[i].start != 0))
            {
                size_t char_size = get_char_size<T>(EASL_PATH_SLASH);

                // Now write the character and move the pointer forward.
                write_char(dest, EASL_PATH_SLASH, char_size);
                dest += char_size;
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

}
}

#endif // __EASL_STRABSOLUTE_H_
