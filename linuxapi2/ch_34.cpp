#define _BSD_SOURCE
#define _XOPEN_SOURCE
#include <sys/capability.h>
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <shadow.h>
#include <sys/types.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

namespace { //=================================================================

// sudo apt-get install libcap-dev (sys/capability.h)

int modifyCap(int capability, cap_flag_value_t setting)
{
    cap_t caps;
    cap_value_t capList[1];

    caps = cap_get_proc();
    if (NULL == caps)
        return -1;

    capList[0] = capability;
    if (-1 == cap_set_flag(caps, CAP_EFFECTIVE, 1, capList, setting)) {
        cap_free(caps);
        return -1;
    }

    if (-1 == cap_set_proc(caps)) {
        cap_free(caps);
        return -1;
    }

    if (-1 == cap_free(caps)) {
        return -1;
    }

    return 0;
}

int raiseCap(int capability)
{
    return modifyCap(capability, CAP_SET);
}

int dropAllCaps()
{
    cap_t empty;
    int s;

    empty = cap_init();
    if (NULL == empty)
        return -1;

    s = cap_set_proc(empty);
    if (-1 == cap_free(empty))
        return -1;

    return s;
}

namespace _1 {

int test()
{
    char* username;
    char* password;
    char* encrypted;
    char* p;
    struct passwd* pwd;
    struct spwd* spwd;
    bool authOk;
    size_t len;
    long lnMax;

    lnMax = sysconf(_SC_LOGIN_NAME_MAX);
    if (-1 == lnMax)
        return 256;

    username = (char*)malloc(lnMax);
    if (NULL == username)
        errorExit("malloc() error.\n");

    printf("username: ");
    fflush(stdout);

    if (NULL == fgets(username, lnMax, stdin))
        exit(EXIT_FAILURE);

    len = strlen(username);
    if (username[len -1] == '\n')
        username[len -1] = '\0';

    pwd = getpwnam(username);
    if (NULL == pwd)
        errorExit("couldn't get password record.\n");

    if (-1 == raiseCap(CAP_DAC_READ_SEARCH))
        errorExit("raiseCap() error.\n");

    spwd = getspnam(username);
    if (NULL == spwd && errno == EACCES)
        errorExit("no permission to read shadow password file");

    if (-1 == dropAllCaps())
        errorExit("dropAllCaps() error.\n");

    if (NULL != spwd)
        pwd->pw_passwd = spwd->sp_pwdp;

    password = getpass("Password: ");

    encrypted = crypt(password, pwd->pw_passwd);
    for (p = password; *p != '\0'; )
        *p++ = '\0';

    if (NULL == encrypted)
        errorExit("crypt error.\n");

    authOk = strcmp(encrypted, pwd->pw_passwd) == 0;
    if (!authOk) {
        printf("incorrect password.\n");
        exit(EXIT_FAILURE);
    }

    printf("successfully authenticated: UID=%ld\n", (long)pwd->pw_uid);

    exit(EXIT_SUCCESS);
}

} //_1 --------------------------------------------------------------

} //namespace =================================================================

void test_ch_34()
{
    _1::test();
}
