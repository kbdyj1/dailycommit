#include "utils.h"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>

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
