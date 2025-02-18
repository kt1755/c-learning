#include <stdio.h>
#include <stdlib.h>

#define SERVER_KEY 0x1aaaaaa1

#define SIZE_OF_MSG = sizeof(int)

struct requestMsg {
    long mtype;
    int seq;
};

struct responseMsg {
    long mtype;
    int seq;
};