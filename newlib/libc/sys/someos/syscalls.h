#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h>
#include <errno.h>

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
    SYSCALL_SIGACTION = 27,
    SYSCALL_SIGRETURN = 28,
    SYSCALL_KILL = 29,
    SYSCALL_GETUID = 30,
    SYSCALL_GETGID = 31,
    SYSCALL_SETUID = 32,
    SYSCALL_SETGID = 33,
    SYSCALL_CHDIR = 34,
    SYSCALL_GETCWD = 35,
    SYSCALL_PIPE = 36,
    SYSCALL_TIMES = 37,
    SYSCALL_PAUSE = 38,
    SYSCALL_ALARM = 39,
    SYSCALL_SIGPENDING = 40,
    SYSCALL_SIGPROCMASK = 41,
    SYSCALL_MKNOD = 42,
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
    asm volatile(
        "ecall;"
        : "=r" (result)
        : "0" (kind), "r" (a1), "r" (a2), "r" (a3), "r" (a4), "r" (a5), "r" (a6)
        : "memory"
    );
    return result;
}

static inline uintptr_t handleErrors(uintptr_t error) {
    if ((int)error < 0 && (int)error >= -35) {
        errno = -error;
        return -1;
    } else {
        return error;
    }
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
    FILE_OPEN_RDONLY = (1 << 10),
    FILE_OPEN_WRONLY = (1 << 11),
} SyscallOpenFlags;

#endif
