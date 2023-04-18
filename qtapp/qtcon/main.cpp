#include <QCoreApplication>

namespace {  //================================================================

} //===========================================================================

void test_qvarlengtharray();
void test_exec(QObject* parent);
void test_qbytearray();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
#if (0) //done
        test_qvarlengtharray();
        test_exec(&a);
#endif

    test_qbytearray();

    return a.exec();
}
