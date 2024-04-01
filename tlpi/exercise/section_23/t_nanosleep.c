/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2024.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* t_nanosleep.c

   Demonstrate the use of nanosleep() to sleep for an interval
   specified in nanoseconds.

   See also t_clock_nanosleep.c.
    macos cmd: clang t_nanosleep.c -o t_nanosleep.out ../../lib/signal_functions.c ../../lib/error_functions.c ../../lib/get_num.c -I/Users/lap-01124/c-learning/tlpi/lib
    ubuntu cmd: gcc t_nanosleep.c -o t_nanosleep.out ../../lib/signal_functions.c ../../lib/error_functions.c ../../lib/get_num.c -I/home/thu/coding/tlpi/lib

*/
#define _POSIX_C_SOURCE 199309
#define _XOPEN_SOURCE 600
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#ifdef TARGET_OS_MAC
#include "/Users/lap-01124/c-learning/tlpi/lib/tlpi_hdr.h"
#elif __linux
#include "/home/thu/coding/tlpi/lib/tlpi_hdr.h"
#endif

static void
sigintHandler(int sig)
{
    return; /* Just interrupt nanosleep() */
}

int main(int argc, char *argv[])
{
    struct timeval start, finish;
    struct timespec request, remain, startspec, finishspec, requestabs;
    struct sigaction sa;
    int s;

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s secs nanosecs\n", argv[0]);

    request.tv_sec = getLong(argv[1], 0, "secs");
    request.tv_nsec = getLong(argv[2], 0, "nanosecs");

    printf("Request for: %9ld secs\n", request.tv_sec);

    /* Allow SIGINT handler to interrupt nanosleep() */

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigintHandler;
    if (sigaction(SIGINT, &sa, NULL) == -1)
        errExit("sigaction");

    // if (gettimeofday(&start, NULL) == -1)
    //     errExit("gettimeofday");

    if (clock_gettime(CLOCK_REALTIME, &startspec) == -1)
        errExit("clock_gettime start");

    requestabs.tv_sec = request.tv_sec + startspec.tv_sec;
    requestabs.tv_nsec = request.tv_nsec + startspec.tv_nsec;

    for (;;)
    {
        // Using clock_nanosleep instead

        // s = nanosleep(&request, &remain);
        // if (s == -1 && errno != EINTR)
        //     errExit("nanosleep");

        // printf("Current s: %d", s);
        

        // if (gettimeofday(&finish, NULL) == -1)
        //     errExit("gettimeofday");
        // printf("Slept for: %9.6f secs\n", finish.tv_sec - start.tv_sec +
        //                                       (finish.tv_usec - start.tv_usec) / 1000000.0);

        // Continue implement
        // Need to pass absolute time as parameter for both request and remain
        s = clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &requestabs, &remain);
        if (s == -1 && errno != EINTR)
            errExit("nanosleep");
        
        if (clock_gettime(CLOCK_REALTIME, &finishspec) == -1)
            errExit("clock_gettime start");
        printf("Slept for: %9.6f secs\n", finishspec.tv_sec - startspec.tv_sec +
                                              (finishspec.tv_nsec - startspec.tv_nsec) / 1000000.0);

        if (s == 0)
            break; /* nanosleep() completed */

        printf("Remaining: %2ld.%09ld\n", (long)remain.tv_sec, remain.tv_nsec);
        // request = remain; /* Next sleep is with remaining time */
        requestabs = remain;
    }

    printf("Sleep complete\n");
    exit(EXIT_SUCCESS);
}
