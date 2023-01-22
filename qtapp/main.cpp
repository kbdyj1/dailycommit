#include <QCoreApplication>

void startServer();
int startClient(int argc, char** argv);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (argc < 2) {
        fprintf(stdout, "nothing...\n");
    } else {
        if (0 == strncmp(argv[1], "s", 1)) {
            startServer();
        } else if (0 == strncmp(argv[1], "c", 1)) {
            startClient(argc, argv);
        }
    }

    return a.exec();
}
