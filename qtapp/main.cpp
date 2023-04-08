#include <QCoreApplication>

namespace { //=================================================================

} //===========================================================================

void test_atomic();
void test_01_ServerClientCommunication();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

#if (0) //done
    test_atomic();
#endif
    test_01_ServerClientCommunication();

    return a.exec();
}
