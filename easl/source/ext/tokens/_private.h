/**
*   \file   _private.h
*   \author Dave Reid
*   \brief  Private header file for the tokens extension.
*/
#ifndef __EASL_TOKENS_PRIVATE_H_
#define __EASL_TOKENS_PRIVATE_H_

namespace easl
{

#define EASL_TOKENS_SET_DEFAULT_SYMBOLS_ENG(symbols) \
    T new_symbols[64] = \
        {'.', ' ', \
         ',', ' ', \
         '(', ' ', \
         ')', ' ', \
         '{', ' ', \
         '}', ' ', \
         '<', ' ', \
         '>', ' ', \
         '[', ' ', \
         ']', ' ', \
         ';', ' ', \
         ':', ' ', \
         '/', ' ', \
         '\\', ' ', \
         '?', ' ', \
         '+', ' ', \
         '-', ' ', \
         '*', ' ', \
         '/', ' ', \
         '`', ' ', \
         '~', ' ', \
         '!', ' ', \
         '@', ' ', \
         '#', ' ', \
         '$', ' ', \
         '%', ' ', \
         '^', ' ', \
         '&', ' ', \
         '=', ' ', \
         '|', ' ', \
         '\0'}; \
    symbols = new_symbols;

#define EASL_TOKENS_SET_DEFAULT_QUOTES_ENG(quotes) \
    T new_quotes[8] = \
        {'\"', ' ', \
         '\'', ' ', \
         '\0'}; \
    quotes = new_quotes;



}

#endif // __EASL_TOKENS_PRIVATE_H_
