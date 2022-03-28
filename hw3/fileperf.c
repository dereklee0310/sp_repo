#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/types.h>
#include <unistd.h>

#define DEFAULT_SIZE 1024

void setBuf(FILE *src, FILE *dest, int bufSize);

int main(int argc, char *argv[])
{   
    if(argc != 4) {
        fprintf(stderr, "number of arguments incorrect");
        exit(1);
    }
    FILE *src = fopen(argv[1], "r");
    FILE *dest = fopen(argv[2], "w+");
    int bufSize = atoi(argv[3]);

    if(bufSize < -1) {
        fprintf(stderr, "size of buf needs to be greater or equal to -1");
        exit(1);
    }
    setBuf(bufSize);

    char wordBuf[80];

    while() {
        
    }

    fclose(src);
    fclose(dest);

    return 0;
}

void setBuf(FILE *src, FILE *dest, int bufSize)
{
    switch(bufSize) {
        case -1:
            setvbuf(src, NULL, _IOLBF, DEFAULT_SIZE);
            setvbuf(dest, NULL, _IOLBF, DEFAULT_SIZE);
            break;
        case 0:
            setvbuf(src, NULL, _IONBF, DEFAULT_SIZE);
            setvbuf(dest, NULL, _IONBF, DEFAULT_SIZE);
            break;
        default:
            setvbuf(src, NULL, _IOFBF, bufSize);
            setvbuf(dest, NULL, _IOFBF, bufSize);
            break;
    }
}