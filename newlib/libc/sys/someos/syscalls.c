
#include <stddef.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/time.h>
#include <sys/unistd.h>

#include "sys/dirent.h"
#include "syscalls.h"

int _close(int file) {
    return handleErrors(SYSCALL(SYSCALL_CLOSE, file));
}

int _execve(const char *name, char* const argv[], char* const env[]) {
    return handleErrors(SYSCALL(SYSCALL_EXECVE, (uintptr_t)name, (uintptr_t)argv, (uintptr_t)env));
}

int _fork() {
    return handleErrors(SYSCALL(SYSCALL_FORK));
}

typedef struct {
    size_t id;
    uint16_t mode;
    size_t nlinks;
    int uid;
    int gid;
    size_t size;
    size_t block_size;
    uint64_t atime;
    uint64_t mtime;
    uint64_t ctime;
    size_t dev;
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

int _getpid() {
    return handleErrors(SYSCALL(SYSCALL_GETPID));
}

int getppid() {
    return handleErrors(SYSCALL(SYSCALL_GETPPID));
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

int _open(const char *name, int flags, int mode) {
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
    return handleErrors(SYSCALL(SYSCALL_OPEN, (uintptr_t)name, sys_flags, mode));
}

ssize_t _read(int file, void* ptr, size_t len) {
    return handleErrors(SYSCALL(SYSCALL_READ, file, (uintptr_t)ptr, len));
}

void* _sbrk(ptrdiff_t incr) {
    return (void*)handleErrors(SYSCALL(SYSCALL_SBRK, incr));
}

int _stat(const char *file, struct stat *st) {
    int fd = open(file, 0);
    int result = fstat(fd, st);
    close(fd);
    return result;
}

clock_t _times(struct tms *buf) {
    return handleErrors(SYSCALL(SYSCALL_TIMES, (uintptr_t)buf));
}

int _unlink(const char *name) {
    return handleErrors(SYSCALL(SYSCALL_UNLINK, (uintptr_t)name));
}

int waitpid(int pid, int* status, int flags) {
    return handleErrors(SYSCALL(SYSCALL_WAIT, pid, (uintptr_t)status));
}

int _wait(int* status) {
    return waitpid(0, status, 0);
}

ssize_t _write(int file, const void* ptr, size_t len) {
    return handleErrors(SYSCALL(SYSCALL_WRITE, file, (uintptr_t)ptr, len));
}

int gettimeofday(struct timeval* __p, void* __tz) {
    // TODO
    return -1;
}

int nanosleep(const struct timespec* time, struct timespec* rem) {
    size_t left = SYSCALL(SYSCALL_SLEEP, time->tv_sec * 1000000000UL + time->tv_nsec);
    rem->tv_sec = left / 1000000000UL;
    rem->tv_nsec = left % 1000000000UL;
    if ((int)left < 0) {
        return -1;
    } else {
        return 0;
    }
}

int pipe(int filedes[2]) {
    return handleErrors(SYSCALL(SYSCALL_PIPE, (uintptr_t)filedes));
}

int gethostname(char* name, size_t size) {
    // TODO
    return -1;
}

int dup3(int src, int dst, int flags) {
    return handleErrors(SYSCALL(SYSCALL_DUP, src, dst, flags));
}

int dup2(int src, int dst) {
    return dup3(src, dst, 0);
}

int dup(int fildes) {
    return dup2(fildes, -1);
}

int _isatty(int file) {
    // TODO
    return -1;
}

int ioctl(int fildes, int request, ...) {
    // TODO
    return -1;
}

int fcntl(int fildes, int request, ...) {
    // TODO
    return -1;
}

int pause() {
    return handleErrors(SYSCALL(SYSCALL_PAUSE));
}

unsigned alarm(unsigned seconds) {
    return handleErrors(SYSCALL(SYSCALL_ALARM, seconds));
}

uid_t getuid() {
    return handleErrors(SYSCALL(SYSCALL_GETUID));
}

gid_t getgid() {
    return handleErrors(SYSCALL(SYSCALL_GETGID));
}

int setuid(uid_t uid) {
    return handleErrors(SYSCALL(SYSCALL_SETUID, uid));
}

int setgid(gid_t gid) {
    return handleErrors(SYSCALL(SYSCALL_SETGID, gid));
}

