/**
 * The tee command reads its standard input until end-of-file, writing a copy of the input
to standard output and to the file named in its command-line argument. (We show
an example of the use of this command when we discuss FIFOs in Section 44.7.)
Implement tee using I/O system calls. By default, tee overwrites any existing file with
the given name. Implement the –a command-line option (tee –a file), which causes tee
to append text to the end of a file if it already exists. (Refer to Appendix B for a
description of the getopt() function, which can be used to parse command-line
options)
 */ 
#include <fcntl.h>
#include <sys/stat.h>
#include "../tlpi_hdr.h"
#include <limits.h>

void failure(const char* functionName) {
    perror(functionName);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    #define MAX_READ 2
    char buffer[MAX_READ];
    int len;
    ssize_t numRead, numWritten;
    int fd = -1;
    int option;
    int openFlags = O_CREAT | O_WRONLY;
    int appendFlag = O_APPEND;
    int truncateFlag = O_TRUNC;
    mode_t filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

    while((option = getopt(argc, argv, "a")) != -1) {
        switch (option) {
            case 'a'/* constant-expression */:
                /* code */
                fd = open(argv[2], openFlags | appendFlag, filePerms);
                break;
            default:
                fd = open(argv[2], openFlags | truncateFlag, filePerms);
                break;
        }
    }
    // no option was specified. so the second argument should hold the file name
    if (fd < 0)
        fd = open(argv[1], openFlags | truncateFlag, filePerms);

    if (fd == -1)
        failure("open");

    while((numRead = read(STDIN_FILENO, buffer, MAX_READ)) > 0) {
        if (write(STDOUT_FILENO, buffer, numRead) != numRead) 
            failure("write: couldn't write whole buffer to stdout");
        
        if (write(fd, buffer, numRead) != numRead) 
            failure("write: couldn't write whole buffer");
    }
    if(numRead == -1)
        failure("read");
    
    if (close(fd) == -1)
        failure("close");    
}