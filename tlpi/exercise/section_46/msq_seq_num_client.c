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
    int pid = getpid();
    printf("Client %d get sequence %d\n", pid, msg->seq);
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

    printf("Client connect queue successful at id %d\n", msqid);

    reqMsg.mtype = 0;
    reqMsg.clientId = pid;
    reqMsg.seq = 10;

    printf("Start send sequence %d to message queue\n", reqMsg.seq);
    int sended = msgsnd(msqid, &reqMsg, 2 * sizeof(int), 0);
    if (sended == -1) {
        perror("Send to queue faield\n");
        exit(EXIT_FAILURE);
    }

    int receivedBytes = msgrcv(msqid, &respMsg, 2 * sizeof(int), pid, 0);
    if (receivedBytes == -1)
    {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }

    handleResponse(&respMsg);

    return 0;
}
