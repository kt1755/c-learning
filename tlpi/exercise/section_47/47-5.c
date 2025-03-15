#define _POSIX_C_SOURCE 200809L

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <time.h>
#include <sys/sem.h>

#define INITIAL_SEM_VALUE 1; // Binary concurrency control

int setEventFlag(int semid, int semnum) {
    struct sembuf sop;
    sop.sem_num = semnum;
    sop.sem_op = -1;
    return semop(semid, &sop, 1);
}

int clearEventFlag(int semid, int semnum)
{
    struct sembuf sop;
    sop.sem_num = semnum;
    sop.sem_op = 1;
    return semop(semid, &sop, 1);
}

int waitForEventFlag(int semid, int semnum) {
    if (setEventFlag(semid, semnum) == -1) // Will block if not available
    {
        perror("setEventFlag");
        return -1;
    }
    
    return clearEventFlag(semid, semnum);
}

int getFlagState(int semid, int semnum) {
    struct sembuf sop;
    sop.sem_num = semnum;
    sop.sem_op = 0;
    sop.sem_flg = IPC_NOWAIT;
    if ( semop(semid, &sop, 1) == -1)
    {
        if (errno == EAGAIN) // Valid, when semaphore is 1
        {
            return 1;
        }

        perror("semop");
        return -1;
    }
    return 0;
}

int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}
