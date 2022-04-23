#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <dirent.h>

/*
    struct inotify_event {
        int      wd;       // Watch descriptor 
        uint32_t mask;     // Mask describing event 
        uint32_t cookie;   // Unique cookie associating related
                            //     events (for rename(2))
        uint32_t len;      // Size of name field 🦐 🦞 🦀 在這裏OS會告訴我們name的長度
        char     name[];   // Optional null-terminated name
    };
*/

//設定每次read最多讀取1000個物件，這裡如果設定太小，可能會有「漏失」某些事件
#define BUF_LEN (1000 * (sizeof(struct inotify_event) + NAME_MAX + 1))
#define FILE_BUF_LEN 4096

bool checkModify(struct inotify_event *event);

int main(int argc, char **argv)
{   
    char *inotify_entity = (char *)malloc(BUF_LEN);
    char *fileBuf = (char *)malloc(FILE_BUF_LEN);
    struct tm lastModifyTime, curModifyTime;
    int fd = inotify_init();
    int wd = inotify_add_watch(fd, argv[1], IN_ALL_EVENTS);
    int fileFd = open(argv[1], O_RDONLY);
    FILE *filePtr = fdopen(fileFd, "r");

    printf("file's last modification time:%s\n", getCurtime()); // arguement!
    printf("file's modified!, modify time:%s\n", getCurtime()); // arguement!
    while(fgets(fileBuf, sizeof(fileBuf), filePtr)) {
        printf("%s", fileBuf);
    }
    putchar('\n'); // formatting

    // get the time

    while(true) {
        int readNum = read(fd, inotify_entity, BUF_LEN);
        for (eventPtr = inotify_entity; eventPtr < inotify_entity + nRead; ) {
			if(!checkModify((struct inotify_event *)eventPtr)) {
                printf("file's last modification time:%s\n", getCurtime()); // arguement!
                printf("file's modified!, modify time:%s\n", getCurtime()); // arguement!
                fseek(filePtr, 0, SEEK_SET);
                while(fgets(fileBuf, sizeof(fileBuf), filePtr)) {
                    printf("%s", fileBuf);
                }
                putchar('\n'); // formatting
            }
			eventPtr += sizeof(IE) + ((IE *)eventPtr)->len;
		}
    }

    free(inotify_entity);

    return 0;
}

bool checkModify(struct inotify_event *event)
{
    if((event->mask & IN_CLOSE_WRITE) || (event->mask & IN_MODIFY))
        return true;
    return false;
}

// void getMTime(int fd, struct tm *tm) {
// 	struct stat statbuf;
// 	struct tm t;
// 	char buf[1024];
// 	fstat(fd, &statbuf);
// 	tzset();
// 	localtime_r(&(statbuf.st_mtime), tm);
// }