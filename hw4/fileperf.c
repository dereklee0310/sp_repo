#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define DEFAULT_SIZE 1024
#define TMP_BUF_SIZE 4096
#define MAX_LINE_SIZE 80

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
    // set buf to NULL, the system will automatically allocate it
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

    char buf[TMP_BUF_SIZE];
    char nextChar;
    int len = 0;
    int pos = 0;

    while(~fscanf(src, "%s", buf)) {
        nextChar = fgetc(src);
        len = strlen(buf);
        if (pos + len > MAX_LINE_SIZE) {
            fputc('\n', dest); // jump into next line
            pos = 0; // reset the current position in line 
        }
        fprintf(dest, "%s", buf);
        if (nextChar != EOF)  // ignore EOF
            fputc(nextChar, dest);
            
        pos += len + 1; // move forward because whitespace
        if (nextChar == '\n') // reset the current position in line 
            pos = 0;
    }

    fclose(src);
    fclose(dest);

    return 0;
}