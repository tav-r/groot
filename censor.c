#include "config.h"
#include "censor.h"

extern const char *HIDDEN_FILES[];

bool
ishidden(const char *filename)
{
    int dirplen;

    for (int i = 0; i < HIDDEN_FILES_NUM; i++) {
        if (!strcmp(filename, HIDDEN_FILES[i])) return true;
    }

    for (int i = 0; i < HIDDEN_DIRS_NUM; i++) {
        dirplen = strlen(HIDDEN_DIRS[i]);
        if (!strncmp(filename, HIDDEN_DIRS[i], dirplen)) return true;
    }

    return false;
}
