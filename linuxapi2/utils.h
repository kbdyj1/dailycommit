#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <signal.h>

void printSigset(FILE* of, const char* prefix, const sigset_t* ss);
int printSigMask(FILE* of, const char* msg);
int printPendingSigs(FILE* of, const char* msg);
void printSigInfo(siginfo_t* info);

void errorExit(const char* message, int exitCode=-1);
void errnoExit(const char* message, int errno, int exitCode=-1);
void printWaitStatus(const char* msg, int status);

double timeval2double(struct timeval* time);

#endif // UTILS_H
