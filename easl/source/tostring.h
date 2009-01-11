/**
*   \file   tostring.h
*   \author Dave Reid
*   \brief  Header file for converting data types to strings.
*/
#ifndef __EASL_TOSTRING_H_
#define __EASL_TOSTRING_H_

#include "types.h"

namespace easl
{

/**
*   \brief                  Converts a value to a string.
*   \param  value    [in]   The value to convert.
*   \param  dest     [out]  Pointer to the buffer that will recieve the string.
*   \param  destSize [in]   The number of characters that can fit inside \c dest including the NULL terminator.
*
*   \remarks
*       The returned string is always NULL terminated. \c destSize specifies
*       the number of characters that can fit in \c dest, _NOT_ the number of
*       bytes.
*/
template <typename U, typename T>
inline void tostring(U value, T *dest, size_t destSize)
{
    // We need to store the value in a wchar_t string first and then convert to our destination.
    wchar_t *temp_dest = new wchar_t[destSize];

    tostring(value, temp_dest, destSize);

    easl::convert(dest, temp_dest);

    delete [] temp_dest;
}

#ifdef EASL_ONLY_ASCII
template <> inline void tostring(__int64 value, char *dest, size_t destSize)
{
    easl::sprintf(dest, destSize, "%I64d", value);
}
template <> inline void tostring(unsigned __int64 value, char *dest, size_t destSize)
{
    easl::sprintf(dest, destSize, "%I64u", value);
}
template <> inline void tostring(int value, char *dest, size_t destSize)
{
    easl::tostring(static_cast<__int64>(value), dest, destSize);
}
template <> inline void tostring(unsigned int value, char *dest, size_t destSize)
{
    easl::tostring(static_cast<unsigned __int64>(value), dest, destSize);
}
template <> inline void tostring(long value, char *dest, size_t destSize)
{
    easl::tostring(static_cast<__int64>(value), dest, destSize);
}
template <> inline void tostring(unsigned long value, char *dest, size_t destSize)
{
    easl::tostring(static_cast<unsigned __int64>(value), dest, destSize);
}
template <> inline void tostring(short value, char *dest, size_t destSize)
{
    easl::tostring(static_cast<__int64>(value), dest, destSize);
}
template <> inline void tostring(unsigned short value, char *dest, size_t destSize)
{
    easl::tostring(static_cast<unsigned __int64>(value), dest, destSize);
}
template <> inline void tostring(char value, char *dest, size_t destSize)
{
    easl::tostring(static_cast<__int64>(value), dest, destSize);
}
template <> inline void tostring(unsigned char value, char *dest, size_t destSize)
{
    easl::tostring(static_cast<unsigned __int64>(value), dest, destSize);
}
template <> inline void tostring(float value, char *dest, size_t destSize)
{
    easl::sprintf(dest, destSize, "%f", value);
}
template <> inline void tostring(double value, char *dest, size_t destSize)
{
    easl::sprintf(dest, destSize, "%.12g", value);
}
template <> inline void tostring(bool value, char *dest, size_t destSize)
{
    // We'll have to do locale dependant strings here.
    if (value)
    {
        easl::strcpy(dest, "true", destSize);
    }
    else
    {
        easl::strcpy(dest, "false", destSize);
    }
}
#endif
template <> inline void tostring(__int64 value, wchar_t *dest, size_t destSize)
{
    easl::sprintf(dest, destSize, L"%I64d", value);
}
template <> inline void tostring(unsigned __int64 value, wchar_t *dest, size_t destSize)
{
    easl::sprintf(dest, destSize, L"%I64u", value);
}

template <> inline void tostring(int value, wchar_t *dest, size_t destSize)
{
    easl::tostring(static_cast<__int64>(value), dest, destSize);
}
template <> inline void tostring(unsigned int value, wchar_t *dest, size_t destSize)
{
    easl::tostring(static_cast<unsigned __int64>(value), dest, destSize);
}

template <> inline void tostring(long value, wchar_t *dest, size_t destSize)
{
    easl::tostring(static_cast<__int64>(value), dest, destSize);
}
template <> inline void tostring(unsigned long value, wchar_t *dest, size_t destSize)
{
    easl::tostring(static_cast<unsigned __int64>(value), dest, destSize);
}

template <> inline void tostring(short value, wchar_t *dest, size_t destSize)
{
    easl::tostring(static_cast<__int64>(value), dest, destSize);
}
template <> inline void tostring(unsigned short value, wchar_t *dest, size_t destSize)
{
    easl::tostring(static_cast<unsigned __int64>(value), dest, destSize);
}

template <> inline void tostring(char value, wchar_t *dest, size_t destSize)
{
    easl::tostring(static_cast<__int64>(value), dest, destSize);
}
template <> inline void tostring(unsigned char value, wchar_t *dest, size_t destSize)
{
    easl::tostring(static_cast<unsigned __int64>(value), dest, destSize);
}

template <> inline void tostring(float value, wchar_t *dest, size_t destSize)
{
    easl::sprintf(dest, destSize, L"%f", value);
}
template <> inline void tostring(double value, wchar_t *dest, size_t destSize)
{
    easl::sprintf(dest, destSize, L"%.12g", value);
}

template <> inline void tostring(bool value, wchar_t *dest, size_t destSize)
{
    // TODO: Need to check the locale and do locale dependant values.
    if (value)
    {
        easl::strcpy(dest, L"true", destSize);
    }
    else
    {
        easl::strcpy(dest, L"false", destSize);
    }
}

template <size_t destSize, typename U, typename T>
inline void tostring(U value, T (&dest)[destSize])
{
    return tostring(value, dest, destSize);
}

}

#endif // __EASL_TOSTRING_H_
