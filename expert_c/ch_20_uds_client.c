#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "ch_20_calc_uds_define.h"

//=============================================================================
//  Unix Socket Stream
//=============================================================================

static void stream_client_loop(int sd)
{
    //TODO
}

int uds_client_main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    char sock_file[] = "/tmp/calc_svc.sock";

    int sd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (-1 == sd) {
        fprintf(stderr, "socket(): %s\n", strerror(errno));
        exit(1);
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, sock_file, strlen(addr.sun_path)-1);

    int result = connect(sd, (struct sockaddr*)&addr, sizeof(addr));
    if (-1 == result) {
        close(sd);
        fprintf(stderr, "connect(): %s\n", strerror(errno));
        exit(2);
    }

    stream_client_loop(sd);

    return 0;
}

//=============================================================================
//  Unix Socket Datagram
//=============================================================================

static void datagram_client_loop(int sd)
{
    //TODO
}

int uds_datagram_client_main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    char server_sock_file[] = CALC_SERVICE_UDS_SERVER_KEY;
    char client_sock_file[] = CALC_SERVICE_UDS_CLIENT_KEY;

    int sd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (-1 == sd) {
        fprintf(stderr, "socket(): %s\n", strerror(errno));
        exit(1);
    }

    unlink(client_sock_file);

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, client_sock_file, strlen(addr.sun_path)-1);

    int result = bind(sd, (struct sockaddr*)&addr, sizeof(addr));
    if (-1 == result) {
        close(sd);
        fprintf(stderr, "bind(): %s\n", strerror(errno));
        exit(2);
    }

    strncpy(addr.sun_path, server_sock_file, strlen(addr.sun_path)-1);
    result = connect(sd, (struct sockaddr*)&addr, sizeof(addr));
    if (-1 == result) {
        close(sd);
        fprintf(stderr, "connect() : %s\n", strerror(errno));
        exit(3);
    }

    datagram_client_loop(sd);

    return 0;
}
