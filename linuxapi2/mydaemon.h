#ifndef MYDAEMON_H
#define MYDAEMON_H

enum Flag {
    NoChDir = 0x1,
    NoCloseFiles = 0x2,
    NoReopenStdFds = 0x4,
    NoUmask = 0x10,

    MaxClose = 8192
};

int startDaemon(int flags);

#endif // MYDAEMON_H
