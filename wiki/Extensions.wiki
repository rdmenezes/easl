#summary Extension Information

= Introduction =
This page will discuss EASL's extension mechanism.


= About Extensions =
EASL's extension mechanism is used to provide additional, not-so-common functionality that  does not necessarily belong in the core section of the library.

As an example, the `paths` extension provides functionality such as converting between absolute and relative paths, determining if a path is relative, etc. This kind of functionality is useful for some applications, but most applications won't need it. Therefore, this functionality is encapsulated in the `paths` extension away from the core part of the library.


= Current Extensions =
This section will discuss each individual extension.

== Paths - `#include <easl/ext/paths.h>` ==
The paths extension is used to provide functionality for working with directory and file paths.

Such functionality includes converting between absolute and relative paths; determining if a path is absolute or relative and extracting the extension of a file.

== Tokens - `#include <easl/ext/tokens.h>` ==
The tokens extension is used to split a string into different tokens. It's main function is to iterate through each token in a string. This extension is mostly useful for things such as source code parsers.

== Search - `#include <easl/ext/search.h>` ==
The search extension is used to perform more complex or not-so-common search functionality than that provided by `easl::findfirst()`, `easl::findfirstof()`, etc.

Such functionality includes determining if a string contains a non-printable character.

== Misc - `#include <easl/ext/misc.h>` ==
The misc extension is where particular functionality that is hard to categorize is placed. Such functionality does not warrant it's own extension, nor does it belong anywhere else.