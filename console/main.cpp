#include <QCoreApplication>

void testStringView();
void testKDBindings();
void testQtContinaer();
void testNetwork();
void test_cxx20_features();
void test_cxx17_features();
void test_variant();
void test_concurrent();
void test_iodevice();
void test_sql();
void test_xml();
void test_json();
void test_locale();

void testConsole()
{
    //testKDBindings();
    //testStringView();
    //testQtContinaer();
    //testNetwork();
    //test_cxx20_features();
    //test_concurrent();
    //test_cxx17_features();
    //test_iodevice();
    //test_sql();
    //test_xml();
    //test_json();
    test_locale();
}

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	
    testConsole();

	return app.exec();
}
