/* Keep the following two lines in this file by themselves */
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#if 0
Do not write anything else inside this file. The header file catch.hpp has
more than 8000+ lines of code. If you #include "catch.hpp" DIRECTLY into
your source (.cpp) file the compiler will have to copy these 8000+ lines
everytime you compile your code, increasing the compilation time.

By keeping the above two lines SEPARATE from your source code (.cpp), your 
compile may generate a much smaller a pre-compiled header (.pch) out of this 
file, making your compilation faster.
#endif