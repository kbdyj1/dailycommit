#include <QCoreApplication>

namespace {  //================================================================

} //===========================================================================

void test_qvarlengtharray();
void test_exec(QObject* parent);
void test_qbytearray();
void test_ownership();
void test_implicitsharing();
void test_json(const char* filename);
void test_datetime();
void test_bytearray(int argc, char* argv[]);
void test_network(QObject* parent);
void test_fileinfo(const char* filename);
void test_ioprio();
void test_cbor();
void test_cache();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    test_ioprio();

#if (0) //done
    test_qvarlengtharray();
    test_exec(&a);
    test_qbytearray();
    test_ownership();
    test_implicitsharing();
    if (1 < argc) {
        test_json(argv[1]);
    }
    test_bytearray(argc, argv);
    test_datetime();
    if (1 < argc) {
        test_fileinfo(argv[1]);
    }
    test_network(&a);
    test_cbor();
#endif

    test_cache();

    return a.exec();
}
