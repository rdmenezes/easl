/**
*   \file   utf_private.h
*   \author Dave Reid
*   \brief  Header file for private UTF encoded string functionality.
*
*   Some of the code here is taken from ftp://www.unicode.org/Public/PROGRAMS/CVTUTF/.
*   The full copyright notice can be found at the bottom of this document.
*
*   The functions in this file should not be used from outside EASL.
*/
#ifndef __EASL_UTFPRIVATE_H_
#define __EASL_UTFPRIVATE_H_

#include "types.h"

namespace easl
{

#define UNI_REPLACEMENT_CHAR    (uchar32_t)0x0000FFFD
#define UNI_MAX_BMP             (uchar32_t)0x0000FFFF
#define UNI_MAX_UTF16           (uchar32_t)0x0010FFFF
#define UNI_MAX_UTF32           (uchar32_t)0x7FFFFFFF
#define UNI_MAX_LEGAL_UTF32     (uchar32_t)0x0010FFFF

#define UNI_SUR_HIGH_START  (uchar32_t)0xD800
#define UNI_SUR_HIGH_END    (uchar32_t)0xDBFF
#define UNI_SUR_LOW_START   (uchar32_t)0xDC00
#define UNI_SUR_LOW_END     (uchar32_t)0xDFFF

static const uchar32_t g_halfShift = 10;
static const uchar32_t g_halfBase = 0x0010000UL;
static const uchar32_t g_halfMask = 0x3FFUL;

/*
 * Index into the table below with the first byte of a UTF-8 sequence to
 * get the number of trailing bytes that are supposed to follow it.
 * Note that *legal* UTF-8 values can't have 4 or 5-bytes. The table is
 * left as-is for anyone who may want to do such conversion, which was
 * allowed in earlier algorithms.
 */
static const char g_trailingBytesForUTF8[256] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5
};

/*
 * Magic values subtracted from a buffer value during UTF8 conversion.
 * This table contains as many values as there might be trailing bytes
 * in a UTF-8 sequence.
 */
static const uchar32_t g_offsetsFromUTF8[6] = {0x00000000UL, 0x00003080UL, 0x000E2080UL, 
		                                       0x03C82080UL, 0xFA082080UL, 0x82082080UL};

/*
 * Once the bits are split out into bytes of UTF-8, this is a mask OR-ed
 * into the first byte, depending on how many bytes follow.  There are
 * as many entries in this table as there are UTF-8 sequence types.
 * (I.e., one byte sequence, two byte... etc.). Remember that sequencs
 * for *legal* UTF-8 will be 4 or fewer bytes total.
 */
static const unsigned char g_firstByteMark[7] = {0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC};

/**
*   \brief              Determines if the next UTF-8 character in the specified string is legal.
*   \param  str [in]    The string whose first character needs to be checked.
*   \param  length [in] The length of the character in the string.
*   \return             True if the character is legal; false otherwise.
*/
inline bool is_legal_utf8(const char *str, unsigned short length)
{
    unsigned char a;
    const char *srcptr = str + length;

    switch (length)
    {
    default:
        {
            return false;
        }

    case 4: 
        {
            a = *--srcptr;
            if (a < 0x80 || a > 0xBF)
            {
                return false;
            }
        }

    case 3: 
        {
            a = *--srcptr;
            if (a < 0x80 || a > 0xBF)
            {
                return false;
            }
        }

    case 2: 
        {
            a = *--srcptr;
            if (a > 0xBF)
            {
                return false;
            }

            // Note that there is no fall-throughs with this switch.
            switch (*str)
            {
            case 0xE0:
                {
                    if (a < 0xA0)
                    {
                        return false;
                    }

                    break;
                }

            case 0xED:
                {
                    if (a < 0x9F)
                    {
                        return false;
                    }

                    break;
                }

            case 0xF0:
                {
                    if (a < 0x90)
                    {
                        return false;
                    }

                    break;
                }

            case 0xF4:
                {
                    if (a < 0x8F)
                    {
                        return false;
                    }

                    break;
                }

            default:
                {
                    if (a < 0x80)
                    {
                        return false;
                    }
                }
            }
        }

    case 1:
        {
            if (*str >= 0x80 && *str < 0xC2)
            {
                return false;
            }
        }
    }

    if (*str > 0xF4)
    {
        return false;
    }

    return true;
}

/**
*   \brief                      Determines the number of T's required to store a given character in the specified encoding.
*   \param  character [in, out] The character to determine the size of.
*   \return                     The number of T's required to store the specified character.
*
*   \remarks
*       If \c character is invalid, the function will set it to the replacement character.
*/
template <typename T>
unsigned short get_char_size(uchar32_t &character)
{
    return 1;
}
template <> unsigned short get_char_size<char>(uchar32_t &character)
{
    if (character < 0x80)
    {
        return 1;
    }
    else if (character < 0x800)
    {
        return 2;
    }
    else if (character < 0x10000)
    {
        return 3;
    }
    else if (character < 0x110000)
    {
        return 4;
    }
    else
    {
        character = UNI_REPLACEMENT_CHAR;
        return 3;
    }
}
template <> unsigned short get_char_size<char16_t>(uchar32_t &character)
{
    if (character <= UNI_MAX_BMP)
    {
        return 1;
    }
    
    return 2;
}
template <> unsigned short get_char_size<wchar_t>(uchar32_t &character)
{
    if (sizeof(wchar_t) == 1)
    {
        return get_char_size<char>(character);
    }
    else if (sizeof(wchar_t) == 2)
    {
        return get_char_size<char16_t>(character);
    }

    return get_char_size<char32_t>(character);
}

/**
*   \brief                  Writes a character to the specified string.
*   \param  str       [in]  The string to write the character to.
*   \param  character [in]  The character to write to the string.
*   \return                 True if the character is written successfully; false otherwise.
*
*   \remarks
*       This function will not modify the input string. This must be done manually.
*/
bool write_char(char *str, uchar32_t character, unsigned short numBytes)
{
    str += numBytes;

    // TODO: We might want to do some sort of overflow check here. The only real
    // way to do this is to have another parameter detailing the size of the
    // destination buffer.

    // Now we need to copy all of our bytes over. We use the fall-through switch
    // like the one in the code by Unicode, Inc.
    switch (numBytes)
    {
    case 4: *--str = static_cast<char>(((character | 0x80) & 0xBF)); character >>= 6;
    case 3: *--str = static_cast<char>(((character | 0x80) & 0xBF)); character >>= 6;
    case 2: *--str = static_cast<char>(((character | 0x80) & 0xBF)); character >>= 6;
    case 1: *--str = static_cast<char>((character | g_firstByteMark[numBytes]));
    }

    return true;
}
bool write_char(char16_t *str, uchar32_t character, unsigned short numBytes)
{
    if (numBytes == 1)
    {
        *str = static_cast<char16_t>(character);
        return true;
    }
    else if (numBytes == 2)
    {
        // We have a surrogate pair.
        character -= g_halfBase;

        *str++ = static_cast<char16_t>((character >> g_halfShift) + UNI_SUR_HIGH_START);
        *str++ = static_cast<char16_t>((character & g_halfMask) + UNI_SUR_LOW_START);

        return true;
    }

    return false;
}
bool write_char(char32_t *str, uchar32_t character, unsigned short numBytes)
{
    *str = character;

    return true;
}

/**
*   \brief                  Validates a UTF-32 character.
*   \param  character [in]  The character to validate.
*   \return                 The validated character.
*
*   \remarks
*       If the input character is already valid, the return value will be the same
*       as the input character. If it is invalid, the replacement character will
*       be returned. If the replacement character can not be used, NULL will be
*       returned.
*/
inline uchar32_t validate_utf32_char(char32_t character)
{
    // We first need to make sure that it is not higher than the 17'th plane.
    if (character <= UNI_MAX_LEGAL_UTF32)
    {
        // We need to make sure that the final character is still valid and not equal
        // to an illegal character code.
        if (character >= UNI_SUR_HIGH_START && character <= UNI_SUR_LOW_END)
        {
            // The character is illegal, so replace it with our replacement character.
            return UNI_REPLACEMENT_CHAR;
        }
        else
        {
            return character;
        }
    }
    else
    {
        // If we've made it here, the final character is larger than the maximum value
        // defined by Unicode (0x10FFFF).
        return UNI_REPLACEMENT_CHAR;
    }

    // Should never make it here. Return 0 just in case something stange happens...
    return 0;
}

}

#endif // __EASL_UTFPRIVATE_H_



/*
 * Copyright 2001-2004 Unicode, Inc.
 * 
 * Disclaimer
 * 
 * This source code is provided as is by Unicode, Inc. No claims are
 * made as to fitness for any particular purpose. No warranties of any
 * kind are expressed or implied. The recipient agrees to determine
 * applicability of information provided. If this file has been
 * purchased on magnetic or optical media from Unicode, Inc., the
 * sole remedy for any claim will be exchange of defective media
 * within 90 days of receipt.
 * 
 * Limitations on Rights to Redistribute This Code
 * 
 * Unicode, Inc. hereby grants the right to freely use the information
 * supplied in this file in the creation of products supporting the
 * Unicode Standard, and to make copies of this file in any form
 * for internal or external distribution as long as this notice
 * remains attached.
 */