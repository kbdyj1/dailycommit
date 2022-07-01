#include <iostream>
#include <sys/stat.h>
#include "utils.h"

namespace { //================================================================= File attribute

void printStat(const char* filename, const struct stat& st)
{
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
    TAB(); PRINT_DEF2(st.st_blocks, "blocks");
    TAB(); PRINT_DEF2(st.st_blksize, "bytes");
    TAB(); PRINT_DEF2(st.st_atim, "access");
    TAB(); PRINT_DEF2(st.st_mtim, "modify");
    TAB(); PRINT_DEF2(st.st_ctim, "status");
}

/*
stat(/bin/grep)
    st.st_dev: 2064 (device id)
    st.st_ino: 1842 (i-node count)
    st_st_mode: -rwxr-xr-x (type & permissions)
    st.st_nlink: 1 (hardlink count)
    st.st_uid: 0 (owner uid)
    st.st_gid: 0 (owner gid)
    st.st_rdev: 0 ()
    st.st_size: 199136 (bytes)
    st.st_blocks: 392 (blocks)
    st.st_blksize: 4096 (bytes)
    st.st_atim: 2022-07-01T13:15:59 (access)
    st.st_mtim: 2020-01-29T09:31:53 (modify)
    st.st_ctim: 2020-10-05T13:40:19 (status)
*/
void test_stat(const char* filename)
{
    struct stat st;
    auto ret = stat(filename, &st);
    if (0 == ret) {
        printStat(filename, st);
    } else {
        printErrno(errno);
    }
}

void test_lstat(const char* filename)
{
    struct stat st;
    auto ret = lstat(filename, &st);
    if (0 == ret) {
        printStat(filename, st);
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
