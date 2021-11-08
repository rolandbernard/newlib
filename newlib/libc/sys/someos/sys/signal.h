#ifndef _SYS_SIGNAL_H
#define _SYS_SIGNAL_H
#define _SIGNAL_H

#include "_ansi.h"
#include <sys/cdefs.h>
#include <sys/features.h>
#include <sys/types.h>
#include <sys/_sigset.h>
#include <sys/_timespec.h>
#include <stdint.h>

#undef NSIG
#define NSIG _NSIG
#undef sigset_t
#define sigset_t __sigset_t

typedef void (*_sig_func_ptr) (int);
typedef _sig_func_ptr __sighandler_t;

struct sigaction {
    __sighandler_t sa_handler;
    sigset_t sa_mask;
    int sa_flags;
    void* sa_sigaction;
};

#define SIG_DFL ((void*)0)
#define SIG_IGN ((void*)1)

#define SIGNONE 0
#define SIGHUP 1
#define SIGINT 2
#define SIGQUIT 3
#define SIGILL 4
#define SIGTRAP 5
#define SIGABRT 6
#define SIGIOT SIGABRT
#define SIGBUS 7
#define SIGEMT 8
#define SIGFPE 9
#define SIGKILL 10
#define SIGUSR1 11
#define SIGSEGV 12
#define SIGUSR2 13
#define SIGPIPE 14
#define SIGALRM 15
#define SIGTERM 16
#define SIGSTKFLT 17
#define SIGCHLD 18
#define SIGCLD SIGCHLD
#define SIGCONT 19
#define SIGSTOP 20
#define SIGTSTP 21
#define SIGTTIN 22
#define SIGTTOU 23
#define SIGURG 24
#define SIGXCPU 25
#define SIGXFSZ 26
#define SIGVTALRM 27
#define SIGPROF 28
#define SIGWINCH 29
#define SIGIO 30
#define SIGPOLL SIGIO
#define SIGPWR 31
#define SIGINFO SIGPWR
#define SIGLOST 32
#define SIGSYS 33
#define SIGUNUSED SIGSYS

#define _NSIG 34

#define SIG_SETMASK 0
#define SIG_BLOCK   1
#define SIG_UNBLOCK 2

int kill(pid_t pid, int sig);
int sigaction(int signum, const struct sigaction* act, struct sigaction* oldact);
int sigaddset(sigset_t* set, int signum);
int sigdelset(sigset_t* set, int signum);
int sigismember(const sigset_t* set, int signum);
int sigfillset(sigset_t* set);
int sigemptyset(sigset_t* set);
int sigpending(sigset_t* set);
int sigsuspend(const sigset_t* mask);
int sigpause(int sigmask);
int sigprocmask(int how, const sigset_t* set, sigset_t* oldset);
sigset_t sigblock(sigset_t mask);
sigset_t sigmask(int signum);
sigset_t sigsetmask(sigset_t mask);
sigset_t siggetmask();

#endif
