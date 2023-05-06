// CH.8 System V IPC
//
// message queue
// semaphore
// shared memory
//
// kernel option: CONFIG_SYSVIPC

#include <sys/stat.h>
#include <sys/msg.h>
#include "utils.h"

namespace { //=================================================================

namespace _1 {

//#define USE_EXCL
#define USE_PRIVATE

#ifndef USE_PRIVATE
#   define USE_FTOK
#   define FTOK_PATH   "/tmp/ftok.path"
#endif

key_t createMsgQ_key()
{
#if defined(USE_PRIVATE)
    return IPC_PRIVATE;
#endif

#if defined(USE_FTOK)
    return ftok(FTOK_PATH, 0);
#endif
}

void test()
{
    int flag = IPC_CREAT | S_IRUSR | S_IWUSR;
    key_t key = createMsgQ_key();

#if defined(USE_EXCL)
    flag |= IPC_EXCL;
#endif

    int id = msgget(key, flag);
    if (-1 == id) {
        errorExit("msgget() error.\n");
    } else {
        fprintf(stdout, "msgget(%ld): %d\n", (long)key, id);
    }

    msqid_ds msqds;

    msgctl(id, IPC_STAT, &msqds);

    printf("IPC_STAT(%d)\n", id);
    printf("\tuid: %d\n", msqds.msg_perm.uid);
    printf("\tgid: %d\n", msqds.msg_perm.gid);
    printf("\tcuid: %d\n", msqds.msg_perm.cuid);
    printf("\tcgid: %d\n", msqds.msg_perm.cgid);
    printf("\tmode: %d\n", msqds.msg_perm.mode);
    printf("\tseq: %d\n", msqds.msg_perm.__seq);

    printf("delete key(%ld)...", (long)key);
    if (-1 == msgctl(id, IPC_RMID, NULL)) {
        printf(" failed.\n");
    } else {
        printf(" done.\n");
    }
}

} //_1 --------------------------------------------------------------

} //namespace =================================================================

void exec_ch_08()
{
    _1::test();
}
