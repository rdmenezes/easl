/**
*   \brief  charwidth.h
*   \author Dave Reid
*   \brief  Header file for the charwidth() implementation.
*/
#ifndef __EASL_CHARWIDTH_H_
#define __EASL_CHARWIDTH_H_

#include "_private.h"

namespace easl
{

/**
*   \brief                 Retrieves the number of T's required to represent the specified character.
*   \param  character [in] The character whose width is to be retrieved.
*   \return                The number of T's required to represent the specified character.
*
*   \remarks
*       If the character is invalid, the function will return 0.
*/
template <typename T>
inline size_t charwidth(uchar32_t character)
{
    // The character is not allowed to be equal to a UTF-16 surrogate. If it is,
    // we return 0.
    if (character >= UNI_SUR_HIGH_START && character <= UNI_SUR_LOW_END)
    {
        return 0;
    }

    // The character is not allowed to be larger than the largest legal character.
    if (character >= UNI_MAX_LEGAL_UTF32)
    {
        return 0;
    }

    return 1;
}

template <> inline size_t charwidth<char>(uchar32_t character)
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

    return 0;
}
template <> inline size_t charwidth<char16_t>(uchar32_t character)
{
    if (character <= UNI_MAX_BMP)
    {
        // The character is not allowed to be equal to a UTF-16 surrogate. If it is,
        // we return 0.
        if (character >= UNI_SUR_HIGH_START && character <= UNI_SUR_LOW_END)
        {
            return 0;
        }

        return 1;
    }

    // The character is not allowed to be larger than the largest legal character.
    if (character >= UNI_MAX_LEGAL_UTF32)
    {
        return 0;
    }
    
    return 2;
}
template <> inline size_t charwidth<wchar_t>(uchar32_t character)
{
    switch (sizeof(wchar_t))
    {
    case 2: return charwidth<char16_t>(character);
    case 4: return charwidth<char32_t>(character);
    }

    return charwidth<char>(character);
}

}

#endif // __EASL_CHARWIDTH_H_
