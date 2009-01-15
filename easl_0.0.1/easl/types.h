/**
*   \file   types.h
*   \author Dave Reid
*   \brief  Header file for the custom data types for strings.
*/
#ifndef __EASL_TYPES_H_
#define __EASL_TYPES_H_

namespace easl
{

// These typedefs (except char8_t) have the same names as those in the current C++0x
// specification draft. We will remove these later on and use the proper versions when
// they're available on most compilers. If char8_t is not a proper type for the final
// C++0x spec, we will keep the typedef for clarity. (It really should be a built-in
// type).
typedef short  char16_t;
typedef long   char32_t;

typedef unsigned short  uchar16_t;
typedef unsigned long   uchar32_t;


// Enumerator for different locales. These are used for locale dependant routines.
/*enum locale
{
    locale_default = 0,

    locale_english,             ///< Default English.
    locale_english_us,          ///< US English
    locale_english_gb,          ///< British English
    locale_english_aus,         ///< Australian English
    locale_english_nz,          ///< New-Zealand English
    locale_english_can,         ///< Canadian English
    
    locale_chinese,             ///< Default Chinese.
    locale_chinese_simplified,  ///< Simplified Chinese
    locale_chinese_traditional, ///< Traditional Chinese

    locale_czech,               ///< Czech

    locale_danish,              ///< Danish

    locale_dutch,               ///< Default Dutch
    locale_dutch_bel,           ///< Belgium Dutch

    locale_finnish,             ///< Finnish

    locale_french,              ///< Default French
    locale_french_bel,          ///< Belgium French
    locale_french_can,          ///< Canadian French
    locale_french_che,          ///< Swiss French

    locale_german,              ///< Default German
};*/


}

#endif // __EASL_TYPES_H_
