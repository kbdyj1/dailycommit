#ifndef STARTDAEMON_H
#define STARTDAEMON_H

#define SD_NO_CHDIR             1   // do not chdir("/")
#define SD_NO_CLOSE_FILES       2
#define SD_NO_REOPEN_STD_FDS    4
#define SD_NO_UMASK0            10
#define SD_MAX_CLOSE            8192

int startDaemon(int flags);

void sig_hup_handler(int sig);

#endif // STARTDAEMON_H
