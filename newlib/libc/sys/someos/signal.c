
#include <stddef.h>
#include <unistd.h>
#include <sys/signal.h>

#include "syscalls.h"

typedef struct {
    __sighandler_t handler;
    uint64_t mask;
    int flags;
    void* sigaction;
    void (*restorer)();
} SignalAction;

int _kill(int pid, int sig) {
    return handleErrors(SYSCALL(SYSCALL_KILL, pid, sig));
}

static void restorer() {
    SYSCALL(SYSCALL_SIGRETURN);
}

__sighandler_t signal(int signal, __sighandler_t handler) {
    volatile SignalAction sigaction;
    sigaction.handler = handler;
    sigaction.mask = 0;
    sigaction.flags = 0;
    sigaction.sigaction = NULL;
    sigaction.restorer = restorer;
    asm volatile ("" : : "r"(&sigaction) : "memory");
    int ret = handleErrors(SYSCALL(SYSCALL_SIGACTION, signal, (uintptr_t)&sigaction, (uintptr_t)&sigaction));
    asm volatile ("" : : "r"(&sigaction) : "memory");
    if (ret == -1) {
        return (void*)(uintptr_t)ret;
    } else {
        return sigaction.handler;
    }
}

int sigaction(int signal, const struct sigaction* new, struct sigaction* old) {
    volatile SignalAction sigaction;
    int ret;
    if (new == NULL) {
        ret = handleErrors(SYSCALL(SYSCALL_SIGACTION, signal, 0, old != NULL ? (uintptr_t)&sigaction : 0));
    } else {
        sigaction.handler = new->sa_handler;
        sigaction.mask = new->sa_mask;
        sigaction.flags = new->sa_flags;
        sigaction.sigaction = new->sa_sigaction;
        sigaction.restorer = restorer;
        asm volatile ("" : : "r"(&sigaction) : "memory");
        ret = handleErrors(SYSCALL(SYSCALL_SIGACTION, signal, (uintptr_t)&sigaction, old != NULL ? (uintptr_t)&sigaction : 0));
        asm volatile ("" : : "r"(&sigaction) : "memory");
    }
    if (old != NULL) {
        old->sa_handler = sigaction.handler;
        old->sa_mask = sigaction.mask;
        old->sa_flags = sigaction.flags;
        old->sa_sigaction = sigaction.sigaction;
    }
    return ret;
}

int sigaddset(sigset_t* set, const int signal) {
    *set |= (1UL << (signal - 1));
    return 0;
}

int sigdelset(sigset_t* set, const int signal) {
    *set &= ~(1UL << (signal - 1));
    return 0;
}

int sigismember(const sigset_t* set, int signal) {
    return (*set & (1UL << (signal - 1))) ? 1 : 0;
}

int sigfillset(sigset_t* set) {
    *set = ~0;
    return 0;
}

int sigemptyset(sigset_t* set) {
    *set = 0;
    return 0;
}

int sigpending(sigset_t* set) {
    sigset_t ret = handleErrors(SYSCALL(SYSCALL_SIGPENDING));
    if ((int)ret == -1) {
        return -1;
    } else {
        if (set != NULL) {
            *set = ret;
        }
        return 0;
    }
}

int sigsuspend(const sigset_t* mask) {
    sigset_t old;
    sigprocmask(SIG_SETMASK, mask, &old);
    int ret = pause();
    sigprocmask(SIG_SETMASK, &old, NULL);
    return ret;
}

int sigpause(int signal) {
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, signal);
    return sigsuspend(&mask);
}

int sigprocmask(int how, const sigset_t* set, sigset_t* oldset) {
    sigset_t mask;
    if (set != NULL) {
        mask = *set;
    } else {
        sigemptyset(&mask);
    }
    sigset_t ret = handleErrors(SYSCALL(SYSCALL_SIGPROCMASK, how, mask));
    if ((int)ret == -1) {
        return -1;
    } else {
        if (oldset != NULL) {
            *oldset = ret;
        }
        return 0;
    }
}

sigset_t sigblock(sigset_t mask) {
    sigset_t old;
    int ret = sigprocmask(SIG_BLOCK, &mask, &old);
    if (ret == -1) {
        return (sigset_t)ret;
    } else {
        return old;
    }
}

sigset_t sigmask(int signum) {
    return 1UL << (signum - 1);
}

sigset_t sigsetmask(sigset_t mask) {
    sigset_t old;
    int ret = sigprocmask(SIG_SETMASK, &mask, &old);
    if (ret == -1) {
        return (sigset_t)ret;
    } else {
        return old;
    }
}

sigset_t siggetmask() {
    sigset_t old;
    sigset_t new;
    sigemptyset(&new);
    int ret = sigprocmask(SIG_BLOCK, &new, &old);
    if (ret == -1) {
        return (sigset_t)ret;
    } else {
        return old;
    }
}

