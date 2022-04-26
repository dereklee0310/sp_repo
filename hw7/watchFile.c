#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <linux/limits.h>
#include <fcntl.h>

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

bool isMoved(struct inotify_event *event);

bool isModify(struct inotify_event *event);

void getModifyTime(int fd, struct tm *tm);

int main(int argc, char **argv)
{   
    char *eventPtr;
    char *inotify_entity = (char *)malloc(FILE_BUF_LEN);
    char *fileBuf = (char *)malloc(BUF_LEN);
    char *timeBuf = (char *)malloc(BUF_LEN);
    struct tm lastModifyTime, curModifyTime;
    int fd;
    int wd;
    int fileDscptr;
    int readNum;
    
    // create a watch descriptor
    fd = inotify_init();
    wd = inotify_add_watch(fd, argv[1], IN_ALL_EVENTS);
    
    // open the file
    fileDscptr = open(argv[1], O_RDONLY);
    FILE *filePtr = fdopen(fileDscptr, "r");
    
    // initialize modify time
    getModifyTime(fileDscptr, &curModifyTime);
    strftime(timeBuf, 1024, "%F %T", &curModifyTime);
    lastModifyTime = curModifyTime;

    // print the current state and content of the file
    printf("previous modified time:%s\n", timeBuf);
    printf("latest modified time  :%s\n", timeBuf);
    while(fgets(fileBuf, sizeof(fileBuf), filePtr)) {
        printf("%s", fileBuf);
    }
    putchar('\n'); // formatting

    while(true) {
        int readNum = read(fd, inotify_entity, BUF_LEN);
        for (eventPtr = inotify_entity; eventPtr < inotify_entity + readNum; ) {
            if(isMoved((struct inotify_event *)eventPtr)) {
                // reopen the file
                close(fileDscptr);
                fclose(filePtr);
                fileDscptr = open(argv[1], O_RDONLY);
                FILE *filePtr = fdopen(fileDscptr, "r");

                // add it back
                inotify_rm_watch(fd, wd);
                // close(fd);
                // fd = inotify_init();
                int wd = inotify_add_watch(fd, argv[1], IN_ALL_EVENTS);
            }

			if(isModify((struct inotify_event *)eventPtr)) {
                strftime(timeBuf, 1024, "%F %T", &lastModifyTime);
                printf("previous modified time:%s\n", timeBuf);
                getModifyTime(fileDscptr, &curModifyTime);
                strftime(timeBuf, 1024, "%F %T", &curModifyTime); // %F: 短的YYYY-MM-DD日期, 相當於%Y-%m-%d | %T: ISO 8601時間格式(HH:MM:SS), 相當於%H:%M:%S
                printf("latest  modified  time:%s\n", timeBuf);
                fseek(filePtr, 0, SEEK_SET); // reset the position indicator
                puts("file content:");
                while(fgets(fileBuf, sizeof(fileBuf), filePtr)) {
                    printf("%s", fileBuf);
                }
                putchar('\n'); // formatting
                lastModifyTime = curModifyTime;
            }
			eventPtr += sizeof(struct inotify_event) + ((struct inotify_event *)eventPtr)->len;
		}
    }

    free(inotify_entity);
    free(fileBuf);
    free(timeBuf);
    fclose(filePtr);
    close(fileDscptr);
    close(fd);

    return 0;
}

bool isMoved(struct inotify_event *event) {
    if(event->mask & IN_MOVE_SELF) {
        // puts("move");
        return true;
    }

    /* test for operations of vim */
    // if(event->mask & IN_DELETE_SELF) {
    //     puts("delete in ismoved");
    //     return true;
    // }
    // if(event->mask & IN_IGNORED) {
    //     puts("ignored in ismoved");
    //     return true;
    // }
    return false;
}

// only check the modification of content
bool isModify(struct inotify_event *event)
{   
    /* origin */ 
    // if((event->mask & IN_CLOSE_WRITE) || (event->mask & IN_MODIFY))
    //     return true;

    /* normal modify */
    if(event->mask & IN_MODIFY) {
        // puts("modify in ismodify");
        return true;
    }

    /* for vim */
    if(event->mask & IN_MOVE_SELF) {
        // puts("move in ismodify");
        return true;
    }
    return false;
}

void getModifyTime(int fd, struct tm *tm) {
	struct stat statbuf;
	struct tm t;
	fstat(fd, &statbuf);
	tzset();
	localtime_r(&(statbuf.st_mtime), tm);
}
