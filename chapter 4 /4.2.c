/**
 * Write a program like cp that, when used to copy a regular file that contains holes
(sequences of null bytes), also creates corresponding holes in the target file.
 */ 

#include "../tlpi_hdr.h"
#include <fcntl.h>
#include <sys/stat.h>

#ifndef MY_BUF_SIZE
#define MY_BUF_SIZE 1024
#endif
int main(int argc, char *argv[]) {

    int inputFd, outputFd;
    mode_t filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    ssize_t numRead, numWritten;
    char buffer[MY_BUF_SIZE];
    char zeroes[MY_BUF_SIZE];
    if (argc < 3 && strcmp(argv[1], "--help") == 0) {
        usageErr("hcp file-1 file-2");
    }

    inputFd = open(argv[1], O_RDONLY, filePerms);

    if (inputFd == -1) {
        errExit("open file1");
    }
    outputFd = open(argv[2], O_CREAT | O_WRONLY, filePerms);
    if (outputFd == -1) {
        errExit("open file2");
    }
    while((numRead = read(inputFd, buffer, MY_BUF_SIZE)) > 0) {
        // check if block read contains file holes
        if (memcmp(buffer, zeroes, numRead) == 0) {
            // holes found. don't copy, just advance the file descriptor. In essence write past the end of the new file to create the hole
            lseek(outputFd, numRead, SEEK_CUR);
        } else {
            // no holes, so copy as normal
            if (write(outputFd, buffer, numRead) != numRead) {
                errExit("write file2");
            }
        }
    }
}