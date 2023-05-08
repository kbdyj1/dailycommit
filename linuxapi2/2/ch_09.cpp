#include <sys/types.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
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

} //namespace =================================================================

void exec_ch_09()
{
#if (0) //done
    _1::test();
#endif

    _2::test();
}
