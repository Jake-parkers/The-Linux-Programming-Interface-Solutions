#include <fcntl.h>
#include "../tlpi_hdr.h"

int dup(int oldfd) {
    if (fcntl(oldfd, F_GETFL) == -1) {
        errno = EBADF;
        return -1;
    }

    int fd = fcntl(oldfd, F_DUPFD);
    if (fd == -1) return -1;
    return fd;
}

int dup2(int oldfd, int newfd) {
    printf("Old = %d, new = %d\n", oldfd, newfd);
    if (fcntl(oldfd, F_GETFL) == -1) {
        errno = EBADF;
        return -1;
    }

    if (oldfd == newfd) return newfd;

    close(newfd);

    int fd = fcntl(oldfd, F_DUPFD, newfd);
    if (fd == -1) return -1;
    return fd;
}

int main(int argc, char* argv[]) {
    int fd;
    #define BUF_SIZE 64
    char buf[BUF_SIZE] = "Active users.";

    fd = open(argv[1], O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    int duplicatedFd = dup2(fd, 8);
    if (duplicatedFd == -1) errExit("dup");
    printf("Dup = %d", duplicatedFd);
    if (write(duplicatedFd, buf, BUF_SIZE) == -1) errExit("write");

    if (lseek(duplicatedFd, 10, SEEK_SET) == -1) errExit("lseek");

    if (write(fd, buf, BUF_SIZE - 10) == -1) errExit("write");

    close(duplicatedFd);

    exit(EXIT_SUCCESS);
}