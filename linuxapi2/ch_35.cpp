#define _GNU_SOURCE
#include <utmp.h>
#include <utmpx.h>
#include <paths.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "utils.h"

namespace { //=================================================================

namespace _1 {

void test()
{
    utmp u;
    utmpx x;
    getutmpx(&u, &x);

    printf("type: %d\n", x.ut_type);
    printf("pid: %ld\n", (long)x.ut_pid);
    printf("terminal device: %s\n", x.ut_line);
    const char* id = x.ut_id;
    printf("id: %c%c%c%c\n", id[0], id[1], id[2], id[3]);
    printf("user: %s\n", x.ut_user);
    printf("host: %s\n", x.ut_host);
    printf("termination: %d, exit: %d\n", x.ut_exit.e_termination, x.ut_exit.e_exit);
    printf("session: %d\n", x.ut_session);
}
} //_1 --------------------------------------------------------------

namespace _2 {

void test(int argc, const char* argv[])
{
    struct utmpx* ut;

    if (1 < argc)
        if (-1 == utmpxname(argv[1]))
            errorExit("utmpxname() error.\n");

    setutxent();

    while ((ut = getutxent()) != NULL) {
        printf("%-8s ", ut->ut_user);
        printf("%-9.9s ",
               (ut->ut_type == EMPTY) ? "EMPTY" :
               (ut->ut_type == RUN_LVL) ? "RUN_LVL" :
               (ut->ut_type == BOOT_TIME) ? "BOOT_TIME" :
               (ut->ut_type == NEW_TIME) ? "NEW_TIME" :
               (ut->ut_type == OLD_TIME) ? "OLD_TIME" :
               (ut->ut_type == INIT_PROCESS) ? "INIT_PR" :
               (ut->ut_type == LOGIN_PROCESS) ? "LOGIN_PR" :
               (ut->ut_type == USER_PROCESS) ? "USER_PR" :
               (ut->ut_type == DEAD_PROCESS) ? "DEAD_PR" : "???"
        );
        printf("%5ld %-6.6s %-3.5s %-9.9s ", (long)ut->ut_pid, ut->ut_line, ut->ut_id, ut->ut_host);
        printf("%s", ctime((time_t*)&(ut->ut_tv.tv_sec)));
    }
    endutxent();
    exit(EXIT_SUCCESS);
}

} //_2 --------------------------------------------------------------

namespace _3 {

void test()
{
    char* p = getlogin();
    printf("login user: %s\n", p);

    char* tty = ttyname(STDIN_FILENO);
    printf("ttyname: %s\n", tty);

    char* logname = getenv("LOGNAME");
    printf("$LOGNAME (non-safe): %s\n", logname);
}

} //_3 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_35(int argc, const char* argv[])
{
    (void)argc;
    (void)argv;

#if (0) //done
    _1::test();
    _2::test(argc, argv);
#endif

    _3::test();
}
