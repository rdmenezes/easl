/**
*   \file   strremovebom.h
*   \author Dave Reid
*   \brief  Header file for strremovebom() implementations.
*/
#ifndef __EASL_STRREMOVEBOM_H_
#define __EASL_STRREMOVEBOM_H_

#include "strskipbom.h"

namespace easl
{

/**
*   \brief                  Removes the BOM from the specified string if one is present.
*   \param  str [in, out]   The string to remove the BOM from.
*/
template <typename T>
void strremovebom(T *str)
{
    T *temp = str;
    if (strskipbom(temp) != 0)
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

#endif // __EASL_STRREMOVEBOM_H_
