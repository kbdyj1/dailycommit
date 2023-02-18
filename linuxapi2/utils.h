#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <signal.h>

void printSigset(FILE* of, const char* prefix, const sigset_t* ss);
int printSigMask(FILE* of, const char* msg);
int printPendingSigs(FILE* of, const char* msg);

#endif // UTILS_H
