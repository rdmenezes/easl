/**
*   \file   ext_funcs.h
*   \author Dave Reid
*   \brief  Header file for extended string functions not found in the C standard library.
*/
#ifndef __EASL_EXTFUNCS_H_
#define __EASL_EXTFUNCS_H_

#include <locale.h>
#include "types.h"
#include "setup.h"
#include "utf_private.h"
#include "std_funcs.h"

namespace easl
{

/**
*   \brief                      Determines if two strings are equal.
*   \param  str1          [in]  The first string to compare.
*   \param  str2          [in]  The second string to compare.
*   \param  caseSensitive [in]  Specifies whether or not the comparison is case sensitive.
*   \return                     True if the two strings are equal; false otherwise.
*
*   \remarks
*       Both strings must be NULL terminated. By default, the comparison is case sensitive.
*/
template <typename T, typename U>
bool strequal(const T *str1, const U *str2, bool caseSensitive = true)
{
    if (str1 == NULL || str2 == NULL)
    {
        return false;
    }

    // We'll need to grab the next character from each string and make it
    // lower case and then compare.
    uchar32_t ch1 = strnextchar(str1);
    uchar32_t ch2 = strnextchar(str2);

    if (caseSensitive)
    {
        while (ch1 != NULL && ch2 != NULL)
        {
            if (ch1 != ch2)
            {
                return false;
            }

            ch1 = strnextchar(str1);
            ch2 = strnextchar(str2);
        }
    }
    else
    {
        while (ch1 != NULL && ch2 != NULL)
        {
            if (tolower(ch1) != tolower(ch2))
		    {
			    return false;
		    }

            ch1 = strnextchar(str1);
            ch2 = strnextchar(str2);
        }
    }

    // If both strings aren't at their null terminators, they must be different.
    if (ch1 != 0 || ch2 != 0)
    {
	    return false;
    }

	return true;
}

/**
*   \brief                  Retrieves the next character in a string and moves the pointer to the start of the next character.
*   \param  str [in, out]   Pointer to the string to retrieve the character from.
*   \return                 The character that was being pointed to by \c str.
*
*   \remarks
*       If this function returns NULL, the null terminator was retrieved. When this occurs, the string
*       is _not_ moved forward to the next character (there is no characters after the null terminator).
*/
inline uchar32_t strnextchar(const char *&str)
{
#ifdef EASL_ONLY_ASCII
    char32_t ch = *str;

    if (ch != NULL)
    {
        ++str;
    }

    return ch;
#else
    const char *source = str;

    // The character we will eventually be returning.
    uchar32_t ch = 0;

    // We need to determine the additional bytes that we need to read in order
    // to reconstruct the final character. We can calculate the total number of
    // bytes used for the character by adding 1 to the returned value.
    unsigned short extra_bytes = g_trailingBytesForUTF8[(unsigned char)*source];

    // We need to make sure that the null terminator is not encounted one once
    // of these bytes. If it is, we need to return 0.
    for (unsigned short i = 1; i <= extra_bytes; ++i)
    {
        if (*(source + i) == NULL)
        {
            return 0;
        }
    }

    // We need to make sure we have a valid UTF-8 character. If it isn't, we will
    // return NULL.
    if (!is_legal_utf8(source, extra_bytes + 1))
    {
        return 0;
    }

    // It is possible that the additional bytes are values of 5 and 4. This used
    // to be valid, but is not anymore. We will use the swith fall-through method
    // used in the sample code by Unicode, Inc.
    switch (extra_bytes)
    {
    case 5: ch += (unsigned char)*source++; ch <<= 6;
    case 4: ch += (unsigned char)*source++; ch <<= 6;
    case 3: ch += (unsigned char)*source++; ch <<= 6;
    case 2: ch += (unsigned char)*source++; ch <<= 6;
    case 1: ch += (unsigned char)*source++; ch <<= 6;
    case 0: ch += (unsigned char)*source++;
    }

    // Now we need to subtract a value from the character depending on the number
    // of extra bytes.
    ch -= g_offsetsFromUTF8[extra_bytes];

    // If the character is a null terminator, we don't want to modify the string.
    if (ch != NULL)
    {
        str = source;
    }

    return ch;
#endif
}
inline uchar32_t strnextchar(const char16_t *&str)
{
    const char16_t *source = str;

    // The character that we'll eventually be returning.
    uchar32_t ch = *source++;

    // We need to check if we have a surrogate pair. If we do, we need to do a conversion.
    // We only want to do this if the character we just retrieved wasn't a null terminator,
    // otherwise 'source' is already pointing to an invalid memory address.
    if (ch != NULL)
    {
        if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_START)
        {
            // Make sure the source isn't at it's null terminator. If it is, we have an
            // error, so we will return 0.
            if (*source != NULL)
            {
                // Grab our next 16-bits. This must be the low surrogate. If it isn't, we
                // have an error and need to return 0.
                char32_t ch2 = *source;

                if (ch2 >= UNI_SUR_LOW_START && ch2 <= UNI_SUR_LOW_END)
                {
                    ch = ((ch - UNI_SUR_HIGH_START) << g_halfShift) + (ch2 - UNI_SUR_LOW_START) + g_halfBase;
                    ++source;
                }
                else
                {
                    // The next 16-bits isn't a low surrogate, so we have an error.
                    return 0;
                }
            }
            else
            {
                // We've prematurely reached the null terminator, so return 0.
                return 0;
            }
        }


        // Set our string.
        str = source;
    }

    return ch;
}
inline uchar32_t strnextchar(const char32_t *&str)
{
    char32_t ch = *str;

    if (ch != NULL)
    {
        ++str;
    }

    return ch;
}
inline uchar32_t strnextchar(const wchar_t *&str)
{
    if (sizeof(wchar_t) == 1)
    {
        return easl::strnextchar((const char *&)str);
    }
    else if (sizeof(wchar_t) == 2)
    {
        return easl::strnextchar((const char16_t *&)str);
    }
    else
    {
        return easl::strnextchar((const char32_t *&)str);
    }
}

template <typename T>
inline uchar32_t strnextchar(T *&str)
{
    return easl::strnextchar((const T *&)str);
}


/**
*   \brief              Converts a string in a particular format to another format.
*   \param  dest  [in]  The string that will recieve the converted string.
*   \param  src   [in]  The null terminated string to convert.
*   \return             The number of basic character units copied over.
*
*   \remarks
*       It is assumed that there is enough room in the destination buffer to store
*       the converted string.
*       \par
*       Custom string types can be converted between each other by simply overloading
*       strconvert(). When doing this, an implementation should also be created for
*       converting between the same types. This keeps the API as generic as possible.
*       \par
*       The return value is the number of basic character units that were copied into
*       the destination buffer. A "basic character unit" is a 'char' for 'char *' strings,
*       a 'wchar_t' for 'wchar_t *' strings, etc. For UTF-8 strings, it will return the
*       number of bytes copied over. For UTF-16 strings, it will return the number of
*       16-bit integers that were copied over. For UTF-32 strings, it will return the
*       number of 32-bit integers.
*       \par
*       If \c dest is set to NULL, the method will return the number of bytes that will
*       be needed to store the converted string, including the null terminator.
*       \par
*       The returned string is always null terminated.
*       \par
*       This function uses the current locale to determine how the strings are converted.
*/
inline size_t strconvert(char *dest, const char *src)
{
    size_t copied_chars = 0;

    // We need only copy the source to the destination.
    while (*src != NULL)
    {
        if (dest != NULL)
        {
            *dest = *src;
            ++dest;
        }

        ++src;
        ++copied_chars;
    }

    // Null terminate.
    if (dest != NULL)
    {
        *dest = NULL;
    }

    return copied_chars + 1;
}
inline size_t strconvert(char *dest, const char16_t *src)
{
    size_t copied_chars = 0;
    
#ifdef EASL_ONLY_ASCII
    // We need only copy the source to the destination.
    while (*src != NULL)
    {
        if (dest != NULL)
        {
            *dest = static_cast<char>((*src) & 0xFF);
            ++dest;
        }

        ++src;
        ++copied_chars;
    }
#else
    // Now we need to extract each character from the source and add it to our destination buffer.
    uchar32_t ch = 0;
    while ((ch = strnextchar(src)) != NULL)
    {
        // Determine the number of bytes that are required to store this character.
        unsigned short num_bytes = get_char_size<char>(ch);        

        // If our destination is NULL, we don't want to set any characters, but we
        // do want to get the number of char32_t's that we'll need to use for the
        // destination buffer.
        if (dest != NULL)
        {
            // Now write our character to our string.
            write_char(dest, ch, num_bytes);

            // The position of the pointer is not modified, so we need to move in front of
            // the character that we just wrote.
            dest += num_bytes;
        }

        // Increment the number of characters we've copied.
        copied_chars += num_bytes;
    }
#endif
    
    // Null terminate.
    if (dest != NULL)
    {
        *dest = NULL;
    }

    return copied_chars + 1;

}
inline size_t strconvert(char *dest, const char32_t *src)
{
    size_t copied_chars = 0;
    
#ifdef EASL_ONLY_ASCII
    // We need only copy the source to the destination.
    while (*src != NULL)
    {
        if (dest != NULL)
        {
            *dest = static_cast<char>((*src) & 0xFF);
            ++dest;
        }

        ++src;
        ++copied_chars;
    }
#else
    // Now we need to extract each character from the source and add it to our destination buffer.
    uchar32_t ch = 0;
    while ((ch = strnextchar(src)) != NULL)
    {
        // Determine the number of bytes that are required to store this character.
        unsigned short num_bytes = get_char_size<char>(ch);

        // If our destination is NULL, we don't want to set any characters, but we
        // do want to get the number of char32_t's that we'll need to use for the
        // destination buffer.
        if (dest != NULL)
        {
            // Now write our character to our string.
            write_char(dest, ch, num_bytes);

            // The position of the pointer is not modified, so we need to move in front of
            // the character that we just wrote.
            dest += num_bytes;
        }

        // Increment the number of characters we've copied.
        copied_chars += num_bytes;
    }
#endif
    
    // Null terminate.
    if (dest != NULL)
    {
        *dest = NULL;
    }

    return copied_chars + 1;
}

inline size_t strconvert(char16_t *dest, const char *src)
{
    size_t copied_chars = 0;
    
#ifdef EASL_ONLY_ASCII
    // We need only copy the source to the destination.
    while (*src != NULL)
    {
        if (dest != NULL)
        {
            *dest = *src;
            ++dest;
        }

        ++src;
        ++copied_chars;
    }
#else
    // Now we need to extract each character from the source and add it to our destination buffer.
    uchar32_t ch = 0;
    while ((ch = strnextchar(src)) != NULL)
    {
        // TODO: We might want to do some sort of overflow check here. The only real
        // way to do this is to have another parameter detailing the size of the
        // destination buffer.

        // We will always be adding at least one character to the string if we've made it here.
        ++copied_chars;

        // If the character is lower or equal to our maximum BMP we can store the character
        // as a single char16_t.
        if (ch <= UNI_MAX_BMP)
        {
            // The character is not allowed to equal a UTF-16 surrogate. If it does, we will
            // replace the character.
            if (dest != NULL)
            {
                if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_END)
                {
                    *dest++ = (char16_t)UNI_REPLACEMENT_CHAR;
                }
                else
                {
                    *dest++ = static_cast<char16_t>(ch);
                }
            }
        }
        else if (ch >= UNI_MAX_UTF16)
        {
            // If we've made it here the character is illegal, so we will replace it.
            if (dest != NULL)
            {
                *dest++ = (char16_t)UNI_REPLACEMENT_CHAR;
            }
        }
        else
        {
            // If we've made it here, the character needs to be divided into two parts.

            // TODO: We should probably do some overflow check here. Again, we will probably
            // need a third parameter detailing the size of the destination buffer.

            // We now need split the character into two char16_t's.
            if (dest != NULL)
            {
                ch -= g_halfBase;

                *dest++ = static_cast<char16_t>((ch >> g_halfShift) + UNI_SUR_HIGH_START);
                *dest++ = static_cast<char16_t>((ch & g_halfMask) + UNI_SUR_LOW_START);
            }

            // We need to increment our counter by one more.
            ++copied_chars;
        }
    }
#endif
    
    // Null terminate.
    if (dest != NULL)
    {
        *dest = NULL;
    }

    return copied_chars + 1;
}
inline size_t strconvert(char16_t *dest, const char16_t *src)
{
    size_t copied_chars = 0;

    // We need only copy the source to the destination.
    while (*src != NULL)
    {
        if (dest != NULL)
        {
            *dest = *src;
            ++dest;
        }

        ++src;
        ++copied_chars;
    }

    // Null terminate.
    if (dest != NULL)
    {
        *dest = NULL;
    }

    return copied_chars + 1;
}
inline size_t strconvert(char16_t *dest, const char32_t *src)
{
    size_t copied_chars = 0;
    
#ifdef EASL_ONLY_ASCII
    // We need only copy the source to the destination.
    while (*src != NULL)
    {
        if (dest != NULL)
        {
            *dest = static_cast<char16_t>(*src);
            ++dest;
        }

        ++src;
        ++copied_chars;
    }
#else
    // Now we need to extract each character from the source and add it to our destination buffer.
    uchar32_t ch = 0;
    while ((ch = strnextchar(src)) != NULL)
    {
        // TODO: We might want to do some sort of overflow check here. The only real
        // way to do this is to have another parameter detailing the size of the
        // destination buffer.

        // We will always be adding at least one character to the string if we've made it here.
        ++copied_chars;

        // If the character is lower or equal to our maximum BMP we can store the character
        // as a single char16_t.
        if (ch <= UNI_MAX_BMP)
        {
            // The character is not allowed to equal a UTF-16 surrogate. If it does, we will
            // replace the character.
            if (dest != NULL)
            {
                if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_END)
                {
                    *dest++ = (char16_t)UNI_REPLACEMENT_CHAR;
                }
                else
                {
                    *dest++ = static_cast<char16_t>(ch);
                }
            }
        }
        else if (ch >= UNI_MAX_UTF16)
        {
            // If we've made it here the character is illegal, so we will replace it.
            if (dest != NULL)
            {
                *dest++ = (char16_t)UNI_REPLACEMENT_CHAR;
            }
        }
        else
        {
            // If we've made it here, the character needs to be divided into two parts.

            // TODO: We should probably do some overflow check here. Again, we will probably
            // need a third parameter detailing the size of the destination buffer.

            // We now need split the character into two char16_t's.
            if (dest != NULL)
            {
                ch -= g_halfBase;

                *dest++ = static_cast<char16_t>((ch >> g_halfShift) + UNI_SUR_HIGH_START);
                *dest++ = static_cast<char16_t>((ch & g_halfMask) + UNI_SUR_LOW_START);
            }

            // We need to increment our counter by one more.
            ++copied_chars;
        }
    }
#endif
    
    // Null terminate.
    if (dest != NULL)
    {
        *dest = NULL;
    }

    return copied_chars + 1;
}

inline size_t strconvert(char32_t *dest, const char *src)
{
    size_t copied_chars = 0;

#ifdef EASL_ONLY_ASCII
    // We need only copy the source to the destination.
    while (*src != NULL)
    {
        if (dest != NULL)
        {
            *dest = *src;
            ++dest;
        }

        ++src;
        ++copied_chars;
    }
#else
    // All we need to do is extract each character from the source string with
    // strnextchar(). This will return NULL when it encounters a null terminator.
    uchar32_t ch = 0;
    while ((ch = strnextchar(src)) != NULL)
    {
        // If our destination is NULL, we don't want to set any characters, but we
        // do want to get the number of char32_t's that we'll need to use for the
        // destination buffer.
        if (dest != NULL)
        {
            // TODO: We might want to do some sort of overflow check here. The only real
            // way to do this is to have another parameter detailing the size of the
            // destination buffer.
            
            // Everything seems alright so far, so we can now set the character and move
            // our destination to the next spot. We want to make sure the character is
            // validated, though.
            *dest++ = validate_utf32_char(ch);
        }

        // Increment the number of characters we've copied.
        ++copied_chars;
    }
#endif

    // Null terminate.
    if (dest != NULL)
    {
        *dest = NULL;
    }

    return copied_chars + 1;
}
inline size_t strconvert(char32_t *dest, const char16_t *src)
{
    size_t copied_chars = 0;

#ifdef EASL_ONLY_ASCII
    // We need only copy the source to the destination.
    while (*src != NULL)
    {
        if (dest != NULL)
        {
            *dest = *src;
            ++dest;
        }

        ++src;
        ++copied_chars;
    }
#else
    // All we need to do is extract each character from the source string with
    // strnextchar(). This will return NULL when it encounters a null terminator.
    uchar32_t ch = 0;
    while ((ch = strnextchar(src)) != NULL)
    {
        // If our destination is NULL, we don't want to set any characters, but we
        // do want to get the number of char32_t's that we'll need to use for the
        // destination buffer.
        if (dest != NULL)
        {
            // TODO: We might want to do some sort of overflow check here. The only real
            // way to do this is to have another parameter detailing the size of the
            // destination buffer.
            
            // Everything seems alright so far, so we can now set the character and move
            // our destination to the next spot. We want to make sure the character is
            // validated, though.
            *dest++ = validate_utf32_char(ch);
        }

        // Increment the number of characters we've copied.
        ++copied_chars;
    }
#endif

    // Null terminate.
    if (dest != NULL)
    {
        *dest = NULL;
    }

    return copied_chars + 1;
}
inline size_t strconvert(char32_t *dest, const char32_t *src)
{
    size_t copied_chars = 0;

    // We need only copy the source to the destination.
    while (*src != NULL)
    {
        if (dest != NULL)
        {
            *dest = *src;
            ++dest;
        }

        ++src;
        ++copied_chars;
    }

    // Null terminate.
    if (dest != NULL)
    {
        *dest = NULL;
    }

    return copied_chars + 1;
}
inline size_t strconvert(wchar_t *dest, const wchar_t *src)
{
    size_t copied_chars = 0;

    // We need only copy the source to the destination.
    while (*src != NULL)
    {
        if (dest != NULL)
        {
            *dest = *src;
            ++dest;
        }

        ++src;
        ++copied_chars;
    }

    // Null terminate.
    if (dest != NULL)
    {
        *dest = NULL;
    }

    return copied_chars + 1;
}


template <typename T>
size_t strconvert(wchar_t *dest, const T *src)
{
    // We now need to determine the encoding format that wchar_t will be
    // using. We will assume that if it is 2 bytes, it is encoded as UTF-16.
    // If it is 4 bytes, we will assume that it is encoded as UTF-32. The
    // compiler should be able to optimize the if statements out.
    if (sizeof(wchar_t) == 1)
    {
        return strconvert(reinterpret_cast<char *>(dest), src);
    }
    else if (sizeof(wchar_t) == 2)
    {
        return strconvert(reinterpret_cast<char16_t *>(dest), src);
    }
    else
    {
        return strconvert(reinterpret_cast<char32_t *>(dest), src);
    }
}

template <typename T>
size_t strconvert(T *dest, const wchar_t *src)
{
    if (sizeof(wchar_t) == 1)
    {
        return strconvert(dest, reinterpret_cast<const char *>(src));
    }
    else if (sizeof(wchar_t) == 2)
    {
        return strconvert(dest, reinterpret_cast<const char16_t *>(src));
    }
    else
    {
        return strconvert(dest, reinterpret_cast<const char32_t *>(src));
    }
}


/**
*   \brief              Determines the size that a destination buffer must be to convert the specified string.
*   \param  str [in]    The string to look at to determine the size of the output buffer.
*   \return             The number of T's that must be allocated in order to store a conversion of the input string including the NULL terminator.
*/
#ifdef EASL_ONLY_ASCII
template <typename T, typename U>
size_t strconvertsize(const U *str)
{
    return easl::strlen(str) + 1;
}
#else
template <typename T, typename U>
size_t strconvertsize(const U *str)
{
    return 0;
}
template <> size_t strconvertsize<char>(const char *str)
{
    return strconvert((char *)NULL, str);
}
template <> size_t strconvertsize<char>(const char16_t *str)
{
    return strconvert((char *)NULL, str);
}
template <> size_t strconvertsize<char>(const char32_t *str)
{
    return strconvert((char *)NULL, str);
}

template <> size_t strconvertsize<char16_t>(const char *str)
{
    return strconvert((char16_t *)NULL, str);
}
template <> size_t strconvertsize<char16_t>(const char16_t *str)
{
    return strconvert((char16_t *)NULL, str);
}
template <> size_t strconvertsize<char16_t>(const char32_t *str)
{
    return strconvert((char16_t *)NULL, str);
}

template <> size_t strconvertsize<char32_t>(const char *str)
{
    return strconvert((char32_t *)NULL, str);
}
template <> size_t strconvertsize<char32_t>(const char16_t *str)
{
    return strconvert((char32_t *)NULL, str);
}
template <> size_t strconvertsize<char32_t>(const char32_t *str)
{
    return strconvert((char32_t *)NULL, str);
}

template <> size_t strconvertsize<wchar_t>(const char *str)
{
    return strconvert((wchar_t *)NULL, str);
}
template <> size_t strconvertsize<wchar_t>(const char16_t *str)
{
    return strconvert((wchar_t *)NULL, str);
}
template <> size_t strconvertsize<wchar_t>(const char32_t *str)
{
    return strconvert((wchar_t *)NULL, str);
}

template <> size_t strconvertsize<wchar_t>(const wchar_t *str)
{
    return strconvert(NULL, str);
}

template <typename T>
size_t strconvertsize(const wchar_t *str)
{
    if (sizeof(wchar_t) == 1)
    {
        return strconvertsize<T>(reinterpret_cast<const char *>(str));
    }
    else if (sizeof(wchar_t) == 2)
    {
        return strconvertsize<T>(reinterpret_cast<const char16_t *>(str));
    }
    else
    {
        return strconvertsize<T>(reinterpret_cast<const char32_t *>(str));
    }
}
#endif


/**
*   \brief              Skips over the BOM of the string if it has one.
*   \param  str [in]    The string whose BOM should be skipped over.
*   \return             The BOM if the string has one, or 0 if it doesn't.
*
*   \remarks
*       This function will move the input pointer to a point just past the BOM if
*       one is present. If there is no BOM, the pointer is not moved.
*/
template <typename T>
uchar32_t strskipbom(const T *&str)
{
    const T *temp = str;
    if (easl::strnextchar(temp) == 0xFEFF)
    {
        str = temp;
        return 0xFEFF;
    }

    return 0;
}
template <typename T>
uchar32_t strskipbom(T *&str)
{
    return strskipbom((const T *&)str);
}

/**
*   \brief              Attaches the BOM to the start of the specified string.
*   \param  str [in]    The string to attach the BOM to.
*/
template <typename T>
void strattachbom(T *str)
{
}


/**
*   \brief              Retrieves the number of characters in the given string.
*   \param  str [in]    The string to check.
*   \return             The number of characters that make up the string; or 0 if an error occurs.
*
*   \remarks
*       The input string must be null terminated. The null terminator should be the same size
*       as that of the strings internal character.
*       \par
*       This function does not return the number of bytes in the string. Rather, it calculates
*       the number of characters in the string, not including the null terminator.
*/
template <typename T>
size_t strchrcount(const T *str)
{
#ifdef EASL_ONLY_ASCII
    return easl::strlen(str);
#else
    size_t count = 0;
    uchar32_t ch;
    while ((ch = easl::strnextchar(str)) != NULL)
    {
        ++count;
    }

    return count;
#endif
}



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
    return easl::atoi64(str);
}
template <> unsigned __int64 parse(const char *str)
{
    return easl::atoui64(str);
}
template <> int parse(const char *str)
{
    return easl::atoi(str);
}
template <> unsigned int parse(const char *str)
{
    return easl::atoui(str);
}
template <> long parse(const char *str)
{
    return static_cast<long>(easl::atoi(str));
}
template <> unsigned long parse(const char *str)
{
    return static_cast<unsigned long>(easl::atoui(str));
}
template <> short parse(const char *str)
{
    return static_cast<short>(easl::atoi(str));
}
template <> unsigned short parse(const char *str)
{
    return static_cast<unsigned short>(easl::atoui(str));
}
template <> char parse(const char *str)
{
    return static_cast<char>(easl::atoi(str));
}
template <> unsigned char parse(const char *str)
{
    return static_cast<unsigned char>(easl::atoui(str));
}
template <> float parse(const char *str)
{
    return static_cast<float>(easl::atof(str));
}
template <> double parse(const char *str)
{
    return easl::atof(str);
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
    return easl::atoi64(str);
}
template <> unsigned __int64 parse(const wchar_t *str)
{
    return easl::atoui64(str);
}

template <> int parse(const wchar_t *str)
{
    return easl::atoi(str);
}
template <> unsigned int parse(const wchar_t *str)
{
    return easl::atoui(str);
}

template <> long parse(const wchar_t *str)
{
    return static_cast<long>(easl::atoi(str));
}
template <> unsigned long parse(const wchar_t *str)
{
    return static_cast<unsigned long>(easl::atoui(str));
}

template <> short parse(const wchar_t *str)
{
    return static_cast<short>(easl::atoi(str));
}
template <> unsigned short parse(const wchar_t *str)
{
    return static_cast<unsigned short>(easl::atoui(str));
}

template <> char parse(const wchar_t *str)
{
    return static_cast<char>(easl::atoi(str));
}
template <> unsigned char parse(const wchar_t *str)
{
    return static_cast<unsigned char>(easl::atoui(str));
}

template <> float parse(const wchar_t *str)
{
    return static_cast<float>(easl::atof(str));
}
template <> double parse(const wchar_t *str)
{
    return easl::atof(str);
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

    easl::strconvert(dest, temp_dest);

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
    // How do we handle different languages here?
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

#endif // __EASL_EXTFUNCS_H_
