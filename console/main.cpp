#include <QCoreApplication>

void testStringView();
void testKDBindings();
void testQtContinaer();
void testNetwork();
void test_cxx20_features();
void test_cxx17_features();
void test_variant();
void test_concurrent();

void testConsole()
{
    //testKDBindings();
    //testStringView();
    //testQtContinaer();
    //testNetwork();
    //test_cxx20_features();
    //test_concurrent();
    test_cxx17_features();
}

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	
    testConsole();

	return app.exec();
}
