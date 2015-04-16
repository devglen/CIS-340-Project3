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
				return EXIT_SUCCESS;
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
				// TODO: add logic for pipe
				cmmnd = cmd;
				while (*cmmnd != NULL) {	/* break command into two: command and pipecmd */
					if (!strcmp(*cmmnd, "|")) {
						pipe_command = cmmnd + 1;
						*cmmnd = NULL;
						break;
					}
					cmmnd++;
				}
				if (redirect_bool > 0) {
					cmmnd = pipe_command;
					while (*cmmnd != NULL) {
						if (!strcmp(*cmmnd, ">")) {
							if ((fd_rdr = open(*(cmmnd+1), (O_WRONLY | O_CREAT | O_TRUNC), 0644)) < 0)
								fprintf(stdout, "cannot create the redirect file");
							*cmmnd = NULL;
							break;
						}
						cmmnd++;
					}
				}
				if (*pipe_command == NULL || pipe_command[0][0] == '\0') {
					fprintf(stdout, "pipeline command error\n");
					fflush(stdout);
					continue;
				}
				if ((pipe(pipe_fd)) != 0)
                                    fprintf(stdout, "cannot open a pipe");

				if ((fork()) == 0) {
					//close(0); dup(fd); close(fd); /* redirect to map 0 to floppy disk */
					close(1); dup(pipe_fd[1]); close(pipe_fd[1]); close(pipe_fd[0]);
					execve(cmd[0], cmd, NULL);
					fprintf(stdout, "shell command not found");
				}
				if ((fork()) ==0) {
					close(0); dup(pipe_fd[0]); close(pipe_fd[0]); close(pipe_fd[1]);
					if (redirect_bool) {close(1); dup(fd_rdr); close(fd_rdr);}
					execvp(pipe_command[0], pipe_command);
					fprintf(stdout, "pipeline command not found");
				}
				close(pipe_fd[0]); close(pipe_fd[1]);
				wait(NULL); wait(NULL);
			}
			else {
				if (redirect_bool > 0) {
					// TODO: add logic for redirect
					cmmnd = cmd;
					while (*cmmnd != NULL) {
						if (!strcmp(*cmd, ">")) {
							if ((fd_rdr = open(*(cmmnd+1), (O_WRONLY | O_CREAT | O_TRUNC), 0644)) < 0)
								fprintf(stdout, "cannot create the redirect file");
							*cmmnd = NULL;
							break;
						}
						cmmnd++;
					}
				}
				if ((fork()) == 0) {
                                        if (redirect_bool) {
                                            close(1); dup(fd_rdr); close(fd_rdr);
                                        }
					execve(cmd[0], cmd, NULL);
					fprintf(stdout, "shell command is invalid, plesae try again.\n");
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
