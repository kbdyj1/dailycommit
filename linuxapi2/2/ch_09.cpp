#define __GNU_SOURCE
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "utils.h"

namespace { //=================================================================

namespace _1 {

const int MESSAGE_TYPE = 1;

struct message {
    long type;
    int len;
    char buf[64];
};

void test()
{
    int id = msgget(IPC_PRIVATE, IPC_CREAT | S_IWUSR | S_IRUSR);
    if (-1 == id) {
        errorExit("msgget() error.\n");
    }

    switch (fork()) {
    case -1:
        errorExit("fork() error.\n");

    case 0:
        {
            printf("msgrcv() call...\n");
            message msg;
            ssize_t size = msgrcv(id, &msg, sizeof(msg), MESSAGE_TYPE, 0);
            if (-1 == size) {
               errorExit("msgrcv() error.\n");
            } else {
               printf("msgrcv() %d bytes received: %s\n", size, msg.buf);
            }
            _exit(EXIT_SUCCESS);
        }
        break;

    default:
        {
            message msg;
            msg.type = MESSAGE_TYPE;
            const char* sendMessage = "Hello, Qt6!!!";
            msg.len = strlen(sendMessage);
            strncpy(msg.buf, sendMessage, msg.len);
            msg.buf[msg.len] = '\0';

            sleep(5);

            if (-1 == msgsnd(id, &msg, sizeof(msg), 0)) {
                errorExit("msgsnd() error.\n");
            } else {
                printf("msgsnd() success.\n");
                exit(EXIT_SUCCESS);
            }
        }
        break;
    }
}

} //_1 -------------------------------------------------------------

namespace _2 {

void test()
{
    msginfo info;

    msgctl(0, IPC_INFO, (msqid_ds*)&info);

    printf("msgmap: %d\n", info.msgmap);
    printf("msgmax: %d\n", info.msgmax);
    printf("msgmnb: %d\n", info.msgmnb);
    printf("msgmni: %d\n", info.msgmni);
    printf("msgseg: %d\n", info.msgseg);
    printf("msgsz : %d\n", info.msgssz);
    printf("msgtql: %d\n", info.msgtql);
    printf("msgpool:%d\n", info.msgpool);
}

} //_2 -------------------------------------------------------------

namespace _3 {

void test()
{
    msginfo info;
    int maxid = msgctl(0, MSG_INFO, (msqid_ds*)&info);
    if (-1 == maxid) {
        errorExit("msgctl(MSG_INFO) error.\n");
    } else {
        printf("maxind: %d\n\n", maxid);
    }

    printf("index    id    key    message\n");

    msqid_ds ds;
    int id;
    for (int i=0; i<maxid; i++) {
        id = msgctl(i, MSG_STAT, &ds);
        if (-1 == id) {
            if (errno != EINVAL && errno != EACCES) {
                errorExit("msgctl(MSG_STAT) error.\n");
            }
            continue;
        }
        printf("%4d %8d 0x%08lx %7ld\n", i, id, (unsigned long)ds.msg_perm.__key, (long)ds.msg_qnum);
    }
    exit(EXIT_SUCCESS);
}

} //_3 -------------------------------------------------------------

namespace _4 {

const int SERVER_KEY = 0x20230509;

const int RESP_MSG_SIZE = 8192;

struct ReqMsg {
    long type;
    int pid;
};

struct ResMsg {
    long type;
    char buffer[64];
};

void test()
{
    for (int i=0; i<4; i++) {
        switch (fork()) {
        case -1:
            errorExit("fork() error.\n");

        case 0:
            {
                int id = msgget(IPC_PRIVATE, S_IRUSR | S_IWUSR | IPC_CREAT);
                if (-1 == id) {
                    errorExit("[client] msgget() error.\n");
                }
                ReqMsg msg;
                msg.type = 1;
                msg.pid = getpid();
                if (-1 == msgsnd(id, &msg, sizeof(ReqMsg), 0)) {
                    errorExit("[client] msgsnd(ClientMsg) error.\n");
                }
            }
            break;

        default:
            {
                int id = msgget(IPC_PRIVATE, S_IRUSR | S_IWUSR | IPC_CREAT);
                if (-1 == id) {
                    errorExit("[server] msgget() error.\n");
                }

                int len;
                ReqMsg msg;
                for ( ;; ) {
                    len = msgrcv(id, &msg, sizeof(ReqMsg), 1, 0);
                }
            }
            break;
        }
    }
}

} //_4 -------------------------------------------------------------

} //namespace =================================================================

void exec_ch_09()
{
#if (0) //done
    _1::test();
    _2::test();
#endif

    _3::test();
}
