/*
Write a program that opens an existing file for writing with the O_APPEND flag, and then seeks to the beginning of the file before writing some data. Where does the data appear in the file? Why?

Ans: The Data appears at the end of the file. Why? When a file is opened with the O_APPEND flag, before any write, the file offset is moved to the end of the file. The modification of the file offset and the write operation are performed as a single atomic step.
 */
#include <fcntl.h>
#include "../tlpi_hdr.h"

int main(int argc, char* argv[]) {
    int fd;

    mode_t filePerms = S_IRUSR | S_IWUSR;
    int flags = O_RDWR | O_APPEND;
    #define BUF_SIZE 10
    char str[BUF_SIZE] = "Workplace streettag leaderboard in Balsidon";

    if (argc < 2 || (strcmp(argv[1], "--help") == -1)) {
        usageErr("%s filename", argv[0]);
    }

    fd = open(argv[1],  flags, filePerms);
    
    if (fd == -1) errExit("open");

    if (lseek(fd, 0, SEEK_SET) == -1) {
        errExit("lseek");
    }

    if (write(fd, str, BUF_SIZE) != BUF_SIZE) {
        errExit("write");
    }
    exit(EXIT_SUCCESS);
}