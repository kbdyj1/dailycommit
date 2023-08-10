#include <QCoreApplication>

namespace {  //================================================================

} //===========================================================================

void test_qvarlengtharray();
void test_exec(QObject* parent);
void test_qbytearray();
void test_ownership();
void test_implicitsharing();
void test_jason(const char* filename);
void test_datetime();
void test_bytearray(int argc, char* argv[]);
void test_network(QObject* parent);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

#if (0) //done
    test_qvarlengtharray();
    test_exec(&a);
    test_qbytearray();
    test_ownership();
    test_implicitsharing();
    if (1 < argc) {
        test_jason(argv[1]);
    }
    test_bytearray(argc, argv);
    test_datetime();
#endif

    test_network(&a);

    return a.exec();
}
