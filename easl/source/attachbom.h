/**
*   \file   attachbom.h
*   \author Dave Reid
*   \brief  Header file for attachbom() implementations.
*/
#ifndef __EASL_ATTACHBOM_H_
#define __EASL_ATTACHBOM_H_

#include "types.h"
#include "_private.h"

namespace easl
{

/**
*   \brief                      Attaches the BOM to the start of the specified string.
*   \param  str       [in, out] The string to attach the BOM to.
*   \param  strSize   [in]      The size of the buffer pointed to by \c str.
*   \param  strLength [in]      The length in T's of the existing string pointed to by \c str, not including the null terminator.
*
*   \remarks
*       There must be enough room in the string buffer to store the BOM. In order
*       to determine how many T's are required to store the BOM, call charwidth().
*/
template <typename T>
void attachbom(T *str, size_t strSize = -1, size_t strLength = -1)
{
    // We need to determine how many T's our BOM is going to take up.
    size_t size = charwidth<T>((uchar32_t)0xFEFF);

    // Now we can move all of our character down one spot. First we need to get to
    // the end.
    T *temp = str;
    while (strLength > 0 && *temp != NULL)
    {
        --strLength;
        ++temp;
    }
    
    // temp is now positioned at the null terminator, so now we need to work our way backwards.
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
    writechar(str, 0xFEFF);
}

}

#endif // __EASL_ATTACHBOM_H_
