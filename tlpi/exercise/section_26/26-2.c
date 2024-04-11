// Answer: grandchild get ppid = 1 ngay khi parent become zombies

#include <signal.h>
#include <libgen.h>             /* For basename() declaration */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define CMD_SIZE 200

int
main(int argc, char *argv[])
{
    char cmd[CMD_SIZE];
    pid_t childPid, parentId;

    setbuf(stdout, NULL);       /* Disable buffering of stdout */

    printf("Parent PID=%ld\n", (long) getpid());

    switch (parentId = fork()) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
        

    case 0:     /* Child: immediately exits to become zombie */
        /* Generate one more child */        
        
        switch (childPid = fork())
        {
        case -1:
            perror("fork in child");
            exit(EXIT_FAILURE);
            break;

        case 0: /*Child*/
            
            sleep(3); //Give time for parent to become zombies
            printf("Child (PID=%ld) have parent (PID=%ld)\n", (long) getpid(), (long) getppid());
            sleep(3); // Give grandparent chance to call wait to reclaim parent
            printf("Child (PID=%ld) have parent (PID=%ld)\n", (long) getpid(), (long) getppid());
        
        default:
            // Immediatety terminated, become zombies process
            printf("Parent terminated\n");
            _exit(EXIT_SUCCESS);
        }

    default:    /* Grandparent */
        sleep(5); //Give time for parent to become zombies
        printf("Grandparent call wait\n");
        wait(NULL);
    }
    _exit(EXIT_SUCCESS);
}