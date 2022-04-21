#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "my_lib.h" //  header file

// use space ' ' to parse the given string
char *parseString(char *string)
{
    char *result = malloc(sizeof(char *) * strlen(string));
    char *strPtr = NULL;

    strPtr = strtok(string, " ");
    while(strPtr) {
        strncat(result, strPtr, sizeof(strPtr));
        strPtr = strtok(NULL, " ");
    }
    return result;
}

// if there is a new line at the end of given string, remove it
void rmNewLine(char *string)
{   
    // string complementary span
    // reference: https://stackoverflow.com/questions/41572713/string-length-with-fgets-function-in-c
    string[strcspn(string, "\n")] = '\0';
}

// convert the number representation to mode representation (rwx)
char *numerToMode(unsigned int modeNumber)
{
    char *modeStr;

    return  modeStr;
}

// get the current time in string format
char *getTimeStr(void)
{
    char *timeStr;

    return timeStr;
}

// copy the file content to a destination file
// if the file we choose to write doesn't exist, create it
void copyFile(char *inFilename, char *outFilename, size_t bufSize)
{   
    // use open(), read(), write(), and close() to manipulate the files
    char *buf = NULL;
    int inputFd, outputFd;
    int inNum, outNum;


    if(!(buf = (char *)malloc(sizeof(char) * bufSize + 1))) {
        fprintf(stderr, "! fail to malloc a buffer, in function: copyFile()");
        exit(EXIT_FAILURE);
    }

    inputFd = open(inFilename, O_RDONLY);
    if(-1 == outputFd) {
        fprintf(stderr, "! the file doesn't exist, filename: %s\n", inFilename);
        exit(EXIT_FAILURE);
    }

    // replace O_TRUNC by O_APPEND if you want to append the file content at the end of file.
    outputFd = open(outFilename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if(-1 == outputFd) {
        fprintf(stderr, "! fail to open or create the file, filename: %s\n", outFilename);
        exit(EXIT_FAILURE);
    }
    
    while((inNum = read(inputFd, buf, bufSize)) > 0) {
        outNum = write(outputFd, buf, inNum);
        if(inNum != outNum) {
            fprintf(stderr, "! numbers of readed and writed bytes doesn't match\n");
            fprintf(stderr, "! current readed bytes: %d\n", inNum);
            fprintf(stderr, "! current writed bytes: %d\n", outNum);
        }
    }

    free(buf);
    close(inputFd);
    close(outputFd);

    // use fopen(), fwrite(), fclose() to manipulate the files
}

void showFile(char *filename, size_t bufSize)
{
    // use open(), read(), write(), and close() to manipulate the files
    char *buf = NULL;
    int fileFd;
    int inNum;

    if(!(buf = (char *)malloc(sizeof(char) * bufSize + 1))) {
        fprintf(stderr, "! fail to malloc a buffer, in function: showFile()");
        exit(EXIT_FAILURE);
    }

    fileFd = open(filename, O_RDONLY);
    if(-1 == fileFd) {
        fprintf(stderr, "! the file doesn't exist, filename: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    while((inNum = read(fileFd, buf, bufSize)) > 0) {
        *(buf + inNum) = '\0';
        printf("%s", buf);
    }

    free(buf);
    close(fileFd);
}

// write the content to a destination file
// if the file we choose to write doesn't exist, create it
void writeFile(char *filename, size_t bufSize)
{   
    char *buf = NULL;
    int fileFd;

    if(!(buf = (char *)malloc(sizeof(char) * bufSize + 1))) {
        fprintf(stderr, "! fail to malloc a buffer, in function: writeFile()");
        exit(EXIT_FAILURE);
    }

    // replace O_TRUNC by O_APPEND if you want to append the file content at the end of file.
    fileFd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if(-1 == fileFd) {
        fprintf(stderr, "! fail to open or create the file, filename: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    while(fgets(buf, bufSize, stdin)) {
        write(fileFd, buf, strlen(buf));
    }

    free(buf);
    close(fileFd);
}