#include <stdio.h>
#include<stdlib.h>
#include <string.h>

extern char **environ;

int main(int argc, char *argv[])
{

}

int execlp_impl(const char *filename, const char *arg, .../* , (char *) NULL */) {
    char *path = NULL;
    if ((path = strstr(filename, "/")) != NULL) { // Filename contain slash, which is path
        if (strcmp(filename, path) == 0) {// Is absolute path

        } else { // Is relative path

        }
    } else {
        // Search file in $PATH
        char *pathEnv = getenv("PATH");
        if pathEnv == NULL {
            // escape
            perror("Không tìm thấy PATH env");
            return EXIT_FAILURE
        }

        char *ePath = strtok(pathEnv, ":");
        char *fullEnvPath;
        while(ePath != NULL) {
            fullEnvPath = strcat(filename, ePath);
        }
    }
}