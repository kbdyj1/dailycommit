#include <QCoreApplication>

namespace {  //================================================================

} //===========================================================================

void test_qvarlengtharray();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    {
        test_qvarlengtharray();
    }
    return a.exec();
}
