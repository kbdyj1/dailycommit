#include <iostream>
#include <unistd.h>
#include <memory>

extern char* end;

namespace { //=================================================================

void print_current_break()
{
    std::cout << "break: " << sbrk(0) << "\n";
}
void test_sbrk()
{
   std::cout << "end: " << static_cast<void*>(end) << "\n";

   auto* _0 = sbrk(0);
   std::cout << _0 << "\n";

   auto* _10 = sbrk(10);
   std::cout << _10 << "\n";
}
/*
break: 0x55555557e000

 *** START ALOC ***
break: 0x5555555c0000

 ...
*/
void test_malloc()
{
    constexpr int MAX_ALLOCS = 10000;
    void* mtable[MAX_ALLOCS];

    print_current_break();

    int numAllocs = MAX_ALLOCS;
    int blockSize = 16;

    std::cout << "\n *** START ALOC ***\n";

    for (auto i=0; i<numAllocs; i++) {
        mtable[i] = malloc(blockSize);
        if (mtable[i] == nullptr) {
            std::cout << "[" << i << "]: alloc fail.";
            break;
        }
    }

    print_current_break();

    std::cout << "\n *** START FREE ***\n";

    int freeMin = 100;
    int freeMax = 9000;
    int freeStep = 2;

    for (auto i=freeMin; i<freeMax; i+=freeStep) {
        free(mtable[i]);
        mtable[i] = nullptr;
    }

    print_current_break();

    std::cout << "\n *** CLEAN UP ***\n";

    for (auto i=0; i<MAX_ALLOCS; i++) {
        if (mtable[i] != nullptr)
            free(mtable[i]);
    }

    print_current_break();
}

} //===========================================================================

void test_ch_7()
{
    test_malloc();
}
