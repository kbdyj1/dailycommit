#define _GNU_SOURCE
#include <utmp.h>
#include <utmpx.h>
#include <stdio.h>
#include "utils.h"

namespace { //=================================================================

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

} //namespace =================================================================

void test_ch_35()
{
    test();
}
