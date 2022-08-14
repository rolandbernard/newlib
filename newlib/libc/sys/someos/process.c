
#include <stddef.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/unistd.h>

#include "syscalls.h"

int _execve(const char *name, char* const argv[], char* const env[]) {
    return handleErrors(SYSCALL(SYSCALL_EXECVE, (uintptr_t)name, (uintptr_t)argv, (uintptr_t)env));
}

int _fork() {
    return handleErrors(SYSCALL(SYSCALL_FORK));
}

int _getpid() {
    return handleErrors(SYSCALL(SYSCALL_GETPID));
}

int getppid() {
    return handleErrors(SYSCALL(SYSCALL_GETPPID));
}

int waitpid(int pid, int* status, int flags) {
    return handleErrors(SYSCALL(SYSCALL_WAIT, pid, (uintptr_t)status));
}

int _wait(int* status) {
    return waitpid(0, status, 0);
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

int pause() {
    return handleErrors(SYSCALL(SYSCALL_PAUSE));
}

unsigned alarm(unsigned seconds) {
    return handleErrors(SYSCALL(SYSCALL_ALARM, seconds));
}

// TODO: Add ruid/euid/suid distinction
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

uid_t geteuid() {
    return getuid();
}

gid_t getegid() {
    return getgid();
}

int seteuid(uid_t uid) {
    return setuid(uid);
}

int setegid(gid_t gid) {
    return setgid(gid);
}

int setreuid(uid_t ruid, uid_t euid) {
    return setuid(ruid) == 0 && seteuid(euid) ? 0 : -1;
}

int setregid(gid_t rgid, gid_t egid) {
    return setgid(rgid) == 0 && setegid(egid) ? 0 : -1;
}

// TODO: Figure out what the following are supposed to do:
long sysconf(int name) {
    return -1;
}

