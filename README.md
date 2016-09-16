except - small OS exceptions library
====================================

This library will transform system exceptions – SEH on Windows, SIGNALs on POSIX, in
C++ exceptions.

This repository contains code for the [NullPointerException in C++](https://cristianadam.eu/20160914/nullpointerexception-in-c-plus-plus/) article.

Structured Exceptions Handling (SEH)
------------------------------------

Library uses `_set_se_translator` from `eh.h` in cooperation with `/EHa` compiler flag.

CMake build option: `USE_WINDOWS_SE`.

POSIX Signals
-------------

Library can use both `signal` and `sigaction` in cooperation with `-fnon-call-exceptions` compiler flag. 

`sigaction` works as expected only with GCC on Linux. `signal` partially works only on... Windows.

CMake build options: `USE_POSIX_SIGNAL` and `USE_POSIX_SIGACTION`.
 
Tested Compilers / Platforms
----------------------------
See `results` for more information.

Build
-----

Library uses CMake. Have a look at `build.sh` and `build.cmd` for adjustments.


Note
----

Since this code was for testing purposes, one should not use it as it is in production. Make sure you select only the exeptions you can handle (null pointer, division by zero) and for the rest just call std::abort.

One should be careful in allocating in POSIX signals, std::ostringstream will allocate memory.
