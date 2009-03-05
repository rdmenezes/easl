/**
*   \file   string.h
*   \author Dave Reid
*   \brief  Header file for the string classes.
*/
#ifndef __EASL_STRING_H_
#define __EASL_STRING_H_

#include "slow_string.h"

namespace easl
{

#ifdef EASL_USING_FAST_STRING
typedef fast_string<char> string;
typedef fast_string<char> string8;
typedef fast_string<char16_t> string16;
typedef fast_string<char32_t> string32;
typedef fast_string<wchar_t> wstring;
#else
typedef slow_string<char> string;
typedef slow_string<char> string8;
typedef slow_string<char16_t> string16;
typedef slow_string<char32_t> string32;
typedef slow_string<wchar_t> wstring;
#endif

}

#endif // __EASL_STRING_H_
