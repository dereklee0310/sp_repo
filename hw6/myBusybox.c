#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>

#define STR_LEN    1024

int main(int argc, char **argv)
{   
    char command[STR_LEN] = "";
    char *exename = basename(argv[0]);

    // fprintf(stderr, "exename: %s\n", exename);

    strncat(command, exename, strlen(exename));

    for(unsigned int i = 1; i < argc; i++){ 
        strcat(command, " ");
        strncat(command, argv[i] , strlen(argv[i]));
    }

    system(command);

    // fprintf(stderr, "command: %s\n", command);

    return 0;
}