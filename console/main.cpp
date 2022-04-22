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
void test_emplace_back();
void test_lambda();
void test_concurrency();
void test_hash();
void test_qt_hash();

void testConsole()
{
    test_qt_hash();
}

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	
    testConsole();

    std::cout.flush();

	return app.exec();
}
