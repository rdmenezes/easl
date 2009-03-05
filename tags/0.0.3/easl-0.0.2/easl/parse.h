/**
*   \file   parse.h
*   \author Dave Reid
*   \brief  Header file for parse() implementations.
*/
#ifndef __EASL_PARSE_H_
#define __EASL_PARSE_H_

#include <stdlib.h>
#include <locale.h>
#include "setup.h"
#include "types.h"
#include "findfirst.h"
#include "equal.h"

namespace easl
{

/**
*   \brief                 Converts a string to another data type.
*   \param  str       [in] The string to convert.
*   \param  strLength [in] The length of the string in T's, not including the null terminator.
*   \return                The value that the string was converted to.
*
*   \remarks
*       Explicit instantiations can be created so that custom data types can be parsed.
*       For an example, take a look at the explicit instantiations for int's, float's etc.
*/
// T is the string type. U is the type we are converting to (int, float, etc).
// It's important to make U first because we want to keep the character encoding type
// hidden to keep the API as simple as possible. We use T for the string type because
// most other functions use T for strings also.
template <typename U, typename T>
inline U parse(const T *str, size_t strLength = -1)
{
    // If a specialisation hasn't been created for the U data type, the compiler should
    // throw an error.

    size_t temp_size = easl::copysize<wchar_t>(str, strLength);
    wchar_t *temp = new wchar_t[temp_size];
    easl::copy(temp, str, temp_size);

    U value = parse<U>(temp);

    delete [] temp;

    return value;
}

#ifdef EASL_ONLY_ASCII
template <> inline __int64 parse(const char *str, size_t strLength)
{
    (void)strLength;

#if ((COMPILER & COMPILER_VC) && COMPILER >= COMPILER_VC80)
    return ::_strtoi64(str, NULL, 10);
#else
    return ::strtoll(str, NULL, 10);
#endif
}
template <> inline unsigned __int64 parse(const char *str, size_t strLength)
{
    (void)strLength;

#if ((COMPILER & COMPILER_VC) && COMPILER >= COMPILER_VC80)
    return ::_strtoui64(str, NULL, 10);
#else
    return ::strtoull(str, NULL, 10);
#endif
}

template <> inline int parse(const char *str, size_t strLength)
{
    return static_cast<int>(parse<__int64>(str, strLength));
}
template <> inline unsigned int parse(const char *str, size_t strLength)
{
    return static_cast<unsigned int>(parse<unsigned __int64>(str, strLength));
}

template <> inline long parse(const char *str, size_t strLength)
{
    return static_cast<long>(parse<__int64>(str, strLength));
}
template <> inline unsigned long parse(const char *str, size_t strLength)
{
    return static_cast<unsigned long>(parse<unsigned __int64>(str, strLength));
}

template <> inline short parse(const char *str, size_t strLength)
{
    return static_cast<short>(parse<__int64>(str, strLength));
}
template <> inline unsigned short parse(const char *str, size_t strLength)
{
    return static_cast<unsigned short>(parse<unsigned __int64>(str, strLength));
}

template <> inline char parse(const char *str, size_t strLength)
{
    return static_cast<signed char>(parse<__int64>(str, strLength));
}
template <> inline unsigned char parse(const char *str, size_t strLength)
{
    return static_cast<unsigned char>(parse<unsigned __int64>(str, strLength));
}

template <> inline float parse(const char *str, size_t strLength)
{
    (void)strLength;

    return static_cast<float>(::atof(str));
}
template <> inline double parse(const char *str, size_t strLength)
{
    (void)strLength;

    return ::atof(str);
}

template <> inline bool parse(const char *str, size_t strLength)
{
    if (str == NULL || equal(str, "false", false, strLength) || equal(str, "0", false, strLength))
    {
        return false;
    }

    return true;
}
#endif

template <> inline __int64 parse(const wchar_t *str, size_t strLength)
{
    (void)strLength;

#if ((COMPILER & COMPILER_VC) && COMPILER >= COMPILER_VC80)
    return ::_wcstoi64(str, NULL, 10);
#else
    return ::wcstoll(str, NULL, 10);
#endif
}
template <> inline unsigned __int64 parse(const wchar_t *str, size_t strLength)
{
    (void)strLength;

#if ((COMPILER & COMPILER_VC) && COMPILER >= COMPILER_VC80)
    return ::_wcstoui64(str, NULL, 10);
#else
    return ::wcstoull(str, NULL, 10);
#endif
}

template <> inline int parse(const wchar_t *str, size_t strLength)
{
    return static_cast<int>(parse<__int64>(str, strLength));
}
template <> inline unsigned int parse(const wchar_t *str, size_t strLength)
{
    return static_cast<unsigned int>(parse<unsigned __int64>(str, strLength));
}

template <> inline long parse(const wchar_t *str, size_t strLength)
{
    return static_cast<long>(parse<__int64>(str, strLength));
}
template <> inline unsigned long parse(const wchar_t *str, size_t strLength)
{
    return static_cast<unsigned long>(parse<unsigned __int64>(str, strLength));
}

template <> inline short parse(const wchar_t *str, size_t strLength)
{
    return static_cast<short>(parse<__int64>(str, strLength));
}
template <> inline unsigned short parse(const wchar_t *str, size_t strLength)
{
    return static_cast<unsigned short>(parse<unsigned __int64>(str, strLength));
}

template <> inline signed char parse(const wchar_t *str, size_t strLength)
{
    return static_cast<signed char>(parse<__int64>(str, strLength));
}
template <> inline unsigned char parse(const wchar_t *str, size_t strLength)
{
    return static_cast<unsigned char>(parse<unsigned __int64>(str, strLength));
}

template <> inline float parse(const wchar_t *str, size_t strLength)
{
    (void)strLength;

    return static_cast<float>(::_wtof(str));
}
template <> inline double parse(const wchar_t *str, size_t strLength)
{
    (void)strLength;

    return ::_wtof(str);
}

template <> inline bool parse(const wchar_t *str, size_t strLength)
{
    // How do we handle different languages for this? We might have to look at the locale and do
    // specific versions based on that locale... seems like it would be quite slow, though.
    wchar_t *locale = ::_wsetlocale(LC_ALL, NULL);

    if (findfirst(locale, L"English") || findfirst(locale, L"english"))
    {
        if (str == NULL || equal(str, L"false", false, strLength) || equal(str, L"0", false, strLength))
        {
            return false;
        }
    }

    return true;
}

template <typename U, typename T>
inline U parse(const reference_string<T> &str)
{
    return parse<U>(str.start, length(str));
}

}

#endif // __EASL_PARSE_H_
