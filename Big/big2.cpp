#include "big2.h"
#include "big.h"

Big2::Big2() : impl(new Big)
{}

Big2::~Big2()
{
    delete impl;
}

void Big2::write(const std::string &message)
{
    impl->write(QString::fromStdString(message));
}
