#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "msq_seq_num.h"

static void handleResponse(const struct responseMsg *msg)
{
    int pid = getpid();
    printf("Client %d get sequence %s\n", pid, msg->mtext);
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

    reqMsg.mtype = pid;
    sprintf(reqMsg.mtext, "%d", pid);
    

    printf("Start send sequence %d to message queue\n", pid);
    int sended = msgsnd(msqid, &reqMsg, REQ_MESSAGE_SIZE, 0);
    if (sended == -1) {
        printf("Send to queue failed with errno: %d\n", errno);
        perror("Send to queue failed");
        exit(EXIT_FAILURE);
    }

    int receivedBytes = msgrcv(msqid, &respMsg, RESP_MESSAGE_SIZE, pid, 0);
    if (receivedBytes == -1)
    {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }

    handleResponse(&respMsg);

    exit(EXIT_SUCCESS);
}
