#include <iostream>

namespace { //=================================================================

// SL.io.1 Use character-level input only when you have to

namespace io1 {

constexpr int MAX_BUFFER = 128;
void bad() {
    char c;
    char buf[MAX_BUFFER];
    int i=0;
    while (std::cin.get(c) && !isspace(c) && i<MAX_BUFFER)
        buf[i++] = c;
    if (i == MAX_BUFFER) {
        //...
    }
}

void better() {
    std::string s;
    s.reserve(MAX_BUFFER);
    std::cin >> s;
}

} // io1

// SL.io.2 When reading, always consider ill-formed input

// SL.io.3 Prefer iostreams for I/O

// SL.io.10 Unless you use printf-family functions call ios_base::sync_with_stdio(false)

// SL.io.50 Avoid endl
//  endl calls redundant flush

// SL.C.1 Don't use setjmp/longjmp
//  ignore destructors

// A.1 Separate stable code from less stable code

// A.2 Express potentially resuable parts as a library

// A.4 There should be no cycles among libraries

} //===========================================================================
