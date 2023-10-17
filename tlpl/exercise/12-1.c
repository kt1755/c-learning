#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{

    unsigned int euid;
    char *username;
    username = getlogin();
    printf("Check username: %s\n", username);

    /* code */
    return 0;
}
