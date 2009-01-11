/**
*   \file   convert.h
*   \author Dave Reid
*   \brief  Header file for converting between different types of strings.
*/
#ifndef __EASL_CONVERT_H_
#define __EASL_CONVERT_H_

#include "types.h"
#include "_private.h"
#include "nextchar.h"

namespace easl
{

/**
*   \brief                  Converts a UTF-16 string to a UTF-8 string.
*   \param  dest   [out]    A pointer to the buffer that will recieve the UTF-8 encoded string.
*   \param  source [in]     The UTF-16 string to convert.
*   \return                 The number of basic character units copied over.
*/
size_t utf16_to_utf8(char *dest, const char16_t *source, size_t destSize)
{
    size_t copied_chars = 0;
    
#ifdef EASL_ONLY_ASCII
    // We need only copy the source to the destination.
    while (*source != NULL)
    {
        if (dest != NULL)
        {
            *dest = static_cast<char>((*source) & 0xFF);
            ++dest;
        }

        ++source;
        ++copied_chars;
    }
#else
    // Now we need to extract each character from the source and add it to our destination buffer.
    uchar32_t ch = 0;
    while ((ch = nextchar(source)) != NULL)
    {
        // Determine the number of bytes that are required to store this character.
        size_t num_bytes = get_char_size<char>(ch);        

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

size_t utf32_to_utf8(char *dest, const char32_t *source, size_t destSize)
{
    size_t copied_chars = 0;
    
#ifdef EASL_ONLY_ASCII
    // We need only copy the source to the destination.
    while (*source != NULL)
    {
        if (dest != NULL)
        {
            *dest = static_cast<char>((*source) & 0xFF);
            ++dest;
        }

        ++source;
        ++copied_chars;
    }
#else
    // Now we need to extract each character from the source and add it to our destination buffer.
    uchar32_t ch = 0;
    while ((ch = nextchar(source)) != NULL)
    {
        // Determine the number of bytes that are required to store this character.
        size_t num_bytes = get_char_size<char>(ch);

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

size_t utf8_to_utf16(char16_t *dest, const char *source, size_t destSize)
{
    size_t copied_chars = 0;
    
#ifdef EASL_ONLY_ASCII
    // We need only copy the source to the destination.
    while (*source != NULL)
    {
        if (dest != NULL)
        {
            *dest = *source;
            ++dest;
        }

        ++source;
        ++copied_chars;
    }
#else
    // Now we need to extract each character from the source and add it to our destination buffer.
    uchar32_t ch = 0;
    while ((ch = nextchar(source)) != NULL)
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
                ch -= UNI_HALF_BASE;

                *dest++ = static_cast<char16_t>((ch >> UNI_HALF_SHIFT) + UNI_SUR_HIGH_START);
                *dest++ = static_cast<char16_t>((ch & UNI_HALF_MASK) + UNI_SUR_LOW_START);
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

size_t utf32_to_utf16(char16_t *dest, const char32_t *source, size_t destSize)
{
    size_t copied_chars = 0;
    
#ifdef EASL_ONLY_ASCII
    // We need only copy the source to the destination.
    while (*source != NULL)
    {
        if (dest != NULL)
        {
            *dest = static_cast<char16_t>(*source);
            ++dest;
        }

        ++source;
        ++copied_chars;
    }
#else
    // Now we need to extract each character from the source and add it to our destination buffer.
    uchar32_t ch = 0;
    while ((ch = nextchar(source)) != NULL)
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
                ch -= UNI_HALF_BASE;

                *dest++ = static_cast<char16_t>((ch >> UNI_HALF_SHIFT) + UNI_SUR_HIGH_START);
                *dest++ = static_cast<char16_t>((ch & UNI_HALF_MASK) + UNI_SUR_LOW_START);
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

size_t utf8_to_utf32(char32_t *dest, const char *source, size_t destSize)
{
    size_t copied_chars = 0;

#ifdef EASL_ONLY_ASCII
    // We need only copy the source to the destination.
    while (*source != NULL)
    {
        if (dest != NULL)
        {
            *dest = *source;
            ++dest;
        }

        ++source;
        ++copied_chars;
    }
#else
    // All we need to do is extract each character from the source string with
    // nextchar(). This will return NULL when it encounters a null terminator.
    uchar32_t ch = 0;
    while ((ch = nextchar(source)) != NULL)
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

size_t utf16_to_utf32(char32_t *dest, const char16_t *source, size_t destSize)
{
    size_t copied_chars = 0;

#ifdef EASL_ONLY_ASCII
    // We need only copy the source to the destination.
    while (*source != NULL)
    {
        if (dest != NULL)
        {
            *dest = *source;
            ++dest;
        }

        ++source;
        ++copied_chars;
    }
#else
    // All we need to do is extract each character from the source string with
    // nextchar(). This will return NULL when it encounters a null terminator.
    uchar32_t ch = 0;
    while ((ch = nextchar(source)) != NULL)
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



size_t convert(char *dest, const char16_t *source, size_t destSize = -1)
{
    return utf16_to_utf8(dest, source, destSize);
}
size_t convert(char *dest, const char32_t *source, size_t destSize = -1)
{
    return utf32_to_utf8(dest, source, destSize);
}

size_t convert(char16_t *dest, const char *source, size_t destSize = -1)
{
    return utf8_to_utf16(dest, source, destSize);
}
size_t convert(char16_t *dest, const char32_t *source, size_t destSize = -1)
{
    return utf32_to_utf16(dest, source, destSize);
}

size_t convert(char32_t *dest, const char *source, size_t destSize = -1)
{
    return utf8_to_utf32(dest, source, destSize);
}
size_t convert(char32_t *dest, const char16_t *source, size_t destSize = -1)
{
    return utf16_to_utf32(dest, source, destSize);
}


template <typename T>
size_t convert(T *dest, const T *source, size_t destSize = -1)
{
    size_t copied_chars = 0;

    // We need only copy the source to the destination.
    while (*source != NULL)
    {
        if (dest != NULL)
        {
            *dest = *source;
            ++dest;
        }

        ++source;
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
size_t convert(wchar_t *dest, const T *source, size_t destSize = -1)
{
    if (sizeof(wchar_t) == 2)
    {
        return convert(reinterpret_cast<char16_t *>(dest), source);
    }
    else if (sizeof(wchar_t) == 4)
    {
        return convert(reinterpret_cast<char32_t *>(dest), source);
    }

    return convert(reinterpret_cast<char *>(dest), source);
}

template <typename T>
size_t convert(T *dest, const wchar_t *source, size_t destSize = -1)
{
    if (sizeof(wchar_t) == 2)
    {
        return convert(dest, reinterpret_cast<const char16_t *>(source));
    }
    else if (sizeof(wchar_t) == 4)
    {
        return convert(dest, reinterpret_cast<const char32_t *>(source));
    }

    return convert(dest, reinterpret_cast<const char *>(source));
}


}

#endif // __EASL_CONVERT_H_
