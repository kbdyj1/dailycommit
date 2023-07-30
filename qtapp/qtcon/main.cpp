#include <QCoreApplication>

namespace {  //================================================================

} //===========================================================================

void test_qvarlengtharray();
void test_exec(QObject* parent);
void test_qbytearray();
void test_ownership();
void test_implicitsharing();
void test_jason(const char* filename);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

#if (0) //done
    test_qvarlengtharray();
    test_exec(&a);
    test_qbytearray();
    test_ownership();
    test_implicitsharing();
#endif

    if (1 < argc) {
        test_jason(argv[1]);
    }

    return a.exec();
}
