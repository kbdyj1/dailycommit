#include <QCoreApplication>
#include <iostream>

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
void test_atomic();
void test_qsettings();
void test_rreference();
void test_promise();
void test_sharedMemory();
void test_typeTraits();
void test_functional();
void test_array();
void test_forwarding();

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
    //test_locale();
    //test_atomic();
    //test_qsettings();
    //test_rreference();
    //test_promise();
    //test_sharedMemory();
    //test_typeTraits();
    //test_functional();
    //test_array();
    test_forwarding();
}

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	
    testConsole();

    std::cout.flush();

	return app.exec();
}
