#include <QCoreApplication>

namespace {  //================================================================

} //===========================================================================

void test_qvarlengtharray();
void test_exec(QObject* parent);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
#if (0) //done
        test_qvarlengtharray();
#endif

    test_exec(&a);

    return a.exec();
}
