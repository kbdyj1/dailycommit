#include <iostream>
#include <memory>
#include <pwd.h>
#include <grp.h>
#ifndef OS_MAC
#include <mcheck.h>
#include <shadow.h>
#endif

#define _XOPEN_SOURCE
#include <unistd.h>

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

char MALLOC_TRACE[] = "MALLOC_TRACE=mcheck.log";
/*
= Start
@ ~/project/dailycommit/build-linuxapi-Desktop-Debug/linuxapi:[0x55555555826d] + 0x55555556f3b0 0x10
@ ~/project/dailycommit/build-linuxapi-Desktop-Debug/linuxapi:[0x555555558282] < 0x55555556f3b0
@ ~/project/dailycommit/build-linuxapi-Desktop-Debug/linuxapi:[0x555555558282] > 0x55555556f3b0 0x20
@ ~/project/dailycommit/build-linuxapi-Desktop-Debug/linuxapi:[0x555555558292] - 0x55555556f3b0
@ /lib/x86_64-linux-gnu/libstdc++.so.6:(_Znwm+0x1c)[0x7ffff7e4bdac] + 0x55555556f3e0 0x48
@ ~/project/dailycommit/build-linuxapi-Desktop-Debug/linuxapi:[0x555555558533] - 0x55555556f3e0
= End
*/
struct Item {
    int id;
    std::string name;
    std::string phoneNumber;
};

#ifndef OS_MAC
void test_mtrace()
{
    putenv(MALLOC_TRACE);

    mtrace();

    auto* p = malloc(16);
    auto* np = realloc(p, 32);
    if (np == nullptr) {
        std::cout << "realloc(p, 32) fail" << "\n";
    } else {
        std::cout << "p: " << p << ", np: " << np << "\n";
        p = np;
    }

    auto* sp = alloca(128); // stack allocation

    free(p);
    {
        auto ptr = std::unique_ptr<Item>(new Item{20, "John", "010-0000-1111"});
    }
    muntrace();
}
#endif

//-------------------------------------------------------------------
//  user & group
//-------------------------------------------------------------------

void test_pw()
{
    const char* user = "ymhong";
    auto* pwd = getpwnam(user);
    if (nullptr == pwd) {
         std::cout << "getpwnam(" << user << "): return nullptr. errno: " << errno << "\n";
    } else {
        std::cout << "pw_name: " << pwd->pw_name << "\n"
                  << "pw_uid: " << pwd->pw_uid << "\n"
                  << "pw_gid: " << pwd->pw_gid << "\n"
                  << "pw_passwd: " << pwd->pw_passwd << "\n"
                  << "pw_shell: " << pwd->pw_shell << "\n"
                  << "pw_gecos: " << pwd->pw_gecos << "\n"
                  << "pw_dir: " << pwd->pw_dir << "\n";
    }
}

void test_group()
{
    const char* groupName = "adm";
    auto* grp = getgrnam(groupName);
    if (nullptr == grp) {
        std::cout << "getgrnam(" << groupName << "): return nullptr. errno: " << errno << "\n";
    } else {
        std::cout << "gr_gid: " << grp->gr_gid << "\n"
                  << "gr_name: " << grp->gr_name << "\n"
                  << "gr_passwd: " << grp->gr_passwd << "\n";
        std::cout << "gr_mem: ";
        char** pp = grp->gr_mem;
        while (*pp != nullptr) {
            std::cout << *pp << " ";
            ++pp;
        }
        std::cout <<  "\n";
    }
}

void test_pw_scan()
{
    passwd* pw;
    while ((pw = getpwent()) != nullptr) {
        std::cout << pw->pw_name << "\n";
    }
    endpwent();
}

#ifndef OS_MAC
void test_shadow_pw_scan()
{
    spwd* pw;
    while ((pw = getspent()) != nullptr) {
        std::cout << pw->sp_namp << " : " << pw->sp_pwdp << "\n";
    }
    endspent();
}
#endif

void test_crypt()
{
    const char*key = "HelloQt";
    const char*salt = "qt";
    auto* crypted = crypt(key, salt);
    std::cout << "(key:" << key << ", salt: " << salt << ") -> " << crypted << "\n";
}

} //===========================================================================

void test_ch_7()
{
    //test_malloc();
    //test_mtrace();
    //test_pw();
    //test_group();
    test_crypt();
}
