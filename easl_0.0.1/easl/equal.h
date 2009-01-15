/**
*   \file   equal.h
*   \author Dave Reid
*   \brief  Header file for equal() implementations.
*/
#ifndef __EASL_EQUAL_H_
#define __EASL_EQUAL_H_

#include "types.h"
#include "nextchar.h"

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
bool equal(const T *str1, const U *str2, bool caseSensitive = true)
{
    if (str1 == NULL || str2 == NULL)
    {
        return false;
    }

    // We need to grab the first characters first.
    uchar32_t ch1 = nextchar(str1);
    uchar32_t ch2 = nextchar(str2);

    if (caseSensitive)
    {
        while (ch1 != NULL && ch2 != NULL)
        {
            if (ch1 != ch2)
            {
                return false;
            }

            ch1 = nextchar(str1);
            ch2 = nextchar(str2);
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

            ch1 = nextchar(str1);
            ch2 = nextchar(str2);
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

#endif // __EASL_EQUAL_H_
