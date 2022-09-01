
#include <errno.h>
#include <fcntl.h>
#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/dirent.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <unistd.h>

#include "syscalls.h"

#define MAX_PATH 1024

mode_t umask(mode_t mask) {
    return handleErrors(SYSCALL(SYSCALL_UMASK, mask));
}

typedef struct {
    size_t dev;
    size_t id;
    uint16_t mode;
    size_t nlinks;
    int uid;
    int gid;
    size_t rdev;
    size_t size;
    size_t block_size;
    size_t blocks;
    uint64_t atime;
    uint64_t mtime;
    uint64_t ctime;
} SyscallStat;

int _fstat(int file, struct stat *st) {
    volatile SyscallStat stat;
    int error = handleErrors(SYSCALL(SYSCALL_STAT, file, (uintptr_t)&stat));
    if (error == 0) {
        st->st_ino = stat.id;
        st->st_mode = stat.mode;
        st->st_nlink = stat.nlinks;
        st->st_uid = stat.uid;
        st->st_gid = stat.gid;
        st->st_size = stat.size;
        st->st_blksize = stat.size;
        st->st_blocks = stat.size / stat.block_size;
        st->st_atim.tv_nsec = stat.atime % 1000000000UL;
        st->st_atim.tv_sec = stat.atime / 1000000000UL;
        st->st_mtim.tv_nsec = stat.mtime % 1000000000UL;
        st->st_mtim.tv_sec = stat.mtime / 1000000000UL;
        st->st_ctim.tv_nsec = stat.ctime % 1000000000UL;
        st->st_ctim.tv_sec = stat.ctime / 1000000000UL;
        st->st_dev = stat.dev;
        st->st_rdev = stat.id;
    }
    return error;
}

int pipe(int filedes[2]) {
    return handleErrors(SYSCALL(SYSCALL_PIPE, (uintptr_t)filedes));
}

int _isatty(int file) {
    return 1 + handleErrors(SYSCALL(SYSCALL_ISATTY, file));
}

int ioctl(int fildes, int request, ...) {
    va_list args;
    va_start(args, request);
    void* arg = va_arg(args, void*);
    va_end(args);
    return handleErrors(SYSCALL(SYSCALL_IOCTL, fildes, request, (uintptr_t)arg));
}

int fcntl(int fildes, int request, ...) {
    int arg = 0;
    if ((request == F_DUPFD) || (request == F_SETFD) || (request == F_SETFL)) {
        va_list args;
        va_start(args, request);
        arg = va_arg(args, int);
        va_end(args);
    }
    return handleErrors(SYSCALL(SYSCALL_FCNTL, fildes, request, arg));
}

int _close(int file) {
    return handleErrors(SYSCALL(SYSCALL_CLOSE, file));
}

int _link(const char *old, const char *new) {
    return handleErrors(SYSCALL(SYSCALL_LINK, (uintptr_t)old, (uintptr_t)new));
}

typedef enum {
    FILE_SEEK_CUR = 0,
    FILE_SEEK_SET = 1,
    FILE_SEEK_END = 2,
} SyscallSeekWhence;

off_t _lseek(int file, off_t ptr, int dir) {
    SyscallSeekWhence whence = 0;
    if (dir == SEEK_END) {
        whence = FILE_SEEK_END;
    } else if (dir == SEEK_SET) {
        whence = FILE_SEEK_SET;
    } else if (dir == SEEK_CUR) {
        whence = FILE_SEEK_CUR;
    }
    return handleErrors(SYSCALL(SYSCALL_SEEK, file, ptr, whence));
}

static SyscallOpenFlags convertFlagsFor(int flags) {
    SyscallOpenFlags sys_flags = 0;
    if ((flags & O_CREAT) != 0) {
        sys_flags |= FILE_OPEN_CREATE;
    }
    if ((flags & O_APPEND) != 0) {
        sys_flags |= FILE_OPEN_APPEND;
    }
    if ((flags & O_TRUNC) != 0) {
        sys_flags |= FILE_OPEN_TRUNC;
    }
    if ((flags & O_DIRECTORY) != 0) {
        sys_flags |= FILE_OPEN_DIRECTORY;
    }
    if ((flags & O_CLOEXEC) != 0) {
        sys_flags |= FILE_OPEN_CLOEXEC;
    }
    if ((flags & O_EXCL) != 0) {
        sys_flags |= FILE_OPEN_EXCL;
    }
    int rw_flags = (flags & 0b11) + 1;
    if ((rw_flags & 0b10) == 0) {
        sys_flags |= FILE_OPEN_RDONLY;
    }
    if ((rw_flags & 0b01) == 0) {
        sys_flags |= FILE_OPEN_WRONLY;
    }
    return sys_flags;
}

int _open(const char *name, int flags, int mode) {
    return handleErrors(SYSCALL(SYSCALL_OPEN, (uintptr_t)name, convertFlagsFor(flags), mode));
}

ssize_t _read(int file, void* ptr, size_t len) {
    return handleErrors(SYSCALL(SYSCALL_READ, file, (uintptr_t)ptr, len));
}

int _stat(const char *file, struct stat *st) {
    int fd = open(file, 0);
    if (fd < 0) {
        return -1;
    }
    int result = fstat(fd, st);
    close(fd);
    return result;
}

int _unlink(const char *name) {
    return handleErrors(SYSCALL(SYSCALL_UNLINK, (uintptr_t)name));
}

ssize_t _write(int file, const void* ptr, size_t len) {
    return handleErrors(SYSCALL(SYSCALL_WRITE, file, (uintptr_t)ptr, len));
}

int dup3(int src, int dst, int flags) {
    if (src == dst) {
        errno = EINVAL;
        return -1;
    }
    return handleErrors(SYSCALL(SYSCALL_DUP, src, dst, (flags & O_CLOEXEC) != 0 ? FILE_OPEN_CLOEXEC : 0));
}

int dup2(int src, int dst) {
    return handleErrors(SYSCALL(SYSCALL_DUP, src, dst, 0));
}

int dup(int fildes) {
    return dup2(fildes, -1);
}

int access(const char* fn, int flags) {
    struct stat s;
    if (stat(fn, &s) != 0) {
        return -1;
    }
    if (
        (flags & R_OK) != 0 && (s.st_mode & S_IREAD) == 0
        || (flags & W_OK) != 0 && (s.st_mode & S_IWRITE) == 0
        || (flags & X_OK) != 0 && (s.st_mode & S_IEXEC) == 0
    ) {
        errno = EPERM;
        return -1;
    }
    return 0;
}

int chdir(const char* path) {
    return handleErrors(SYSCALL(SYSCALL_CHDIR, (uintptr_t)path));
}

char* getcwd(char *buf, size_t size) {
    if (buf == NULL) {
        buf = (char *) malloc(MAX_PATH);
    }
    int res = SYSCALL(SYSCALL_GETCWD, (uintptr_t)buf, size);
    if (res < 0) {
        errno = -res;
        return NULL;
    } else {
        return buf;
    }
}

int	mknod(const char *pathname, mode_t mode, dev_t dev) {
    return handleErrors(SYSCALL(SYSCALL_MKNOD, (uintptr_t)pathname, mode, dev));
}

int mkdir(const char *pathname, mode_t mode) {
    return mknod(pathname, (mode & 0777) | S_IFDIR, 0);
}

int	mkfifo(const char *pathname, mode_t mode) {
    return mknod(pathname, (mode & 0777) | S_IFIFO, 0);
}

int getdents(int fd, struct dirent* dp, int count) {
    size_t written = SYSCALL(SYSCALL_READDIR, fd, (uintptr_t)dp, count);
    return handleErrors(written);
}

// TODO: Implement this after vfs rewrite.
int chroot(const char *path) {
    return chdir(path);
}

#define OPEN_DO_CLOSE(ACTION, FLAGS)    \
    int fd = open(path, FLAGS);         \
    if (fd < 0) {                       \
        return -1;                      \
    }                                   \
    if (ACTION < 0) {                   \
        close(fd);                      \
        return -1;                      \
    }                                   \
    return close(fd);

int	truncate(const char* path, off_t length) {
    OPEN_DO_CLOSE(ftruncate(fd, length), O_WRONLY);
}

int	ftruncate(int fd, off_t length) {
    return handleErrors(SYSCALL(SYSCALL_TRUNC, fd, length));
}

int	chmod(const char* path, mode_t mode) {
    OPEN_DO_CLOSE(fchmod(fd, mode), O_WRONLY);
}

int fchmod(int fd, mode_t mode) {
    return handleErrors(SYSCALL(SYSCALL_CHMOD, fd, mode));
}

int	chown(const char* path, uid_t owner, gid_t group) {
    OPEN_DO_CLOSE(fchown(fd, owner, group), O_WRONLY);
}

int fchown(int fd, uid_t owner, gid_t group) {
    return handleErrors(SYSCALL(SYSCALL_CHOWN, fd, owner, group));
}

int mount(const char* source, const char* target, const char* filesystemtype, unsigned long mountflags, const void* data) {
    return handleErrors(SYSCALL(SYSCALL_MOUNT, (uintptr_t)source, (uintptr_t)target, (uintptr_t)filesystemtype, (uintptr_t)data));
}

int umount(const char* target) {
    return handleErrors(SYSCALL(SYSCALL_UMOUNT, (uintptr_t)target));
}

// TODO: Implement this after vfs rewrite.
int select(int nfds, fd_set* readfs, fd_set* writefds, fd_set* errorfds, struct timeval* timeout) {
    return -1;
}

