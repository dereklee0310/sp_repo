#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define SPACE ' ' // character for strtok?
#define NEWlINE '\n'
#define NULL_CHARACTER '\0'

#define SWAP(x, y) int tmp = x; x = y; y = tmp;

// use space ' ' to parse the given string
char *parseString(char *string)
{
    char *result = malloc(sizeof(char *) * strlen(string));
    char strPtr = NULL;

    strPtr = strtok(string, SPACE);
    while(strPtr) {
        strncat(result, strPtr, sizeof(strPtr));
        strPtr = strtok(NUll, SPACE);
    }
    return result;
}

// if there is a new line at the end of given string, remove it
void rmNewLine(char *string)
{
    size_t pos = strlen(string) - 1;
    if(string[pos] == NEWlINE)
        string[pos] = NULL_CHARACTER;
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

