#include <iostream>
#include <sys/stat.h>
#include "utils.h"

namespace { //================================================================= File attribute

void test_stat(const char* filename)
{
    struct stat st;
    auto ret = stat(filename, &st);
    if (0 == ret) {
        std::cout << "stat(" << filename << ")\n";
        auto st_st_mode = modeString(st.st_mode);
        TAB(); PRINT_DEF2(st.st_dev, "device id");
        TAB(); PRINT_DEF2(st.st_ino, "i-node count");
        TAB(); PRINT_DEF2(st_st_mode, "type & permissions");
        TAB(); PRINT_DEF2(st.st_nlink, "hardlink count");
        TAB(); PRINT_DEF2(st.st_uid, "owner uid");
        TAB(); PRINT_DEF2(st.st_gid, "owner gid");
        TAB(); PRINT_DEF2(st.st_rdev, "");
        TAB(); PRINT_DEF2(st.st_size, "bytes");
        TAB(); PRINT_DEF2(st.st_blksize, "blocks");
        TAB(); PRINT_DEF2(st.st_atim, "access");
        TAB(); PRINT_DEF2(st.st_mtim, "modify");
        TAB(); PRINT_DEF2(st.st_ctim, "status");
    } else {
        printErrno(errno);
    }
}

void test_lstat(const char* filename)
{
    struct stat st;
    auto ret = lstat(filename, &st);
    if (0 == ret) {
        std::cout << "stat(" << filename << ")\n";
        auto st_st_mode = modeString(st.st_mode);
        TAB(); PRINT_DEF2(st.st_dev, "device id");
        TAB(); PRINT_DEF2(st.st_ino, "i-node count");
        TAB(); PRINT_DEF2(st_st_mode, "type & permissions");
        TAB(); PRINT_DEF2(st.st_nlink, "hardlink count");
        TAB(); PRINT_DEF2(st.st_uid, "owner uid");
        TAB(); PRINT_DEF2(st.st_gid, "owner gid");
        TAB(); PRINT_DEF2(st.st_rdev, "");
        TAB(); PRINT_DEF2(st.st_size, "bytes");
        TAB(); PRINT_DEF2(st.st_blksize, "blocks");
        TAB(); PRINT_DEF2(st.st_atim, "access");
        TAB(); PRINT_DEF2(st.st_mtim, "modify");
        TAB(); PRINT_DEF2(st.st_ctim, "status");
    } else {
        printErrno(errno);
    }
}

} //===========================================================================

void test_ch_15()
{
    test_stat("/bin/grep");
    test_lstat("/bin/vi");
}
