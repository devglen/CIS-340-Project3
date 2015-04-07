#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "flop.h"

int main(void)
{
    while (1) {
        printf("\n flop: ");
        char input[50], command[50], arg[50];
        char *const args[10], *const env[10];

        fgets(input, 50, stdin); //read line
        sscanf(input, "%s", command); //separate out first arg, consider using strtok() instead of sscanf()

        if (strcmp("exit", command) == 0) {
            printf("Exiting the floppy disk shell... \n");
            return EXIT_SUCCESS;
        } else if (strcmp("help", command) == 0) {
            execve("./help", args, env);
        } else if (strcmp("fmount", command) == 0) {
            sscanf(input, "%s %s", command, arg);
            //fmount((const char*) arg);
        } else if (strcmp("fumount", command) == 0) {
            //fumount(fd);
        } else if (strcmp("structure", command) == 0) {
            execve("./structure", args, env);
        } else if (strcmp("traverse", command) == 0) {
            if (strstr(input, "-l") == NULL) {
                execve("./traverse", args, env);
            } else if (strstr(input, "-l") != NULL) {
                sscanf(input, "%s %s", command, arg);
                execve("./traverse", args, env);
            } else {
                printf("Error invalid argument, please try again! \n");
            }
        } else if (strcmp("showsector", command) == 0) {
            sscanf(input, "%s %s", command, arg);
            execve("./show_sector", args, env);
        } else if (strcmp("showfat", command) == 0) {
            execve("./show_fat", args, env);
        } else {
            printf("Error: invalid command\n");
        }
    }

    return 0;
}
