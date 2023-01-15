#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>

//=============================================================================

#define SHM_KEY     "/shm0"
#define SHM_SIZE    16

#define SHM_SERVER

#if defined(SHM_SERVER)
#   define SHM_OPEN_OPTION  (O_CREAT | O_RDWR)
#   define MMAP_PROT        PROT_WRITE
#else
#   define SHM_OPEN_OPTION  O_RDONLY
#   define MMAP_PROT        PROT_READ
#endif

static void _1_test()
{
    int fd = shm_open(SHM_KEY,  SHM_OPEN_OPTION, 0600);
    if (fd < 0) {
        fprintf(stderr, "failed to create shared memory: %s\n", strerror(errno));
        return;
    }

    fprintf(stdout, "shared memory fd: %d\n", fd);

#if defined(SHM_SERVER)
    if (ftruncate(fd, SHM_SIZE*sizeof(char)) < 0) {
        fprintf(stderr, "truncation failed: %s\n", strerror(errno));
        return;
    }
    fprintf(stdout, "the memory region is truncated.\n");
#endif

    void* map = mmap(0, SHM_SIZE, MMAP_PROT, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        fprintf(stderr, "mapping failed: %s\n", strerror(errno));
        return;
    }

    char* str = (char*)map;
#if defined(SHM_SERVER)
    str[0] = 'N';
    str[1] = 'e';
    str[2] = 'w';
    str[3] = ' ';
    str[4] = 'J';
    str[5] = 'e';
    str[6] = 'a';
    str[7] = 'n';
    str[8] = 's';
    str[9] = '\n';
    str[10]= '\0';

    fprintf(stdout, "data is written to the shared memory.\n");
#else
    fprintf(stdout, "the contents of shared memory: %s\n", str);
#endif

    if (munmap(str, SHM_SIZE) < 0) {
        fprintf(stderr, "unmapping failed: %s\n", strerror(errno));
        return;
    }

    if (close(fd) < 0) {
        fprintf(stderr, "closing shared memory failed: %s\n", strerror(errno));
        return;
    }

#if !defined(SHM_SERVER)
    if (shm_unlink(SHM_KEY) < 0) {
        fprintf(stderr, "unlink shared memory failed: %s\n", strerror(errno));
        return;
    }
#endif
}

//=============================================================================

void test_ch_17_shm()
{
    _1_test();
}
