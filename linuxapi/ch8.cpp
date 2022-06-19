#include <iostream>
#include <pwd.h>
#include <grp.h>
#ifndef OS_MAC
#include <shadow.h>
#endif

#define _XOPEN_SOURCE
#include <unistd.h>

namespace { //=================================================================

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

void test_ch_8()
{
    test_crypt();
}
