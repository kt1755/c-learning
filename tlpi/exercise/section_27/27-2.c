#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>

extern char **environ;

int main(int argc, char *argv[])
{
}

int execlp_impl(const char *filename, const char *arg, ... /* , (char *) NULL */)
{
    // Init args and evp
    va_list ap;
    char c, *s;
    int d
    char *p
    int sizeArg = sizeof(arg) / sizeof()
    va_start(ap, arg) ;
    for (int i = 0; i )
    {
        if arg == NULL {
            break;
        }

        switch (*arg++)
        {
        case 's': /* string */
            s = va_arg(ap, char *);
            printf("string %s\n", s);
            break;
        case 'd': /* int */
            d = va_arg(ap, int);
            printf("int %d\n", d);
            break;
        case 'c': /* char */
            /* need a cast here since va_arg only
               takes fully promoted types */
            c = (char)va_arg(ap, int);
            printf("char %c\n", c);
            break;
        }
    }

    char *path = NULL;
    if ((path = strstr(filename, "/")) != NULL)
    { // Filename contain slash, which is path
    }
    else
    {
        // Search file in $PATH
        char *pathEnv = getenv("PATH");
        if pathEnv
            == NULL
            {
                // escape
                perror("Không tìm thấy PATH env");
                return EXIT_FAILURE
            }

        char *ePath = strtok(pathEnv, ":");
        char *fullEnvPath;
        while (ePath != NULL)
        {
            fullEnvPath = strcat(filename, ePath);
        }
    }

    

    // using execve()
    return execve(NULL, arg, *environ);
}