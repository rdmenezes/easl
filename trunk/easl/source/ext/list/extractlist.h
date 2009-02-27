/**
*   \file   extractlist.h
*   \author Dave Reid
*   \brief  Header file for extractlist() implementations.
*/
#ifndef __EASL_LIST_EXTRACTLIST_H_
#define __EASL_LIST_EXTRACTLIST_H_

#include <vector>
#include "../tokens.h"

namespace easl
{
namespace list
{

template <typename T>
struct LIST_OPTIONS
{
    /**
    *   \brief  Constructor.
    */
    LIST_OPTIONS() : quotes(NULL), seperator(','), escapeCharacter(NULL)
    {
    }

    /**
    *   \brief The list of symbols that should be used for quotes.
    *
    *   Quotes are used in a string to "group" a section of tokens and keep their formatting.
    *   They're mostly used for things like strings in program code. Quotes are enclosed in
    *   a special symbol. These symbols are defined in this variable. \c quotes should be a
    *   null terminated. Each type of quote designator should be a single character. The
    *   \c quotes string's can not contain letters or numbers. Results are undefined in this
    *   case.
    */
    T *quotes;

    /**
    *   \brief  The character that seperates each item in the list.
    */
    uchar32_t seperator;

    /**
    *   \brief The escape character.
    *
    *   The escape character is used to allow a quote character or string to be contained within
    *   a quote itself. The escape character is placed just before the quote string. The escape
    *   character will be included in the returned token. With this tokeniser, the escape character
    *   is only required for the appropriate quote type in the given situation. When the escape
    *   character is found outside a quote, it is treated like a normal symbol.
    */
    uchar32_t escapeCharacter;
};


/**
*   \brief                  Extracts a list of strings based on a string representing a list.
*   \param  str       [in]  The string to listify.
*   \param  list      [out] A reference to the vector that will recieve the strings.
*   \param  strLength [in]  The length in T's of the specified string.
*
*   \remarks
*       An example of \c str is <tt>"{this, is, a, list}"</tt>. Upon calling this function,
*		a list of strings will be created that contains each of the items in the list represented
*		in \c str. The resulting list will contain the following items:
*			- \c "this"
*			- \c "is"
*			- \c "a"
*			- \c "list"
*		\par
*		The list must be enclosed in brackets with each item seperated by commas. The
*		brackets can be of any type, but the entry and closing brackets must logically
*		match up. The following pairs of brackets are allowed:
*			- \c ()
*			- \c {}
*			- \c []
*			- \c <>
*		\par
*		Items in the list with spacing between words must be enclosed in quotation marks.
*		If this is not the case, the item will lose it's spacing.
*/
template <typename T>
void extractlist(T *str, std::vector<reference_string<T> > &list, const LIST_OPTIONS<T> *options, size_t strLength = -1)
{
    // TODO: This whole thing is errorneous. The tokeniser method is completely wrong.

    // We need to first extract a list of tokens.
    std::vector<reference_string<T> > tokens;

    if (options != NULL)
    {
        easl::tokens::TOKEN_OPTIONS<T> token_options;
        token_options.quotes = options->quotes;
        token_options.escapeCharacter = options->escapeCharacter;

        easl::tokens::extracttokens(str, tokens, NULL, &token_options, strLength);
    }
    else
    {
        easl::tokens::extracttokens(str, tokens, NULL, strLength);
    }

    // The first and last tokens must be matching and valid brackets.
    if (equal(tokens.front(), "{"))
    {
        if (!equal(tokens.back(), "}"))
        {
            return;
        }
    }
    else if (equal(tokens.front(), "("))
    {
        if (!equal(tokens.back(), ")"))
        {
            return;
        }
    }
    else if (equal(tokens.front(), "["))
    {
        if (!equal(tokens.back(), "]"))
        {
            return;
        }
    }
    else if (equal(tokens.front(), "<"))
    {
        if (!equal(tokens.back(), ">"))
        {
            return;
        }
    }
    else
    {
        return;
    }

    // Look at each token.
    for (size_t i = 1; i < tokens.size() - 1; ++i)
    {
        if (!equal(tokens[i], ","))
        {
            list.push_back(tokens[i]);
        }
    }
}


template <typename T>
void extractlist(T *str, std::vector<reference_string<T> > &list, size_t strLength = -1)
{
    extractlist(str, list, (const LIST_OPTIONS<T> *)NULL, strLength);
}


}
}

#endif // __EASL_LIST_EXTRACTLIST_H_
