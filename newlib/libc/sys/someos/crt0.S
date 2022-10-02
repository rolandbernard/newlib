
#include "newlib.h"

.text
.global _start
.type   _start, @function
_start:
# Initialize global pointer
.option push
.option norelax
    1:auipc gp, %pcrel_hi(__global_pointer$)
    addi gp, gp, %pcrel_lo(1b)
.option pop
    la t0, environ
    sd a2, (t0)
    mv s0, a0 # argc
    mv s1, a1 # argv
    mv s2, a2 # envs

# Clear the bss segment
    la a0, __bss_start
    la a2, _end
    sub a2, a2, a0
    li a1, 0
    call memset
#ifdef _LITE_EXIT
    .weak atexit
    la a0, atexit
    beqz a0, .Lweak_atexit
    .weak __libc_fini_array
#endif

    la a0, __libc_fini_array # Register global termination functions
    call atexit              # to be called upon exit
#ifdef _LITE_EXIT
    .Lweak_atexit:
#endif
    call __libc_init_array # Run global initialization functions

    mv a0, s0 # argc
    mv a1, s1 # argv
    mv a2, s2 # envs
    call main
    tail exit
.size  _start, .-_start
