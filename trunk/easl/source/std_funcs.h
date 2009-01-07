/**
*   \file   std_funcs.h
*   \author Dave Reid
*   \brief  Header file for the C-style string function wrappers.
*/
#ifndef __EASL_STD_FUNCS_H_
#define __EASL_STD_FUNCS_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <assert.h>
#include "setup.h"
#include "types.h"

namespace easl
{

/**
*   \brief              Retrieves the length of a C style string.
*   \param  str [in]    The string to retrieve the length of.
*   \return             The length of the C style string.
*
*   \remarks
*       This function return the number of basic character units in the string. A
*       "basic character unit" is a 'char' for char* strings and a 'wchar_t' in
*       whcar_t* strings. The same sort of things applies for UTF strings.
*       \par
*       To retrieve the number of characters in the string insteaad of basic character
*       units, use strchrcount() instead.
*/
template <typename T>
inline size_t strlen(const T *str)
{
    assert(str != NULL);

    const T *temp = str;

    while (*temp != NULL)
    {
        ++temp;
    }

    return temp - str;
}
template <> inline size_t strlen(const char *str)
{
    assert(str != NULL);

    return ::strlen(str);
}
template <> inline size_t strlen(const wchar_t *str)
{
    assert(str != NULL);

    return ::wcslen(str);
}

/**
*   \brief              Compares two strings for equality.
*   \param  str1 [in]   The first string to compare.
*   \param  str2 [in]   The second string to compare.
*   \return             0 if the two strings are equal; -1 if \c str1 is lower than \c str2; +1 otherwise.
*/
// NOTE: Not sure about the implementation of strcmp(). Do we do the comparisons
// T by T, or by character by character?
template <typename T>
inline int strcmp(const T *str1, const T *str2)
{
    assert(str1 != NULL);
    assert(str2 != NULL);

    int ret = 0;

    for (;;)
    {
        ret = *str1 - *str2;

        if (ret != 0 || *str2 == NULL)
        {
            break;
        }

        ++str1;
        ++str2;
    }

    if (ret < 0)
    {
        return -1;
    }
    else if (ret > 0)
    {
        return 1;
    }

    return 0;
}
#ifdef EASL_ONLY_ASCII
template <> inline int strcmp(const char *str1, const char *str2)
{
    assert(str2 != NULL);
    assert(str2 != NULL);

    // NOTE: If the comparison is character by character and not byte by byte, we can't
    // use this method unless we're doing english only.
    return ::strcmp(str1, str2);
}
#endif
template <> inline int strcmp(const wchar_t *str1, const wchar_t *str2)
{
    assert(str2 != NULL);
    assert(str2 != NULL);

    return ::wcscmp(str1, str2);
}

/**
*   \brief                  Copies one string to another.
*   \param  dest     [out]  The destination string.
*   \param  src      [in]   The source string.
*   \param  destSize [in]   The size in T's of the buffer pointed to by \c dest.
*   \return                 Zero is successful; an error otherwise.
*/
template <typename T>
inline errno_t strcpy(T *dest, const T *src, size_t destSize)
{
    if (dest == NULL || src == NULL || destSize == 0)
    {
        return EINVAL;
    }

    // Now we loop through and copy character by character.
    while (destSize - 1 > 0 && *src != NULL)
    {
        *dest = *src;

        ++dest;
        ++src;
        --destSize;
    }

    // Ensure that we have a null terminator.
    *dest = NULL;

    return 0;
}
inline errno_t strcpy(char *dest, const char *src, size_t destSize)
{
#if ((COMPILER & COMPILER_VC) && COMPILER >= COMPILER_VC80)
    return ::strncpy_s(dest, destSize, src, destSize);
#else
	::strncpy(dest, src, destSize);
    return 0;
#endif
}
inline errno_t strcpy(wchar_t *dest, const wchar_t *src, size_t destSize)
{
#if ((COMPILER & COMPILER_VC) && COMPILER >= COMPILER_VC80)
    return ::wcsncpy_s(dest, destSize, src, destSize);
#else
	::wcsncpy(dest, src, destSize);
    return 0;
#endif
}

template <size_t destSize, typename T>
inline errno_t strcpy(T (&dest)[destSize], const T *src)
{
    return easl::strcpy(dest, src, destSize);
}


/**
*   \brief                  Appends a string onto another string.
*   \param  dest     [out]  The string that will be appended with the other string.
*   \param  src      [in]   The string to append to the destination string.
*   \param  destSize [in]   The size in T's of the buffer pointed to by \c dest.
*   \return                 Zero is successful; an error on failure.
*
*   \remarks
*       The destination buffer must be NULL terminated to indicate the end of the string.
*/
template <typename T>
inline errno_t strcat(T *dest, const T *src, size_t destSize)
{
    if (dest == NULL || src == NULL || destSize == 0)
    {
        return EINVAL;
    }

    // We need to get to the end of the destination string.
    while (*dest != NULL)
    {
        ++dest;
        --destSize;
    };

    // Now simply copy the string over.
    easl::strcpy(dest, src, destSize);

    return 0;
}
#ifdef EASL_ONLY_ASCII
template <> inline errno_t strcat(char *dest, const char *src, size_t destSize)
{
#if ((COMPILER & COMPILER_VC) && COMPILER >= COMPILER_VC80)
    return ::strncat_s(dest, destSize, src, destSize);
#else
	::strncat(dest, src, destSize);
    return 0;
#endif
}
#endif
template <> inline errno_t strcat(wchar_t *dest, const wchar_t *src, size_t destSize)
{
#if ((COMPILER & COMPILER_VC) && COMPILER >= COMPILER_VC80)
    return ::wcsncat_s(dest, destSize, src, destSize);
#else
	::wcsncat(dest, src, destSize);
    return 0;
#endif
}

template <size_t destSize, typename T> 
inline errno_t strcat(char (&dest)[destSize], const T *src)
{
    return easl::strcat(dest, src, destSize);
}


/**
*   \brief                  Finds the first occurance of a character inside a string.
*   \param  str       [in]  The string to look for the character in.
*   \param  character [in]  The character to look for.
*   \return                 If the character is found, returns a pointer to that character. Otherwise, a NULL pointer is returned.
*/
template <typename T>
inline T * strchr(T *str, uchar32_t character)
{
    T *tmp = str;

    uchar32_t ch;
    while ((ch = easl::strnextchar(str)) != character && ch != NULL)
    {
        str = tmp;
    }

    return str;
}
#ifdef EASL_ONLY_ASCII
template <> inline char * strchr(char *str, uchar32_t character)
{
    return ::strchr(str, character);
}
#endif
template <> inline wchar_t * strchr(wchar_t *str, uchar32_t character)
{
    return ::wcschr(str, (wchar_t)character);
}

template <typename T>
inline const T * strchr(const T *str, uchar32_t character)
{
    const T *tmp = str;

    uchar32_t ch;
    while ((ch = easl::strnextchar(tmp)) != character && ch != NULL)
    {
        str = tmp;
    }

    return str;
}
#ifdef EASL_ONLY_ASCII
template <> inline const char * strchr(const char *str, uchar32_t character)
{
    return ::strchr(str, character);
}
#endif
template <> inline const wchar_t * strchr(const wchar_t *str, uchar32_t character)
{
    return ::wcschr(str, (wchar_t)character);
}


/**
*   \brief              Finds a string from within another string.
*   \param  str1 [in]   The string to be scanned.
*   \param  str2 [in]   The string to look for inside \c str1.
*   \return             A pointer to the first occurance of \c str2; or NULL if the string is not found.
*/
template <typename T>
inline T * strstr(T *str1, const T *str2)
{
    return NULL;
}
#ifdef EASL_ONLY_ASCII
template <> inline char * strstr(char *str1, const char *str2)
{
    return ::strstr(str1, str2);
}
#endif
template <> inline wchar_t * strstr(wchar_t *str1, const wchar_t *str2)
{
    return ::wcsstr(str1, str2);
}

template <typename T>
inline const T * strstr(const T *str1, const T *str2)
{
    return NULL;
}
#ifdef EASL_ONLY_ASCII
template <> inline const char * strstr(const char *str1, const char *str2)
{
    return ::strstr(str1, str2);
}
#endif
template <> inline const wchar_t * strstr(const wchar_t *str1, const wchar_t *str2)
{
    return ::wcsstr(str1, str2);
}


/**
*   \brief                  Retrieves the first occurance in a string of any character contained in another string.
*   \param  str [in]        Null terminated string to search.
*   \param  charSet [in]    The set of characters to look for in \c str.
*   \return                 A pointer in \c str of the first occurance of any character in \c charSet; or NULL if no characters are found.
*/
template <typename T>
inline T * strpbrk(T *str, const T *charSet)
{
    return NULL;
}
#ifdef EASL_ONLY_ASCII
template <> inline char * strpbrk(char *str, const char *charSet)
{
    return ::strpbrk(str, charSet);
}
#endif
template <> inline wchar_t * strpbrk(wchar_t *str, const wchar_t *charSet)
{
    return ::wcspbrk(str, charSet);
}

template <typename T>
inline const T * strpbrk(const T *str, const T *charSet)
{
    return NULL;
}
#ifdef EASL_ONLY_ASCII
template <> inline const char * strpbrk(const char *str, const char *charSet)
{
    return ::strpbrk(str, charSet);
}
#endif
template <> inline const wchar_t * strpbrk(const wchar_t *str, const wchar_t *charSet)
{
    return ::wcspbrk(str, charSet);
}


/**
*   \brief              Converts a string to an integer.
*   \param  str [in]    The string to convert.
*   \return             The integer that was just converted from the string, or 0 if the string can not be converted.
*/
template <typename T>
inline int atoi(const T *str)
{
    return 0;
}
#ifdef EASL_ONLY_ASCII
template <> inline int atoi(const char *str)
{
    return ::atoi(str);
}
#endif
template <> inline int atoi(const wchar_t *str)
{
    return ::_wtoi(str);
}


/**
*   \brief              Converts a string to an unsigned integer.
*   \param  str [in]    The string to convert.
*   \return             The integer that was just converted from the string, or 0 if the string can not be converted.
*/
template <typename T>
inline int atoui(const T *str)
{
    return 0;
}

// We will cheat and use the __int64 versions and cast.
#ifdef EASL_ONLY_ASCII
template <> inline int atoui(const char *str)
{
#if ((COMPILER & COMPILER_VC) && COMPILER >= COMPILER_VC80)
    return static_cast<unsigned int>(::_strtoui64(str, NULL, 10));
#else
	return static_cast<unsigned int>(::_atoull(dest, src, destSize));
#endif
}
#endif
template <> inline int atoui(const wchar_t *str)
{
#if ((COMPILER & COMPILER_VC) && COMPILER >= COMPILER_VC80)
    return static_cast<unsigned int>(::_wcstoui64(str, NULL, 10));
#else
	return static_cast<unsigned int>(::_wtoull(dest, src, destSize));
#endif
}



/**
*   \brief              Converts a string to a 64-bit integer.
*   \param  str [in]    The string to convert.
*   \return             The integer that was just converted from the string, or 0 if the string can not be converted.
*/
template <typename T>
inline __int64 atoi64(const T *str)
{
    return 0;
}
#ifdef EASL_ONLY_ASCII
template <> inline __int64 atoi64(const char *str)
{
    return ::_atoi64(str);
}
#endif
template <> inline __int64 atoi64(const wchar_t *str)
{
    return ::_wtoi64(str);
}

/**
*   \brief              Converts a string to an unsigned 64-bit integer.
*   \param  str [in]    The string to convert.
*   \return             The integer that was just converted from the string, or 0 if the string can not be converted.
*/
template <typename T>
inline __int64 atoui64(const T *str)
{
    return 0;
}
#ifdef EASL_ONLY_ASCII
template <> inline __int64 atoui64(const char *str)
{
#if ((COMPILER & COMPILER_VC) && COMPILER >= COMPILER_VC80)
    return ::_strtoui64(str, NULL, 10);
#else
	return ::_atoull(dest, src, destSize);
#endif
}
#endif
template <> inline __int64 atoui64(const wchar_t *str)
{
#if ((COMPILER & COMPILER_VC) && COMPILER >= COMPILER_VC80)
    return ::_wcstoui64(str, NULL, 10);
#else
	return ::_wtoull(dest, src, destSize);
#endif
}


/**
*   \brief              Converts a string to a float.
*   \param  str [in]    The string to convert.
*   \return             The float that was just converted from the string, or 0.0f if the string can not be converted.
*/
template <typename T>
inline double atof(const T *str)
{
    return 0.0;
}
#ifdef EASL_ONLY_ASCII
inline double atof(const char *str)
{
    return ::atof(str);
}
#endif
inline double atof(const wchar_t *str)
{
    return ::_wtof(str);
}



/**
*   \brief                  Writes formatted data to a string from a pointer to a list of arguments.
*   \param  dest     [out]  Pointer to the buffer that will recieve the formatted string.
*   \param  destSize [in]   The size of the buffer in T's.
*   \param  format   [in]   The string describing the format of the formatted string.
*   \param  args     [in]   The pointer to the arguments to use for the formatted string.
*   \return                 The number of characters that are written to the string; or -1 if an error occured.
*/
template <typename T>
inline int vsprintf(T *dest, size_t destSize, const T *format, va_list args)
{
    // We will convert to a wchar_t * string and call that implementation. This is slow,
    // but it's the simplest way to do it.
    wchar_t *tmp_dest = new wchar_t[easl::strconvertsize<wchar_t>(dest)];
    easl::strconvert(tmp_dest, dest);

    wchar_t *tmp_format = new wchar_t[easl::strconvertsize<wchar_t>(format)];
    easl::strconvert(tmp_format, format);

    int result = easl::vsprintf(tmp_dest, destSize, tmp_format, args);

    // Convert our string back.
    easl::strconvert(dest, tmp_dest);

    delete [] tmp_dest;
    delete [] tmp_format;

    return result;
}
#ifdef EASL_ONLY_ASCII
template <> inline int vsprintf(char *dest, size_t destSize, const char *format, va_list args)
{
#if ((COMPILER & COMPILER_VC) && COMPILER >= COMPILER_VC80)
    return ::vsprintf_s(dest, destSize, format, args);
#else
    return ::vsprintf(dest, format, args);
#endif
}
#endif
template <> inline int vsprintf(wchar_t *dest, size_t destSize, const wchar_t *format, va_list args)
{
#if ((COMPILER & COMPILER_VC) && COMPILER >= COMPILER_VC80)
    return ::vswprintf_s(dest, destSize, format, args);
#else
    return ::vswprintf(dest, format, args);
#endif
}

template <size_t destSize, typename T>
inline int vsprintf(T (&dest)[destSize], const T *format, va_list args)
{
    return easl::vsprintf(dest, destSize, format, args);
}


/**
*   \brief                  Writes formatted data to a string.
*   \param  dest     [out]  Pointer to the buffer that will recieve the formatted string.
*   \param  destSize [in]   The size of the buffer in T's.
*   \param  format   [in]   The string describing the format of the formatted string.
*   \param  args     [in]   The additional arguments to use based on the format string.
*   \return                 The number of characters written to the buffer; or -1 if an error occured.
*/
template <typename T>
inline int sprintf(T *dest, size_t destSize, const T *format, ...)
{
    va_list args;
    va_start(args, format);

    int ret = easl::vsprintf(dest, destSize, format, args);

    va_end(args);

    return ret;
}

template <size_t destSize, typename T>
inline int sprintf(T (&dest)[destSize], const T *format, ...)
{
    va_list args;
    va_start(args, format);

    int ret = easl::vsprintf(dest, destSize, format, args);

    va_end(args);

    return ret;
}



}

#endif // __EASL_STD_FUNCS_H_
