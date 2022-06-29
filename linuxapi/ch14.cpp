#include <iostream>
#include <sys/mount.h>
#include <sys/statvfs.h>
#include <string.h>
#include "utils.h"

namespace { //================================================================= File system

// Device
// Device driver (h/w io, open, close, read, write, mmap, ioctl)
// Real device (mouse, disk, tape, ...)
// Virtual device
// Device type (character device, block device)

// Disk partition
//  file system
//  data area
//  swap area

// File system
//  ext2
//  FAT, FAT32, NTFS
//  ISO 9660 CD-ROM
//  apple HFS
//  Sun NFS
//  IBM, Microsoft SMB
//  Novell NCP
//  Coda
//  ext3, ext4, Reiserfs, JFS, XFS, Btrfs (jourling)

// File system structure
//  boot block
//  super block (fs parameters)
//  i-node table
//  data block

// VFS
//  open, read, write, lseek, close, truncate, stat, mount, umount, mmap, mkdir, link, unlink, symlink, rename, ...

//-----------------------------------------------------------------------------
//  test sequence
//
//  1. su
//  2. mkdir /demo
//  3. run ./linuxapi
//  4. try running anything in /demo/bin
//-----------------------------------------------------------------------------
void test_mount()
{
    const char* source = "/dev/sdb";
    const char* target = "/demo";
    unsigned long flags= MS_NOEXEC;
    const char* fstype = "ext4";
    void* data = nullptr;

    auto ret = mount(source, target, fstype, flags, data);
    if (ret == -1) {
        std::cerr << "mount(" << source << ", " << target << ", " << fstype << ", " << flags << ", nullptr) failed.\n";
    }
}

//-----------------------------------------------------------------------------
//  test sequence
//
//  1. run ./linuxapi
//  2. mount | grep demo
//-----------------------------------------------------------------------------
void test_umount()
{
    const char* target = "/demo";
    auto ret = umount(target);
    if (ret == -1) {
        std::cerr << "umount(" << target << ") failed.\n";
    }
}

void test_fs_info()
{
    struct statvfs svfs;
    auto ret = statvfs("/demo", &svfs);
    if (ret == -1) {
        std::cerr << strerror(errno) << "\n";
    } else {
        PRINT_DEF(svfs.f_bsize);
        PRINT_DEF(svfs.f_blocks);
        PRINT_DEF(svfs.f_bavail);
        PRINT_DEF(svfs.f_bfree);
        PRINT_DEF(svfs.f_fsid);
        PRINT_DEF(svfs.f_files);
        PRINT_DEF(svfs.f_ffree);
        PRINT_DEF(svfs.f_flag);
        PRINT_DEF(svfs.f_namemax);

        std::cout << "\n";
        std::cout << "reserved: " << (svfs.f_bfree - svfs.f_bavail) << "\n";
    }
}

} //===========================================================================

void test_ch_14()
{
    //test_mount();
    //test_umount();
    test_fs_info();
}
