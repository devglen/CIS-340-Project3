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

int main()
{
	int i, redirect_bool, pipe_bool, fd_rdr;
	char **cmd;
	char **cmmnd;
	char **pipe_command;
	char buf[512];
	int command_bytes;
	int pipe_fd[2];

    while (1) {
        char arg[50], mod[50];

        fprintf(stdout, "\nflop: ");
        fflush(stdout);

        // reset pipe and redirect values
        pipe_bool = 0;
        redirect_bool = 0;

        cmd = (char **)malloc(20*sizeof(char *));
		for (i = 0; i < 20; i++) {
			cmd[i] = (char *) malloc(128);
		}

		if ((command_bytes = read(0, buf, 512)) < 0) {
			fprintf(stdout, "There was an issue reading the command, please try again.\n");
			break;
		}

		buf[command_bytes] = '\0';
		if (parse_cmd(buf, cmd)) {
			if (strcmp("exit", cmd[0]) == 0) {
				fprintf(stdout, "Exiting the floppy disk shell... \n");
                                free(cmd);
                                exit(1);
			}

            pipe_bool = is_pipe(cmd);
			redirect_bool = is_redirection(cmd);

			if (strcmp("path", cmd[0]) == 0) {
				if (cmd[1])
					strcpy(arg,cmd[1]);
				else
					strcpy(arg, "");
				if (cmd[2])
					strcpy(mod,cmd[2]);
				else strcpy(mod, "");
				if ((strchr(arg,'+') == NULL) && (strchr(arg,'-') == NULL)) {
					path();
					continue;
				} else if (!strcmp("+", arg)) {
					if(strchr(mod,'#') == NULL){
						path_add(mod);
						strcpy(arg,"");
						continue;
					} else {
						fprintf(stdout, "Error invalid argument, please try again! \n");
						continue;
					}
				} else if (!strcmp("-", arg)) {
					if(strchr(mod,'#') == NULL){
						path_sub(mod);
						strcpy(arg,"");
						continue;
					} else{
						fprintf(stdout, "Error invalid argument, please try again! \n");
					}
				} else {
					fprintf(stdout, "Error invalid argument, please try again! \n");
				}
			} else if (!strcmp("cd", cmd[0])) {
				change_dir((const char*)cmd[1]);
				continue;
			}
                        

			if (pipe_bool > 0) {
                                // copy command string to not alter original 
				cmmnd = cmd;
                                // iterate over command to find pipe portion
				while (*cmmnd != NULL) {
					if (!strcmp(*cmmnd, "|")) {
						pipe_command = cmmnd + 1;
						*cmmnd = NULL;
						break;
					}
					cmmnd++;
				}
                                // iterate over command string to find redirection portion
				if (redirect_bool > 0) {
					cmmnd = pipe_command;
					while (*cmmnd != NULL) {
						if (!strcmp(*cmmnd, ">")) {
							if ((fd_rdr = open(*(cmmnd+1), (O_WRONLY | O_CREAT | O_TRUNC), 0644)) < 0)
								fprintf(stdout, "There was an error creating file for redirection\n");
							*cmmnd = NULL;
							break;
						}
						cmmnd++;
					}
				}
				if (*pipe_command == NULL || pipe_command[0][0] == '\0') {
					fprintf(stdout, "There was an error in the pipe command\n");
					fflush(stdout);
					continue;
				}
				if ((pipe(pipe_fd)) != 0)
					fprintf(stdout, "There was an error creating the pipe\n");

				if ((fork()) == 0) {
					close(1);
                                        dup(pipe_fd[1]);
                                        close(pipe_fd[1]);
                                        close(pipe_fd[0]);
                                        // execute command 
					execve(cmd[0], cmd, NULL);
					
                                        // above command was not found so try path_name environment
					char* temp;
					char* token;
					temp = path_name; 
					token = strtok(path_name,":");
					while(token != NULL){
						char *slash = "/";
						path_name = temp;
						//add command to end of each path to search
						strcat(token,slash);
						strcat(token,cmd[0]);
						execl(token,cmd[0],cmd[1],cmd[2],NULL);
						token = strtok(NULL,":");
					}
					
					fprintf(stdout, "There was an invalid command in string\n");
				}
				if ((fork()) == 0) {
					close(0);
                                        dup(pipe_fd[0]);
                                        close(pipe_fd[0]);
                                        close(pipe_fd[1]);
					if (redirect_bool) {
                                            close(1);
                                            dup(fd_rdr);
                                            close(fd_rdr);
                                        }
                                        
                                        // execute pipe 
					execvp(pipe_command[0], pipe_command);
                                        
					// above command was not found so try path_name environment
					char* temp;
					char* token;
					temp = path_name; 
					token = strtok(path_name,":");
					while(token != NULL){
						char *slash = "/";
						path_name = temp;
						//add command to end of each path to search
						strcat(token,slash);
						strcat(token,cmd[0]);
						execl(token,cmd[0],cmd[1],cmd[2],NULL);
						token = strtok(NULL,":");
					}
					
					fprintf(stdout, "There was an error in pipe command\n");
				}
				close(pipe_fd[0]);
                                close(pipe_fd[1]);
				wait(NULL);
                                wait(NULL);
			}
			else {
                            // pipe was not found
				if (redirect_bool > 0) {
                                        // copy command to not alter original string
					cmmnd = cmd;
                                        // find redirect file name and create file for writing.
					while (*cmmnd != NULL) {
						if (!strcmp(*cmmnd, ">")) {
							if ((fd_rdr = open(*(cmmnd+1), (O_WRONLY | O_CREAT | O_TRUNC), 0644)) < 0)
								fprintf(stdout, "There was an issue creating redirect file\n");
							*cmmnd = NULL;
							break;
						}
						cmmnd++;
					}
				}
				if ((fork()) == 0) {
					if (redirect_bool) {
						close(1);
                                                dup(fd_rdr);
                                                close(fd_rdr);
					}
					execve(cmd[0], cmd, NULL);
					
                                        // above command was not found so try path_name environment
					char* temp;
					char* token;
					temp = path_name; 
					token = strtok(path_name,":");
					while(token != NULL){
						char *slash = "/";
						path_name = temp;
						//add command to end of each path to search
						strcat(token,slash);
						strcat(token,cmd[0]);
						execl(token,cmd[0],cmd[1],cmd[2],NULL);
						token = strtok(NULL,":");
					}
					
					fprintf(stdout, "There was an invalid command in command string.\n");
				}
				if (redirect_bool)
					close(fd_rdr);
				wait(NULL);
			}
		}
        free(cmd);
    }

    return 0;
}
