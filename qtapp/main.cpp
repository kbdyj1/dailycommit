#include <QCoreApplication>

namespace { //=================================================================

} //===========================================================================

void test_atomic();
void test_01_ServerClientCommunication();
void test_process(QObject* parent);
void test_semaphore();
void test_buffer(QObject* parent);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

#if (0) //done
    test_atomic();
    test_01_ServerClientCommunication();
    test_process(&a);
    test_semaphore();
#endif

    test_buffer(&a);

    return a.exec();
}
