#ifndef _SYS_SIGNAL_H
#define _SYS_SIGNAL_H

#include "_ansi.h"
#include <stdint.h>
#include <sys/_sigset.h>
#include <sys/_timespec.h>
#include <sys/cdefs.h>
#include <sys/features.h>
#include <sys/types.h>

union sigval {
    int sival_int;
    void* sival_ptr;
};

struct sigevent {
    int sigev_notify;
    int sigev_signo;
    union sigval sigev_value;
};

#define SI_USER 1
#define SI_QUEUE 2
#define SI_TIMER 3
#define SI_ASYNCIO 4
#define SI_MESGQ 5

typedef struct {
    int si_signo;
    int si_code;
    union sigval si_value;
} siginfo_t;

#define SA_NOCLDSTOP 0x01
#define SA_ONSTACK 0x02
#define SA_RESETHAND 0x04
#define SA_RESTART 0x08
#define SA_SIGINFO 0x10
#define SA_NOCLDWAIT 0x20
#define SA_NODEFER 0x40

typedef void (*_sig_func_ptr)(int);

struct sigaction {
    union {
        _sig_func_ptr sa_handler;
        void (*sa_sigaction)(int, siginfo_t*, void*);
    };
    sigset_t sa_mask;
    int sa_flags;
    void (*sa_restorer)();
};

typedef struct sigaltstack {
    void* ss_sp;
    int ss_flags;
    size_t ss_size;
} stack_t;

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

#define SIGHUP 1        /* hangup */
#define SIGINT 2        /* interrupt */
#define SIGQUIT 3       /* quit */
#define SIGILL 4        /* illegal instruction (not reset when caught) */
#define SIGTRAP 5       /* trace trap (not reset when caught) */
#define SIGIOT 6        /* IOT instruction */
#define SIGABRT 6       /* used by abort, replace SIGIOT in the future */
#define SIGEMT 7        /* EMT instruction */
#define SIGFPE 8        /* floating point exception */
#define SIGKILL 9       /* kill (cannot be caught or ignored) */
#define SIGBUS 10       /* bus error */
#define SIGSEGV 11      /* segmentation violation */
#define SIGSYS 12       /* bad argument to system call */
#define SIGPIPE 13      /* write on a pipe with no one to read it */
#define SIGALRM 14      /* alarm clock */
#define SIGTERM 15      /* software termination signal from kill */
#define SIGURG 16       /* urgent condition on IO channel */
#define SIGSTOP 17      /* sendable stop signal not from tty */
#define SIGTSTP 18      /* stop signal from tty */
#define SIGCONT 19      /* continue a stopped process */
#define SIGCHLD 20      /* to parent on child stop or exit */
#define SIGCLD 20       /* System V name for SIGCHLD */
#define SIGTTIN 21      /* to readers pgrp upon background tty read */
#define SIGTTOU 22      /* like TTIN for output if (tp->t_local&LTOSTOP) */
#define SIGIO 23        /* input/output possible signal */
#define SIGPOLL SIGIO   /* System V name for SIGIO */
#define SIGXCPU 24      /* exceeded CPU time limit */
#define SIGXFSZ 25      /* exceeded file size limit */
#define SIGVTALRM 26    /* virtual time alarm */
#define SIGPROF 27      /* profiling time alarm */
#define SIGWINCH 28     /* window changed */
#define SIGLOST 29      /* resource lost (eg, record-lock lost) */
#define SIGUSR1 30      /* user defined signal 1 */
#define SIGUSR2 31      /* user defined signal 2 */
#define NSIG 32         /* signal 0 implied */

#include <signal.h>
#endif
