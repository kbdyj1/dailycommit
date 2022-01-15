#include <QCoreApplication>

void testStringView();
void testKDBindings();

void testConsole()
{
    testKDBindings();
    //testStringView();
}

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	
    testConsole();

	return app.exec();
}
