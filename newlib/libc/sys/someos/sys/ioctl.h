#ifndef _SYS_IOCTL_H
#define _SYS_IOCTL_H

#define TCGETS      0x5401
#define TCSETS      0x5402
#define TCSETSW     0x5403
#define TCSETSF     0x5404
#define TIOCGPGRP   0x540F
#define TIOCSPGRP   0x5410
#define TCXONC      0x540A
#define TCFLSH      0x540B
#define FIONREAD    0x541B

int ioctl(int fd,int request,...);

#endif
