#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define __USE_GNU
#include <sched.h>
#include <pthread.h>

#define ADJUSTMENT  5
#define ALARM_TIME  5

unsigned long long cnt = 0;

void sigHandler(int sig);

int main() {
    pid_t pid = fork();
    cpu_set_t mask;
    cpu_set_t get;

    CPU_ZERO(&mask); // reset
    CPU_SET(0, &mask); // add cpu 0 to set

    // let the process run on cpu 0
    if(sched_setaffinity(0, sizeof(mask), &mask)) {
        fprintf(stderr, "fail to set cpu affinity");
        return EXIT_FAILURE;
    }

    if(0 == pid) {
        // child
        nice(ADJUSTMENT);
        printf("cnt of child:  ");
    } else {
        // parent
        printf("cnt of parent: ");
    }

    signal(SIGALRM, sigHandler);
    alarm(ALARM_TIME);
    
    while(true)
        cnt++;

    return 0;
}

void sigHandler(int sig)
{
    printf("%10llu\n", cnt); // width: 10, align: right 
    exit(EXIT_SUCCESS);
}


/*  
 * reference:
 * https://blog.csdn.net/Z_Stand/article/details/107883684
 * https://www.cntofu.com/book/46/linux_system/linuxxi_tong_bian_cheng_zhi_xin_hao_ff08_san_ff09_.md
 * https://tw.gitbook.net/unix_system_calls/alarm.html
 */