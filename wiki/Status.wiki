#summary Current project status

= Introduction =
This page will discuss the current development status of the project.


= Platform and Compiler Support =
Below is a table summarising the supported platforms and compilers.

|| || *VC++ 8.0* || *VC++ 9.0* || *GCC 3.x* || *GCC 4.x* || *Borland C++* || *Intel C++* ||
|| *Windows XP*    || Yes || Yes || Yes (MinGW) || Yes (MinGW) || ? || ? ||
|| *Windows Vista* || Yes || Yes || Yes (MinGW) || Yes (MinGW) || ? || ? ||
|| *Linux 2.4.x*   || N/A || N/A || ? || ? || ? || ? ||
|| *Linux 2.6.x*   || N/A || N/A || ? || ? || ? || ? ||
|| *Mac OS X*      || N/A || N/A || ? || ? || ? || ? ||

The library should work on pretty much everything. Borland C++ and Intel C++ compilers havn't been tested.

Support on Linux and OSX is unknown, but should work OK. Like above, Linux and OSX has not been tested yet.

The library will not provide support for operating systems older than Windows XP, Linux kernal 2.4.x and Mac OS X. Likewise, compilers older than VC++ 8.0 and GCC 3.x will not be supported. That said, the library might still work on older platforms and compilers, but no explicit support will be included in EASL.