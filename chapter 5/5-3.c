/*
This exercise is designed to demonstrate why the atomicity guaranteed by opening a file with the O_APPEND flag is necessary. Write a program that takes up to three command-line arguments:
$ atomic_append filename num-bytes [x]
This file should open the specified filename (creating it if necessary) and append num-bytes bytes to the file by using write() to write a byte at a time. By default, the program should open the file with the O_APPEND flag, but if a third command-line argument (x) is supplied, then the O_APPEND flag should be omitted, and instead the
program should perform an lseek(fd, 0, SEEK_END) call before each write(). Run two instances of this program at the same time without the x argument to write 1 million bytes to the same file:
$ atomic_append f1 1000000 & atomic_append f1 1000000
Repeat the same steps, writing to a different file, but this time specifying the x
argument:
$ atomic_append f2 1000000 x & atomic_append f2 1000000 x
List the sizes of the files f1 and f2 using ls –l and explain the difference.

Ans: 
-rw-------  1 joshuaboateng  staff  1000000 Oct 12 14:01 f1
-rw-------  1 joshuaboateng  staff  2000000 Oct 12 14:02 f2

The file written to without O_APPEND is half the size of that written to with it. This is because when running $atomic_append f1 1000000 & atomic_append f1 1000000, the first process is interuptted between calls to lseek and write so as for the second process to be scheduled to run. Now when this happens both processes set their offset to the same position, hence the second process ends up overwriting the data written by the first process every time it runs

*/
#include <fcntl.h>
#include "../tlpi_hdr.h"

int main(int argc, char* argv[]) {
    int fd;

    if (argc < 3) {
        usageErr("%s filename num-bytes [x]", argv[0]);
    }

    char* filepath = argv[1];
    int num_bytes = atoi(argv[2]);
    int use_append_flag = 1;

    if (argv[3] == NULL) use_append_flag = 0;

    fd = open(filepath, O_CREAT | O_RDWR | (use_append_flag ? O_APPEND: 0), S_IWUSR | S_IRUSR);

    for (size_t i = 0; i < num_bytes; i++)
    {
        if (use_append_flag) {
            if(lseek(fd, 0, SEEK_END) == -1)  
                errExit("lseek");
        }
        if (write(fd, &i, 1) != 1) errExit("write");
    }
    

}