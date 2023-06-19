#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include "utils.h"

namespace { //=================================================================

const char* baudString(int baud)
{
    switch (baud) {
    case B0:    return "hangup";
    case B50:   return "50";
    case B75:   return "75";
    case B110:  return "110";
    case B134:  return "134";
    case B150:  return "150";
    case B200:  return "200";
    case B300:  return "300";
    case B600:  return "600";
    case B1200: return "1200";
    case B1800: return "1800";
    case B2400: return "2400";
    case B4800: return "4800";
    case B9600: return "9600";
    case B19200:return "19200";
    case B38400:return "38400";
    default:
        return "Invalid";
    }
}

const char* bsDelayString(int delay)
{
    switch (delay)
    {
    case BS0: return "BS0";
    case BS1: return "BS1";
    default:
        return "Invalid";
    }
}

const char* crDelayString(int delay)
{
    switch (delay) {
    case CR0: return "CR0";
    case CR1: return "CR1";
    case CR2: return "CR2";
    case CR3: return "CR3";
    default:
        return "Invalid";
    }
}

const char* ffDelayString(int delay)
{
    switch (delay)
    {
    case FF0: return "FF0";
    case FF1: return "FF1";
    default:
        return "Invalid";
    }
}

const char* nlDelayString(int delay)
{
    switch (delay)
    {
    case NL0: return "NL0";
    case NL1: return "NL1";
    default:
        return "Invalid";
    }
}

const char* tabDelayString(int delay)
{
    switch (delay) {
    case TAB0: return "TAB0";
    case TAB1: return "TAB1";
    case TAB2: return "TAB2";
    case TAB3: return "TAB3";
    default:
        return "Invalid";
    }
}

const char* charSizeString(int csize)
{
    switch (csize) {
    case CS5: return "CS5";
    case CS6: return "CS6";
    case CS7: return "CS7";
    case CS8: return "CS8";
    default:
        return "Invalid";
    }
}

namespace _1 {

void print_termios(const char* name, int fd)
{
    termios t;
    tcgetattr(fd, &t);
    printf("%s\n", name);
    printf("\tc_iflag: %d\n", t.c_iflag);
    printf("\tc_oflag: %d\n", t.c_oflag);
    printf("\tc_cflag: %d\n", t.c_cflag);
    printf("\tc_lflag: %d\n\n", t.c_lflag);

    printf("\t[%c] Ctrl+C\n", (t.c_cc[VINTR] ? 'v' : ' '));
    printf("\t[%c] Ctrl+\\\n", (t.c_cc[VQUIT] ? 'v' : ' '));
    printf("\t[%c] Del\n", (t.c_cc[VERASE] ? 'v' : ' '));
    printf("\t[%c] @\n", (t.c_cc[VKILL] ? 'v' : ' '));
    printf("\t[%c] Ctrl+D\n", (t.c_cc[VEOF] ? 'v' : ' '));
    printf("\t[%c] inter-character timer\n", (t.c_cc[VTIME] ? 'v' : ' '));
    printf("\t[%c] blocking read until 1 character arrives\n", (t.c_cc[VINTR] ? 'v' : ' '));
    printf("\t[%c] \\0\n", (t.c_cc[VINTR] ? 'v' : ' '));
    printf("\t[%c] Ctrl+Q\n", (t.c_cc[VSTART] ? 'v' : ' '));
    printf("\t[%c] Ctrl+S\n", (t.c_cc[VSTOP] ? 'v' : ' '));
    printf("\t[%c] Ctrl+Z\n", (t.c_cc[VSUSP] ? 'v' : ' '));
    printf("\t[%c] EOL\n", (t.c_cc[VEOL] ? 'v' : ' '));
    printf("\t[%c] Ctrl+R\n", (t.c_cc[VREPRINT] ? 'v' : ' '));
    printf("\t[%c] Ctrl+U\n", (t.c_cc[VDISCARD] ? 'v' : ' '));
    printf("\t[%c] Ctrl+W\n", (t.c_cc[VWERASE] ? 'v' : ' '));
    printf("\t[%c] Ctrl+V\n", (t.c_cc[VLNEXT] ? 'v' : ' '));
    printf("\t[%c] EOL0\n\n", (t.c_cc[VEOL2] ? 'v' : ' '));

    printf("\t[%c] %s\n", t.c_iflag & BRKINT ? 'v' : ' ' , "BREAK -> SIGINT");
    printf("\t[%c] %s\n", t.c_iflag & ICRNL ? 'v' : ' ', "CR -> NL");
    printf("\t[%c] %s\n", t.c_iflag & IGNBRK ? 'v' : ' ', "IGNBRK");
    printf("\t[%c] %s\n", t.c_iflag & IGNCR ? 'v' : ' ', "IGNCR");
    printf("\t[%c] %s\n", t.c_iflag & IGNPAR ? 'v' : ' ', "IGNPAR (parity error)");
    printf("\t[%c] %s\n", t.c_iflag & BRKINT ? 'v' : ' ', "IMAX -> SIGINT");
    printf("\t[%c] %s\n", t.c_iflag & ISTRIP ? 'v' : ' ', "ISTRIP");
    printf("\t[%c] %s\n", t.c_iflag & IUTF8 ? 'v' : ' ', "IUTF8");
    printf("\t[%c] %s\n", t.c_iflag & IUCLC ? 'v' : ' ', "IUCLC : convert to lowercase");
    printf("\t[%c] %s\n", t.c_iflag & IXANY ? 'v' : ' ', "IXANY");
    printf("\t[%c] %s\n", t.c_iflag & IXOFF ? 'v' : ' ', "IXOFF");
    printf("\t[%c] %s\n", t.c_iflag & IXON ? 'v' : ' ', "IXON");
    printf("\t[%c] %s\n\n", t.c_iflag & PARMRK ? 'v' : ' ', "PARMRK : display parity error");

    printf("\t[%s] %s\n", bsDelayString(t.c_oflag & BSDLY), "BSDLY");
    printf("\t[%s] %s\n", crDelayString(t.c_oflag & CRDLY), "CRDLY");
    printf("\t[%s] %s\n", ffDelayString(t.c_oflag & FFDLY), "FFDLY form feed");
    printf("\t[%s] %s\n", nlDelayString(t.c_oflag & NLDLY), "NLDLY");
    printf("\t[%c] %s\n", t.c_oflag & OCRNL ? 'v' : ' ', "OCRNL");
    printf("\t[%c] %s\n", t.c_oflag & OFDEL ? 'v' : ' ', "OFDEL : use DEL(0177)");
    printf("\t[%c] %s\n", t.c_oflag & OFILL ? 'v' : ' ', "OFILL");
    printf("\t[%c] %s\n", t.c_oflag & OLCUC ? 'v' : ' ', "OLCUC : convert to uppercase");
    printf("\t[%c] %s\n", t.c_oflag & ONLCR ? 'v' : ' ', "ONLCR");
    printf("\t[%c] %s\n", t.c_oflag & ONLRET ? 'v' : ' ', "ONLRET");
    printf("\t[%c] %s\n", t.c_oflag & ONOCR ? 'v' : ' ', "ONOCR");
    printf("\t[%c] %s\n", t.c_oflag & OPOST ? 'v' : ' ', "OPOST");
    printf("\t[%s] %s\n", tabDelayString(t.c_oflag & TABDLY), "TABDLY");
    printf("\t[%c] %s\n\n", t.c_oflag & VTDLY ? 'v' : ' ', "VTDLY (VT0)");

    int baud = t.c_cflag & CBAUD;
    printf("\t[%s] %s\n", baudString(baud), "CBAUD");
    printf("\t[%c] %s\n", t.c_cflag & CBAUDEX ? 'v' : ' ', "CBAUDEX");
    printf("\t[%c] %s\n", t.c_cflag & CIBAUD ? 'v' : ' ', "CIBAUD");
    printf("\t[%c] %s\n", t.c_cflag & CLOCAL ? 'v' : ' ', "CLOCAL - ignore carrier signal");
    printf("\t[%c] %s\n", t.c_cflag & CMSPAR ? 'v' : ' ', "CMSPAR - use 'stick' parity");
    printf("\t[%c] %s\n", t.c_cflag & CREAD ? 'v' : ' ', "CREAD");
    printf("\t[%c] %s\n", t.c_cflag & CRTSCTS ? 'v' : ' ', "CRTSCTS - enable flow control (RTS/CTS)");
    printf("\t[%s] %s\n", charSizeString(t.c_cflag & CSIZE), "CIZE");
    printf("\t[%c] %s\n", t.c_cflag & CSTOPB ? '2' : '1', "CSTOPB - stop bit(s) per char");
    printf("\t[%c] %s\n", t.c_cflag & HUPCL ? 'v' : ' ', "HUPCL");
    printf("\t[%c] %s\n", t.c_cflag & PARENB ? 'v' : ' ', "PARENB - enable parity");
    printf("\t[%c] %s\n\n", t.c_cflag & PARODD ? 'v' : ' ', "PARODD");

    printf("\t[%c] %s\n", t.c_lflag & ECHO ? 'v' : ' ', "ECHO");
    printf("\t[%c] %s\n", t.c_lflag & ECHOCTL ? 'v' : ' ', "ECHOCTL - (ex: ^L)");
    printf("\t[%c] %s\n", t.c_lflag & ECHOE ? 'v' : ' ', "ECHOE - Erase");
    printf("\t[%c] %s\n", t.c_lflag & ECHOK ? 'v' : ' ', "ECHOK - Kill");
    printf("\t[%c] %s\n", t.c_lflag & ECHOKE ? 'v' : ' ', "ECHOKE");
    printf("\t[%c] %s\n", t.c_lflag & ECHONL ? 'v' : ' ', "ECHONL");
    printf("\t[%c] %s\n", t.c_lflag & ECHOPRT ? 'v' : ' ', "ECHOPRT");
    printf("\t[%c] %s\n", t.c_lflag & FLUSHO ? 'v' : ' ', "FLUSHO");
    printf("\t[%c] %s\n", t.c_lflag & ICANON ? 'v' : ' ', "ICANON");
    printf("\t[%c] %s\n", t.c_lflag & IEXTEN ? 'v' : ' ', "IEXTEN");
    printf("\t[%c] %s\n", t.c_lflag & ISIG ? 'v' : ' ', "ISIG (INTR, QUIT, SUSP)");
    printf("\t[%c] %s\n", t.c_lflag & NOFLSH ? 'v' : ' ', "NOFLSH (INTR, QUIT, SUSP)");
    printf("\t[%c] %s\n", t.c_lflag & PENDIN ? 'v' : ' ', "PENDIN");
    printf("\t[%c] %s\n", t.c_lflag & TOSTOP ? 'v' : ' ', "TOSTOP");
    printf("\t[%c] %s\n", t.c_lflag & XCASE ? 'v' : ' ', "XCASE");
}

void test()
{
    int mc = pathconf("/", _PC_MAX_CANON);
    printf("_PC_MAX_CANON: %d\n", mc);

    int mi = pathconf("/", _PC_MAX_INPUT);
    printf("_PC_MAX_INPUT: %d\n", mi);

    print_termios("STDIN_FILENO", STDIN_FILENO);    
}

} //_1 --------------------------------------------------------------

namespace _2 {

void test(int argc, const char* argv[])
{
    int interruptCh;
    if (1 == argc) {
        interruptCh = fpathconf(STDIN_FILENO, _PC_VDISABLE);
        if (-1 == interruptCh) {
            errnoExit("fpathconf(STDIN_FILENO, _PC_VDISABLE)", errno);
        }
    } else if (isdigit(argv[1][0])) {
        interruptCh = strtoul(argv[1], NULL, 0);
    } else {
        interruptCh = argv[1][0];
    }

    termios ti;

    if (-1 == tcgetattr(STDIN_FILENO, &ti)) {
        errnoExit("tcgetattr", errno);
    }

    ti.c_cc[VINTR] = interruptCh;

    if (-1 == tcsetattr(STDIN_FILENO, TCSAFLUSH, &ti)) {
        errnoExit("tcsetattr", errno);
    }
    exit(EXIT_SUCCESS);
}

} //_2 --------------------------------------------------------------

namespace _3 {

const int BUF_SIZE = 100;

void test()
{
    termios ti, save;
    char buf[BUF_SIZE];

    if (-1 == tcgetattr(STDIN_FILENO, &ti)) {
        errnoExit("tcgetattr", errno);
    }
    save = ti;
    ti.c_lflag &= ~ECHO;
    if (-1 == tcsetattr(STDIN_FILENO, TCSAFLUSH, &ti)) {
        errnoExit("tcsetattr", errno);
    }

    printf("Enter text: ");
    fflush(stdout);

    if (NULL == fgets(buf, BUF_SIZE, stdin)) {
        printf("Got EOF/error on fgets\n");
    } else {
        printf("\nRead: %s\n", buf);
    }

    if (-1 == tcsetattr(STDIN_FILENO, TCSANOW, &save)) {
        errnoExit("tcsetattr", errno);
    }
    exit(EXIT_SUCCESS);
}

} //_3 --------------------------------------------------------------

namespace _4 {

int ttySetCBreak(int fd, termios* ti)
{
    termios t;
    if (-1 == tcgetattr(fd, &t)) {
        return -1;
    }
    if (NULL != ti)
        *ti = t;

    t.c_lflag &= ~(ICANON | ECHO);
    t.c_lflag |= ISIG;

    t.c_iflag &= ~ICRNL;

    t.c_cc[VMIN] = 1;
    t.c_cc[VTIME] = 0;

    if (-1 == tcsetattr(fd, TCSAFLUSH, &t)) {
        return -1;
    }

    return 0;
}

int ttySetRaw(int fd, termios* ti)
{
    termios t;
    if (-1 == tcgetattr(fd, &t)) {
        return -1;
    }
    if (NULL != ti)
        *ti = t;

    t.c_lflag &= ~(ICANON | ISIG | IEXTEN | ECHO);

    t.c_iflag &= ~(BRKINT | ICRNL | IGNBRK | IGNCR | INLCR | INPCK | ISTRIP | IXON | PARMRK);

    t.c_oflag &= ~OPOST;

    t.c_cc[VMIN] = 1;
    t.c_cc[VTIME] = 0;

    if (-1 == tcsetattr(fd, TCSAFLUSH, &t)) {
        return -1;
    }

    return 0;
}

termios userTermios;

void sigHandler(int sig)
{
    if (-1 == tcsetattr(STDIN_FILENO, TCSAFLUSH, &userTermios)) {
        errnoExit("tcsetattr", errno);
    }
    _exit(EXIT_SUCCESS);
}

void tstpSigHandler(int sig)
{
    termios ti;
    sigset_t tstpMask, prevMask;
    struct sigaction sa;
    int saveErrno = errno;

    if (-1 == tcgetattr(STDIN_FILENO, &ti)) {
        errnoExit("tcgetattr", errno);
    }
    if (-1 == tcsetattr(STDIN_FILENO, TCSAFLUSH, &userTermios)) {
        errnoExit("tcsetattr", errno);
    }

    if (SIG_ERR == signal(SIGTSTP, SIG_DFL)) {
        errorExit("signal(SIGTSTP, SIG_DFL)");
    }
    raise(SIGTSTP);

    sigemptyset(&tstpMask);
    sigaddset(&tstpMask, SIGTSTP);
    if (-1 == sigprocmask(SIG_UNBLOCK, &tstpMask, &prevMask)) {
        errnoExit("sigprocmask(SIG_UNBLOCK, ...)", errno);
    }

    // after SIGCONT ...

    if (-1 == sigprocmask(SIG_SETMASK, &prevMask, NULL)) {
        errnoExit("sigpromask", errno);
    }
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = tstpSigHandler;
    if (-1 == sigaction(SIGTSTP, &sa, NULL)) {
        errnoExit("sigaction(SIGTSTP, ...)", errno);
    }

    if (-1 == tcgetattr(STDIN_FILENO, &userTermios)) {
        errnoExit("tcgetattr", errno);
    }
    errno = saveErrno;
}

void test(int argc, const char* argv[])
{
    char c;
    struct sigaction sa, prev;
    ssize_t n;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if (1 < argc) {
       if (-1 == ttySetCBreak(STDIN_FILENO, &userTermios)) {
           errnoExit("ttySetCBreat", errno);
       }

       sa.sa_handler = sigHandler;

       if (-1 == sigaction(SIGQUIT, NULL, &prev)) {
           errnoExit("sigaction(SIGQUIT)", errno);
       }
       if (SIG_IGN != prev.sa_handler) {
           if (-1 == sigaction(SIGQUIT, &sa, NULL)) {
               errnoExit("sigaction(SIGQUIT) #2", errno);
           }
       }

       if (-1 == sigaction(SIGINT, NULL, &prev)) {
           errnoExit("sigaction(SIGINT)", errno);
       }
       if (SIG_IGN != prev.sa_handler) {
           if (-1 == sigaction(SIGINT, &sa, NULL)) {
               errnoExit("sigaction(SIGINT) #2", errno);
           }
       }

       sa.sa_handler = tstpSigHandler;

       if (-1 == sigaction(SIGTSTP, NULL, &prev)) {
           errnoExit("sigaction(SIGTSTP)", errno);
       }
       if (SIG_IGN != prev.sa_handler) {
           if (-1 == sigaction(SIGTSTP, &sa, NULL)) {
               errnoExit("sigaction(SIGTSTP) #2", errno);
           }
       }
    } else {
        if (-1 == ttySetRaw(STDIN_FILENO, &userTermios)) {
            errnoExit("ttySetRaw", errno);
        }
    }

    sa.sa_handler = sigHandler;

    if (-1 == sigaction(SIGTERM, &sa, NULL)) {
        errnoExit("sigaction(SIGTERM)", errno);
    }

    setbuf(stdout, NULL);

    for ( ;; ) {
        n = read(STDIN_FILENO, &c, 1);
        if (-1 == n) {
            errorExit("read() failed.\n");
        }
        if (0 == n)
            break;

        if (isalpha(c)) {
            putchar(tolower(c));
        } else if (c == '\n' || c == '\r') {
            putchar(c);
        } else if (iscntrl(c)) {
            printf("^%c", c^64);
        } else {
            putchar('*');
        }

        if ('q' == c)
            break;
    }

    if (-1 == tcsetattr(STDIN_FILENO, TCSAFLUSH, &userTermios)) {
        errnoExit("tcsetattr", errno);
    }
    exit(EXIT_SUCCESS);
}

} //_4 --------------------------------------------------------------

namespace _5 {

void test()
{
    termios ti;

    if (-1 == tcgetattr(STDIN_FILENO, &ti)) {
        errnoExit("tcgetattr", errno);
    }

    speed_t rate = cfgetispeed(&ti);
    printf("stdin baud rate: %s\n", baudString(rate));
}

} //_5 --------------------------------------------------------------

} //namespace =================================================================

void exec_ch_25(int argc, const char* argv[])
{
#if (0) //done
    _1::test();
    _2::test(argc, argv);
    _3::test();
    _4::test(argc, argv);
#endif

    _5::test();
}
