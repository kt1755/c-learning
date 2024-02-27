#define _GNU_SOURCE
#include <signal.h>
#include <stdio.h>
#include <string.h>

static volatile sig_atomic_t gotSigint = 0;
static volatile sig_atomic_t gotSigint = 0;

static void handler(int sig)
{
    if (sig == SIGINT)
    {
        gotSigint = 1;
        printf("Caught SIGINT\n");
    }
    else
        printf("Caught %s\n", strsignal(sig));
}

int main(int argc, char const *argv[])
{
    sigset_t pendingSig, prevMask;
    struct sigaction act, oldact;

    sigemptyset(&pendingSig);
    sigaddset(&pendingSig, SIGSTOP);

// Use SIG_BLOCK to check
    if (sigprocmask(SIG_BLOCK, &pendingSig, &prevMask) == -1)
        errExit("sigprocmask1");

    for (int n = 1; n < NSIG; n++)
    {
        act.__sigaction_u.__sa_handler = handler;

        (void)signal(n, SIG_IGN);
    }
}
