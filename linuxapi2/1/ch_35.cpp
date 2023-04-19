#define _GNU_SOURCE
#include <utmp.h>
#include <utmpx.h>
#include <paths.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
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

namespace _4 { // update login, logout

void test(int argc, const char* argv[])
{
    if (argc < 2) {
        fprintf(stderr, "usage: %s username [sleep-time]\n");
        exit(EXIT_FAILURE);
    }

    struct utmpx ut;
    char* dev;

    memset(&ut, 0, sizeof(struct utmpx));
    ut.ut_type = USER_PROCESS;
    strncpy(ut.ut_user, argv[1], sizeof(ut.ut_user));
    if (-1 == time((time_t*)&ut.ut_tv.tv_sec)) {
        errorExit("time() error.\n");
    }

    ut.ut_pid = getpid();

    dev = ttyname(STDIN_FILENO);
    if (NULL == dev) {
        errorExit("ttyname(STDIN_FILENO) error.\n");
    }

    if (strlen(dev) <= 8) {
        fprintf(stderr, "Terminal name is too short: %s\n", dev);
        exit(EXIT_FAILURE);
    }

    strncpy(ut.ut_line, dev+5, sizeof(ut.ut_line));
    strncpy(ut.ut_id, dev+8, sizeof(ut.ut_id));

    printf("Creating login entries in utmp and wtmp\n");
    printf("        using pid %ld, line %.*s, id %.*s\n",
           (long)ut.ut_pid,
           (int)sizeof(ut.ut_line), ut.ut_line,
           (int)sizeof(ut.ut_id), ut.ut_id);

    setutxent();

    if (NULL == pututxline(&ut)) {
        errorExit("pututxline(&ut) error.\n");
        exit(EXIT_FAILURE);
    }

    updwtmpx(_PATH_WTMP, &ut);

    sleep((2 < argc) ? atoi(argv[2]) : 15);

    // logout

    ut.ut_type = DEAD_PROCESS;
    time((time_t*)&ut.ut_tv.tv_sec);
    memset(&ut.ut_user, 0, sizeof(ut.ut_user));

    printf("Creating logout entries in utmp and wtmp\n");

    setutxent();

    if (NULL == pututxline(&ut)) {
        errorExit("pututxline(&ut) error.\n");
    }

    updwtmpx(_PATH_WTMP, &ut);
    exit(EXIT_SUCCESS);
}

} //_4 --------------------------------------------------------------

namespace _5 { //lastlog

void test(int argc, const char* argv[])
{
    if (1 < argc && strcmp(argv[1], "--help") == 0) {
        fprintf(stdout, "usage: %s [username...]\n", argv[0]);
        exit(EXIT_SUCCESS);
    }

    int fd = open(_PATH_LASTLOG, O_RDONLY);
    if (-1 == fd) {
        fprintf(stderr, "open(%s, O_RDONLY) error.\n", _PATH_LASTLOG);
        exit(EXIT_FAILURE);
    }

    struct lastlog llog;

    for (int j=1; j<argc; j++) {
        uid_t uid = atoi(argv[j]);
        if (-1 == uid) {
            fprintf(stderr, "No such user: %s\n", argv[j]);
            continue;
        }

        if (-1 == lseek(fd, uid * sizeof(struct lastlog), SEEK_SET)) {
            fprintf(stderr, "lseek() error.\n");
            exit(EXIT_FAILURE);
        }

        if (0 >= read(fd, &llog, sizeof(struct lastlog))) {
            fprintf(stderr, "read() error.\n");
            continue;
        }

        printf("%-8.8s %-6.6s %-20.20s %s\n", argv[j], llog.ll_line, llog.ll_host, ctime((time_t*)&llog.ll_time));
    }
}

} //_5 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_35(int argc, const char* argv[])
{
    (void)argc;
    (void)argv;

#if (0) //done
    _1::test();
    _2::test(argc, argv);
    _3::test();
    _4::test(argc, argv);
#endif

    _5::test(argc, argv);
}
