#include <stdio.h>
#include <stdlib.h>
 #include <stddef.h>

#define SERVER_KEY 0x1aaaaaa1

#define SIZE_OF_MSG = sizeof(int)

struct requestMsg {
    long mtype;
    int clientId;
    int seq;
};

#define REQ_MESSAGE_SIZE (offsetof(struct requestMsg, mtype)))

struct responseMsg {
    long mtype;
    int seq;
};

