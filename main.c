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
        printf("\nflop: ");
        char input[50], command[50], arg[50], mod[50];
        char *const args[10], *const env[10];

        fgets(input, 50, stdin); //read line
        sscanf(input, "%s", command); //separate out first arg, consider using strtok() instead of sscanf()

        if (!strcmp("exit", command)) {
            printf("Exiting the floppy disk shell... \n");
            return EXIT_SUCCESS;
        } else if (!strcmp("path", command)) {
            sscanf(input, "%s %s %s", command, arg, mod);
            if ((strchr(arg,'+') == NULL) && (strchr(arg,'-') == NULL)) {
                path();
            } else if (!strcmp("+", arg)) {
                if(strchr(mod,'#') == NULL){
                    path_add((const char*)mod);
                    strcpy(arg,"");
                } else {
                    printf("Error invalid argument, please try again! \n");
                }
            } else if (!strcmp("-", arg)) {
                if(strchr(mod,'#') == NULL){
                    path_sub((const char*)mod);
                    strcpy(arg,"");
                } else{
                    printf("Error invalid argument, please try again! \n");
                }
            } else {
                printf("Error invalid argument, please try again! \n");
            }
        } else if (!strcmp("help", command)) {
            execve("./help", args, env);
        } else if (!strcmp("fmount", command)) {
            sscanf(input, "%s %s", command, arg);
            //fmount((const char*) arg);
        } else if (!strcmp("fumount", command)) {
            //fumount(fd);
        } else if (!strcmp("structure", command)) {
            execve("./structure", args, env);
        } else if (!strcmp("traverse", command)) {
            if (strstr(input, "-l") == NULL) {
                execve("./traverse", args, env);
            } else if (strstr(input, "-l") != NULL) {
                sscanf(input, "%s %s", command, arg);
                execve("./traverse", args, env);
            } else {
                printf("Error invalid argument, please try again! \n");
            }
        } else if (!strcmp("showsector", command)) {
            sscanf(input, "%s %s", command, arg);
            execve("./show_sector", args, env);
        } else if (!strcmp("showfat", command)) {
            execve("./show_fat", args, env);
        } else {
            printf("Error: invalid command\n");
        }
    }

    return 0;
}
