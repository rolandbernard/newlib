
#include <grp.h>
#include <stddef.h>
#include <string.h>
#include <sys/mprotect.h>
#include <sys/times.h>
#include <sys/types.h>

#include "syscalls.h"

void* _sbrk(ptrdiff_t incr) {
    return (void*)handleErrors(SYSCALL(SYSCALL_SBRK, incr));
}

int mprotect(void *addr, size_t len, int prot) {
    return handleErrors(SYSCALL(SYSCALL_PROTECT, addr, len, prot));
}

clock_t _times(struct tms *buf) {
    return handleErrors(SYSCALL(SYSCALL_TIMES, buf));
}

int _gettimeofday(struct timeval* p, void* tz) {
    uint64_t nanos = handleErrors(SYSCALL(SYSCALL_GET_NANOSECONDS));
    if (nanos == -1) {
        return -1;
    } else {
        p->tv_usec = nanos / 1000 % 1000000;
        p->tv_sec = nanos / 1000000000;
        return 0;
    }
}

int settimeofday(const struct timeval* p, void* tz) {
    uint64_t nanos = p->tv_sec * 1000000000UL + p->tv_usec * 1000UL;
    return handleErrors(SYSCALL(SYSCALL_SET_NANOSECONDS, nanos));
}

int gethostname(char* name, size_t namelen) {
    strncpy(name, "someos", namelen);
    return 0;
}

struct group* getgrent() {
    return NULL;
}

void setgrent() { }

void endgrent() { }

