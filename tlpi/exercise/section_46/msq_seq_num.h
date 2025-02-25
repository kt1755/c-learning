#include <stdio.h>
#include <stdlib.h>
 #include <stddef.h>

#define SERVER_KEY 0x1aaaaa
#define REQ_MESSAGE_SIZE 1024

struct requestMsg {
    long mtype;
    char mtext[REQ_MESSAGE_SIZE];
};



struct responseMsg {
    long mtype;
    char mtext[REQ_MESSAGE_SIZE];
};

