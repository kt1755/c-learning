#include <stdio.h>
#include <unistd.h>
#include "../lib/error_fuctions.h"
#include <sys/syscall.h> /* Definition of SYS_* constants */
#define MAX_READ 20;

using namespace std;

int main(int argc, char const *argv[])
{

    char buffer[MAX_READ];
    if (read(STDIN_FILENO, buffer, MAX_READ) == -1)
        errExit("read");
    printf("The input data was: %s\n", buffer);
}
