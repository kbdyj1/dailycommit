#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/time.h>

#include "utils.h"

namespace { //=================================================================

namespace _1 {

void handler(int sig)
{
    printf("signal caughted. %d (%s)\n", sig, strsignal(sig));
}

void test()
{
    signal(SIGTSTP, handler);

    fprintf(stdout, "test getrusage()...\n");
    for (int i=0; i<100; i++) {
        struct timeval time;

        gettimeofday(&time, NULL);

        usleep(1);
    }
    fprintf(stdout, "pause()...\n");
    fflush(stdout);

    pause();

    rusage usage;

    if (-1 == getrusage(RUSAGE_SELF, &usage)) {
        errorExit("getrusage() error.\n");
    }
    printf("CPU user: %f\n", timeval2double(&usage.ru_utime));
    printf("CPU sys: %f\n", timeval2double(&usage.ru_stime));
    printf("RSS\n");
    printf("\tMAX: %ldK\n", usage.ru_maxrss);
    printf("\t IX: %ldK\n", usage.ru_ixrss);
    printf("\t ID: %ldK\n", usage.ru_idrss);
    printf("\t IS: %ldK\n", usage.ru_isrss);
    printf("Min Fault: %ld\n", usage.ru_minflt);
    printf("Maj Fault: %ld\n", usage.ru_majflt);
    printf("inblock: %ld\n", usage.ru_inblock);
    printf("oublock: %ld\n", usage.ru_oublock);
    printf("self yield(ru_nvcsw): %ld\n", usage.ru_nvcsw);
    printf("context switch(nivcsw): %ld\n", usage.ru_nivcsw);
}

} //_1 --------------------------------------------------------------

namespace _2 {

int print_rlimit(const char* msg, int resource)
{
    struct rlimit lim;

    if (-1 == getrlimit(resource, &lim))
        return -1;

    printf("%s soft=", msg);

    if (RLIM_INFINITY == lim.rlim_cur)
        printf("Infinite");
#ifdef RLIM_SAVED_CUR
    else if (RLIM_SAVED_CUR == lim.rlim_cur)
        printf("Unrepresentable");
#endif
    else
        printf("%lld", (long long)lim.rlim_cur);

    printf("; hard=");

    if (RLIM_INFINITY == lim.rlim_max)
        printf("Infinite");
#ifdef RLIM_SAVED_MAX
    else if (RLIM_SAVED_MAX == lim.rlim_max)
        printf("Unrepresentable");
#endif
    else
        printf("%lld", (long long)lim.rlim_max);

    printf("\n");

    return 0;
}

void test_rlimit_nproc(int argc, const char* argv[])
{
    struct rlimit lim;
    int j;
    pid_t child;

    if (argc < 2 || argc > 3) {
        printf("Usage: %s soft-limit [hard-limit]\n");
        exit(EXIT_FAILURE);
    }

    print_rlimit("Initial maximul process limits: ", RLIMIT_NPROC);

    lim.rlim_cur = (argv[1][0] == 'i') ? RLIM_INFINITY : atoi(argv[1]);
    lim.rlim_max = (argc == 2) ? lim.rlim_cur : (argv[2][0] == 'i') ? RLIM_INFINITY : atoi(argv[2]);

    if (-1 == setrlimit(RLIMIT_NPROC, &lim))
        errorExit("setrlimit() error.\n");

    print_rlimit("New maximul process limits: ", RLIMIT_NPROC);

    for (j=1; ; j++) {
        switch (child = fork()) {
        case -1:
            errorExit("fork() error.\n");

        case 0:
            _exit(EXIT_SUCCESS);

        default:
            printf("Child %d (PID=%ld) started.\n", j, (long)child);
            break;
        }
    }
}

#define PRINT_RLIMIT(resource)  print_rlimit(#resource, resource)

void test_rlimit_all()
{
    PRINT_RLIMIT(RLIMIT_AS);
    PRINT_RLIMIT(RLIMIT_CORE);
    PRINT_RLIMIT(RLIMIT_CPU);
    PRINT_RLIMIT(RLIMIT_DATA);
    PRINT_RLIMIT(RLIMIT_FSIZE);
    PRINT_RLIMIT(RLIMIT_MEMLOCK);
    PRINT_RLIMIT(RLIMIT_MSGQUEUE);
    PRINT_RLIMIT(RLIMIT_NICE);
    PRINT_RLIMIT(RLIMIT_NOFILE);
    PRINT_RLIMIT(RLIMIT_NPROC);
    PRINT_RLIMIT(RLIMIT_RSS);
    PRINT_RLIMIT(RLIMIT_RTPRIO);
    PRINT_RLIMIT(RLIMIT_RTTIME);
    PRINT_RLIMIT(RLIMIT_SIGPENDING);
    PRINT_RLIMIT(RLIMIT_STACK);
}

void test(int argc, const char* argv[])
{
    //test_rlimit_nproc(argc, argv);

    test_rlimit_all();
}

} //_2 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_31(int argc, const char* argv[])
{
    (void)argc;
    (void)argv;

#if (0) //done
    _1::test();
#endif

    _2::test(argc, argv);
}
