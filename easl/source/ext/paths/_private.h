/**
*   \file   _private.h
*   \author Dave Reid
*   \brief  Header file for non-public functions for the paths extension.
*/
#ifndef __EASL_PATHS_PRIVATE_H_
#define __EASL_PATHS_PRIVATE_H_

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


}
}

#endif // __EASL_PATHS_PRIVATE_H_
