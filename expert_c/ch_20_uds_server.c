#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

#include "ch_20_calc_uds_define.h"

//=============================================================================
//  Unix Socket Stream
//=============================================================================

static void* client_handler(void* arg)
{
    return NULL;
}

static void accept_forever(int sd)
{
    while (1) {
        int client_sd = accept(sd, NULL, NULL);
        if (-1 == client_sd) {
            close(sd);
            fprintf(stderr, "accept(): %s\n", strerror(errno));
            exit(4);
        }

        pthread_t client_handler_thread;
        int* arg = (int*)malloc(sizeof(int));
        *arg = client_sd;
        int result = pthread_create(&client_handler_thread, NULL, &client_handler, arg);
        if (result) {
            close(client_sd);
            close(sd);
            free(arg);
            fprintf(stderr, "pthread_create(): %s\n", strerror(errno));

            exit(5);
        }
    }
}

int uds_server_main(int argc, char** argv)
{
    int sd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (-1 == sd) {
        fprintf(stderr, "socket(): %s\n", strerror(errno));
        exit(1);
    }

    char sock_file[] = CALC_SERVICE_UDS_SERVER_KEY;

    unlink(sock_file);

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, sock_file, sizeof(addr.sun_path) - 1);

    int result = bind(sd, (struct sockaddr*)&addr, sizeof(addr));
    if (-1 == result) {
        close(sd);
        fprintf(stderr, "bind(): %s\n", strerror(errno));
        exit(2);
    }

    int backlog_size = 10;

    result = listen(sd, backlog_size);
    if (-1 == result) {
        close(sd);
        fprintf(stderr, "listen(): %s\n", strerror(errno));
        exit(3);
    }

    accept_forever(sd);

    return 0;
}

//=============================================================================
//  Unix Socket Datagram
//=============================================================================

static void server_forever(int sd)
{
    char buffer[64];

    while (1) {
        struct sockaddr* addr = (struct sockaddr*)malloc(sizeof(struct sockaddr));
        socklen_t len = sizeof(struct sockaddr);
        int read_bytes = recvfrom(sd, buffer, sizeof(buffer), 0, addr, &len);
        if (-1 == read_bytes) {
            close(sd);
            fprintf(stderr, "could not read from datagram socket: %s\n", strerror(errno));
            exit(1);
        }

        //TODO
    }
}

int uds_datagram_server_main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    int sd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (-1 == sd) {
        fprintf(stderr, "socket(): %s\n", strerror(errno));
        exit(1);
    }

    char sock_file[] = CALC_SERVICE_UDS_SERVER_KEY;

    unlink(sock_file);

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, sock_file, strlen(addr.sun_path) - 1);

    int result = bind(sd, (struct sockaddr*)&addr, sizeof(addr));
    if (-1 == result) {
        close(sd);
        fprintf(stderr, "bind(): %s\n", strerror(errno));
        exit(2);
    }

    server_forever(sd);

    return 0;
}
