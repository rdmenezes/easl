/**
*   \file   setup.h
*   \author Dave Reid
*   \brief  Header file for compiler and platform macros.
*/
#ifndef __EASL_SETUP_H_
#define __EASL_SETUP_H_

// The different bitness for various things.
#define BITS8               0x00000008
#define BITS16              0x00000010
#define BITS32              0x00000020
#define BITS64              0x00000040

// The different compilers.
// Visual C
#define COMPILER_VC			0x10000000
#define COMPILER_VC60		0x10000001
#define COMPILER_VC70		0x10000002
#define COMPILER_VC71		0x10000004
#define COMPILER_VC80		0x10000008
#define COMPILER_VC90		0x10000010

// GCC
#define COMPILER_GCC		0x20000000
#define COMPILER_GCC34		0x20000001
#define COMPILER_GCC35		0x20000002
#define COMPILER_GCC40		0x20000004
#define COMPILER_GCC41		0x20000008
#define COMPILER_GCC42		0x20000010
#define COMPILER_GCC43		0x20000020


// Setup our compiler.
#ifndef COMPILER

// Visual C
#ifdef _MSC_VER

// Our architecture
#ifdef _WIN64
#define TARGET_BITS			BITS64
#elif _WIN32
#define TARGET_BITS			BITS32
#endif

#if (_MSC_VER == 1200)
#define COMPILER COMPILER_VC60
#endif

#if (_MSC_VER == 1300)
#define COMPILER COMPILER_VC70
#endif

#if (_MSC_VER == 1310)
#define COMPILER COMPILER_VC71
#endif

#if (_MSC_VER == 1400)
#define COMPILER COMPILER_VC80
#endif

#if (_MSC_VER == 1500)
#define COMPILER COMPILER_VC90
#endif

#endif	// _MSC_VER

// GCC
#ifdef __GNUC__

// Our architecture
#if(defined(__WORDSIZE) && (__WORDSIZE == 64))
#define TARGET_BITS			BITS64
#else
#define TARGET_BITS			BITS32
#endif

#if (__GNUC__ == 3) && (__GNUC_MINOR__ == 4)
#define COMPILER			COMPILER_GCC34
#endif

#if (__GNUC__ == 3) && (__GNUC_MINOR__ == 5)
#define COMPILER			COMPILER_GCC35
#endif

#if (__GNUC__ == 4) && (__GNUC_MINOR__ == 0)
#define COMPILER			COMPILER_GCC40
#endif

#if (__GNUC__ == 4) && (__GNUC_MINOR__ == 1)
#define COMPILER			COMPILER_GCC41
#endif

#if (__GNUC__ == 4) && (__GNUC_MINOR__ == 2)
#define COMPILER			COMPILER_GCC42
#endif

#if (__GNUC__ == 4) && (__GNUC_MINOR__ == 3)
#define COMPILER			COMPILER_GCC43
#endif

#endif	// __GNUC__
#endif	// COMPILER


#endif // __EASL_SETUP_H_
