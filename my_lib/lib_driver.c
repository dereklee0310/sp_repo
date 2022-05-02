#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <stdbool.h>


#include "my_lib.h"

#define BUF_SIZE 1024


#define NDEBUG // switch

int main(int argc, char **argv) {
    char buf[BUF_SIZE];
    char inFilename[BUF_SIZE];
    char outFilename[BUF_SIZE];
    char *tmp = NULL;
    char **parseResult = NULL;
    puts("This is the driver of my_lib");
    puts("----------------------------------------");

    #ifndef NDEBUG
    puts("*now testing rmNewLine() to copy the content of input to output...*");
    puts("enter a sentence to test the funtionality of rmNewLine():");
    fgets(buf, BUF_SIZE, stdin);
    printf("original  string:%s", buf);
    rmNewLine(buf);
    printf("processed string:%s", buf);
    puts("|this is the end of the string|"); // formatting

    puts("----------------------------------------");

    puts("*now testing writeFile()...*");
    puts("please enter some sentences (Ctrl + D to send an EOF signal):");
    writeFile("input", BUF_SIZE);
    puts("\nrecieve EOF, terminate writeFile()");
    
    puts("----------------------------------------");

    puts("*now testing showFile() to show the content of file: input...*");
    sleep(3);
    showFile("input", BUF_SIZE);

    puts("----------------------------------------");

    puts("*now calling copyFile() to copy input to output...*");
    sleep(3);
    copyFile("input", "output", BUF_SIZE);

    puts("----------------------------------------");

    puts("*now testing readFile() to read the content of file: output...*");
    sleep(3);
    tmp = readFile("output", BUF_SIZE);
    puts("the content of file: output:");
    printf("%s\n", tmp);

    puts("----------------------------------------");

    puts("*now calling copyHoleFile() to copy myHole to myHole2...*");
    sleep(3);
    copyHoleFile("myHole", "myHole2", BUF_SIZE);
    puts("file information of myHole2:");
    system("ls -alhs myHole2");

    puts("----------------------------------------");

    free(tmp);

    #endif
    
    puts("*now calling SWAP() to swap value of a and b...*");
    sleep(3);
    
    int a = 5, b = 3;
    printf("before: a = %d, b = %d\n", a, b);
    SWAP(a, b);
    printf("after : a = %d, b = %d\n", a, b);


    puts("----------------------------------------");

    puts("process end! :)");

    return 0;
}