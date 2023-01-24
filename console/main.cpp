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
void test_crtp();
void test_sfinae();
void test_duck_typing();
void test_implicitSharing();
void test_qcache();
void test_regexp();
void test_law_of_demeter();
void test_cbor();
void test_adl();

void testConsole()
{
    test_adl();
}

//-----------------------------------------------------------------------------
//  Meta Programming
//-----------------------------------------------------------------------------
void meta_test_binary();
void test_meta_ch2();
void test_traits();
void test_blob();

void test_meta_programming()
{
    //meta_test_binary();
    //test_meta_ch2();
    test_traits();
    //test_blob();
}

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	
    testConsole();
    //test_meta_programming();

    std::cout.flush();

	return app.exec();
}
