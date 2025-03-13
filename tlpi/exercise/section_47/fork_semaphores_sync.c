/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2024.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* fork_sig_sync.c

   Demonstrate how signals can be used to synchronize the actions
   of a parent and child process.
*/
#define _POSIX_C_SOURCE 200809L

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <time.h>
#include <sys/sem.h>

#define SYNC_SIG SIGUSR1 /* Synchronization signal */

static void /* Signal handler - does nothing but return */
handler(int sig)
{
}

char *currTime(const char *format)
{

    // int BUFSIZE = 1000;
    static char buf[1000]; /* Nonreentrant */
    time_t t;
    size_t s;
    struct tm *tm;

    t = time(NULL);
    tm = localtime(&t);
    if (tm == NULL)
        return NULL;

    s = strftime(buf, 1000, (format != NULL) ? format : "%c", tm);

    return (s == 0) ? NULL : buf;
}

int main(int argc, char *argv[])
{
    pid_t childPid;
    sigset_t blockMask, origMask, emptyMask;
    struct sigaction sa;

    setbuf(stdout, NULL); /* Disable buffering of stdout */

    int semid = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
    if (semid == -1)
    {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    printf("Init semaphore done \n");

    struct sembuf sop;

    // Init semaphore
    if ( semctl(semid, 0, SETVAL, 1) == -1)
    {
        perror("semctl");
        exit(EXIT_FAILURE);
    }

    switch (childPid = fork())
    {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);

    case 0: /* Child */

        /* Child does some required action here... */

        printf("[%s %ld] Child started - doing some work\n",
               currTime("%T"), (long)getpid());
        sleep(2); /* Simulate time spent doing some work */

        /* And then signals parent that it's done */

        printf("[%s %ld] Child about to signal parent\n",
               currTime("%T"), (long)getpid());

        sop.sem_num = 0;
        sop.sem_op = -1;
        if (semop(semid, &sop, 1) == -1)
        {
            perror("semop child");
            _exit(EXIT_FAILURE);
        }

        /* Now child can do other things... */

        _exit(EXIT_SUCCESS);

    default: /* Parent */

        /* Parent may do some work here, and then waits for child to
           complete the required action */

        printf("[%s %ld] Parent about to wait for signal\n",
               currTime("%T"), (long)getpid());

        sop.sem_num = 0;
        sop.sem_op = 0;
        if (semop(semid, &sop, 1) == -1)
        {
            perror("semop parent");
            exit(EXIT_FAILURE);
        }

        /* Parent carries on to do other things... */
        printf("[%s %ld] Parent got awake\n", currTime("%T"), (long)getpid());

        exit(EXIT_SUCCESS);
    }
}