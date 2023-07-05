#ifndef BIG2_H
#define BIG2_H

#include <string>

class Big;

class Big2
{
public:
    Big2();
    ~Big2();

    void write(const std::string& message);

private:
    Big* impl;
};

#endif // BIG2_H
