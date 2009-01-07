/**
*   \file   easl.h
*   \author Dave Reid
*   \brief  Primary header file for EASL.
*/
#ifndef __EASL_H_
#define __EASL_H_

#include <tchar.h>
#include "slow_string.h"

namespace easl
{

#ifdef EASL_USING_FAST_STRING
typedef fast_string<char> string;
typedef fast_string<char16_t> string16;
typedef fast_string<char32_t> string32;
typedef fast_string<wchar_t> wstring;
#else
typedef slow_string<char> string;
typedef slow_string<char16_t> string16;
typedef slow_string<char32_t> string32;
typedef slow_string<wchar_t> wstring;
#endif

}

#endif // __EASL_H_