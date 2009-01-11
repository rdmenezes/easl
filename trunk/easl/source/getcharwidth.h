/**
*   \brief  getcharwidth.h
*   \author Dave Reid
*   \brief  Header file for the getcharwidth() implementation.
*/
#ifndef __EASL_GETCHARWIDTH_H_
#define __EASL_GETCHARWIDTH_H_

namespace easl
{

/**
*   \brief                  Retrieves the number of T's required to represent the specified character.
*   \param  character [in]  The character whose width is to be retrieved.
*   \return                 The number of T's required to represent the specified character.
*
*   \remarks
*       If the character is invalid, the function will return 0.
*/
template <typename T>
size_t getcharwidth(uchar32_t character)
{
    (void)character;    // Warning silencer.

    return 1;
}

template <> size_t getcharwidth<char>(uchar32_t character)
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
template <> size_t getcharwidth<char16_t>(uchar32_t character)
{
    if (character <= UNI_MAX_BMP)
    {
        return 1;
    }
    
    return 2;
}
template <> size_t getcharwidth<wchar_t>(uchar32_t character)
{
    switch (sizeof(wchar_t))
    {
    case 2: return getcharwidth<char16_t>(character);
    case 4: return getcharwidth<char32_t>(character);
    }

    return getcharwidth<char>(character);
}

}

#endif // __EASL_GETCHARWIDTH_H_
