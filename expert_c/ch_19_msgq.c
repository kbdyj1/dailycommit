#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>
#include <errno.h>

#define MSG_Q_KEY   "/mq0"

void test_ch_19_msg_q()
{
    int child = fork();

    if (-1 == child) {
        fprintf(stderr, "fork error. %s\n", strerror(errno));
        exit(1);
    }
    if (0 == child) {
        sleep(1);

        mqd_t mq = mq_open(MSG_Q_KEY, O_WRONLY);

        char str[] = "Hello, Qt!";
        fprintf(stdout, "CHILD: waiting for 2 seconds...\n");
        sleep(2);
        fprintf(stdout, "CHILD: writing to parent...\n");

        mq_send(mq, str, strlen(str)+1, 0);
        mq_close(mq);
    } else {

        struct mq_attr attr;
        attr.mq_flags = 0;
        attr.mq_maxmsg = 10;
        attr.mq_msgsize = 32;
        attr.mq_curmsgs = 0;

        mqd_t mq = mq_open(MSG_Q_KEY, O_RDONLY | O_CREAT, 0644, &attr);

        char buf[32];
        fprintf(stdout, "PARENT: reading from child...\n");
        int read_bytes = mq_receive(mq, buf, 32, NULL);
        fprintf(stdout, "PARENT: received. [%s]\n", buf);

        mq_close(mq);
        mq_unlink(MSG_Q_KEY);
    }
}
