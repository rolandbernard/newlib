
#include <grp.h>
#include <stddef.h>
#include <sys/mman.h>
#include <sys/times.h>
#include <sys/types.h>
#include <string.h>

#include "syscalls.h"

void* _sbrk(ptrdiff_t incr) {
    return (void*)handleErrors(SYSCALL(SYSCALL_SBRK, incr));
}

int mprotect(void *addr, size_t len, int prot) {
    return handleErrors(SYSCALL(SYSCALL_PROTECT, (uintptr_t)addr, len, prot));
}

clock_t _times(struct tms *buf) {
    return handleErrors(SYSCALL(SYSCALL_TIMES, (uintptr_t)buf));
}

int _gettimeofday(struct timeval* p, void* tz) {
    uint64_t nanos = handleErrors(SYSCALL(SYSCALL_NANOSECONDS));
    if (nanos == -1) {
        return -1;
    } else {
        nanos %= 24 * 60 * 60 * 1000000000L;
        p->tv_usec = nanos / 1000 % 1000000;
        p->tv_sec = nanos / 1000000000;
        return 0;
    }
}

int gethostname(char* name, size_t namelen) {
    strncpy(name, "someos", namelen);
    return 0;
}

