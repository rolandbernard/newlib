
.section .text
.global _exit

# Writing this in assembly because I can't get the compiler to compile it correctly
_exit:
    mv a1, a0
    li a0, 1
    li a2, 0
    li a3, 0
    li a4, 0
    li a5, 0
    li a6, 0
    ecall

