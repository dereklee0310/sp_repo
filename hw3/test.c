#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#define MaxLineCnt 80
#define MaxReadBufCnt (1 << 16)
#define MaxBufSize 20971520  // 20MB

void usage(char *fileName);

int main(int argc, char **argv) {
    if (argc != 4) {
        usage(argv[0]);
        exit(1);
    }

    const size_t bufSize = (size_t)atol(argv[3]);

    if ((long long)bufSize < -1 || (long long)bufSize > MaxBufSize) {
        usage(argv[0]);
        exit(1);
    }


    FILE *input = fopen(argv[1], "r");
    assert(input != NULL);
    FILE *output = fopen(argv[2], "w");
    assert(output != NULL);
    unsigned char *wordBuf = calloc(bufSize, sizeof(unsigned char));
    unsigned char readBuf[MaxReadBufCnt];
    unsigned char nextChar;
    int bufLen = 0;
    int linePos = 0;

    if (bufSize == -1) {
        setvbuf(input, NULL, _IOLBF, 1024);
        setvbuf(output, NULL, _IOLBF, 1024);
    } else if (bufSize == 0) {
        setvbuf(input, NULL, _IONBF, 0);
        setvbuf(output, NULL, _IONBF, 0);
    } else {
        setvbuf(input, NULL, _IOFBF, bufSize);
        setvbuf(output, NULL, _IOFBF, bufSize);
    }

    while (~fscanf(input, "%s", readBuf)) {  // scan a word to readBuf, stop at white space and keep white space in input stream.
        nextChar = fgetc(input);  // may be white space or EOF.
        bufLen = strlen(readBuf);
        if (linePos + bufLen > MaxLineCnt) {
            fputc('\n', output);
            linePos = 0;
        }
        fprintf(output, "%s", readBuf);
        if ((char)nextChar != EOF) {  // Don't print EOF
            fputc(nextChar, output);
        }
        linePos += bufLen + 1;  // white space
        if (nextChar == '\n') {
            linePos = 0;
        }
    }
    
    return 0;
}

void usage(char *fileName) {
    fprintf(stderr, "format error!!!\n");
    fprintf(stderr, "%s src dst, bufSize\n", fileName);
    fprintf(stderr, "\nThe default setting of the buffer is _IOFBF.\n");
    fprintf(stderr, "If bufSize = 0, then _IOLBF; if bufSize = -1, then _IONBF, both of the default bufSize is 1KB.\n");
    fprintf(stderr, "The bufSize can't greater than 20MB.\n");
}