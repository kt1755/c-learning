#if !defined(_XOPEN_SOURCE) || _XOPEN_SOURCE < 600
#define _XOPEN_SOURCE 600
#endif
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>

static volatile sig_atomic_t gotAlarm = 0; /* Set nonzero on receipt of SIGALRM */
static volatile sig_atomic_t remainSeconds = 0;

static void handlesigalarm(int sig)
{
    gotAlarm = 1;
    struct itimerval ti;
    if (getitimer(ITIMER_REAL, &ti) == -1)
        return;

    remainSeconds = ti.it_value.tv_sec;
}

unsigned int alarm_impl(unsigned int seconds)
{
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = handlesigalarm;
    sa.sa_flags = 0;
    if (sigaction(SIGALRM, &sa, NULL) == -1)
    {
        _exit(1);
    }

    struct itimerval ti;
    ti.it_value.tv_sec = seconds;

    if (setitimer(ITIMER_REAL, &ti, NULL) == -1)
    {
        _exit(1);
    };

    if (gotAlarm == 1)
    {
        return remainSeconds;
    }
}

int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}
