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
void test_future(QObject* parent);
void test_file(int argc, char *argv[]);
void test_eventloop(QCoreApplication* app);
void test_image(const QStringList& filename);
void test_thread(QObject* parent);
void test_metaobject();
void test_list();
void test_latin1string();
void test_stringview();
void test_locale();
void test_dlt();
void test_user(int argc, char* argv[]);
void test_char();
void test_qpointer();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

#if (0) //done
    test_ioprio();

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
    test_cache();
    test_future(&a);
    test_file(argc, argv);
    test_eventloop(&a);
    if (1 < argc) {
        QStringList filenames;
        for (int i=1; i<argc; i++)
            filenames.push_back(argv[i]);

        test_image(filenames);
    }
    test_thread(&a);
    test_metaobject();
    test_list();
    test_latin1string();
    test_stringview();
    test_locale();
    test_dlt();
    test_user(argc, argv);
    test_char();
#endif

    test_qpointer();

    return a.exec();
}
