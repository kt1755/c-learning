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

static void handleResponse(const struct responseMsg *msg)
{
}

int main(int argc, char const *argv[])
{
    struct requestMsg reqMsg;
    struct responseMsg respMsg;
    int msqid;
    int pid = getpid();

    msqid = msgget(SERVER_KEY, S_IRUSR | S_IWUSR);
    if (msqid == -1)
    {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    reqMsg.mtype = 0;
    reqMsg.clientId = pid;
    reqMsg.seq = 10;

    msgsnd(msqid, &reqMsg, 2 * sizeof(int), 0);

    for (;;)
    {
        msgrcv(msqid, &respMsg, 2 * sizeof(int), pid, 0);
    }
    return 0;
}
