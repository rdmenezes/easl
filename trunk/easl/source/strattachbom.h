/**
*   \file   strattachbom.h
*   \author Dave Reid
*   \brief  Header file for strattachbom() implementations.
*/
#ifndef __EASL_STRATTACHBOM_H_
#define __EASL_STRATTACHBOM_H_

#include "types.h"
#include "_private.h"

namespace easl
{

/**
*   \brief                  Attaches the BOM to the start of the specified string.
*   \param  str [in, out]   The string to attach the BOM to.
*
*   \remarks
*       There must be enough room in the string buffer to store the BOM. In order
*       to determine how many T's are required to store the BOM, call getcharsize().
*/
template <typename T>
void strattachbom(T *str)
{
    // We need to determine how many T's our BOM is going to take up.
    size_t size = get_char_size<T>((uchar32_t)0xFEFF);

    // Now we can move all of our character down one spot. First we need to get to
    // the end.
    T *temp = str;
    while (*temp != NULL)
    {
        ++temp;
    }
    
    // temp is now positioned at the NULL terminator, so now we need to work our way backwards.
    while (temp != str)
    {
        for (unsigned short i = 0; i < size; ++i)
        {
            *(temp + size) = *temp--;

            if (temp == str)
            {
                *(temp + size) = *temp;
                break;
            }
        }
    }

    // Now we can set the BOM.
    write_char(str, 0xFEFF, size);
}

}

#endif // __EASL_STRATTACHBOM_H_
