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
    fprintf(stdout, "path: %s",path_name);
}

void path_add(char* str)
{
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

void trim(char *str)
{
    int i;
    int begin = 0;
    int end = strlen(str) - 1;

    while (isspace(str[begin])) {
        begin++;
    }
    while ((end >= begin) && isspace(str[end])) {
        end--;
    }
    // Shift all characters back to the start of the string array.
    for (i = begin; i <= end; i++) {
        str[i - begin] = str[i];
    }
    str[i - begin] = '\0'; // Null terminate string.
}
/*
void print_files(char *buf, char *directory, char *flag)
{
    cluster = (((unsigned short) buf[26]) & 0xff) | (((unsigned short) buf[27]) & 0x0f)<<8;
    char filename[256], file[9], extension[4];
    unsigned short date, time, month, day,  year, hour, minute, second;
    char attributes[] = "----";
    unsigned filesize;

    strncpy(file, buf, 8);
    strncpy(extension, buf+8, 3);
    file[8] = '\0';
    extension[3] = '\0';
    trim(file);
    trim(extension);
    strcpy(filename, directory);
    strcat(filename, file);
    if (extension[0] != '\0') {
        strcat(filename, ".");
        strcat(filename, extension);
    }

    if (strcmp("-l", flag) == 0) {
        time = ((unsigned short) buf[22] & 0xff) | ((unsigned short) buf[23] & 0xff)<<8;
        date = ((unsigned short) buf[24] & 0xff) | ((unsigned short) buf[25] & 0xff)<<8;
        second = (time & 0x1f) % 60;
        minute = time >> 5 & 0x3f;
        hour = time >> 11 & 0x1f;
        day = date & 0x1f;
        month = date >> 5 & 0xf;
        year = (date >> 9 & 0x7f) + 1980;

        if (buf[11] & 0x20) {
            attributes[0] = 'A';
        }
        if (buf[11] & 0x1) {
            attributes[1] = 'R';
        }
        if (buf[11] & 0x2) {
            attributes[2] = 'H';
        }
        if (buf[11] & 0x4) {
            attributes[3] = 'S';
        }

        fprintf(stdout, "-%-9s", attributes);
        fprintf(stdout, "%2.2d/%2.2d/%4d %2.2d:%2.2d:%-10.2d", month, day, year, hour, minute, second);
        if (buf[11]&0x10) {
            fprintf(stdout, "%-10s     ", "<DIR>");
        } else {
            filesize = (buf[31] & 0xff) << 24 | (buf[30] & 0xff) << 16 | (buf[29] & 0xff) << 8 | (buf[28] & 0xff);
            fprintf(stdout, "%10d     ", filesize);
        }
        fprintf(stdout, "%-40s", filename);
        fprintf(stdout, "%10d \n", cluster);
    } else {
        fprintf(stdout, "%-40s", filename);
        if (buf[11] & 0x10) {
            fprintf(stdout, "\t%s", "<DIR>");
        }
        fprintf(stdout, "\n");
    }
}

void print_directories(unsigned short cluster, char *directory, unsigned short file_bytes,
                       unsigned short num_of_sectors, unsigned short bytes_per_sector, char *flag)
{
    char buf[32], dirname[9], single_dir[256], extension[4];
    unsigned short new_cluster, low, high;
    unsigned ptr;
    int counter = 0;

    ptr = (file_bytes + (cluster - 2)*num_of_sectors)*bytes_per_sector;
    buf[0] = '$';
    strcpy(single_dir, directory);
    if ((lseek(fd, ptr, 0)) != ptr) {
        fprintf(stdout, "Error setting the file pointer to beginning\n");
        exit(1);
    }

    if ((read(fd, buf, sizeof(buf))) != sizeof(buf)) {
        fprintf(stdout, "Error reading file entry\n");
        exit(1);
    }

    while (buf[0] != 0x0) {
        if (counter++ >= num_of_sectors*bytes_per_sector/32) {
            if (cluster%2) {
                low = (((unsigned short) fat_buffer[(3*cluster - 1)/2])>>4) & 0x000f;
                high = (((unsigned short) fat_buffer[(3*cluster + 1)/2])<<4) & 0x0ff0;
            } else {
                low = ((unsigned short) fat_buffer[3*cluster/2]) & 0x00ff;
                high = (((unsigned short) fat_buffer[(3*cluster + 2)/2])<<8) & 0x0f00;
            }
            cluster = low | high;
            if (cluster == 0x0 || cluster > 0xff0) {
                break;
            } else {
                counter = 0;
                ptr = (file_bytes + (cluster - 2)*num_of_sectors)*bytes_per_sector;
                if ((lseek(fd, ptr, SEEK_SET)) != ptr) {
                    fprintf(stdout, "There was a problem moving the pointer \n");
                    exit(1);
                }
                if ((read(fd, buf, sizeof(buf))) != sizeof(buf)) {
                    fprintf(stdout, "Error reading file entry\n");
                    exit(1);
                }
                continue;
            }
        }

        // exclude free space and removed files
        if (isprint((unsigned short) (buf[0] & 0xff)) && (unsigned short) (buf[0] & 0xff) != 0xe5 && (unsigned short) (buf[11]&0x08) != 0x08) {
            print_files(buf, directory, flag);
            if (buf[11]&0x10) {
                new_cluster = (((unsigned short) buf[26]) & 0xff) | (((unsigned short) buf[27]) & 0x0f) << 8;
                strncpy(dirname, buf, 8);
                strncpy(extension, buf+8, 3);
                dirname[8] = '\0';
                extension[3] = '\0';
                trim(dirname);
                trim(extension);
                if (strcmp(dirname, ".") && strcmp(dirname, "..")) {
                    strcat(single_dir, dirname);
                    if (extension[0] != '\0') {
                        strcat(single_dir, ".");
                        strcat(single_dir, extension);
                    }
                    strcat(single_dir, "/");
                    print_directories(new_cluster, single_dir, file_bytes, num_of_sectors, bytes_per_sector, flag);
                    // reset pointer
                    lseek(fd, ptr + counter * 32, SEEK_SET);
                }
            }
        }
        if ((read(fd, buf, sizeof(buf))) != sizeof(buf)) {
            fprintf(stdout, "There was a problem reading the next file/entry\n");
            exit(1);
        }
    }
}

void traverse(char* flag)
{
    unsigned short sectors, root_bytes, file_bytes, new_clust;
    char buf[32], directory[256], file[9], extension[4];
    int i;

    if ((lseek(fd, SEEK_SET, SEEK_SET)) != 0) {
        fprintf(stdout, "There was a problem setting pointer to beginning of file\n");
        exit(1);
    }

    if ((read(fd, buf, sizeof(buf)) != sizeof(buf))) {
        fprintf(stdout, "Error reading from floppy disk, try again.\n");
        exit(1);
    }

    sectors = num_of_root_dir * 32 / bytes_per_sector;
    root_bytes = sectors_per_fat * num_of_fats + 1;
    file_bytes = root_bytes + sectors;

    strcpy(directory, "/");

    if (strcmp("-l", flag) == 0) {
        fprintf(stdout, " *****************************\n");
        fprintf(stdout, " ** FILE ATTRIBUTE NOTATION **\n");
        fprintf(stdout, " ** **\n");
        fprintf(stdout, " ** R ------ READ ONLY FILE **\n");
        fprintf(stdout, " ** S ------ SYSTEM FILE **\n");
        fprintf(stdout, " ** H ------ HIDDEN FILE **\n");
        fprintf(stdout, " ** A ------ ARCHIVE FILE **\n");
        fprintf(stdout, " *****************************\n\n");
    }

    for (i = 0;i < num_of_root_dir; i++) {
        // go to root entry
        if ((lseek(fd, root_bytes * bytes_per_sector + i * 32, 0))
                != root_bytes * bytes_per_sector + i * 32) {
            fprintf(stdout, "There was an error reading entries in floppy \n");
            exit(1);
        }

        if ((read(fd, buf, sizeof(buf))) != sizeof(buf)) {
            fprintf(stdout, "There was an error reading directory \n");
            exit(1);
        }

        // dont process deleted files or free space
        if (((unsigned short) (buf[0]&0xff) == 0xe5) || ((unsigned short) (buf[0]&0xff) == 0x0) || ((unsigned short) (buf[11]&0x08) == 0x08)) {
            continue;
        }

        print_files(buf, directory, flag);
        if (buf[11] & 0x10) {
            new_clust = (((unsigned short) buf[26]) & 0xff) | (((unsigned short) buf[27]) & 0x0f) << 8;
            strncpy(file, buf, 8);
            strncpy(extension, buf+8, 3);
            file[8] = '\0';
            extension[3] = '\0';
            trim(file);
            trim(extension);
            strcat(directory, file);
            if (extension[0] != '\0') {
                strcat(directory, ".");
                strcat(directory, extension);
            }
            strcat(directory, "/");
            print_directories(new_clust, directory, file_bytes, sectors_per_cluster, bytes_per_sector, flag);
            // go back to root dir
            strcpy(directory, "/");
        }
    }
}
*/
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
