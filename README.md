except - small OS exceptions library
====================================

This library will transform system exceptions – SEH on Windows, SIGNALs on POSIX, in
C++ exceptions.

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
See `Results` for more information.

Build
-----

Library uses CMake. Have a look at `build.sh` and `build.cmd` for adjustments.
