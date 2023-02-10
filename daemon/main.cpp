#include <iostream>
#include <syslog.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
#include "startDaemon.h"

using namespace std;

#define SLEEP_INTERVAL_SEC  10

#define LOG_FILENAME    "/tmp/sd.log"
#define CONFIG_FILENAME "/tmp/sd.conf"

void sig_hup_handler(int sig);

void openLog(const char* filename)
{
    fprintf(stdout, "openLog(%s)\n", filename);
}

void closeLog(const char* filename)
{
    fprintf(stdout, "closeLog(%s)\n", filename);
}

void readConfig(const char* filename)
{
    fprintf(stdout, "readConfig(%s)\n", filename);
}

static volatile sig_atomic_t sig_hup_received = 0;

void sig_hup_handler(int sig)
{
    sig_hup_received = 1;
}

int main()
{
    int count = 0;
    int remain;
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = sig_hup_handler;
     if (-1 == sigaction(SIGHUP, &sa, NULL)) {
        fprintf(stderr, "sigaction(): failed.\n");
        return -1;
    }

    if (-1 == startDaemon(0)){
        fprintf(stderr, "startDaemon(): failed.\n");
        return -2;
    }

    openLog(LOG_FILENAME);
    readConfig(CONFIG_FILENAME);

    remain = SLEEP_INTERVAL_SEC;

    for ( ;; ) {
        remain = sleep(remain);

        if (sig_hup_received) {
            closeLog(LOG_FILENAME);
            openLog(LOG_FILENAME);
            readConfig(CONFIG_FILENAME);
            sig_hup_received = 0;
        }

        if (remain == 0) {
            count++;
            fprintf(stdout, "Main: %d", count);
            remain = SLEEP_INTERVAL_SEC;
        }
    }

    return 0;
}
