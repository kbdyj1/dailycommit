#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <unistd.h>

void printOpenFail(const std::string& filename);
void printOpenSucceeded(const std::string& filename);
void catFile(const std::string& filename);

#define PRINT_DEF(NAME) \
    std::cout << #NAME << ": " << NAME << "\n"

#define PRINT_SYSCONF(NAME) \
    std::cout << "sysconf(" << #NAME << "): " << sysconf(NAME) << "\n"

#endif // UTILS_H
