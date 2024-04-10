/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2024.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* make_zombie.c

   Demonstrate how a child process becomes a zombie in the interval between
   the time it exits, and the time its parent performs a wait (or exits, at
   which time it is adopted by init(8), which does a wait, thus releasing
   the zombie).
*/
#include <signal.h>
#include <libgen.h>             /* For basename() declaration */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define CMD_SIZE 200

int
main(int argc, char *argv[])
{
    char cmd[CMD_SIZE];
    pid_t childPid;

    setbuf(stdout, NULL);       /* Disable buffering of stdout */

    printf("Parent PID=%ld\n", (long) getpid());

    switch (childPid = fork()) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
        

    case 0:     /* Child: immediately exits to become zombie */
        printf("Child (PID=%ld) have parent (PID=%ld)\n", (long) getpid(), (long) getppid());
        
        sleep(3); /* Give child a chance to parent killed immediately */
        printf("After 3 second get ppid (PID=%ld):\n", (long) getppid());

        exit(EXIT_SUCCESS);
        

    default:    /* Parent */
        _exit(EXIT_SUCCESS);
        
    }
}