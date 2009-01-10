/**
*   \file   strnextchar.h
*   \author Dave Reid
*   \brief  Header file for the nextchar() family of functions.
*/
#ifndef __EASL_NEXTCHAR_H_
#define __EASL_NEXTCHAR_H_

#include "types.h"
#include "_private.h"

namespace easl
{

/**
*   \brief                  Retrieves the next character in a UTF-8 string.
*   \param  str [in, out]   The string whose next character should be retrieved.
*   \return                 The character that was being pointed to by \c str.
*
*   \remarks
*       If this function returns NULL, the null terminator was retrieved. When this occurs, the string
*       is _not_ moved forward to the next character (there is no characters after the null terminator).
*/
uchar32_t strnextchar_utf8(const char *&str)
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

uchar32_t strnextchar_utf16(const char16_t *&str)
{
#ifdef EASL_ONLY_ASCII
    char32_t ch = *str;

    if (ch != NULL)
    {
        ++str;
    }

    return ch;
#else
    const char16_t *source = str;

    // The character that we'll eventually be returning.
    uchar32_t ch = (uchar32_t)(uchar16_t)*source++;

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
                uchar32_t ch2 = (uchar32_t)(uchar16_t)*source;

                if (ch2 >= UNI_SUR_LOW_START && ch2 <= UNI_SUR_LOW_END)
                {
                    ch = ((ch - UNI_SUR_HIGH_START) << UNI_HALF_SHIFT) + (ch2 - UNI_SUR_LOW_START) + UNI_HALF_BASE;
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
#endif
}

uchar32_t strnextchar_utf32(const char32_t *&str)
{
    char32_t ch = *str;

    if (ch != NULL)
    {
        ++str;
    }

    return ch;
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
    return strnextchar_utf8(str);
}
inline uchar32_t strnextchar(const char16_t *&str)
{
    return strnextchar_utf16(str);
}
inline uchar32_t strnextchar(const char32_t *&str)
{
    return strnextchar_utf32(str);
}
inline uchar32_t strnextchar(const wchar_t *&str)
{
    if (sizeof(wchar_t) == 2)
    {
        return strnextchar_utf16((const char16_t *&)str);
    }
    else if (sizeof(wchar_t) == 4)
    {
        return strnextchar_utf32((const char32_t *&)str);
    }

    return strnextchar_utf8((const char *&)str);
}

template <typename T>
inline uchar32_t strnextchar(T *&str)
{
    return strnextchar((const T *&)str);
}

}

#endif // __EASL_NEXTCHAR_H_
