#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <signal.h>
#include <errno.h>
#include "msq_seq_num.h"

// Avoid zombie process
static void grimReaper(int sig)
{
    int savedErrno = errno;
    while (waitpid(-1, NULL, WNOHANG) > 0)
    {
        continue;
    }

    errno = savedErrno;
}

int main(int argc, char const *argv[])
{
    int opt, serverID;
    struct requestMsg reqMsg;
    struct responseMsg respMsg;
    struct sigaction sa;

    int flags = IPC_NOWAIT;

    while ((opt = getopt(argc, argv, "s")) != -1)
    {
        if (opt == 's')
        {
            flags |= IPC_CREAT;
        }
    }

    serverID = msgget(SERVER_KEY, flags);
    if (serverID == -1)
    {
        perror("msgget");
    }

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = grimReaper;

    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
    }

    return 0;
}
