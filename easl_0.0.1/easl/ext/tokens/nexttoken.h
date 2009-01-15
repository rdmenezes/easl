/**
*   \file   nexttoken.h
*   \author Dave Reid
*   \brief  Header file for nexttoken() implementations.
*/
#ifndef __EASL_TOKENS_NEXTTOKEN_H_
#define __EASL_TOKENS_NEXTTOKEN_H_

#include "../../reference_string.h"
#include "_private.h"

namespace easl
{
namespace tokens
{

/**
*   \brief                      Retrieves the next token and moves the pointer to the end of that token.
*   \param  str     [in, out]   The string to retrieve the next token from.
*   \param  token   [out]       The reference string that will recieve the next token.
*   \param  line    [out]       The integer that will recieve the zero based line that the token is on.
*   \param  symbols [in]        A string that represents the symbols that are considered tokens. Each symbol should be seperated by a space.
*   \return                     True if a token is retrieved; false otherwise.
*
*   \remarks
*       When the token is retrieved, the input pointer is moved to the position just the
*       end of the returned token and _not_ to the start of the next token. Therefore, when
*       when the function returns, \c str == \c token.end.
*       \par
*       The line parameter is used to determine the line in the string that the token is
*       found on. This is useful for things like source code parses so that they know the
*       line that the token is on. This can then be used to output useful debug information
*       or whatnot.
*       \par
*       The symbols string specifies all of the symbols that are to be considered tokens.
*       If this parameter is NULL, each individual symbol is considered it's own token. This
*       parameter is NULL by default. If, however, the \c symbol parameter is not NULL,
*       _every_ symbol that should be considered a seperate token must be specified. An
*       exception is made for decimal placed numbers. A number in the form "123.456" will
*       be considered one token. A number in the form "123. 456" will be considered three
*       seperate tokens (123 + '.' + 456) if the period symbol is part of the \c symbols
*       string. If the period is not part of the \c symbols string, the number will be
*       considered to be two seperate tokens (123. + 456). If \c symbols is not NULL, any
*       token that is not part of the \c symbols string will be considered a normal character.
*       Quote symbols should _not_ be part of this list.
*       \par
*       A token is _always_ seperated by non-printable characters, spaces and tabs. If this
*       is insufficient, use a different tokeniser. In addition, these non-printable
*       characters, spaces and tabs will never be part of any tokens except those enclosed
*       by quotes.
*       \par
*       The opening and closing quotes are defined by the \c quotes and parameters. If this
*       parameter is NULL, it is set to "\" '" by default. It is NULL by default. Quotes are
*       used to essentially group a series of tokens so that their formatting is kept exactly
*       correct. Anything inside the quotes is kept exactly the same. These quoted sections
*       are considered a single token. The quotes are included in the token so that applications
*       can determine if a given token is indeed a quote or not. When a quote extends over more
*       than one line, the \c line parameter will recieve the line at which the quote ends.
*       Different types of quotes should be seperated by spaces.
*       \par
*       When the function returns false, the input string is not modified.
*/
template <typename T>
bool nexttoken(T *&str, reference_string<T> &token, size_t &line, const T *symbols = NULL, const T *quotes = NULL)
{
    assert(str != NULL);

    if (symbols == NULL)
    {
        EASL_TOKENS_SET_DEFAULT_SYMBOLS_ENG(symbols);
    }

    if (quotes == NULL)
    {
        EASL_TOKENS_SET_DEFAULT_QUOTES_ENG(quotes);
    }

    // To extract the next token, we need to start looking at each character. We will
    // loop until we reach the end of the string or we break out of the loop.
    T *temp = str;
    uchar32_t ch;
    while ((ch = nextchar(temp)) != NULL)
    {
        
    }


    return false;
}

}
}

#endif // __EASL_TOKENS_NEXTTOKEN_H_
