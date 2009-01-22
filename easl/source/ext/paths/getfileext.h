/**
*   \brief  getfileext.h
*   \author Dave Reid
*   \brief  Header file for the getfileext() implementation.
*/
#ifndef __EASL_PATHS_GETFILEEXT_H_
#define __EASL_PATHS_GETFILEEXT_H_

#include <assert.h>
#include "../../nextchar.h"
#include "../../string.h"
#include "../../reference_string.h"

namespace easl
{
namespace paths
{

/**
*   \brief                 Retrieves the extension of a file.
*   \param  str       [in] The string that contains the file name whose extension is to be retrieved.
*   \param  dest      [in] Pointer to the buffer that will store the extension.
*   \param  srcLength [in] The length in T's of the source string, not including the null terminator.
*   \return                The number of T's that were copied into dest.
*
*   \remarks
*       If an extension is not found, an empty string is sent to \c dest. The destination
*       string is always null terminated.
*       \par
*       If \c dest is NULL, the function will return the number of T's that are required to
*       store the null terminated extension string inside \c dest.
*       \par
*       The extension of the string is considered to be the part of the string after the
*       last period ('.'). It is valid to specify a full file path if it's more convienient.
*       However, in doing so, the function will consider _anything_ after the last period
*       in the string to be the extension. In addition, there will be added overhead to the
*       function because it must scan a larger string if the full file path is specified.
*/
template <typename T>
size_t getfileext(T *dest, const T *src, size_t srcLength = -1)
{
    assert(src != NULL);

    // Need to account for localisation here.

    // Stores the start of the next character.
    T *start_of_ch = src;

    // We need to find the last occurance of a full stop characters. We'll do this
    // by storing each occurance of a full stop that we find. The one that stored when
    // we reach the end of the file will be the last one.
    const T *last_pos = NULL;
    uchar32_t ch;
    while (srcLength > 0 && (ch = nextchar(src)) != NULL)
    {
        if (ch == '.')
        {
            last_pos = src;
        }

        srcLength -= src - start_of_ch;
        start_of_ch = src;
    }

    // If we didn't find a decimal point, there is no extension.
    if (last_pos == NULL)
    {
        if (dest != NULL)
        {
            dest[0] = NULL;
        }

        return 1;
    }

    // Now calculate the number of T's that are required to store the extension.
    size_t ext_size = (src - last_pos) + 1;

    if (dest != NULL)
    {
        // Now we need only do a string copy into the destination.
        easl::copy(dest, last_pos, ext_size);
    }

    return ext_size;
}

template <typename T>
void getfileext(slow_string<T> &dest, const T *src, size_t srcLength = -1)
{
    assert(src != NULL);

    // Need to account for localisation here.

    // Stores the start of the next character.
    const T *start_of_ch = src;

    // We need to find the last occurance of a full stop characters. We'll do this
    // by storing each occurance of a full stop that we find. The one that stored when
    // we reach the end of the file will be the last one.
    const T *last_pos = NULL;
    uchar32_t ch;
    while (srcLength > 0 && (ch = easl::nextchar(src)) != NULL)
    {
        if (ch == '.')
        {
            last_pos = src;
        }

        srcLength -= src - start_of_ch;
        start_of_ch = src;
    }

    // If we didn't find a decimal point, there is no extension.
    if (last_pos == NULL)
    {
        dest = slow_string<T>();
    }
    else
    {
        dest = last_pos;
    }
}


inline string8 getfileext(const char *src, size_t srcLength = -1)
{
    string8 ret;
    getfileext(ret, src, srcLength);

    return ret;
}
inline string16 getfileext(const char16_t *src, size_t srcLength = -1)
{
    string16 ret;
    getfileext(ret, src, srcLength);

    return ret;
}
inline string32 getfileext(const char32_t *src, size_t srcLength = -1)
{
    string32 ret;
    getfileext(ret, src, srcLength);

    return ret;
}
inline wstring getfileext(const wchar_t *src, size_t srcLength = -1)
{
    wstring ret;
    getfileext(ret, src, srcLength);

    return ret;
}


template <typename T>
reference_string<T> getfileext(T *src, size_t srcLength = -1)
{
    assert(src != NULL);

    // Need to account for localisation here.

    // Stores the start of the next character.
    T *start_of_ch = src;

    // We need to find the last occurance of a full stop characters. We'll do this
    // by storing each occurance of a full stop that we find. The one that stored when
    // we reach the end of the file will be the last one.
    T *last_pos = NULL;
    uchar32_t ch;
    while (srcLength > 0 && (ch = nextchar(src)) != NULL)
    {
        if (ch == '.')
        {
            last_pos = src;
        }

        srcLength -= src - start_of_ch;
        start_of_ch = src;
    }

    // Our return value.
    reference_string<T> ret;
    ret.end = src;

    if (last_pos == NULL)
    {
        ret.start = src;
    }
    else
    {
        ret.start = last_pos;
    }

    return ret;
}


}
}

#endif // __EASL_PATHS_GETFILEEXT_H_
