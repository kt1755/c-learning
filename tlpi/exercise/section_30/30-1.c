/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2024.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* 
gcc 30-1.c ../../lib/signal_functions.c ../../lib/error_functions.c ../../lib/get_num.c -I/Users/lap-01124/c-learning/tlpi/lib -o 30-1.out
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/syscall.h>

#include "error_functions.h"
#include "get_num.h"


#ifdef TARGET_OS_MAC
#include "/Users/lap-01124/c-learning/tlpi/lib/tlpi_hdr.h"
#elif __linux
#include "/home/thu/coding/tlpi/lib/tlpi_hdr.h"
#endif

static volatile int glob = 0; /* "volatile" prevents compiler optimizations
                                 of arithmetic operations on 'glob' */

struct threadArg {
    pthread_t* tid;
    int* loop;
    char* filePath;
};


/* Loop 'arg' times incrementing 'glob' */
static void *threadFunc(void *arg)
{
    // int loops = *((int *)arg);
    struct threadArg* thra = ((struct threadArg*) arg);
    int loc, j;

    FILE *fp = fopen("threadFunc.txt", "w+");
    setbuf(fp, NULL);
    for (j = 0; j < *(thra->loop); j++)
    {
        loc = *(thra->loop);
        loc++;
        fprintf(fp, "Glob in thread %ld is %d\n", *thra->tid, loc);
        *thra->loop = loc;
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t t1, t2;
    int loops, s;
    struct threadArg thr1, thr2;

    loops = (argc > 1) ? getInt(argv[1], GN_GT_0, "num-loops") : 10000000;

    char* path = getcwd(NULL, PATH_MAX);
    
    thr1.tid = &t1;
    thr1.loop = &loops;
    thr1.filePath = path;

    thr2.tid = &t2;
    thr2.loop = &loops;
    thr2.filePath = path;

    s = pthread_create(&t1, NULL, threadFunc, &thr1);
    if (s != 0)
        errExitEN(s, "pthread_create");
        
    s = pthread_create(&t2, NULL, threadFunc,  &thr2);
    if (s != 0)
        errExitEN(s, "pthread_create");

    s = pthread_join(t1, NULL);
    if (s != 0)
        errExitEN(s, "pthread_join");
    s = pthread_join(t2, NULL);
    if (s != 0)
        errExitEN(s, "pthread_join");

    printf("glob = %d\n", glob);

    free(path);
    
    exit(EXIT_SUCCESS);
}