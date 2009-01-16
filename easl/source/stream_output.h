/**
*   \file   stream_output.h
*   \author Dave Reid
*   \brief  Header file for outputting different strings to C++ output streams.
*/
#ifndef __EASL_STREAMOUTPUT_H_
#define __EASL_STREAMOUTPUT_H_

#include <ostream>
#include "string.h"
#include "reference_string.h"
#include "nextchar.h"


template <typename T>
std::basic_ostream<T> & operator << (std::basic_ostream<T> &stream, const easl::slow_string<T> &str)
{
    return stream.write(str.c_str(), static_cast<std::streamsize>(str.length()));
}

template <typename T, typename U>
std::basic_ostream<T> & operator << (std::basic_ostream<T> &stream, const easl::slow_string<U> &str)
{
    const U *temp = str.c_str();

    easl::uchar32_t ch;
    while ((ch = easl::nextchar(temp)) != NULL)
    {
        stream.put((T)ch);
    }

    return stream;
}


template <typename T>
std::basic_ostream<T> & operator << (std::basic_ostream<T> &stream, const easl::reference_string<T> &str)
{
    return stream.write(str.start, static_cast<std::streamsize>(str.end - str.start));
}

template <typename T, typename U>
std::basic_ostream<T> & operator << (std::basic_ostream<T> &stream, const easl::reference_string<U> &str)
{
    U *temp = str.start;

    easl::uchar32_t ch = easl::nextchar(temp);
    while (temp <= str.end)
    {
        stream.put((T)ch);
        ch = easl::nextchar(temp);
    }

    return stream;
}




#endif // __EASL_STREAMOUTPUT_H_
