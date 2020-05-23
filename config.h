#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>
#include <string.h>

/* List of filepaths to hide from *readlink*. The files will be
 * hidden, so for example, if you hide /path/to/dir/ it will
 * still be possible to list the contents of /path/to/dir. To
 * hide a whole directory, use the HIDDEN_DIRS array below.
 */
const char *HIDDEN_FILES[] = {
};

/* List of directories to hide from *readlink*
 */
const char *HIDDEN_DIRS[] = {
};

#define HIDDEN_FILES_NUM (sizeof(HIDDEN_FILES) / sizeof(char *))
#define HIDDEN_DIRS_NUM (sizeof(HIDDEN_DIRS) / sizeof(char *))

#endif
