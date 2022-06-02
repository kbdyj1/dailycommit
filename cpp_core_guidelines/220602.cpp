#include <iostream>

namespace { //=================================================================

// CPL.1 Prefer C++ to C

// CPL.2 If you must use C, use the common subset of C and C++, and compile the C code as C++

#define COMPILER_C

void test_cpl_2()
{
#ifndef __cplusplus
    // C
    int* p0 = malloc(10*sizeof(int));
    free(p0);
#else
    // C++
    int* p1 = static_cast<int*>(malloc(10*sizeof(int)));
    int* p2 = new int[10];

    delete [] p2;
    free(p1);
#endif
    // C and C++
    int* p3 = (int*)malloc(10*sizeof(int));

    free(p3);
}

// CPL.3 If you must use C for interfaces, use C++ in the calling code using such interface

// SF.1 Use a .cpp suffix for code files and .h for interface files if your project doesn't already follow another convention

// SF.2 A .h file must not contain object definitions or non-inline function definitions

// SF.3 Use .h files for all declarations used in multiple source files

// SF.4 Include .h files before other declarations in a file

// SF.5 A .cpp file must include the .h file(s) that defines its interface

// SF.6 Use using namespace directives for transition, for foundation libraries (such as std), or within a local scope(only)

// SF.7 Don't write using namespace at global scope in a header file
//  exception - using namespace std::literals

// SF.8 Use #include guards for all .h files

// SF.9 Avoid cyclic dependencies among source files

// SF.10 Avoid dependencies on implicitly #included names

// SF.11 Header files should be self-contained

// SF.12 Prefer the quoted form of #include for files relative to the including file and the angle braket form everywhere else

// SF.20 Use namespace to express logical structure

// SF.21 Don't use an unnamed (anonymous) namespace in a header

// SF.22 Use an unnamed (anonymous) namespace for all internal/non-exported entities

} //===========================================================================
