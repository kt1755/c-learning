#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

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

static void serveRequest(int msqid, const struct requestMsg *msg)
{
    struct responseMsg respMsg;

    printf("Serve client %d\n", msg->clientId);

    respMsg.mtype = msg->clientId;
    respMsg.seq = msg->seq;

    msgsnd(msqid, &respMsg, sizeof(int), 0);
}

int main(int argc, char const *argv[])
{
    int opt, msqid;
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

    msqid = msgget(SERVER_KEY, flags | S_IRUSR | S_IWUSR);
    if (msqid == -1)
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

    for (;;)
    {
        int received = msgrcv(msqid, &reqMsg, 2 * sizeof(int), 0, 0);
        if (received == -1)
        {
            if (errno == EINTR)
            {
                continue;
            }

            perror("msgrcv");
            break;
        }

        int pid = fork();
        if (pid == -1)
        {
            perror("fork\n");
            break;
        }

        if (pid == 0)
        { // Child process
            serveRequest(msqid, &reqMsg);
            _exit(EXIT_SUCCESS);
        }
    }

    return 0;
}
