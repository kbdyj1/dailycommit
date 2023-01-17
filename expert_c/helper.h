#ifndef HELPER_H
#define HELPER_H

#include <string.h>
#include <stdlib.h>

#define CHECK_RESULT(result, message, exitcode)                         \
    if (result < 0) {                                                   \
        fprintf(stderr, "%s failed: %s\n", message, strerror(errno));   \
        exit(exitcode);                                                 \
    }

#endif // HELPER_H
