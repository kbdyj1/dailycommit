#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <signal.h>

void printSigset(FILE* of, const char* prefix, const sigset_t* ss);
int printSigMask(FILE* of, const char* msg);
int printPendingSigs(FILE* of, const char* msg);
void printSigInfo(siginfo_t* info);

void errorExit(const char* message, int error=-1);
void printWaitStatus(const char* msg, int status);

#endif // UTILS_H
