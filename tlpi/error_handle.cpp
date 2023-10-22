#include <stdio.h>
#include <linux/reboot.h> /* Definition of LINUX_REBOOT_* constants */
#include <sys/syscall.h>  /* Definition of SYS_* constants */
#include <unistd.h>       /* Import for sync() */
#include <sys/reboot.h>   /* Import for reboot() */

#define hello printf("Hello world\n");

using namespace std;

int main(int argc, char const *argv[])
{
    /* code */
    hello;

    sync();
    setuid(0);

    int enumber;
    enumber = reboot(LINUX_REBOOT_MAGIC1);
    if (enumber == -1)
    {
        perror("Error happen"); // Wil get error Operation not permitted because of linux permission
    }

    return 0;
}
