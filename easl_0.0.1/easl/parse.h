/**
*   \file   parse.h
*   \author Dave Reid
*   \brief  Header file for parsing strings.
*/
#ifndef __EASL_PARSE_H_
#define __EASL_PARSE_H_

#include <stdlib.h>
#include <locale.h>
#include "setup.h"
#include "types.h"
#include "strstr.h"
#include "strequal.h"

namespace easl
{

/**
*   \brief              Converts a string to another data type.
*   \param  str [in]    The string to convert.
*   \return             The value that the string was converted to.
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
U parse(const T *str)
{
    // If a specialisation hasn't been created for the U data type, the compiler should
    // throw an error.

    wchar_t *temp = new wchar_t[easl::strconvertsize<wchar_t>(str)];
    easl::strconvert(temp, str);

    U value = parse<U>(temp);

    delete [] temp;

    return value;
}

#ifdef EASL_ONLY_ASCII
template <> __int64 parse(const char *str)
{
#if ((COMPILER & COMPILER_VC) && COMPILER >= COMPILER_VC80)
    return ::_strtoi64(str, NULL, 10);
#else
    return ::strtoll(str, NULL, 10);
#endif
}
template <> unsigned __int64 parse(const char *str)
{
#if ((COMPILER & COMPILER_VC) && COMPILER >= COMPILER_VC80)
    return ::_strtoui64(str, NULL, 10);
#else
    return ::strtoull(str, NULL, 10);
#endif
}

template <> int parse(const char *str)
{
    return static_cast<int>(parse<__int64>(str));
}
template <> unsigned int parse(const char *str)
{
    return static_cast<unsigned int>(parse<unsigned __int64>(str));
}

template <> long parse(const char *str)
{
    return static_cast<long>(parse<__int64>(str));
}
template <> unsigned long parse(const char *str)
{
    return static_cast<unsigned long>(parse<unsigned __int64>(str));
}

template <> short parse(const char *str)
{
    return static_cast<short>(parse<__int64>(str));
}
template <> unsigned short parse(const char *str)
{
    return static_cast<unsigned short>(parse<unsigned __int64>(str));
}

template <> char parse(const char *str)
{
    return static_cast<signed char>(parse<__int64>(str));
}
template <> unsigned char parse(const char *str)
{
    return static_cast<unsigned char>(parse<unsigned __int64>(str));
}

template <> float parse(const char *str)
{
    return static_cast<float>(::atof(str));
}
template <> double parse(const char *str)
{
    return ::atof(str);
}

template <> bool parse(const char *str)
{
    if (str == NULL || easl::strequal(str, "false", false) || easl::strequal(str, "0", false))
    {
        return false;
    }

    return true;
}
#endif

template <> __int64 parse(const wchar_t *str)
{
#if ((COMPILER & COMPILER_VC) && COMPILER >= COMPILER_VC80)
    return ::_wcstoi64(str, NULL, 10);
#else
    return ::wcstoll(str, NULL, 10);
#endif
}
template <> unsigned __int64 parse(const wchar_t *str)
{
#if ((COMPILER & COMPILER_VC) && COMPILER >= COMPILER_VC80)
    return ::_wcstoui64(str, NULL, 10);
#else
    return ::wcstoull(str, NULL, 10);
#endif
}

template <> int parse(const wchar_t *str)
{
    return static_cast<int>(parse<__int64>(str));
}
template <> unsigned int parse(const wchar_t *str)
{
    return static_cast<unsigned int>(parse<unsigned __int64>(str));
}

template <> long parse(const wchar_t *str)
{
    return static_cast<long>(parse<__int64>(str));
}
template <> unsigned long parse(const wchar_t *str)
{
    return static_cast<unsigned long>(parse<unsigned __int64>(str));
}

template <> short parse(const wchar_t *str)
{
    return static_cast<short>(parse<__int64>(str));
}
template <> unsigned short parse(const wchar_t *str)
{
    return static_cast<unsigned short>(parse<unsigned __int64>(str));
}

template <> signed char parse(const wchar_t *str)
{
    return static_cast<signed char>(parse<__int64>(str));
}
template <> unsigned char parse(const wchar_t *str)
{
    return static_cast<unsigned char>(parse<unsigned __int64>(str));
}

template <> float parse(const wchar_t *str)
{
    return static_cast<float>(::_wtof(str));
}
template <> double parse(const wchar_t *str)
{
    return ::_wtof(str);
}

template <> bool parse(const wchar_t *str)
{
    // How do we handle different languages for this? We might have to look at the locale and do
    // specific versions based on that locale... seems like it would be quite slow, though.
    wchar_t *locale = ::_wsetlocale(LC_ALL, NULL);

    if (easl::strstr(locale, L"English") || easl::strstr(locale, L"english"))
    {
        if (str == NULL || easl::strequal(str, L"false", false) || easl::strequal(str, L"0", false))
        {
            return false;
        }
    }

    return true;
}

}

#endif // __EASL_PARSE_H_
