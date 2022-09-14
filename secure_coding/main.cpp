#include <iostream>
#include <cstring>

void test_ch_02();

void printProgramName(const char* name) {
    size_t len = strlen(name);
    char* program = new char[len+1];
    strncpy(program, name, len);
    program[len] = '\0';

    std::cout << "program: " << program << std::endl;
}

int main(int argc, char* argv[])
{
    if (0 < argc) {
        printProgramName(argv[0]);
    }
    test_ch_02();

    return 0;
}
