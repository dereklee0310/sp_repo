#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>

#include "my_lib.h" //  header file


// broke!
// reference:https://stackoverflow.com/questions/36054052/is-using-strtok-in-two-functions-safe
// use space ' ' to parse the given string
/*
char **parseString(char *string)
{   
    char *delimiter = NULL;
    char **result = NULL;
    char **resultPtr = result;
    char *strPtr = NULL;
    unsigned int wordCnt;

    if(!(delimiter = (char *)malloc(sizeof(char) * strlen(" \t\n")))) {
        fprintf(stderr, "! fail to malloc a buffer, in function: parseString()");
        exit(EXIT_FAILURE);
    }
    strcpy(delimiter, " \t\n");

    fprintf(stderr, "t1");

    // calculate the number of words
    strPtr = string;
    wordCnt = 1;
    while(*(strPtr + 1)) {
        if(!strchr(delimiter, *(strPtr))) {
            strPtr++;
            continue;
        }
        if(strchr(delimiter, *(strPtr + 1))) {
            wordCnt++;
        }
        strPtr++;
    }

    fprintf(stderr, "t2");


    if(!(resultPtr = (char **)malloc(sizeof(char *) * wordCnt))) {
        fprintf(stderr, "! fail to malloc a buffer, in function: parseString()");
        exit(EXIT_FAILURE);
    }

    char tmp[] = "1 2 3 4 5\n";
    strPtr = strtok(tmp, delimiter);
    fprintf(stderr, "t2_1");
    
    while(strPtr) {
        if(!(*resultPtr = (char *)malloc(sizeof(char) * strlen(strPtr)))) {
            fprintf(stderr, "! fail to malloc a buffer, in function: parseString()");
            exit(EXIT_FAILURE);
        }
        fprintf(stderr, "tb");
        strcpy(*resultPtr++, strPtr);
        fprintf(stderr, "tf");
        strPtr = strtok(NULL, delimiter);
    }

    fprintf(stderr, "t3");


    free(delimiter);

    return result;
}
*/
// if there is a new line at the end of given string, remove it
void rmNewLine(char *string)
{   
    // string complementary span
    // reference: https://stackoverflow.com/questions/41572713/string-length-with-fgets-function-in-c
    string[strcspn(string, "\n")] = '\0';
}

/*

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

*/

// read the file and return it as a string
char *readFile(char *filename, size_t bufSize)
{
    // use open(), read(), and close() to manipulate the files
    char *buf = NULL;
    char *content = NULL;
    int fileFd;
    int inNum;
    size_t fileSize;

    if(!(buf = (char *)malloc(sizeof(char) * bufSize + 1))) {
        fprintf(stderr, "! fail to malloc a buffer, in function: readFile()");
        exit(EXIT_FAILURE);
    }

    fileFd = open(filename, O_RDONLY);
    if(-1 == fileFd) {
        fprintf(stderr, "! the file doesn't exist, filename: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    fileSize = lseek(fileFd, 0, SEEK_END);
    lseek(fileFd, 0, SEEK_SET);
    
    if(!(content = (char *)malloc(sizeof(char) * fileSize + 1))) {
        fprintf(stderr, "! fail to malloc a buffer, in function: readFile()");
        exit(EXIT_FAILURE);
    }

    while((inNum = read(fileFd, buf, bufSize)) > 0) {
        *(buf + inNum) = '\0';
        strcat(content, buf);
    }

    free(buf);
    close(fileFd);

    return content;
}

// write the content to a destination file
// if the file we choose to write doesn't exist, create it
void writeFile(char *filename, size_t bufSize)
{   
    // use open(), write(), and close() to manipulate the files
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

// copy the file content to a destination file (hole support version)
// if the file we choose to write doesn't exist, create it
void copyHoleFile(char *inFilename, char *outFilename, size_t bufSize)
{
    // use open(), read(), write(), and close() to manipulate the files
    char *buf = NULL;
    int inputFd, outputFd;
    int inNum, outNum;
    size_t fileSize;
    size_t blockSize;
    off_t blockSt = 0, blockEnd = 0;

    if(!(buf = (char *)malloc(sizeof(char) * bufSize + 1))) {
        fprintf(stderr, "! fail to malloc a buffer, in function: copyHoleFile()");
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

    fileSize = lseek(inputFd, 0, SEEK_END);
    lseek(inputFd, 0, SEEK_SET);
    
    while(true) {
        blockSt = lseek(inputFd, blockSt, SEEK_DATA);
        blockEnd = lseek(inputFd, blockEnd, SEEK_HOLE);
        blockSize = blockEnd - blockSt;
        lseek(inputFd, blockSt, SEEK_SET);
        lseek(outputFd, blockSt, SEEK_SET);
        while((inNum = read(inputFd, buf, bufSize)) > 0) {
            outNum = write(outputFd, buf, inNum);
            if(inNum != outNum) {
                fprintf(stderr, "! numbers of readed and writed bytes doesn't match\n");
                fprintf(stderr, "! current readed bytes: %d\n", inNum);
                fprintf(stderr, "! current writed bytes: %d\n", outNum);
            }
            blockSize -= inNum;
            if(blockSize == 0) {
                break;
            }
        }
        if(lseek(outputFd, 0, SEEK_CUR) == fileSize) {
            break;
        }
    }

    free(buf);
    close(inputFd);
    close(outputFd);
}


void showFile(char *filename, size_t bufSize)
{
    // use open(), read(), and close() to manipulate the files
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