#include <pwd.h>
#include <grp.h>
#include <shadow.h>
#include <QDebug>
#include <string.h>
#include <unistd.h>

namespace { //=================================================================

void print_password(const passwd* pwd)
{
    if (pwd) {
        qDebug() << "pw_name: " << pwd->pw_name;
        qDebug() << "pw_passwd: " << pwd->pw_passwd;
        qDebug() << "pw_uid: " << pwd->pw_uid;
        qDebug() << "pw_gid: " << pwd->pw_gid;
        qDebug() << "pw_gecos: " << pwd->pw_gecos;  // General Electric Comprehensive OS
        qDebug() << "pw_dir: " << pwd->pw_dir;
        qDebug() << "pw_shell: " << pwd->pw_shell;
    }
}

void print_group(const group* grp)
{
    if (grp) {
        qDebug() << "gr_name: " << grp->gr_name;
        qDebug() << "gr_passwd: " << grp->gr_passwd;
        qDebug() << "gid: " << grp->gr_gid;
        for (auto** user = grp->gr_mem; *user != NULL; user++) {
            qDebug() << "\t" << *user;
        }
    }
}

namespace _1 {

void test(int argc, char* argv[])
{
    if (1 < argc) {
        const char* name = argv[1];
        auto* pwd = getpwnam(name);

        if (pwd == NULL) {
            if (0 != errno) {
                qDebug() << "[error] getpwnam(): " << strerror(errno);
            }
        }
        print_password(pwd);
    }
}

} //_1 --------------------------------------------------------------

namespace _2 {

void test(int argc, char* argv[])
{
    if (1 < argc) {
        uid_t id = atoi(argv[1]);
        auto* pwd = getpwuid(id);
        if (pwd == NULL) {
            if (0 != errno) {
                qDebug() << "[error] getpwnam(): " << strerror(errno);
            }
        }
        print_password(pwd);
    }
}

} //_2 --------------------------------------------------------------

namespace _3 {

void test(int argc, char* argv[])
{
    if (1 < argc) {
        const char* grp = argv[1];
        auto* group = getgrnam(grp);

        print_group(group);
    }
}

} //_3 --------------------------------------------------------------

namespace _4 {

void print_spwd(const spwd* sp)
{
    qDebug() << "sp_namp: " << sp->sp_namp;
    qDebug() << "sp_pwdp: " << sp->sp_pwdp;
    qDebug() << "sp_lstchg: " << sp->sp_lstchg;
    qDebug() << "sp_min: " << sp->sp_min;
    qDebug() << "sp_max: " << sp->sp_max;
    qDebug() << "sp_warn: " << sp->sp_warn;
    qDebug() << "sp_inact: " << sp->sp_inact;
    qDebug() << "sp_expire: " << sp->sp_expire;
    qDebug() << "sp_flag: " << sp->sp_flag;
    qDebug() << "\n";
}

void test(int argc, char* argv[])
{
    if (1 < argc) {
        const char* name = argv[1];
        auto* sp = getspnam(name);
        if (sp) {
            print_spwd(sp);
        }
    } else {
        setspent();
        spwd* sp = nullptr;
        while ((sp = getspent()) != NULL) {
            print_spwd(sp);
        }
        endspent();
    }
}

} //_4 --------------------------------------------------------------

namespace _5 {

void test(int argc, char* argv[])
{
    if (1 < argc) {
        const char* username = argv[1];

        passwd* pwd = getpwnam(username);
        spwd* spwd = getspnam(username);
        if (spwd == NULL && errno == EACCES) {
            fprintf(stderr, "no permission");
            return;
        }
        pwd->pw_passwd = spwd->sp_pwdp;

        char* input = getpass("enter passwd: ");    //legacy

        char* encrypted = crypt(input, pwd->pw_passwd);

        for (auto* p = input; *p != '\0'; p++)
            *p = '\0';

        //qDebug() << "crypt(" << input << ", " << pwd->pw_passwd << "): " << encrypted;

        if (encrypted == NULL) {
            fprintf(stderr, "crypt error");
            return;
        }

        auto authOk = strcmp(encrypted, pwd->pw_passwd) == 0;
        if (!authOk) {
            fprintf(stdout, "incorrect password.");
            fflush(stdout);

            return;
        }

        printf("successfully authenticated: uid=%ld\n", (long)pwd->pw_uid);
    }
}

} //_5 --------------------------------------------------------------

namespace _6 {

void test_getuid()
{
    qDebug() << "uid: " << getuid();
    qDebug() << "euid: " << geteuid();
    qDebug() << "gid: " << getgid();
    qDebug() << "egid: " << getegid();
}

void test_setuid(uid_t id)
{
    auto ret = setuid(id);
    if (0 != ret) {
        qDebug() << "setuid: " << strerror(errno);
    } else {
        test_getuid();
    }
}

void test_seteuid(uid_t id)
{
    auto ret = seteuid(id);
    if (0 != ret) {
        qDebug() << "seteuid: " << strerror(errno);
    } else {
        test_getuid();
    }
}

void test(int argc, char* argv[])
{
    if (argc <= 1)
        return;

    uid_t id = atoi(argv[1]);
    test_seteuid(id);
}

} //_6 --------------------------------------------------------------

} //===========================================================================

void test_user(int argc, char* argv[])
{
#if (0) //done
    _1::test(argc, argv);
    _2::test(argc, argv);
    _3::test(argc, argv);
    _4::test(argc, argv);
    _5::test(argc, argv);
#endif

    _6::test(argc, argv);
}
