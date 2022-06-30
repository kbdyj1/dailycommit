#include "utils.h"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>

std::ostream& operator<<(std::ostream& os, timespec t)
{
    char buf[32];
    struct tm tm;
    gmtime_r(&t.tv_sec, &tm);
    strftime(buf, 21, "%Y-%m-%dT%H:%M:%S", &tm);
    os << buf;
    return os;
}

void printMode(int mode)
{

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

    ret += ((mode & __S_IFDIR) ? "d" : "-");
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
