/**
*   \brief  writechar.h
*   \author Dave Reid
*   \brief  Header file for the writechar() implementation.
*/
#ifndef __EASL_WRITECHAR_H_
#define __EASL_WRITECHAR_H_

#include "_private.h"
#include "getcharwidth.h"

namespace easl
{

/**
*   \brief                  Writes a character to the specified string
*   \param  dest      [in]  The string that the character should be written to.
*   \param  character [in]  The character that should be written.
*   \return                 The number of T's that were just written to the string.
*
*   \remarks
*       This function will move the input pointer to the position just past the character
*       that was just inserted. The input pointer can be moved back to its original
*       position by looking at the return value. The return value is indicitive of how
*       many character units to move the pointer backwards to get it back to its original
*       position. A "character unit" is a char for a char* string, a char16_t for a char16_t*
*       string, etc.
*/
size_t writechar(char *&dest, uchar32_t character)
{
    // Grab the width of the character.
    size_t char_width = getcharwidth<char>(character);
    if (char_width == 0)
    {
        char_width = 3;
        character = UNI_REPLACEMENT_CHAR;
    }

    if (dest != NULL)
    {
        dest += char_width;

        // TODO: We might want to do some sort of overflow check here. The only real
        // way to do this is to have another parameter detailing the size of the
        // destination buffer.

        // Now we need to copy all of our bytes over. We use the fall-through switch
        // like the one in the code by Unicode, Inc.
        switch (char_width)
        {
        case 4: *--dest = static_cast<char>(((character | 0x80) & 0xBF)); character >>= 6;
        case 3: *--dest = static_cast<char>(((character | 0x80) & 0xBF)); character >>= 6;
        case 2: *--dest = static_cast<char>(((character | 0x80) & 0xBF)); character >>= 6;
        case 1: *--dest = static_cast<char>((character | g_firstByteMark[char_width]));
        }

        dest += char_width;
    }

    return char_width;
}

// \copydoc writechar(char *, uchar32_t)
size_t writechar(char16_t *&dest, uchar32_t character)
{
    // Our character width will always be equal to at least 1.
    size_t char_width = 1;

    // If the character is lower or equal to our maximum BMP we can store the character
    // as a single char16_t.
    if (character <= UNI_MAX_BMP)
    {
        // The character is not allowed to equal a UTF-16 surrogate. If it does, we will
        // replace the character.
        if (dest != NULL)
        {
            if (character >= UNI_SUR_HIGH_START && character <= UNI_SUR_LOW_END)
            {
                *dest++ = (char16_t)UNI_REPLACEMENT_CHAR;
            }
            else
            {
                *dest++ = static_cast<char16_t>(character);
            }
        }
    }
    else if (character > UNI_MAX_UTF16)
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
            character -= UNI_HALF_BASE;

            *dest++ = static_cast<char16_t>((character >> UNI_HALF_SHIFT) + UNI_SUR_HIGH_START);
            *dest++ = static_cast<char16_t>((character & UNI_HALF_MASK) + UNI_SUR_LOW_START);
        }

        // We need to increment our counter by one more.
        ++char_width;
    }

    return char_width;
}

// \copydoc writechar(char *, uchar32_t)
size_t writechar(char32_t *&dest, uchar32_t character)
{
    if (dest != NULL)
    {
        *dest++ = validate_utf32_char(character);
    }

    return 1;
}

// \copydoc writechar(char *, uchar32_t)
size_t writechar(wchar_t *&dest, uchar32_t character)
{
    switch (sizeof(wchar_t))
    {
    case 2: return writechar((char16_t *&)dest, character);
    case 4: return writechar((char16_t *&)dest, character);
    }

    return writechar((char32_t *&)dest, character);
}


template <typename T>
size_t writechar(T *dest, uchar32_t character)
{
    return writechar((T *&)dest, character);
}


}

#endif // __EASL_WRITECHAR_H_
