#include <QCoreApplication>

namespace {  //================================================================

} //===========================================================================

void test_qvarlengtharray();
void test_exec(QObject* parent);
void test_qbytearray();
void test_ownership();
void test_implicitsharing();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

#if (0) //done
    test_qvarlengtharray();
    test_exec(&a);
    test_qbytearray();
    test_ownership();
#endif

    test_implicitsharing();

    return a.exec();
}
