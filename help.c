#include <stdio.h>

int main(void)
{
    printf("The available commands are:\n");
    printf("    help                    - display a list of available commands.\n");
    printf("    fmount <file_name>      - mount the specified image file\n");
    printf("    fumount                 - umount the mounted floppy disk. \n");
    printf("    cd <path>               - change directories. \n");
    printf("    path [+][-] <path>      - print current path or modify the path. \n");
    printf("    structure               - list the structure of the floppy disk image.\n");
    printf("    traverse [-l]           - list the contents in the root directory. Optional -l flag gives a long listing of the root directory.\n");
    printf("    showsector <sector_num> - show the content of the given sector.\n");
    printf("    showfat                 - show the content of the FAT table.\n");
    printf("    exit                    - exit the floppy shell \n");
    return 0;
}
