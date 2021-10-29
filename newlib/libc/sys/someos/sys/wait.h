#ifndef _WAIT_H
#define _WAIT_H

#define WNOHANG 1
#define WUNTRACED 2

#define WIFEXITED(val) (((val >> 8) & 0xff) == 0)
#define WEXITSTATUS(val) (val & 0xff)
#define WIFSIGNALED(val) (((val >> 8) & 0xff) != 0)
#define WTERMSIG(val) ((val >> 8) & 0xff)
#define WIFSTOPPED(val) (0)
#define WSTOPSIG(val) ((val >> 8) & 0xff)

typedef int pid_t;

pid_t wait(int *);

pid_t waitpid(int, int *, int);

#endif /* _SYS_WAIT_H */

