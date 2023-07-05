#include <iostream>
#include <memory>
#include "big2.h"

using namespace std;

int main()
{
    auto big = std::make_shared<Big2>();

    big->write("Hello, Qt6");

    return 0;
}
