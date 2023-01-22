#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>

#include "Connection.h"

int startClient(int argc, char** argv)
{
    struct sockaddr_un addr;
    int ret;
    int data_socket;
    char buffer[BUFFER_SIZE];

    // Create local socket

    data_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (-1 == data_socket) {
        fprintf(stderr, "socket failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(addr));

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, CONNECTION_KEY, sizeof(addr.sun_path) - 1);

    ret = connect(data_socket, (const struct sockaddr*)&addr, sizeof(addr));
    if (-1 == ret) {
        fprintf(stderr, "connect failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Send arguments

    for (int i=2; i<argc; i++) {
        ret = write(data_socket, argv[1], strlen(argv[1]) + 1);
        fprintf(stderr, "write failed: %s\n", strerror(errno));
        break;
    }

    // Request result

    strcpy(buffer, "END");
    ret = write(data_socket, buffer, strlen(buffer) + 1);
    if (-1 == ret) {
        fprintf(stderr, "write failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Receive result

    ret = read(data_socket, buffer, sizeof(buffer));
    if (-1 == ret) {
        fprintf(stderr, "read failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    buffer[sizeof(buffer) - 1] = 0;

    fprintf(stdout, "Result: %s\n", buffer);

    close(data_socket);

    exit(EXIT_SUCCESS);
}
