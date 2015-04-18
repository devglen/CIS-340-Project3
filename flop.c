#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include "flop.h"

void append(char* s, char c)
{
    int len = strlen(s);
    s[len] = c;
    s[len+1] = '\0';
}

void path()
{
    if(path_name == NULL){ 
       path_name = (char *) malloc(20); 
    }

    fprintf(stdout, "path: %s",path_name);
}

void path_add(char* str)
{
    if(path_name == NULL){ 
       path_name = (char *) malloc(20); 
    }	
    char c = ':';
    if(strlen(path_name) == 0){
    } else {
        append(path_name,c);
    }
    strcat(path_name,str);
    fprintf(stdout, "path: %s",path_name);
}

void path_sub(char* str)
{
    if(path_name == NULL){ 
       path_name = (char *) malloc(20); 
    }
    int i;
    char f[50] = ":";
    char c[50] = "";
    if (path_name[1] != str[1]) {
        strcat(c,f);
        strcat(c,str);
    } else {
        strcat(c,str);
    }

    if (strstr(path_name, c) != NULL) {
        char *search = strstr(path_name,c);
        char *end = search + strlen(c);
        char beginning[60] = "";
        for (i=0; i<((strlen(path_name) - (strlen(c) + strlen(end)))); i++){
            beginning[i] = path_name[i];
        }
        strcat(beginning,end);
        strcpy(path_name,beginning);
        fprintf(stdout, "path: %s",path_name);
    } else {
        fprintf(stdout, "\nstring not found in path!");
    }
}

void change_dir(const char *path)
{
    chdir(path);
}

int parse_cmd(char *cmd, char *arg[]) {
	int a = 0, b = 0, c = 0;

	while (cmd[a] != '\n') {
		if (cmd[a] != ' ') {
			arg[c][b] = cmd[a];
			b++;
		} else {
			if (b) {
				arg[c][b] = '\0';
				c++;
				b = 0;
			}
		}
		a++;
	}
	arg[c][b] = '\0';
	arg[c+1] = NULL;
	if (b | c) {
		return c+1;
	}
	else
		return c;
}

int is_pipe(char **command) {
	char **cmd = command;
	while (*cmd != NULL) {
		if (!strcmp(*cmd, "|")) {
			return 1;
		}
		cmd++;
	}
	return 0;
}

int is_redirection(char **command) {
	char **cmd = command;
	while (*cmd != NULL) {
		if (!strcmp(*cmd, ">")) {
			return 1;
		}
		cmd++;
	}
	return 0;
}
