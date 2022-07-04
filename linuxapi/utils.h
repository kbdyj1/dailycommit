#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <unistd.h>
#include <iostream>

std::ostream& operator<<(std::ostream& os, timespec t);

void printOpenFail(const std::string& filename);
void printOpenSucceeded(const std::string& filename);
void printErrno(int no);
void catFile(const std::string& filename);

std::string modeString(int mode);
std::string inodeAttrString(int attr);

#define TAB() \
    std::cout << "\t"

#define PRINT_DEF(NAME) \
    std::cout << #NAME << ": " << NAME << "\n"

#define PRINT_DEF2(NAME, DESC) \
    std::cout << #NAME << ": " << NAME << " (" << DESC << ")\n"

#define PRINT_SYSCONF(NAME) \
    std::cout << "sysconf(" << #NAME << "): " << sysconf(NAME) << "\n"

#endif // UTILS_H
