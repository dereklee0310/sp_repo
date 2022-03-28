#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/types.h>
#include <unistd.h>

#define DEFAULT_SIZE 1024

void setLineBuf(FILE *src, FILE *dest, char *srcBuf, char *destBuf);

void setNoBuf(FILE *src, FILE *dest);

void setFullBuf(FILE *src, FILE *dest, char *srcBuf, char *destBuf, int bufSize);

int main(int argc, char *argv[])
{   
    if(argc != 4) {
        fprintf(stderr, "number of arguments incorrect");
        exit(1);
    }


    FILE *src = fopen(argv[1], "r");
    FILE *dest = fopen(argv[2], "w+");
    int bufSize = atoi(argv[3]);

    char *srcBuf = NULL;
    char *destBuf = NULL;
    switch(bufSize) {
        case -1:
            setLineBuf(src, dest, srcBuf, destBuf);
            break;
        case 0:
            setNoBuf(bufSize);
            break;
        default:
            setFullBuf(src, dest, srcBuf, destBuf, bufSize);
            break;
    }

    char wordBuf[80];

    while(true) {

    }

    free(srcBuf);
    free(destBuf);
    fclose(src);
    fclose(dest);

    return 0;
}

void setLineBuf(FILE *src, FILE *dest, char *srcBuf, char *destBuf)
{
    srcBuf = (char *)malloc(sizeof(char) * DEFAULT_SIZE);
    destBuf = (char *)malloc(sizeof(char) * DEFAULT_SIZE);
    setvbuf(src, srcBuf, _IOLBF, DEFAULT_SIZE);
    setvbuf(dest, destBuf, _IOLBF, DEFAULT_SIZE);
}

void setNoBuf(FILE *src, FILE *dest)
{
    setvbuf(src, NULL, _IONBF, DEFAULT_SIZE);
    setvbuf(dest, NULL, _IONBF, DEFAULT_SIZE);
}

void setFullBuf(FILE *src, FILE *dest, char *srcBuf, char *destBuf, int bufSize)
{   
    srcBuf = (char *)malloc(sizeof(char) * bufSize);
    destBuf = (char *)malloc(sizeof(char) * bufSize);
    setvbuf(src, buf, _IOFBF, bufSize);
    setvbuf(dest, buf, _IOFBF, bufSize);
}