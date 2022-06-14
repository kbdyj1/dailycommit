#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <setjmp.h>
#include "utils.h"

extern char *etext, *edata, *end;

namespace { //=================================================================

void test_process_id()
{
    auto ppid = getppid();
    std::cout << "ppid: " << ppid << "\n";

    auto pid = getpid();
    std::cout << "pid: " << pid << "\n";

    catFile("/proc/sys/kernel/pid_max");    //32768
}

// Process memory layout
//  1. text segment
//  2. initialized data segment (global & static)
//  3. uninitialized data segment (bss segment)
//  4. stack (stack frame. local variable, parameter, return value
//  5. heap

constexpr int BUF_SIZE = 64;
char buf[BUF_SIZE]; // uninitialized data segment
int primes[] = { 2, 3, 5, 7, 11 };  // initialized data segment

void test_process_segment()
{
    std::cout << "etext: " << std::addressof(etext) << "\n";    //etext: 0x555555557f75
    std::cout << "edata: " << std::addressof(edata) << "\n";    //edata: 0x55555555a034
    std::cout << "end: " << std::addressof(end) << "\n";        //end: 0x55555555a0c8
}

void test_virtual_memory_page_size()
{
    std::cout << "VMEM PAGE SIZE: " << sysconf(_SC_PAGESIZE) << "\n";
}

void test_sigseg()
{
    char *p;
    p = (char*)0x4000000;

    *p = '\0';
}

//-------------------------------------------------------------------
//  DEFINES += _GNU_SOURCE
//  <errno.h>
//
//  program: /home/user/project/dailycommit/build-linuxapi-Desktop-Debug/linuxapi
//  program(short): linuxapi
//-------------------------------------------------------------------
void test_program_name()
{
    std::cout << "program: " << program_invocation_name << "\n";
    std::cout << "program(short): " << program_invocation_short_name << "\n";

    std::cout << "ARG_MAX: " << sysconf(_SC_ARG_MAX) << "\n";   // 2097152
}

char ch6env[32] = "CH6_NAME=Process";
//-------------------------------------------------------------------
//  SHELL=/bin/bash
//  ...
//  CH6_NAME=Ch6.Process
//-------------------------------------------------------------------
void test_environment()
{
    putenv(ch6env);

    strcpy(ch6env, "CH6_NAME=Ch6.Process");

    //clearenv();

    if (environ != nullptr) {
        for (char** env = environ; *env != nullptr; env++) {
            std::cout << *env << "\n";
        }
    }
}

jmp_buf jmp;
void test_set_jump()
{
    switch (setjmp(jmp)) {
    case 0:
        std::cout << "setjmp(jmp)" << "\n";
        break;
    case 2:
        std::cout << "jumped: 2" << "\n";
        break;
    default:
        std::cout << "jumped: ?" << "\n";
        break;
    }
}

} //===========================================================================

void test_ch_6()
{
    //test_virtual_memory_page_size();    // VMEM PAGE SIZE: 4096
    //test_sigseg();
    //test_environment();
}
