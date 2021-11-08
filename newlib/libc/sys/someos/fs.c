
#include "syscalls.h"

#include <errno.h>
#include <fcntl.h>
#include <malloc.h>
#include <stdio.h>
#include <sys/dirent.h>
#include <sys/stat.h>

#define MAX_PATH 1024

int mkdir(const char *pathname, mode_t mode) {
    return handleErrors(SYSCALL(SYSCALL_OPEN, pathname, FILE_OPEN_CREATE | FILE_OPEN_EXCL | FILE_OPEN_DIRECTORY, mode));
}

int chdir(const char* path) {
    return handleErrors(SYSCALL(SYSCALL_CHDIR, (uintptr_t)path));
}

char *getcwd(char *buf, size_t size) {
    if (buf == NULL) {
        buf = (char *) malloc(MAX_PATH);
    }
    return handleErrors(SYSCALL(SYSCALL_GETCWD, buf, size));
}

int	mknod(const char *pathname, mode_t mode, dev_t dev) {
    return handleErrors(SYSCALL(SYSCALL_MKNOD, pathname, mode, dev));
}

int	mkfifo(const char *pathname, mode_t mode) {
    return mknod(pathname, mode | S_IFIFO, 0);
}

int getdents(int fd, struct dirent* dp, int count) {
    size_t written = SYSCALL(SYSCALL_READDIR, fd, (uintptr_t)dp, count);
    return handleErrors(written);
}

int chroot(const char *path) {
    // TODO?
    return chdir(path);
}

