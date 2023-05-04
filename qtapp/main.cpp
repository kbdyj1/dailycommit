#include <QCoreApplication>

namespace { //=================================================================

} //===========================================================================

void test_atomic();
void test_01_ServerClientCommunication();
void test_process(QObject* parent);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

#if (0) //done
    test_atomic();
    test_01_ServerClientCommunication();
#endif

    test_process(&a);

    return a.exec();
}
