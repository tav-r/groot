#ifndef HOOKS_H
#define HOOKS_H

enum hooked_functions {
    LIBC_READDIR = 0,
    LIBC_CHDIR = 1,
};

int (*real_chdir)(const char *);
struct dirent *(*real_readdir)(DIR *);

/*
 * Resolve symbol by given enum value
 */
void
lookup_sym(int index)
{
    switch(index) {
        case LIBC_READDIR:
            real_readdir = dlsym(RTLD_NEXT, "readdir");
            break;
        case LIBC_CHDIR:
            real_chdir = dlsym(RTLD_NEXT, "chdir");
            break;
        default:
            return;
    }
}

#define NUM_HOOKED_FUN 2
#endif
