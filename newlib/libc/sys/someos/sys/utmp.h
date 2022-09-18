#ifndef _SYS_UTMP_H
#define _SYS_UTMP_H

#include <sys/types.h>

#define UTMP_FILE "/etc/utmp"

struct utmp {
    char ut_user[8];
    char ut_id[4];
    char ut_line[12];
    short ut_pid;
    short ut_type;
    struct exit_status {
        short e_termination;
        short e_exit;
    } ut_exit;
    time_t ut_time;
};

#define EMPTY 0
#define RUN_LVL 1
#define BOOT_TIME 2
#define OLD_TIME 3
#define NEW_TIME 4
#define INIT_PROCESS 5
#define LOGIN_PROCESS 6
#define USER_PROCESS 7
#define DEAD_PROCESS 8
#define ACCOUNTING 9
#define UTMAXTYPE ACCOUNTING

#endif
