#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/types.h>
#include <unistd.h>
#define SIZE 8

int main(int argc, char *argv[])
{
    char buf[SIZE];
    int curNum;

    int fd = open("./flock.db", O_RDWR, S_IRUSR | S_IWUSR);

    // lock
    int ret = flock(fd, LOCK_EX);
    if (ret != 0)
        perror("error occured when using flock:");

    for (int i = 0; i < 1000; i++)
    {   
        // wait for 0.001s
        usleep(1000);

        // read the current number
        lseek(fd, -4, SEEK_END); 
        read(fd, buf, 4);

        // convert to integer, move forward and write a new number
        sscanf(buf, "%d", &curNum);
        lseek(fd, curNum, SEEK_END);
        sprintf(buf, "%d", curNum + 1);
        write(fd, buf, 4);
    }

    // unlock
    flock(fd, LOCK_UN);
    close(fd);

    return 0;
}
