/**
*   \file   strskipbom.h
*   \author Dave Reid
*   \brief  Header file for strskipbom() implementations.
*/
#ifndef __EASL_STRSKIPBOM_H_
#define __EASL_STRSKIPBOM_H_

#include "strnextchar.h"

namespace easl
{

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

}

#endif // __EASL_STRSKIPBOM_H_
