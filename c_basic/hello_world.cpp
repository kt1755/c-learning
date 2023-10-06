/*welcome.c (program name)*/
/*The following program displays the message "Welcome to C Programming"*/
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

using namespace std;

int main()
{
    char str[10];
    str[0] = 'A';
    str[1] = 'C';
    printf("%s \n", str);

    return 0;
}