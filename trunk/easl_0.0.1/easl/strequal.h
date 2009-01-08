/**
*   \file   strequal.h
*   \author Dave Reid
*   \brief  Header file for strequal() implementations.
*/
#ifndef __EASL_STREQUAL_H_
#define __EASL_STREQUAL_H_

#include "types.h"
#include "strnextchar.h"

namespace easl
{

/**
*   \brief                      Determines if two strings are equal.
*   \param  str1          [in]  The first string to compare.
*   \param  str2          [in]  The second string to compare.
*   \param  caseSensitive [in]  Specifies whether or not the comparison is case sensitive.
*   \return                     True if the two strings are equal; false otherwise.
*
*   \remarks
*       Both strings must be NULL terminated. By default, the comparison is case sensitive.
*/
template <typename T, typename U>
bool strequal(const T *str1, const U *str2, bool caseSensitive = true)
{
    if (str1 == NULL || str2 == NULL)
    {
        return false;
    }

    // We'll need to grab the next character from each string and make it
    // lower case and then compare.
    uchar32_t ch1 = strnextchar(str1);
    uchar32_t ch2 = strnextchar(str2);

    if (caseSensitive)
    {
        while (ch1 != NULL && ch2 != NULL)
        {
            if (ch1 != ch2)
            {
                return false;
            }

            ch1 = strnextchar(str1);
            ch2 = strnextchar(str2);
        }
    }
    else
    {
        while (ch1 != NULL && ch2 != NULL)
        {
            if (tolower(ch1) != tolower(ch2))
		    {
			    return false;
		    }

            ch1 = strnextchar(str1);
            ch2 = strnextchar(str2);
        }
    }

    // If both strings aren't at their null terminators, they must be different.
    if (ch1 != 0 || ch2 != 0)
    {
	    return false;
    }

	return true;
}

}

#endif // __EASL_STREQUAL_H_
