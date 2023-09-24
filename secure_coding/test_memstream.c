#include <stdio.h>
#include <stdlib.h>

static void test()
{
    char* buf;
    size_t size;

    FILE* fp = open_memstream(&buf, &size);
    if (NULL == fp) {
        fprintf(stderr, "open_memstream return NULL.\n");
    } else {
        fprintf(fp, "Hello, Qt%d", 6);
        fflush(fp);
        printf("buf: '%s', size: %zu\n", buf, size);

        fprintf(fp, "Secure Coding.\n");
        fclose(fp);
        printf("buf: '%s', size: %zu\n", buf, size);

        free(buf);
    }
}

void test_memstream()
{
    test();
}
