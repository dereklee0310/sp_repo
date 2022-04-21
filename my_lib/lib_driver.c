#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#include "my_lib.h"

#define BUF_SIZE 1024

int main(int argc, char **argv) {
    char buf[BUF_SIZE];
    char inFilename[BUF_SIZE];
    char outFilename[BUF_SIZE];

    puts("This is the driver of my_lib");
    puts("----------------------------------------");

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
    puts("recieve EOF, terminate writeFile()");
    
    puts("----------------------------------------");

    puts("*now testing showFile() to show the content of file: input...*");
    sleep(3);

    showFile("input", BUF_SIZE);

    puts("----------------------------------------");

    puts("*now calling copyFile() copy input to output...*");
    sleep(3);

    copyFile("input", "output", BUF_SIZE);

    puts("----------------------------------------");

    puts("*now testing showFile() to show the content of file: output...*");
    sleep(3);

    showFile("output", BUF_SIZE);
    
    puts("----------------------------------------");

    puts("process end! :)");

    return 0;
}