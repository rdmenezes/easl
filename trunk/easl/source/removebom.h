/**
*   \file   removebom.h
*   \author Dave Reid
*   \brief  Header file for removebom() implementations.
*/
#ifndef __EASL_REMOVEBOM_H_
#define __EASL_REMOVEBOM_H_

#include "skipbom.h"

namespace easl
{

/**
*   \brief                  Removes the BOM from the specified string if one is present.
*   \param  str [in, out]   The string to remove the BOM from.
*/
template <typename T>
void removebom(T *str)
{
    T *temp = str;
    if (skipbom(temp) != 0)
    {
        // If we've made it here, the BOM was present, so we now want to
        // move our string down some spots.
        size_t bom_size = temp - str;

        while (*temp != NULL)
        {
            for (size_t i = 0; i < bom_size; ++i)
            {
                if (*temp == NULL)
                {
                    break;
                }

                *str = *temp;

                ++str;
                ++temp;
            }
        }

        // Null terminate it.
        *str = NULL;
    }
}

}

#endif // __EASL_REMOVEBOM_H_
