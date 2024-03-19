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
    struct itimerval ti, oldTi;
    ti.it_value.tv_sec = seconds;

    if (setitimer(ITIMER_REAL, &ti, &oldTi) == -1)
    {
        _exit(1);
    };

    return oldTi.it_value.tv_sec;
}

int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}
