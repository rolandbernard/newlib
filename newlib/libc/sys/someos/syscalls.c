
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/time.h>
#include <sys/unistd.h>

typedef enum {
    SYSCALL_PRINT = 0,
    SYSCALL_EXIT = 1,
    SYSCALL_YIELD = 2,
    SYSCALL_FORK = 3,
    SYSCALL_SLEEP = 4,
    SYSCALL_OPEN = 5,
    SYSCALL_LINK = 6,
    SYSCALL_UNLINK = 7,
    SYSCALL_RENAME = 8,
    SYSCALL_CLOSE = 9,
    SYSCALL_READ = 10,
    SYSCALL_WRITE = 11,
    SYSCALL_SEEK = 12,
    SYSCALL_STAT = 13,
    SYSCALL_DUP = 14,
    SYSCALL_TRUNC = 15,
    SYSCALL_CHMOD = 16,
    SYSCALL_CHOWN = 17,
    SYSCALL_MOUNT = 18,
    SYSCALL_UMOUNT = 19,
    SYSCALL_EXECVE = 20,
    SYSCALL_READDIR = 21,
    SYSCALL_GETPID = 22,
    SYSCALL_GETPPID = 23,
    SYSCALL_WAIT = 24,
    SYSCALL_SBRK = 25,
    SYSCALL_PROTECT = 26,
} Syscalls;

static inline uintptr_t make_syscall(
    uintptr_t _kind, uintptr_t _a1, uintptr_t _a2, uintptr_t _a3, uintptr_t _a4, uintptr_t _a5, uintptr_t _a6
) {
    register uintptr_t kind asm("a0") = _kind;
    register uintptr_t a1 asm("a1") = _a1;
    register uintptr_t a2 asm("a2") = _a2;
    register uintptr_t a3 asm("a3") = _a3;
    register uintptr_t a4 asm("a4") = _a4;
    register uintptr_t a5 asm("a5") = _a5;
    register uintptr_t a6 asm("a6") = _a6;
    register uintptr_t result asm("a0");
    asm(
        "ecall"
        : "=r" (result)
        : "0" (kind), "r" (a1), "r" (a2), "r" (a3), "r" (a4), "r" (a5), "r" (a6)
    );
    return result;
}

#define CONSOME(...)
#define KEEP(...) __VA_ARGS__

#define IFN(...) CONSOME __VA_OPT__(()KEEP)

#define IFE(...) KEEP __VA_OPT__(()CONSOME)

#define Z6(...) IFE(__VA_ARGS__)(0, 0, 0, 0, 0, 0) IFN(__VA_ARGS__)(Z5(__VA_ARGS__))
#define Z5(A1, ...) A1, IFE(__VA_ARGS__)(0, 0, 0, 0, 0) IFN(__VA_ARGS__)(Z4(__VA_ARGS__))
#define Z4(A1, ...) A1, IFE(__VA_ARGS__)(0, 0, 0, 0) IFN(__VA_ARGS__)(Z3(__VA_ARGS__))
#define Z3(A1, ...) A1, IFE(__VA_ARGS__)(0, 0, 0) IFN(__VA_ARGS__)(Z2(__VA_ARGS__))
#define Z2(A1, ...) A1, IFE(__VA_ARGS__)(0, 0) IFN(__VA_ARGS__)(Z1(__VA_ARGS__))
#define Z1(A1, ...) A1, IFE(__VA_ARGS__)(0) IFN(__VA_ARGS__)(__VA_ARGS__)

#define SYSCALL(KIND, ...) make_syscall(KIND, Z6(__VA_ARGS__))

static inline uintptr_t handleErrors(uintptr_t error) {
    // TODO: set errno
    return (int)error < 0 ? -1 : error;
}

void _exit() {
    for (;;) {
        SYSCALL(SYSCALL_EXIT);
    }
}

int close(int file) {
    return handleErrors(SYSCALL(SYSCALL_EXIT));
}

char **environ;

int execve(const char *name, char* const argv[], char* const env[]) {
    return handleErrors(SYSCALL(SYSCALL_EXECVE, (uintptr_t)argv, (uintptr_t)env));
}

int fork() {
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
} SyscallStat;

int fstat(int file, struct stat *st) {
    SyscallStat stat;
    int error = handleErrors(SYSCALL(SYSCALL_STAT, file, (uintptr_t)&stat));
    st->st_ino = stat.id;
    st->st_mode = stat.mode;
    st->st_nlink = stat.nlinks;
    st->st_uid = stat.uid;
    st->st_gid = stat.gid;
    st->st_size = stat.size;
    st->st_blksize = stat.size;
    st->st_blocks = stat.size / stat.block_size;
    st->st_atim.tv_nsec = 0;
    st->st_atim.tv_sec = stat.atime;
    st->st_mtim.tv_nsec = 0;
    st->st_mtim.tv_sec = stat.mtime;
    st->st_ctim.tv_nsec = 0;
    st->st_ctim.tv_sec = stat.ctime;
    st->st_dev = 0;
    st->st_rdev = 0;
    return error;
}

int getpid() {
    return handleErrors(SYSCALL(SYSCALL_GETPID));
}

int getppid() {
    return handleErrors(SYSCALL(SYSCALL_GETPPID));
}

int isatty(int file) {
    // TODO
    return -1;
}

int kill(int pid, int sig) {
    // TODO
    return -1;
}

int link(const char *old, const char *new) {
    return handleErrors(SYSCALL(SYSCALL_LINK, (uintptr_t)old, (uintptr_t)new));
}

typedef enum {
    FILE_SEEK_CUR = 0,
    FILE_SEEK_SET = 1,
    FILE_SEEK_END = 2,
} SyscallSeekWhence;

off_t lseek(int file, off_t ptr, int dir) {
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

typedef enum {
    FILE_OPEN_CREATE = (1 << 0),
    FILE_OPEN_APPEND = (1 << 1),
    FILE_OPEN_TRUNC = (1 << 2),
    FILE_OPEN_DIRECTORY = (1 << 3),
    FILE_OPEN_READ = (1 << 4),
    FILE_OPEN_WRITE = (1 << 5),
    FILE_OPEN_EXECUTE = (1 << 6),
    FILE_OPEN_REGULAR = (1 << 7),
    FILE_OPEN_CLOEXEC = (1 << 8),
    FILE_OPEN_EXCL = (1 << 9),
} SyscallOpenFlags;

int open(const char *name, int flags, ...) {
    int mode = 0;
    SyscallOpenFlags sys_flags = 0;
    if ((flags & O_CREAT) != 0) {
        sys_flags |= FILE_OPEN_CREATE;
        va_list args;
        va_start(args, flags);
        mode = va_arg(args, int);
        va_end(args);
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
    return handleErrors(SYSCALL(SYSCALL_OPEN, (uintptr_t)name, sys_flags, mode));
}

ssize_t read(int file, void* ptr, size_t len) {
    return handleErrors(SYSCALL(SYSCALL_READ, file, (uintptr_t)ptr, len));
}

void* sbrk(ptrdiff_t incr) {
    return (void*)handleErrors(SYSCALL(SYSCALL_SBRK, incr));
}

int stat(const char *file, struct stat *st) {
    int fd = open(file, 0);
    int result = fstat(fd, st);
    close(fd);
    return result;
}

clock_t times(struct tms *buf) {
    // TODO
    return -1;
}

int unlink(const char *name) {
    return  handleErrors(SYSCALL(SYSCALL_UNLINK, (uintptr_t)name));
}

int wait(int *status) {
    return  handleErrors(SYSCALL(SYSCALL_WAIT, (uintptr_t)status));
}

ssize_t write(int file, const void* ptr, size_t len) {
    return handleErrors(SYSCALL(SYSCALL_READ, file, (uintptr_t)ptr, len));
}

int gettimeofday(struct timeval* __p, void* __tz) {
    // TODO
    return -1;
}

