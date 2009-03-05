/**
*   \file   extracttokens.h
*   \author Dave Reid
*   \brief  Header file for extracttokens.h implementations.
*/
#ifndef __EASL_TOKENS_EXTRACTTOKENS_H_
#define __EASL_TOKENS_EXTRACTTOKENS_H_

#include <vector>
#include "nexttoken.h"

namespace easl
{
namespace tokens
{

/**
*   \brief                 Extracts a list of tokens from the specified string.
*   \param  str       [in] The string to tokenise.
*   \param  tokens    [in] A reference to the list that will recieve the tokens.
*   \param  lines     [in] A reference to the list that will recieve the lines of each token.
*   \param  options   [in] The various options to use when extracting the tokens.
*   \param  strLength [in] The length in T's of the input string, not including the null terminator.
*
*   \remarks
*       The values in \c tokens and \c lines correspond to each other. So, the line of item 3 in \c tokens
*       will be stored in item 3 in \c lines. The number of items in \c tokens and \c lines will always be
*       equal. The \c lines list is a pointer so that it can be NULL if lines are not needed.
*/
template <typename T>
void extracttokens(T *str, std::vector<reference_string<T> > &tokens, std::vector<size_t> *lines,
                   const TOKEN_OPTIONS<T> *options, size_t strLength = -1)
{
    reference_string<T> cur_token;
    size_t cur_line;
    while (nexttoken(str, cur_token, &cur_line, options, strLength))
    {
        tokens.push_back(cur_token);

        if (lines != NULL)
        {
            lines->push_back(cur_line);
        }
    }
}


template <typename T>
void extracttokens(T *str, std::vector<reference_string<T> > &tokens, std::vector<size_t> *lines, size_t strLength = -1)
{
    extracttokens(str, tokens, lines, (const TOKEN_OPTIONS<T> *)NULL, strLength);
}


}
}

#endif // __EASL_TOKENS_EXTRACTTOKENS_H_
