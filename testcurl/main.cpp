#include <QCoreApplication>

void test_non_qt();
void test_qt(QObject* parent);

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

#if (0) //done
    test_non_qt();
#else
    test_qt(&a);
#endif

    return a.exec();
}
