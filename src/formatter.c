//formatter.c
#include "formatter.h"
#include <stdio.h>
#include <string.h>
#include "syscall_table.h"
/*

extern struct SyscallEvent
{
    syspeek_pid_t pid;        // Process ID
    int syscall_num;          // Syscall number
    const char *syscall_name; // Syscall name
    syspeek_word_t args[6];   // Up to 6 syscall arguments
    syspeek_word_t ret;       // Return value
    syspeek_word_t err;       // Error code
    int has_error;            // Flag indicating if there was an error
    EventPhase phase;         // 0 for entry, 1 for exit
};

## Default line shape
[PID] SYSCALL(arg0, arg1, arg2, arg3, arg4, arg5) = RET [ERRNO]

*/

int format_line(const struct SyscallEvent *ev, char *out, size_t out_sz){
    if(ev == NULL || out == NULL){
        return -1;
    }
    if(out_sz == 0){
        return -1;
    }
    const char *name;
    if (ev->syscall_name == NULL){
        name = syspeek_syscall_name(ev->syscall_num);
        if(name == NULL){ // using syscall number if name is unknown
            static char buffer[32];
            snprintf(buffer, sizeof(buffer), "syscall(%d)", ev->syscall_num);
            name = buffer;
        }
    }else {
        name = ev->syscall_name;
    }
    if(ev->has_error == 1){
        snprintf(out, out_sz, "[%d] %s(%lu, %lu, %lu, %lu, %lu, %lu) = %lu [ERR: %lu]",
            ev->pid,
            name,
            ev->args[0],
            ev->args[1],
            ev->args[2],
            ev->args[3],
            ev->args[4],
            ev->args[5],
            ev->ret,
            ev->err
        );
        return 0;
    }else{
        snprintf(out, out_sz, "[%d] %s(%lu, %lu, %lu, %lu, %lu, %lu) = %lu",
            ev->pid,
            name,
            ev->args[0],
            ev->args[1],
            ev->args[2],
            ev->args[3],
            ev->args[4],
            ev->args[5],
            ev->ret
        );
        return 0;
    }
}