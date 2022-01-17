#include <QCoreApplication>

void testStringView();
void testKDBindings();
void testQtContinaer();
void testNetwork();

void testConsole()
{
    //testKDBindings();
    //testStringView();
    //testQtContinaer();
    testNetwork();
}

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	
    testConsole();

	return app.exec();
}
