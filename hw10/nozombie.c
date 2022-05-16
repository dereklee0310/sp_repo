/*
usage: ./NoZombie 10000
*/
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

void manyChild(int num) {
    int i, pid;
    for (int i = 0; i < num; i++) {
        pid = fork();
        if (pid == 0) {
            /***************
             * modify here *
             ***************/
            sleep(i + 1); // i from 0 to 99
            exit(0);
        } else {
            continue;
        }
    }
}

int main(int argc, char** argv) {
    int pid, num;
    sscanf(argv[1], "%d", &num);
    pid = fork();
    //child會執行manyChild，child產生了『孫子」以後，child就會死掉
    //child死掉以後，「孫子就沒有父親」，UNIX會自動將這些沒有父親的process
    //轉到 init 或者 systemd 門下，init或systemd都會不斷的執行wait
    //因此可以確保孫子不會變成殭屍
    if (pid == 0) {
        manyChild(num);
        exit(0);
    } else {
        /***************
         * modify here *
         ***************/
        int wstatus;
        wait(&wstatus);
    }
    getchar();
}