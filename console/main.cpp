#include <QCoreApplication>

void testStringView();
void testKDBindings();
void testQtContinaer();

void testConsole()
{
    //testKDBindings();
    //testStringView();
    testQtContinaer();
}

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	
    testConsole();

	return app.exec();
}
