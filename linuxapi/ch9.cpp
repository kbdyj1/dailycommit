#include <iostream>
#include <unistd.h>
#include <limits.h>
#include <grp.h>
#include <memory>

namespace { //=================================================================

void test_getxxx()
{
    std::cout << "uid: " << getuid() << "\n";
    std::cout << "euid: " << geteuid() << "\n";
    std::cout << "gid: " << getgid() << "\n";
    std::cout << "egid: " << getegid() << "\n";
}

void test_getresxid()
{
    uid_t ruid;
    uid_t euid;
    uid_t suid;
    auto res = getresuid(&ruid, &euid, &suid);
    if (res == 0) {
        std::cout << "ruid: " << ruid << ", euid: " << euid << ", suid: " << suid << "\n";
    }

    gid_t rgid;
    gid_t egid;
    gid_t sgid;
    res = getresgid(&rgid, &egid, &sgid);
    if (res == 0) {
        std::cout << "rgid: " << rgid << ", egid: " << egid << ", sgid: " << sgid << "\n";
    }
}

void test_groups()
{
    // "cat /proc/sys/kernel/ngroups_max" -> NGROUPS_MAX
    gid_t grouplist[NGROUPS_MAX + 1];
    auto res = getgroups(NGROUPS_MAX, grouplist);

    if (0 < res) {
        for (auto i=0; i<res; i++) {
            std::cout << "group[" << i << "] : " << grouplist[i] << "\n";
        }
    }
}

} //===========================================================================

void test_ch_9()
{
    //test_getresxid();
    test_groups();
}
