#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <grp.h>
#include <sys/wait.h>

#define CHILD   0 // if the pid is 0, it's a child

//! #include <limits.h> 

// remove the left most non-ascii characters
char *ltrim(char *s) {
    while(isspace(*s)) s++;
    return s;
};

// remove the right non-ascii characters
char *rtrim(char *s)
{
    char *back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
};

// trim the given string
char *trim(char *s)
{
    return rtrim(ltrim(s));
};

int main(int argc, char *argv[])
{
    char username[1024];
    char *namePtr;

    char passwordStr[sysconf(_SC_PASS_MAX)];
    struct passwd passwd_ent;
    struct passwd *result;
    struct group *gr;
    char buffer[1024];

    long ngroups_max;
    gid_t gid;
    gid_t groups[sysconf(_SC_NGROUPS_MAX)];
    int nGroup = sysconf(_SC_NGROUPS_MAX);
    int ret;

relogin:
    puts("username:");
    namePtr = fgets(username, 1024, stdin);
    namePtr = trim(namePtr);

    ret = getpwnam_r(namePtr, &passwd_ent, buffer, 1024, &result);
    if(NULL == result) {
        printf("user:%s doesn't exist\n", namePtr);
        goto relogin;
    }

    // passwd
    strncpy(passwordStr, getpass("password:"), sysconf(_SC_PASS_MAX));
    puts("actually, it doesn't matter whether your password is correct :)");
    if(0 != ret) {
        perror("error occured when calling getpwnam_r:");
        goto relogin;
    }

    int pid;
    pid = fork();
    if(CHILD == pid) {
        ret = getgrouplist(namePtr, passwd_ent.pw_gid, groups, &nGroup);
        printf("getgrouplist = %d\n", ret);
        printf("使用者編號: %d\n", passwd_ent.pw_uid);
        printf("使用者名稱: %s\n", passwd_ent.pw_name);
        printf("群組編號：%d\n", passwd_ent.pw_gid);
        printf("家目錄: %s\n", passwd_ent.pw_dir);
        printf("其他訊息 %s\n", buffer);
        printf("所隸屬的所有群組： ");
        printf("共%d個\n", nGroup);
        for (int i=0; i< nGroup; i++) {
            gr = getgrgid(groups[i]);
            printf("%s, ", gr->gr_name);
        }
        printf("\n");

        assert(0 == setgid(passwd_ent.pw_gid));
        assert(0 == chdir(passwd_ent.pw_dir));
        setenv("HOME", passwd_ent.pw_dir, 1);
        setgroups(0, NULL);
        setgroups(sysconf(_SC_NGROUPS_MAX), groups);
        assert(0 == setuid(passwd_ent.pw_uid));
    
        ret = execvp("bash", argv);
    } else {
        int stat;
        wait(&stat);
    }

    goto relogin;
}