#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <errno.h>
#include <string.h>
#include <sched.h>
#include <sys/times.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>

static void consumeSigAlarm()
{
    printf("Start in process %d\n", getpid());
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

        // Not reachable code, result show process log in 1 sec each
        if ((quarter = (t.tms_stime - old_tick_quarter) / times_tick_conf) >= 0.25)
        {
            // Do quarter action
            printf("Process %d cpu amount: %.2f secs\n", getpid(), quarter);
            old_tick_quarter = t.tms_stime;
        }

        if ((sec = (t.tms_stime - old_tick_sec) / times_tick_conf) >= 1)
        {
            // Do 1 sec action
            printf("Process %d cpu execute: %.2f secs\n", getpid(), sec);
            old_tick_sec = t.tms_stime;
            sched_yield();
        }

        // terminate after 3 sec
        if ((t.tms_stime / times_tick_conf) >= 3)
        {
            return;
        }
    }
}

int main(int argc, char *argv[])
{

    struct sched_param sp;
    sp.sched_priority = sched_get_priority_max(SCHED_FIFO);
    // sp.sched_priority = 1;

    if (sched_setscheduler(0, SCHED_FIFO, &sp) == -1)
    {
        perror("sched_setscheduler");
        return EXIT_FAILURE;
    }

    // struct sigaction sa;
    // sigemptyset(&sa.sa_mask);
    // sa.sa_handler = consumeSigAlarm;
    // sa.sa_flags = 0;
    // if (sigaction(SIGALRM, &sa, NULL) == -1)
    // {
    //     perror("sigaction SIGALRM");
    //     return EXIT_FAILURE;
    // }

    setbuf(stdout, NULL);
    fcntl(STDOUT_FILENO, F_SETFD, FD_CLOEXEC);

    switch (fork())
    {
    case -1:
        perror("fork()");
        return EXIT_FAILURE;
        break;

    case 0: // Child
        consumeSigAlarm();
        printf("Child %d reach end, terminating...\n", getpid());
        _exit(EXIT_SUCCESS);

    default: // parent
        consumeSigAlarm();
        wait(NULL);

        printf("Parent %d reach end, terminating...\n", getpid());
        exit(EXIT_SUCCESS);
    }

    // return EXIT_SUCCESS;
}
