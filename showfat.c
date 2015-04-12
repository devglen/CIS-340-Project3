/*
 * showfat.c
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

int main() {
	unsigned short bytes_per_sector;
	unsigned short num_of_sectors;
	unsigned short fatbytes;
	unsigned short cluster;
	char *fat_buffer;

	if ((fd = open("/Users/Glen/Downloads/imagefile.img", O_RDONLY)) < 0) {
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

	fatbytes = bytes_per_sector * num_of_sectors;

	fat_buffer = (char *) malloc(fatbytes);
	offset = lseek(fd, bytes_per_sector, 0);
	if (offset != bytes_per_sector) {
		fprintf(stdout, "There was an issue setting the cursor");
		exit(1);
	}

	if ((bytes_read = read(fd, fat_buffer, fatbytes)) != fatbytes) {
		fprintf(stdout, "There was an issue reading the sector");
		exit(1);
	}

	// set up horizontal entry hex values
	fprintf(stdout, "\n");
	for (int i=0;i<16;i++) {
		fprintf(stdout, "\t %x", i);
	}

	fprintf(stdout, "\n \t \t"); // first two entries are reserved
	for (int x = 2; x < fatbytes*2/3; x++) {
		unsigned short low, high;
		unsigned short temp = (unsigned short) x;
		// print the vertical hex entry values
		if ((x%16) == 0) {
			fprintf(stdout, "\n %x", x);
		}

		if (temp%2) {
				low = (((unsigned short) fat_buffer[(3*temp - 1)/2])>>4) & 0x000f;
				high = (((unsigned short) fat_buffer[(3*temp + 1)/2])<<4) & 0x0ff0;
		}else {
				low = ((unsigned short) fat_buffer[3*temp/2]) & 0x00ff;
				high = (((unsigned short) fat_buffer[(3*temp + 2)/2])<<8) & 0x0f00;
		}
		cluster = low | high;

		if (cluster)
			fprintf(stdout, "\t %x", cluster);
		else
			fprintf(stdout, "\t FREE");
	}

	fprintf(stdout, "\n");
	free(fat_buffer);
	return 0;
}
