#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <errno.h>
#include <string.h>
#include <sched.h>
#include <sys/times.h>
#include <signal.h>

static void consumeSigAlarm(int sig)
{
    struct tms t;
    clock_t n, old_tick_quarter, old_tick_sec;
    old_tick_quarter = 0;
    old_tick_sec = 0;
    double sec, quarter;
    long times_tick_conf = sysconf(_SC_CLK_TCK);
    while (1 == 1)
    {
        if (n = times(&t) == -1)
        {
            perror("Times syscall");
            return;
        };

        if ((quarter = (t.tms_utime - old_tick_quarter) / times_tick_conf) == 0.25)
        {
            // Do quarter action
            printf("Process %d cpu amount: %.2f secs\n", getpid(), quarter);
            old_tick_quarter = t.tms_utime;
        }

        if ((sec = (t.tms_utime - old_tick_sec) / times_tick_conf) == 1)
        {
            // Do 1 sec action
            printf("Process %d cpu execute: %.2f secs\n", getpid(), quarter);
            old_tick_sec = t.tms_utime;
            sched_yield();
        }
    }
}

int main(int argc, char *argv[])
{
    struct sched_param sp;
    sp.sched_priority = 0;

    if (sched_setscheduler(0, SCHED_FIFO, &sp) == -1)
    {
        perror("sched_setscheduler");
        return EXIT_FAILURE;
    }

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGALRM, &sa, NULL) == -1)
    {
        perror("sigaction SIGALRM");
        return EXIT_FAILURE;
    }

    switch (fork())
    {
    case -1:
        perror("fork()");
        return EXIT_FAILURE;
        break;

    default: // both parent and child

        break;
    }
    return EXIT_SUCCESS;
}
