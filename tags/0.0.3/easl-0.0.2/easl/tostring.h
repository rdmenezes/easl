/**
*   \file   tostring.h
*   \author Dave Reid
*   \brief  Header file for converting data types to strings.
*/
#ifndef __EASL_TOSTRING_H_
#define __EASL_TOSTRING_H_

#include "types.h"
#include "copy.h"

namespace easl
{

#ifdef EASL_ONLY_ASCII
inline void tostring(__int64 value, char *dest, size_t destSize)
{
    easl::format(dest, destSize, "%I64d", value);
}
inline void tostring(unsigned __int64 value, char *dest, size_t destSize)
{
    easl::format(dest, destSize, "%I64u", value);
}
inline void tostring(int value, char *dest, size_t destSize)
{
    easl::tostring(static_cast<__int64>(value), dest, destSize);
}
inline void tostring(unsigned int value, char *dest, size_t destSize)
{
    easl::tostring(static_cast<unsigned __int64>(value), dest, destSize);
}
inline void tostring(long value, char *dest, size_t destSize)
{
    easl::tostring(static_cast<__int64>(value), dest, destSize);
}
inline void tostring(unsigned long value, char *dest, size_t destSize)
{
    easl::tostring(static_cast<unsigned __int64>(value), dest, destSize);
}
inline void tostring(short value, char *dest, size_t destSize)
{
    easl::tostring(static_cast<__int64>(value), dest, destSize);
}
inline void tostring(unsigned short value, char *dest, size_t destSize)
{
    easl::tostring(static_cast<unsigned __int64>(value), dest, destSize);
}
inline void tostring(char value, char *dest, size_t destSize)
{
    easl::tostring(static_cast<__int64>(value), dest, destSize);
}
inline void tostring(unsigned char value, char *dest, size_t destSize)
{
    easl::tostring(static_cast<unsigned __int64>(value), dest, destSize);
}
inline void tostring(float value, char *dest, size_t destSize)
{
    easl::format(dest, destSize, "%f", value);
}
inline void tostring(double value, char *dest, size_t destSize)
{
    easl::format(dest, destSize, "%.12g", value);
}
inline void tostring(bool value, char *dest, size_t destSize)
{
    // We'll have to do locale dependant strings here.
    if (value)
    {
        easl::copy(dest, "true", destSize, 4);
    }
    else
    {
        easl::copy(dest, "false", destSize, 5);
    }
}
#endif


inline void tostring(__int64 value, wchar_t *dest, size_t destSize)
{
    easl::format(dest, destSize, L"%I64d", value);
}
inline void tostring(unsigned __int64 value, wchar_t *dest, size_t destSize)
{
    easl::format(dest, destSize, L"%I64u", value);
}

inline void tostring(int value, wchar_t *dest, size_t destSize)
{
    easl::tostring(static_cast<__int64>(value), dest, destSize);
}
inline void tostring(unsigned int value, wchar_t *dest, size_t destSize)
{
    easl::tostring(static_cast<unsigned __int64>(value), dest, destSize);
}

inline void tostring(long value, wchar_t *dest, size_t destSize)
{
    easl::tostring(static_cast<__int64>(value), dest, destSize);
}
inline void tostring(unsigned long value, wchar_t *dest, size_t destSize)
{
    easl::tostring(static_cast<unsigned __int64>(value), dest, destSize);
}

inline void tostring(short value, wchar_t *dest, size_t destSize)
{
    easl::tostring(static_cast<__int64>(value), dest, destSize);
}
inline void tostring(unsigned short value, wchar_t *dest, size_t destSize)
{
    easl::tostring(static_cast<unsigned __int64>(value), dest, destSize);
}

inline void tostring(char value, wchar_t *dest, size_t destSize)
{
    easl::tostring(static_cast<__int64>(value), dest, destSize);
}
inline void tostring(unsigned char value, wchar_t *dest, size_t destSize)
{
    easl::tostring(static_cast<unsigned __int64>(value), dest, destSize);
}

inline void tostring(float value, wchar_t *dest, size_t destSize)
{
    easl::format(dest, destSize, L"%f", value);
}
inline void tostring(double value, wchar_t *dest, size_t destSize)
{
    easl::format(dest, destSize, L"%.12g", value);
}

inline void tostring(bool value, wchar_t *dest, size_t destSize)
{
    // TODO: Need to check the locale and do locale dependant values.
    if (value)
    {
        easl::copy(dest, L"true", destSize, 4);
    }
    else
    {
        easl::copy(dest, L"false", destSize, 5);
    }
}

/**
*   \brief                 Converts a value to a string.
*   \param  value    [in]  The value to convert.
*   \param  dest     [out] Pointer to the buffer that will recieve the string.
*   \param  destSize [in]  The number of characters that can fit inside \c dest including the null terminator.
*
*   \remarks
*       The returned string is always NULL terminated. \c destSize specifies
*       the number of characters that can fit in \c dest, _NOT_ the number of
*       bytes.
*/
template <typename U, typename T>
inline void tostring(U value, T *dest, size_t destSize)
{
    // NOTE: This is bugged. destSize can be smaller than is required. It won't crash, but the
    // results might not be optimal.

    // We need to store the value in a wchar_t string first and then convert to our destination.
    wchar_t *temp_dest = new wchar_t[destSize];

    tostring(value, temp_dest, destSize);

    easl::copy(dest, temp_dest, destSize);

    delete [] temp_dest;
}

template <size_t destSize, typename U, typename T>
inline void tostring(U value, T (&dest)[destSize])
{
    return tostring(value, (T *)dest, destSize);
}

}

#endif // __EASL_TOSTRING_H_
