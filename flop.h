#ifndef __FLOP_H__
#define __FLOP_H__

int fd;
unsigned short bytes_per_sector;
unsigned short num_of_sectors;
unsigned short sectors_per_cluster;
unsigned short fat_bytes;
unsigned short num_of_fats;
unsigned short num_of_root_dir;
unsigned short sectors_per_fat;
unsigned short cluster;
char *fat_buffer;

void append(char* s, char c);
void path();
void path_add(char *str);
void path_sub(char *str);
void setup_variables();
void fmount(const char *);
void fumount();
void structure();
void traverse(char*);
void show_sector();
void show_fat();
void trim(char*);
void print_files(char*, char*, char*);
void print_directories(unsigned short, char*, unsigned short, unsigned short, unsigned short, char*);

#endif
