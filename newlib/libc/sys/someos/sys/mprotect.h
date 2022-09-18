#ifndef _SYS_MPROTECT_H
#define _SYS_MPROTECT_H

// This file is not called mman.h, because it makes some ./configure (e.g. for libgcc) think we have
// mmap, which we don't.

#define	PROT_NONE   0
#define	PROT_READ   1
#define	PROT_WRITE  2
#define	PROT_EXEC   4

#include <stddef.h>

int	mprotect(void *, size_t, int);

#endif
