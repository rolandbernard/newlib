#ifndef _SYS_DIRENT_H
#define _SYS_DIRENT_H

typedef struct _dirdesc {
    int dd_fd;
    long dd_loc;
    long dd_size;
    char *dd_buf;
    int dd_len;
    long dd_seek;
} DIR;

# define __dirfd(dp) ((dp)->dd_fd)

#include <sys/types.h>

struct dirent {
    size_t d_ino;
    size_t d_off;
    size_t d_reclen;
    int d_type;
    char d_name[1];
};

#endif
