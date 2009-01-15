/**
*   \file   reference_string.h
*   \author Dave Reid
*   \brief  Header file for the reference_string structure.
*/
#ifndef __EASL_REFERENCESTRING_H_
#define __EASL_REFERENCESTRING_H_

namespace easl
{

/**
*   \brief  Structure representing a reference string.
*
*   A reference string contains two properties: a pointer to the start of the
*   string; and a pointer that points at the position in memory just past the
*   last character of the string.
*
*   As the name suggests, a reference_string _references_ an existing string.
*   It does not contain it's own pool of memory to store the string. Instead
*   the pointers reference an existing pool of memory that is owned elsewhere
*   in the program.
*
*   reference_strings are mainly used to reference a particular sub-section
*   of an existing string. By doing this, the additional memory does not need
*   to be allocated, and data does not need to be copied.
*
*   It's important to be careful when using reference_strings because once the
*   host string goes out of scope, the reference string will become invalid.
*/
template <typename T>
struct reference_string
{
    /// Pointer to the start of the string.
    T *start;

    /// Pointer to the memory location just past the last character of the string.
    T *end;
};

}

#endif // __EASL_REFERENCESTRING_H_
