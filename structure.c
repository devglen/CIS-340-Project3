/*
 * structure.c
 *
 *  Created on: Apr 5, 2015
 *      Author: Glen
 */
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int fd;

int main(int argc, char **argv) {
	unsigned short bytes_per_sector;
	unsigned short num_of_sectors;

	unsigned char sectors_per_cluster;
	unsigned char num_of_fats;
	unsigned short max_root_dir;
	unsigned short sectors_per_fat;
	int dir_entries_per_sector = 16;

	if ((fd = open("imagefile.img", O_RDONLY)) < 0) {
		fprintf(stdout, "There was an error mounting floppy\n");
		exit(1);
	}

	unsigned short low, high;
	char buf[32];
	size_t number_of_bytes;
	ssize_t bytes_read;
	number_of_bytes = sizeof(buf);
	off_t offset;

	offset = lseek(fd, SEEK_SET, SEEK_SET);
	if (offset == -1) {
		fprintf(stdout, "There was an issue reading beginning of floppy.");
		exit(1);
	}

	bytes_read = read(fd, buf, number_of_bytes);
	if (bytes_read != number_of_bytes) {
		fprintf(stdout, "There was an issue reading the floppy.");
		exit(1);
	}

	low = ((unsigned short) buf[11]) & 0xff;
	high = ((unsigned short) buf[12]) & 0xff;
	bytes_per_sector = low | (high << 8);

	low = ((unsigned short) buf[22]) & 0xff;
	high = ((unsigned short) buf[23]) & 0xff;
	num_of_sectors = low | (high << 8);

	offset = lseek(fd, bytes_per_sector, 0);
	if (offset != bytes_per_sector) {
		fprintf(stdout, "There was an issue setting the cursor");
		exit(1);
	}

	int count = 0;
	fprintf(stdout, "Structure of the floppy image: \n");

	lseek(fd,11,SEEK_SET); //skip ignored bytes
	read(fd, &bytes_per_sector, 2);
	read(fd, &sectors_per_cluster, 1);
	lseek(fd,2,SEEK_CUR); //skip reserved sectors
	read(fd, &num_of_fats, 1);
	read(fd, &max_root_dir, 2);
	read(fd, &num_of_sectors, 2);
	lseek(fd,1,SEEK_CUR); //skip ignored byte
	read(fd, &sectors_per_fat,2);

	fprintf(stdout, "\nnumber of fats: \t%d", num_of_fats);
	fprintf(stdout, "\nsectors used by fat: \t%d", sectors_per_fat);
	fprintf(stdout, "\nsectors per cluster: \t%d", sectors_per_cluster);
	fprintf(stdout, "\nnumber of ROOT Entries: \t%d", max_root_dir);
	fprintf(stdout, "\nnumber of bytes per sector: \t%d", bytes_per_sector);
	fprintf(stdout, "\n---Sector #---     ---Sector Types---");
	fprintf(stdout, "\n       %d                  BOOT        ", count);
	for(count = 0;count<num_of_fats;count++){
	fprintf(stdout, "\n    %02d -- %02d              FAT%d", (count * sectors_per_fat) + 1,(1 + count)*sectors_per_fat,count + 1);
	}
	fprintf(stdout, "\n    %d -- %d              ROOT DIRECTORY",sectors_per_fat*num_of_fats+1, max_root_dir/dir_entries_per_sector + num_of_fats*sectors_per_fat);
	
	close(fd);
	return 0;
}
