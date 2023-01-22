#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>

#include "Connection.h"

void startServer()
{
    struct sockaddr_un name;
    int down_flag = 0;
    int ret;
    int connection_socket;
    int data_socket;
    int result;
    char buffer[BUFFER_SIZE];

    // Create local socket

    connection_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (-1 == connection_socket) {
        fprintf(stderr, "socket failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    memset(&name, 0, sizeof(name));

    // Bind socket to socket name

    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, CONNECTION_KEY, sizeof(name.sun_path) - 1);

    ret = bind(connection_socket, (const struct sockaddr*)&name, sizeof(name));
    if (-1 == ret) {
        fprintf(stderr, "bind failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Prepare for accepting connections.

    ret = listen(connection_socket, 20);
    if (-1 == ret) {
        fprintf(stderr, "listen failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    for ( ;; ) {
        data_socket = accept(connection_socket, NULL, NULL);
        if (-1 == data_socket) {
            fprintf(stderr, "accept failed: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        for ( ;; ) {
            ret = read(data_socket, buffer, sizeof(buffer));
            if (-1 == ret) {
                fprintf(stderr, "read failed: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }

            // Make 0-terminated

            buffer[sizeof(buffer) - 1] = 0;

            // Handle command

            if (!strncmp(buffer, "DOWN", sizeof(buffer))) {
                down_flag = 1;
                break;
            }
            if (!strncmp(buffer, "END", sizeof(buffer))) {
                break;
            }

            // Add received command

            result += atoi(buffer);
        }

        // Send result

        sprintf(buffer, "%d\n", result);
        ret = write(data_socket, buffer, sizeof(buffer));
        if (-1 == ret) {
            fprintf(stderr, "write failed: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        result = 0;
    }

    close(data_socket);

    unlink(CONNECTION_KEY);

    exit(EXIT_SUCCESS);
}
