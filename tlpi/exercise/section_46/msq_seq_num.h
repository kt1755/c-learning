#include <stdio.h>
#include <stdlib.h>
 #include <stddef.h>

#define SERVER_KEY 0x1aaaaaa1
#define MESSAGE_DATA_SIZE 1024

struct requestMsg {
    long mtype;
    char mtext[MESSAGE_DATA_SIZE];
};



struct responseMsg {
    long mtype;
    char mtext[MESSAGE_DATA_SIZE];
};

#define REQ_MESSAGE_SIZE offsetof(struct requestMsg, mtext) - offsetof(struct requestMsg, mtype) + MESSAGE_DATA_SIZE
#define RESP_MESSAGE_SIZE offsetof(struct responseMsg, mtext) - offsetof(struct responseMsg, mtype) + MESSAGE_DATA_SIZE