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
*       This function does not move the pointer forward to where a subsequent character
*       will be written. This must be done manually by looking at the return value. The
*       return value is indicitive of how many T's the pointer must be increment to
*       write a subsequent character.
*/
size_t writechar(char *dest, uchar32_t character)
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
    }

    return char_width;
}

// \copydoc writechar(char *, uchar32_t)
size_t writechar(char16_t *dest, uchar32_t character)
{
    size_t char_width = getcharwidth<char16_t>(character);

    if (dest != NULL)
    {
        if (char_width == 1)
        {
            *dest = static_cast<char16_t>(character);
        }
        else // char_width is equal to 2.
        {
            // We have a surrogate pair.
            character -= UNI_HALF_BASE;

            *dest++ = static_cast<char16_t>((character >> UNI_HALF_SHIFT) + UNI_SUR_HIGH_START);
            *dest++ = static_cast<char16_t>((character & UNI_HALF_MASK) + UNI_SUR_LOW_START);
        }
    }

    return char_width;
}

// \copydoc writechar(char *, uchar32_t)
size_t writechar(char32_t *dest, uchar32_t character)
{
    if (dest != NULL)
    {
        *dest = character;
    }

    return 1;
}

// \copydoc writechar(char *, uchar32_t)
size_t writechar(wchar_t *dest, uchar32_t character)
{
    switch (sizeof(wchar_t))
    {
    case 2: return writechar((char16_t *)dest, character);
    case 4: return writechar((char16_t *)dest, character);
    }

    return writechar((char32_t *)dest, character);
}

}

#endif // __EASL_WRITECHAR_H_
