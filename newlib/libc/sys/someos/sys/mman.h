#ifndef _SYS_MMAN_H_
#define _SYS_MMAN_H_

#define	PROT_NONE   0
#define	PROT_READ   1
#define	PROT_WRITE  2
#define	PROT_EXEC   4

#include <stddef.h>

int	mprotect(void *, size_t, int);

#endif
