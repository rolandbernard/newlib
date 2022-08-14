
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

// TODO: Look at what hardware we have in the VM for this.
int _gettimeofday(struct timeval* __p, void* __tz) {
    return -1;
}

int gethostname(char* name, size_t namelen) {
    strncpy(name, "someos", namelen);
    return 0;
}

