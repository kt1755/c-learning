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

static int msqid;

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

static void cleanup(void)
{
    printf("Cleanup message queue identitfy %d\n", msqid);
    msgctl(msqid, IPC_RMID, NULL);
}

int main(int argc, char const *argv[])
{
    int opt;
    struct requestMsg reqMsg;
    struct responseMsg respMsg;
    struct sigaction sa;

    int flags = IPC_NOWAIT | IPC_CREAT;

    msqid = msgget(SERVER_KEY, flags | S_IRUSR | S_IWUSR);
    if (msqid == -1)
    {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    printf("Server start queue successful at id %d\n", msqid);
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = grimReaper;

    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
    }

    atexit(cleanup);

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
            printf("Child %d handle message of queue %d", getpid(), msqid);
            serveRequest(msqid, &reqMsg);
            _exit(EXIT_SUCCESS);
        }
    }

    return 0;
}
