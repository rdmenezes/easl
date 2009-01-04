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
typedef fast_string<TCHAR> string;
#else

#ifdef EASL_DEFAULT_STRING_UTF32
typedef slow_string<char32_t> string;
#elif  EASL_DEFAULT_STRING_UTF16
typedef slow_string<char16_t> string;
#else
typedef slow_string<char> string;
#endif

#endif

}

#endif // __EASL_H_