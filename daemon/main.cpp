#include <iostream>
#include <syslog.h>
#include <sys/stat.h>
#include <signal.h>
#include "startDaemon.h"

using namespace std;

#define SLEEP_INTERVAL_SEC  10

int main()
{
    int count = 0;
    int remain;
    struct sigaction sa;

    startDaemon(0);

    return 0;
}
