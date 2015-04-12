#ifndef __FLOP_H__
#define __FLOP_H__

char *path_name;

void append(char* s, char c);
void path();
void path_add(char *str);
void path_sub(char *str);
void change_dir(const char *path);
void trim(char*);
//void print_files(char*, char*, char*);
//void print_directories(unsigned short, char*, unsigned short, unsigned short, unsigned short, char*);
int parse_cmd(char *cmd, char *arg[]);
int is_pipe(char **command);
int is_redirection(char **command);

#endif /* FLOP_H_ */
