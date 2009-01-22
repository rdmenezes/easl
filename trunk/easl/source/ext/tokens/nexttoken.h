/**
*   \file   nexttoken.h
*   \author Dave Reid
*   \brief  Header file for nexttoken() implementations.
*/
#ifndef __EASL_TOKENS_NEXTTOKEN_H_
#define __EASL_TOKENS_NEXTTOKEN_H_

#include "../../reference_string.h"
#include "../../findfirst.h"
#include "_private.h"

namespace easl
{
namespace tokens
{

/**
*   \brief  Structure describing the options for extracting tokens.
*/
template <typename T>
struct NEXT_TOKEN_OPTIONS
{
    /**
    *   \brief The list of symbol groups that should be considered seperate tokens.
    *
    *   This should be a null terminated string. Each group of symbols should be seperated with
    *   a space. Each symbol group should contain more than 1 symbol, and should not contain
    *   letters or numbers. A symbol group should also not contain any of the characters in the
    *   \c quotes string.
    */
    T *symbolGroups;

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
    *   \brief The escape character.
    *
    *   The escape character is used to allow a quote character or string to be contained within
    *   a quote itself. The escape character is placed just before the quote string. The escape
    *   character will be included in the returned token. With this tokeniser, the escape character
    *   is only required for the appropriate quote type in the given situation. When the escape
    *   character is found outside a quote, it is treated like a normal symbol.
    */
    uchar32_t escapeCharacter;


    /**
    *   \brief The list of starting ignore block strings.
    */
    T *ignoreBlockStart;

    /**
    *   \brief The list of ending ignore block strings.
    */
    T *ignoreBlockEnd;
};


/**
*   \brief                    Retrieves the next token and moves the pointer to the end of that token.
*   \param  str     [in, out] The string to retrieve the next token from.
*   \param  token   [out]     The reference string that will recieve the next token.
*   \param  line    [out]     The integer that will recieve the zero based line that the token is on.
*   \param  options [in]      The various options to use when retrieving the next token.
*   \return                   True if a token is retrieved; false otherwise.
*
*   \remarks
*       This function does not behave like strtok().
*       \par
*       The source string must be null terminated. When the null terminator is reached, the
*       function will return false and the pointer will not be modified.
*       \par
*       When the token is retrieved, the input pointer is moved to the position just the
*       end of the returned token and _not_ to the start of the next token. Therefore, when
*       when the function returns, \c str == \c token.end.
*       \par
*       The line parameter is used to determine the line in the string that the token is
*       found on. This is useful for things like source code parses so that they know the
*       line that the token is on. This can then be used to output useful debug information
*       or whatnot.
*       \par
*       A token is _always_ seperated by non-printable characters, spaces and tabs. If this
*       is insufficient, use a different tokeniser. In addition, these non-printable
*       characters, spaces and tabs will never be part of any tokens except those enclosed
*       by quotes.
*       \par
*       Any non-letter and non-number is considered a symbol. Each symbol is it's own token.
*       A group of symbols can be considered to be a single token by setting the \c symbolGroups
*       string in the \c options structure.
*       \par
*       A quote is considered a single token. The quote symbols are included in the token.
*       Quotes are useful for keeping the formatting of a particular part of the string. A quote
*       character itself can be part of a quote, but must be preceeded by the escape character.
*       The escape character is always included in the returned string.
*       \par
*       Sections of the string can be ignored by setting the \c ignoreBlockStart and
*       \c ignoreBlockEnd strings in the \c options structure. If a starting ignore block string
*       is found, but a matching ending ignore block string is not found before the null
*       terminator, the section from the start of the ignore block to the end of the string is
*       ignored.
*       \par
*       When the function returns false, the input string is not modified.
*/
template <typename T>
inline bool nexttoken(T *&str, reference_string<T> &token, size_t &line, const NEXT_TOKEN_OPTIONS<T> &options)
{
    assert(str != NULL);

    // Initialise the line.
    line = 0;

    // The type of the token. The value of this value is as follows:
    //    - 0 means that there is no token yet.
    //    - 1 means the token is a word.
    //    - 2 means the token is a number.
    //    - 3 means the token is a symbol.
    //    - 4 means the token is a quote.
    //    - 5 means the token is currently an ignore block.
    int type = 0;

    // Stores the previous character that was just retrieved.
    uchar32_t prev_ch = '\0';

    // The character that has started a quote. If the token is not in a quote, this will
    // be equal to 0.
    uchar32_t starting_quote_ch = '\0';

    // Determines if we've found a decimal point in a number. This is only used when type == 2.
    bool found_decimal = false;

    // Our string containing the start of our ignore block. This is only applicable when type == 5.
    reference_string<T> ignore_block_start;
    ignore_block_start.start = NULL;
    ignore_block_start.end = NULL;

    // To extract the next token, we need to start looking at each character. We will
    // loop until we reach the end of the string or we break out of the loop.
    T *temp = str;
    uchar32_t ch;
    while ((ch = nextchar(temp)) != NULL)
    {
        // We need to check which character we've got. If it's a non-printable character,
        // we want to ignore it and continue to the next character.
        if (ch > 0 && ch <= ' ')
        {
            // We have a non-printable character. If we've started the token, we want
            // to end it.
            if (type != 0 && type != 4 && type != 5)
            {
                // End the token by setting the end position and returning.
                token.end = str;
                return true;
            }
            
            // If we've made it here, we do not yet have a token. But we need to check if
            // we have a new line. If so, we need to increment our line parameter. We don't
            // need to check for a \r\n pair. We need only look for a \n character.
            if (ch == '\n')
            {
                ++line;

                // It is valid for a new line character to be an ending ignore block.
                if (_check_ignore_block_end(str, ignore_block_start, options.ignoreBlockStart, options.ignoreBlockEnd))
                {
                    // We've reached the end of our ignore block, so now we can reset our type.
                    type = 0;
                }
            }

            str = temp;
        }
        else
        {
            // Now we need to look at the character. We need to make a distinction between letters,
            // numbers and symbols.
            if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_')
            {
                // We have a letter.
                if (type == 0)
                {
                    token.start = str;
                    type = 1;
                }

                str = temp;
            }
            else if (ch >= '0' && ch <= '9')
            {
                // We have a number.
                if (type == 0)
                {
                    token.start = str;
                    type = 2;
                }

                str = temp;
            }
            else
            {
                // We must have a symbol.
                if (type == 0)
                {
                    token.start = str;

                    // If the character is a quote character, we are starting a quote.
                    if (findfirst(options.quotes, ch) != NULL)
                    {
                        starting_quote_ch = ch;
                        type = 4;

                        str = temp;
                    }
                    else
                    {
                        type = 3;

                        // Here we check to see if the token is part of a token group. If it is, we move to
                        // the end of the token group, set the appropriate variables and then return.
                        if (_check_token_group(str, options.symbolGroups))
                        {
                            token.end = str;
                            return true;
                        }

                        // Here we need to check if we are at the start of an ignore block. We will also need
                        // to store the string that has opened the ignore block.
                        if (_check_ignore_block_start(str, options.ignoreBlockStart, ignore_block_start))
                        {
                            temp = str;
                            type = 5;
                        }
                        else
                        {
                            str = temp;
                            token.end = temp;
                            return true;
                        }
                    }
                }
                else if (type == 1)
                {
                    // The token is a word, so we want to return no matter what.
                    token.end = str;
                    return true;
                }
                else if (type == 2)
                {
                    // The token is a number. Numbers are allowed to have a single decimal point. Therefore,
                    // if this character is a decimal point and there is not yet any in the number, we don't
                    // have to return.
                    if (found_decimal)
                    {
                        token.end = str;
                        return true;
                    }
                    
                    if (ch == '.')
                    {
                        found_decimal = true;

                        // We need to make sure that the character after the decimal point is also a number.
                        // If it isn't, the decimal point needs to be considered a seperate token.
                        uchar32_t next_ch = nextchar(temp);
                        if (!(next_ch >= '0' && next_ch <= '9'))
                        {
                            token.end = str;
                            return true;
                        }
                    }
                    else
                    {
                        token.end = str;
                        return true;
                    }

                    str = temp;
                }
                else if (type == 4)
                {
                    // We're in a quote, so we need to check the character. If the character is the same
                    // as 'starting_quote_ch', we will move to the next character, set the appropriate
                    // values and then return true.
                    if (ch == starting_quote_ch && prev_ch != options.escapeCharacter)
                    {
                        str = temp;
                        token.end = str;
                        return true;
                    }

                    str = temp;
                }
                else if (type == 5)
                {
                    // We're in the ignore block, so we need to check if we've now got a matching ending
                    // ignore block string.
                    if (_check_ignore_block_end(str, ignore_block_start, options.ignoreBlockStart, options.ignoreBlockEnd))
                    {
                        // We've reached the end of our ignore block, so now we can reset our type.
                        temp = str;
                        type = 0;
                    }
                    else
                    {
                        str = temp;
                    }
                }
            }
        }

        prev_ch = ch;
    }

    // We now need to ensure that if we have a token, we return true.
    if (type != 0 && type != 5)
    {
        token.end = str;
        return true;
    }

    return false;
}

}
}

#endif // __EASL_TOKENS_NEXTTOKEN_H_
