#define GNU_SOURCE

#include <unistd.h>

#include <sys/syscall.h>    //!POSIX

#include <string.h>

static void test_syscall()
{
    char message[] = "Hello, Qt6!\n";
    size_t len = strlen(message);

    syscall(__NR_write, 1, message, len);
}

void test_ch_11()
{
    test_syscall();
}
