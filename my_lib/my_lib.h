#ifndef MY_LIB_H
#define MY_LIB_H

// alias of special character (char)
// #define SPACE ' ' 
// #define NEWLINE '\n'
// #define NULL_CHARACTER '\0'

#define SWAP(x, y) int tmp = x; x = y; y = tmp;

// char **parseString(char *string);

void rmNewLine(char *string);

// char *numerToMode(unsigned int modeNumber);

// char *getTimeStr(void);

char *readFile(char *filename, size_t bufSize);

void writeFile(char *filename, size_t bufSize);

void copyFile(char *inFilename, char *outFilename, size_t bufSize);

void copyHoleFile(char *inFilename, char *outFilename, size_t bufSize);

void showFile(char *filename, size_t bufSize);

#endif