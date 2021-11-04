
#include <sys/signal.h>

#include "syscalls.h"

typedef struct {
    void* handler;
    void* restorer;
} SignalAction;

typedef void (*__sighandler_t)(int);

int _kill(int pid, int sig) {
    return handleErrors(SYSCALL(SYSCALL_KILL, pid, sig));
}

static void restorer() {
    SYSCALL(SYSCALL_SIGRETURN);
}

__sighandler_t signal(int signal, __sighandler_t handler) {
    volatile SignalAction sigaction;
    sigaction.handler = handler;
    sigaction.restorer = restorer;
    asm volatile ("" : : "r"(&sigaction) : "memory");
    int ret = handleErrors(SYSCALL(SYSCALL_SIGACTION, signal, (uintptr_t)&sigaction, (uintptr_t)&sigaction));
    if (ret == -1) {
        return (void*)(uintptr_t)ret;
    } else {
        return sigaction.handler;
    }
}

