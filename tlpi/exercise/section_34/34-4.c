/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2024.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* disc_SIGHUP.c

   This program demonstrates that when a "terminal disconnect" occurs, SIGHUP
   is sent to all the members of the foreground process group for this terminal.

   Try using the following command to run this program in an X-window, and then
   closing the window:

        exec disc_SIGHUP > sig.log 2>&1

   (Since the above will replace the shell with this program, it will be the
   controlling process for the terminal.)
*/
#define _GNU_SOURCE /* Get strsignal() declaration from <string.h> */
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
// #include "tlpi_hdr.h"

static void handler(int sig) /* Handler for SIGHUP */
{
    printf("PID %ld: caught signal %2d (%s)\n", (long)getpid(),
           sig, strsignal(sig));
    /* UNSAFE (see Section 21.1.2) */
}

int main(int argc, char *argv[])
{
    pid_t parentPid, childPid;
    int j;
    struct sigaction sa;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
    {
        printf("%s {d|s}... [ > sig.log 2>&1 ]\n", argv[0]);
        return EXIT_FAILURE;
    }

    setbuf(stdout, NULL); /* Make stdout unbuffered */

    parentPid = getpid();
    printf("PID of parent process is:       %ld\n", (long)parentPid);
    printf("Foreground process group ID is: %ld\n",
           (long)tcgetpgrp(STDIN_FILENO));

    for (j = 1; j < argc; j++)
    { /* Create child processes */
        childPid = fork();
        if (childPid == -1)
        {
            perror("fork");
            return EXIT_FAILURE;
        }

        if (childPid == 0)
        {                          /* If child... */
            if (argv[j][0] == 'd') /* 'd' --> to different pgrp */
                if (setpgid(0, 0) == -1)
                {
                    perror("setpgid");
                    return EXIT_FAILURE;
                }

            sigemptyset(&sa.sa_mask);
            sa.sa_flags = 0;
            sa.sa_handler = handler;
            if (sigaction(SIGHUP, &sa, NULL) == -1)
                perror("sigaction");
            return EXIT_FAILURE;
            break; /* Child exits loop */
        }
    }

    /* All processes fall through to here */

    alarm(60); /* Ensure each process eventually terminates */

    printf("PID=%ld PGID=%ld\n", (long)getpid(), (long)getpgrp());
    for (;;)
        pause(); /* Wait for signals */
}