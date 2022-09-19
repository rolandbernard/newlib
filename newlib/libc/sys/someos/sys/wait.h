#ifndef _SYS_WAIT_H
#define _SYS_WAIT_H

#include <sys/types.h>

#define WNOHANG 1
#define WUNTRACED 2
#define WCONTINUED 4

#define WIFEXITED(val) (((val) >> 8) == 0)
#define WIFSIGNALED(val) (((val) >> 8) == 1)
#define WIFSTOPPED(val) (((val) >> 8) == 2)
#define WIFCONTINUED(val) (((val) >> 8) == 3)
#define WEXITSTATUS(val) ((val) & 0xff)
#define WTERMSIG(val) ((val) & 0xff)
#define WSTOPSIG(val) ((val) & 0xff)

pid_t wait (int *);
pid_t waitpid (pid_t, int *, int);

#endif
