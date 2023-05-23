#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "utils.h"

namespace { //=================================================================

namespace _1 {

void test(int argc, const char* argv[])
{
    mq_attr*pattr = NULL;
    mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 50;
    attr.mq_msgsize = 2048;
    attr.mq_curmsgs = 0;
    int flags = O_RDWR;

    if (2 <= argc) {
        attr.mq_maxmsg = atoi(argv[1]);
        pattr = &attr;
    }
    if (3 <= argc) {
        attr.mq_msgsize = atoi(argv[2]);
    }
    if (4 <= argc) {
        if (argv[3][0] == 'c') {
            flags |= O_CREAT;
        } else if (argv[3][0] == 'x') {
            flags |= O_CREAT;
            flags |= O_EXCL;
        }
    }
    mode_t perms = S_IRUSR | S_IWUSR;

    mqd_t id = mq_open("/test", flags, perms, pattr);
    if (-1 == id) {
        fprintf(stderr, "errno: %s\n", strerror(errno));
        errorExit("mq_open() failed.\n");
    } else {
        printf("mq_open(): %d\n", id);
    }

    exit(EXIT_SUCCESS);
}

} //_1 --------------------------------------------------------------

namespace _2 {

void test(int argc, const char* argv[])
{
    mqd_t id = mq_open("/test", O_RDONLY);
    if (-1 == id) {
        errorExit("mq_open() failed.\n");
    }

    mq_attr attr;
    if (-1 == mq_getattr(id, &attr)) {
        errorExit("mq_getattr() failed.\n");
    }

    printf("mq_flags: %d\n", attr.mq_flags);
    printf("mq_maxmsg: %d\n", attr.mq_maxmsg);
    printf("mq_msgsize: %d\n", attr.mq_msgsize);
    printf("mq_curmsgs: %d\n", attr.mq_curmsgs);

    exit(EXIT_SUCCESS);
}

} //_2 --------------------------------------------------------------

} //namespace =================================================================

void exec_ch_15(int argc, const char* argv[])
{
#if (0) //done
    _1::test(argc, argv);
#endif

    _2::test(argc, argv);
}
