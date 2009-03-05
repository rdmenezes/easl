/**
*   \file   removequotes.h
*   \author Dave Reid
*   \brief  Header file for removequotes() implementations.
*/
#ifndef __EASL_MISC_REMOVEQUOTES_H_
#define __EASL_MISC_REMOVEQUOTES_H_

#include "../../findfirstof.h"
#include "../../_private.h"

namespace easl
{
namespace misc
{

/**
*   \brief                      Removes quotation marks from the string.
*   \param  str       [in, out] The string whose quotation marks should be removed.
*   \param  quotes    [in]      The string containing the different quote characters to consider quotes.
*   \param  escape    [in]      The escape character.
*   \param  strLength [in]      The length in T's of the input string, not including the null terminator.
*   \param
*
*   \remarks
*       If the first character of the specified string must be a character inside the 'quotes'
*       string. If it isn't, the function returns immediately.
*       \par
*       The quotes that removed must mach. For example, if the 'quotes' parameter is equal to "\"'"
*       and the first character is a "'", then the function will remove the next "'" that is not
*       prefixed with the escape character. By default, the escape character is a backslash.
*       \par
*       This function is useful for things like string parsers where values inside quotation marks
*       will be used.
*       \par
*       The \c quotes string must be null terminated.
*/
template <typename T>
void removequotes(T *str, const T *quotes, uchar32_t escape = '\\', size_t strLength = -1)
{
    assert(str != NULL);
    assert(quotes != NULL);
    assert(strLength > 0);

    T *temp = str;

    // Check the first character. If must be contained in quotes string.
    uchar32_t quote_ch = nextchar(temp);
    if (findfirstof(quotes, quote_ch) != NULL)
    {
        // Grab the width of our quote character.
        size_t quote_width = charwidth<T>(quote_ch);

        // We need to ensure that we've adjusted the length of the string after retrieving
        // the quote character.
        strLength -= quote_width;

        // Now move the string down to remove the quote.
        _movestrdown(temp, quote_width, strLength);

        // The last character we've retrieved. We use this to determine if the character
        // has been escaped.
        uchar32_t last_ch = '\0';

        // Now we need to loop through each character and watch for a matching quote character.
        uchar32_t ch;
        while (strLength > 0 && (ch = nextchar(temp)) != '\0')
        {
            if (ch == quote_ch && last_ch != escape)
            {
                // We've found our character. We now need move the string after the quote
                // down a spot.
                _movestrdown(temp, quote_width, strLength);
            }

            last_ch = ch;
            strLength -= charwidth<T>(ch);
            str = temp;
        }
    }
}

}
}

#endif // __EASL_MISC_REMOVEQUOTES_H_
