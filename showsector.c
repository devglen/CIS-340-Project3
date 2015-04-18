
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
	unsigned short sec;
	int arg_count;
	for (argc--, argv++; argc > 0; argc -= arg_count, argv += arg_count) {
		arg_count = 1;
		sec = (unsigned short) atoi(*argv);
	}
	
	unsigned short bytes_per_sector;
	unsigned short num_of_sectors;

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

	int i;
	unsigned char hex[bytes_per_sector];
	fprintf(stdout, "\nhex dump of sector : %d", sec);

	// set up horizontal entry hex values
	fprintf(stdout, "\n");
	fprintf(stdout, "\t 0 \t 1 \t 2 \t 3 \t 4 \t 5 \t 6 \t 7 \t 8 \t 9 \t A \t B \t C \t D \t E \t F");

	lseek(fd,sec*bytes_per_sector,SEEK_SET);
	for(i=0;i<bytes_per_sector;i++) {
		read(fd,&hex[i],1);
		if(i % 16 == 0){
			fprintf(stdout, "\n %x ",i);
		}
		fprintf(stdout, "\t %x ", hex[i]);
	}
	fprintf(stdout, "\n");
	close(fd);
	return 0;
}
