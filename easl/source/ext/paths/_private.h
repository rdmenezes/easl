/**
*   \file   _private.h
*   \author Dave Reid
*   \brief  Header file for non-public functions for the paths extension.
*/
#ifndef __EASL_PATHS_PRIVATE_H_
#define __EASL_PATHS_PRIVATE_H_

#include <ctype.h>

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

// Structure representing a non-null terminated string. The end of the string
// is determined by another pointer.
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
    while ((ch = easl::nextchar(temp)) != NULL)
    {
        if (ch == '\\' || ch == '/')
        {
            current_pair.end = path;

            if (current_pair.end - current_pair.start != 0)
            {
                dest[count++] = current_pair;

                // We need to return straight away if we have too many sub directories.
                if (count == 128)
                {
                    return;
                }
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
    uchar32_t ch1 = easl::nextchar(temp);
    uchar32_t ch2 = easl::nextchar(temp);
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
            uchar32_t ch = easl::nextchar(temp);

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
*   \brief              Compares two sub directory pairs for equality.
*   \param  dir1[in]    The first operand.
*   \param  dir2[in]    The second operand.
*   \return             True if the two directories are equal.
*
*   \remarks
*       The comparison is not case sensitive.
*/
template <typename T>
bool _compare_pair(const _SubDirPair<T> &str1, const _SubDirPair<T> &str2)
{
    const T *temp1 = str1.start;
    const T *temp2 = str2.start;

    uchar32_t ch1 = easl::nextchar(temp1);
    uchar32_t ch2 = easl::nextchar(temp2);

    while (temp1 != str1.end && temp2 != str2.end)
    {
        if (tolower(ch1) != tolower(ch2))
        {
            return false;
        }

        ch1 = easl::nextchar(temp1);
        ch2 = easl::nextchar(temp2);
    }

    // If both strings aren't at their ends then they are not equal.
    if (temp1 != str1.end && temp2 != str2.end)
    {
        return false;
    }

    return true;
}

/**
*   \brief              Writes the "../" directory to the destination.
*   \param  dest [out]  The string that will recieve the "../" string.
*   \return             The number of T's that are written to the destination.
*
*   \remarks
*       If \c dest is NULL, the function will return the number of T's that are required
*       to store the "../" string.
*       \par
*       On Windows platforms, the string is "..\".
*       \par
*       This function will move the destination pointer forward to just past the newly
*       written string.
*/
template <typename T>
size_t _write_parent_dir(T *&dest)
{
    size_t count = 0;

    size_t char_size = get_char_size<T>('.');
    if (dest != NULL)
    {
        write_char(dest, '.', char_size);
        dest += char_size;
        write_char(dest, '.', char_size);
        dest += char_size;
    }
    count += char_size * 2;

    char_size = get_char_size<T>(EASL_PATH_SLASH);
    if (dest != NULL)
    {
        write_char(dest, EASL_PATH_SLASH, char_size);
        dest += char_size;
    }
    count += char_size;

    return count;
}


}
}

#endif // __EASL_PATHS_PRIVATE_H_
