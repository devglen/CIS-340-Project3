#include <stdio.h>

int main()
{
    fprintf(stdout, "The available commands are:\n");
    fprintf(stdout, "    help                    - display a list of available commands.\n");
    fprintf(stdout, "    cd <path>               - change directories. \n");
    fprintf(stdout, "    path [+][-] [path]      - print current path or modify the path. \n");
    fprintf(stdout, "    structure               - list the structure of the floppy disk image.\n");
    fprintf(stdout, "    traverse [-l]           - list the contents in the root directory. Optional -l flag gives a long listing of the root directory.\n");
    fprintf(stdout, "    showsector <sector_num> - show the content of the given sector.\n");
    fprintf(stdout, "    showfat                 - show the content of the FAT table.\n");
    fprintf(stdout, "    exit                    - exit the floppy shell \n");
    return 0;
}
