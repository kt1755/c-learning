#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>

#define BLOCK_SIZE 10

char *uppercase(char *str)
{
    for (size_t i = 0; str[i] != '\0'; i++)
    {
        if (str[i] >= 'a' && str[i] <= 'z')
        {
            str[i] = str[i] - 32;
        }
    }

    return str;
}

int main(int argc, char const *argv[])
{
    int parentToChild[2];
    int childToParent[2];

    char *writeBuffer = malloc(BLOCK_SIZE * sizeof(char));

    char *parrentReadBuffer = malloc(BLOCK_SIZE * sizeof(char));

    if (pipe(parentToChild) == -1)
    {
        perror("open pipe parentToChild failed");
        exit(EXIT_FAILURE);
    }

    if (pipe(childToParent) == -1)
    {
        perror("open pipe parentToChild failed");
        exit(EXIT_FAILURE);
    }

    switch (fork())
    {
    case -1:
        perror("fork failed");
        exit(EXIT_FAILURE);
        break;

    case 0: //
        close(parentToChild[1]);
        close(childToParent[0]);

        while (1 == 1)
        {
            if (read(parentToChild[0], writeBuffer, BLOCK_SIZE) == -1)
            {
                perror("Reading from parent failed\n");
                exit(EXIT_FAILURE);
            }

            char *converted = uppercase(writeBuffer);
            if (write(childToParent[1], converted, BLOCK_SIZE) == -1)
            {
                perror("Error write back to parent\n");
                exit(EXIT_FAILURE);
            }
        }

        break;

    // close(childToParent[1]);
    default:
        break;
    }

    // Parent jump to here
    close(parentToChild[0]);
    close(childToParent[1]);

    while (1 == 1)
    {
        if (read(STDIN_FILENO, writeBuffer, BLOCK_SIZE) == -1)
        {
            perror("Reading from stdin failed\n");
            exit(EXIT_FAILURE);
        }

        if (write(parentToChild[1], writeBuffer, BLOCK_SIZE) == -1)
        {
            perror("Write to child failed\n");
            exit(EXIT_FAILURE);
        }

        if (read(childToParent[0], parrentReadBuffer, BLOCK_SIZE) == -1)
        {
            perror("Read back from child failed\n");
            exit(EXIT_FAILURE);
        }

        write(STDOUT_FILENO, parrentReadBuffer, BLOCK_SIZE);

        printf("End loop, wait again\n");
    }

    return 0;
}
