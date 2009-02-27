/**
*   \file   istype.h
*   \author Dave Reid
*   \brief  Header file for istype() implementations.
*/
#ifndef __EASL_ISTYPE_H_
#define __EASL_ISTYPE_H_

#include "nextchar.h"
#include <ctype.h>      // For tolower().

namespace easl
{

template <typename T>
bool _istype_fd(const T *str, size_t strLength, uchar32_t suffix)
{
    // The previous character that we've checked.
    uchar32_t prev_ch = '\0';

    // Determines if we've yet found a decimal point. We use this to ensure that
    // we don't have more than one decimal point in the number.
    bool found_decimal = false;

    // Determines if we've found a digit. We don't have a real integer if
    // we dont have a digit.
    bool found_digit = false;

    uchar32_t ch;
    while (strLength > 0 && (ch = nextchar(str)) != NULL)
    {
        // Stole this check from http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2271.html.
        // Apendix - 23 - Algorithmic optimization vs. hardware optimization.
        // It's essentially the same as isdigit(). We may end up using isdigit() if it helps with
        // internationalisation.
        if (!((uchar32_t)(ch - '0') <= 9))
        {
            if (ch == '.')
            {
                if (found_decimal)
                {
                    return false;
                }

                found_decimal = true;
            }
            else if (ch == '-')
            {
                if (prev_ch != '\0')
                {
                    return false;
                }
            }
            else
            {
                // We can have a suffix.
                if ((uchar32_t)tolower(ch) == suffix)
                {
                    // We need to ensure that the suffix is the last character in the string.
                    if (strLength - charwidth<T>(ch) == 0)
                    {
                        if (prev_ch != '.')
                        {
                            return true;
                        }

                        return false;
                    }
                    else
                    {
                        const T *temp = str;
                        uchar32_t ch2 = nextchar(temp);
                        if (ch2 == '\0')
                        {
                            if (prev_ch != '.')
                            {
                                return true;
                            }

                            return false;
                        }
                    }
                }

                return false;
            }
        }
        else
        {
            found_digit = true;
        }

        prev_ch = ch;
        strLength -= charwidth<T>(ch);
    }

    return found_digit && found_decimal && prev_ch != '.';
}


/**
*   \brief                 Determines if a string represents a value of a particular data type.
*   \param  str       [in] The string to check.
*   \param  strLength [in] The length in T's of the string.
*   \return                True if the string is correctly represented as a value of the type specified by U.
*
*   \remarks
*       In order for the appropriate implementation to return true, the type must be exactly represented.
*       For example, the 'float' implementation will return false if no decimal point is contained in the
*       number. This type of number is more correctly an integer. Another example is that the unsigned
*       versions can not contain negative signs in front of the number.
*       \par
*       C/C++ data types can have the standard C/C++ suffixes appropriate for that type. For example, the
*       float implementation will still return true if the number is suffixed with an 'f'.
*/
// NOTE: We have to use this structure in order to do the operations. It's a pain, but it's the only way
//       while we don't have partial specialisation for functions.
template <typename U, typename T>
struct _istype
{
    static bool call(const T *str, size_t strLength)
    {
        (void)str;
        (void)strLength;

        // If we make it here, the type can not be converted.
        assert(false);
        return false;
    }
};

template <typename T>
struct _istype<__int64, T>
{
    static bool call(const T *str, size_t strLength)
    {
        // The previous character that we've checked.
        uchar32_t prev_ch = '\0';

        // Determines if we've found a digit. We don't have a real integer if
        // we dont have a digit.
        bool found_digit = false;

        uchar32_t ch;
        while (strLength > 0 && (ch = nextchar(str)) != NULL)
        {
            // Stole this check from http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2271.html.
            // Apendix - 23 - Algorithmic optimization vs. hardware optimization.
            // It's essentially the same as isdigit(). We may end up using isdigit() if it helps with
            // internationalisation.
            if (!((uchar32_t)(ch - '0') <= 9))
            {
                if (!(prev_ch == '\0' && ch == '-'))
                {
                    return false;
                }
            }
            else
            {
                found_digit = true;
            }

            prev_ch = ch;
            strLength -= charwidth<T>(ch);
        }

        return found_digit;
    }
};
template <typename T>
struct _istype<unsigned __int64, T>
{
    static inline bool call(const T *str, size_t strLength)
    {
        // Determines if we've found a digit. We don't have a real integer if
        // we dont have a digit.
        bool found_digit = false;

        uchar32_t ch;
        while (strLength > 0 && (ch = nextchar(str)) != NULL)
        {
            // Stole this check from http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2271.html.
            // Apendix - 23 - Algorithmic optimization vs. hardware optimization.
            // It's essentially the same as isdigit(). We may end up using isdigit() if it helps with
            // internationalisation.
            if (!((uchar32_t)(ch - '0') <= 9))
            {
                return false;
            }
            else
            {
                found_digit = true;
            }

            strLength -= charwidth<T>(ch);
        }

        return found_digit;
    }
};

template <typename T>
struct _istype<int, T>
{
    static inline bool call(const T *str, size_t strLength)
    {
        return _istype<__int64, T>::call(str, strLength);
    }
};
template <typename T>
struct _istype<unsigned int, T>
{
    static inline bool call(const T *str, size_t strLength)
    {
        return _istype<unsigned __int64, T>::call(str, strLength);
    }
};

template <typename T>
struct _istype<long, T>
{
    static inline bool call(const T *str, size_t strLength)
    {
        return _istype<__int64, T>::call(str, strLength);
    }
};
template <typename T>
struct _istype<unsigned long, T>
{
    static inline bool call(const T *str, size_t strLength)
    {
        return _istype<unsigned __int64, T>::call(str, strLength);
    }
};

template <typename T>
struct _istype<short, T>
{
    static inline bool call(const T *str, size_t strLength)
    {
        return _istype<__int64, T>::call(str, strLength);
    }
};
template <typename T>
struct _istype<unsigned short, T>
{
    static inline bool call(const T *str, size_t strLength)
    {
        return _istype<unsigned __int64, T>::call(str, strLength);
    }
};

template <typename T>
struct _istype<char, T>
{
    static inline bool call(const T *str, size_t strLength)
    {
        return _istype<__int64, T>::call(str, strLength);
    }
};
template <typename T>
struct _istype<unsigned char, T>
{
    static inline bool call(const T *str, size_t strLength)
    {
        return _istype<unsigned __int64, T>::call(str, strLength);
    }
};


template <typename T>
struct _istype<float, T>
{
    static inline bool call(const T *str, size_t strLength)
    {
        return _istype_fd(str, strLength, 'f');
    }
};

template <typename T>
struct _istype<double, T>
{
    static inline bool call(const T *str, size_t strLength)
    {
        return _istype_fd(str, strLength, 'd');
    }
};

template <typename T>
struct _istype<bool, T>
{
    static inline bool call(const T *str, size_t strLength)
    {
        return equal(str, "false", false, strLength) ||
               equal(str, "0", false, strLength) ||
               equal(str, "true", false, strLength) ||
               equal(str, "1", false, strLength);
    }
};


template <typename U, typename T>
inline bool istype(const T *str, size_t strLength = -1)
{
    return _istype<U, T>::call(str, strLength);
}


}

#endif // __EASL_ISTYPE_H_
