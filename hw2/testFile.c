#include <stdio.h>
#include <stdlib.h>

#define NUM 100000

int main() {
    FILE *filePtr = fopen("data1", "w");

    if(NULL == filePtr) {
        fprintf(stderr, "open fail!\n");
        exit(1);
    }

    for(int i = 0; i < NUM; i++)
        fprintf(filePtr, "%d\n", rand());
    
    fclose(filePtr);

    return 0;

}