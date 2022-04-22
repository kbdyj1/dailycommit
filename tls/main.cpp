#include <QCoreApplication>
#include <iostream>

void test_caesar_cipher();
void test_powm();
void test_primitiveRoot();
void test_gmputil();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //test_caesar_cipher();
    //test_powm();
    //test_primitiveRoot();
    test_gmputil();

    std::cout.flush();

    return a.exec();
}
