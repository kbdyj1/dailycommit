#include <iostream>
#include <memory>
#include <string.h>
#include <sys/xattr.h>
#include "utils.h"

namespace { //=================================================================

// supported namespace
// user
// trusted
// system
// security

struct ExtData {
    int version;
    char company[20];
};

void test_setxattr(const char* filename)
{
    std::cout << "*** set attr : " << filename << " ***\n";

    auto ext = ExtData { 100, "Routine" };
    auto ret = setxattr(filename, "user.version", &ext.version, sizeof(ext.version), 0);
    if (-1 == ret) {
        printErrno(errno);
        return;
    }

    ret = setxattr(filename, "user.company", ext.company, sizeof(ext.company), 0);
    if (-1 == ret) {
        printErrno(errno);
    }
}

void test_getxattr(const char* filename)
{
    std::cout << "*** get attr : " << filename << " ***\n";

    auto ext = ExtData {};
    auto ret = getxattr(filename, "user.version", &ext.version, sizeof(ext.version));
    if (-1 == ret) {
        printErrno(errno);
        return;
    }

    ret = getxattr(filename, "user.company", ext.company, sizeof(ext.company));
    if (-1 == ret) {
        printErrno(errno);
        return;
    }

    std::cout << "user.version: " << ext.version << "\n";
    std::cout << "user.company: " << ext.company << "\n";
}

void test_listxattr(const char* filename)
{
    std::cout << "*** list attr : " << filename << " ***\n";

    auto size = listxattr(filename, 0, 0);
    auto list = std::unique_ptr<char[]>(new char[size + 1]);
    auto ret = listxattr(filename, list.get(), size+1);
    if (-1 == ret) {
        printErrno(errno);
        return;
    }

    char* p = list.get();
    for (auto i=0; i < size; i++) {
        if (p[i] == '\0') {
            std::cout << p << "\n";
            p += (i+1);
        }
    }
}

void test_removexattr(const char* filename)
{
    std::cout << "*** remove attr : " << filename << " ***\n";

    auto ret = removexattr(filename, "user.version");
    if (-1 == ret) {
        printErrno(errno);
        return;
    }

    ret = removexattr(filename, "user.company");
    if (-1 == ret) {
        printErrno(errno);
    }
}

} //===========================================================================

void test_ch_16()
{
    system("touch tfile");
    test_setxattr("tfile");
    test_listxattr("tfile");
    test_getxattr("tfile");
    test_removexattr("tfile");
    test_listxattr("tfile");
}
