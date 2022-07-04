#include "utils.h"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <linux/fs.h>

std::ostream& operator<<(std::ostream& os, timespec t)
{
#if (1)
    char buf[32];
    struct tm tm;
    gmtime_r(&t.tv_sec, &tm);
    strftime(buf, 21, "%Y-%m-%dT%H:%M:%S", &tm);
    os << buf;
#else
    os << ctime((time_t*)&t);
#endif
    return os;
}

void printOpenFail(const std::string& filename)
{
    std::cout << "open fail. " << filename << "\n";
}

void printOpenSucceeded(const std::string& filename)
{
    std::cout << "open succeeded. " << filename << "\n";
}

void catFile(const std::string& filename)
{
    auto fd = open(filename.c_str(), O_RDONLY);
    if (fd == -1) {
        printOpenFail(filename.c_str());
    } else {
        char buffer[BUFSIZ] = {0, };
        auto readSize = read(fd, buffer, BUFSIZ);
        if (0 < readSize && readSize < BUFSIZ) {
            buffer[readSize] = 0;
        }
        std::cout << buffer;

        close(fd);
    }
}

void printErrno(int no)
{
    std::cerr << strerror(no) << "\n";
}

std::string modeString(int mode)
{
    std::string ret;

    auto type = mode & S_IFMT;

    switch (type) {
    case S_IFDIR:
        ret += "d";
        break;
    case S_IFCHR:
        ret += "c";
        break;
    case S_IFBLK:
        ret += "b";
        break;
    case S_IFSOCK:
        ret += "s";
        break;
    case S_IFLNK:
        ret += "l";
        break;

    case S_IFREG:
    default:
        ret += "-";
        break;
    }

    ret += ((mode & S_IRUSR) ? "r" : "-");
    ret += ((mode & S_IWUSR) ? "w" : "-");
    ret += ((mode & S_IXUSR) ? "x" : "-");
    ret += ((mode & S_IRGRP) ? "r" : "-");
    ret += ((mode & S_IWGRP) ? "w" : "-");
    ret += ((mode & S_IXGRP) ? "x" : "-");
    ret += ((mode & S_IROTH) ? "r" : "-");
    ret += ((mode & S_IWOTH) ? "w" : "-");
    ret += ((mode & S_IXOTH) ? "x" : "-");

    return ret;
}

#define WRITE_ATTR_IF_MATCH(str, attr, flag) \
    if (attr & flag) { \
        str += flag; \
        str += "\n"; \
    }

std::string inodeAttrString(int attr)
{
    std::string ret;

    WRITE_ATTR_IF_MATCH(ret, attr, FS_APPEND_FL);
    WRITE_ATTR_IF_MATCH(ret, attr, FS_COMPR_FL);
    WRITE_ATTR_IF_MATCH(ret, attr, FS_DIRSYNC_FL);
    WRITE_ATTR_IF_MATCH(ret, attr, FS_IMMUTABLE_FL);
    WRITE_ATTR_IF_MATCH(ret, attr, FS_JOURNAL_DATA_FL);
    WRITE_ATTR_IF_MATCH(ret, attr, FS_NOATIME_FL);
    WRITE_ATTR_IF_MATCH(ret, attr, FS_NODUMP_FL);
    WRITE_ATTR_IF_MATCH(ret, attr, FS_NOTAIL_FL);
    WRITE_ATTR_IF_MATCH(ret, attr, FS_SECRM_FL);
    WRITE_ATTR_IF_MATCH(ret, attr, FS_SYNC_FL);
    WRITE_ATTR_IF_MATCH(ret, attr, FS_TOPDIR_FL);
    WRITE_ATTR_IF_MATCH(ret, attr, FS_UNRM_FL);

    return ret;
}
