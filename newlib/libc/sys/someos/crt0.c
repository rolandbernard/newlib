
#include <reent.h>

extern void exit(int code);
extern int main(int argc, char* argv[], char* envs[]);

extern char** environ;

void _start(int argc, char* argv[], char* envs[]) {
    asm("la gp, __global_pointer$");
    _REENT_INIT_PTR(_REENT);
    _REENT_INIT_PTR(_GLOBAL_REENT);
    __sinit(_GLOBAL_REENT);
    environ = envs;
    int ex = main(argc, argv, envs);
    exit(ex);
}

