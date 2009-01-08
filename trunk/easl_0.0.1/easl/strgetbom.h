/**
*   \file   strgetbom.h
*   \author Dave Reid
*   \brief  Header file for strgetbom() implementations.
*/
#ifndef __EASL_STRGETBOM_H_
#define __EASL_STRGETBOM_H_

#include "strnextchar.h"

namespace easl
{

/**
*   \brief              Retrieves the BOM from a given string.
*   \param  str [in]    The string to retrieve the BOM from.
*   \return             0xFEFF if the BOM if found, 0 otherwise.
*
*   \remarks
*       This function will not modify the input string in any way. To retrieve the
*       BOM and move passed it, use strskipbom() instead.
*/
template <typename T>
uchar32_t strgetbom(const T *str)
{
    const T *temp;
    if (easl::strnextchar(temp) == 0xFEFF)
    {
        return 0xFEFF;
    }

    return 0;
}

template <typename T>
uchar32_t strgetbom(T *str)
{
    return strgetbom((const T *)str);
}


}

#endif // __EASL_STRGETBOM_H_
