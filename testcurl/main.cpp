#include <QCoreApplication>

void test_non_qt();
void test_qt(QObject* parent);
void test_non_qt_class();

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

#if (1) //done
    test_non_qt_class();
#else
    test_qt(&a);
#endif

    return a.exec();
}
