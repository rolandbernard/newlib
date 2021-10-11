#ifndef _WAIT_H
#define _WAIT_H

#define WNOHANG 1
#define WUNTRACED 2

#define WIFEXITED(val) ((val & 0xff) != 0)
#define WEXITSTATUS(val) (val & 0xff)
#define WIFSIGNALED(val) (((val >> 8) & 0xff) != 0)
#define WTERMSIG(val) ((val >> 8) & 0xff)
#define WIFSTOPPED(val) (0)
#define WSTOPSIG(val) ((val >> 8) & 0xff)

#endif /* _SYS_WAIT_H */

