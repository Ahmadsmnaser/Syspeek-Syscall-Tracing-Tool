//main.c
#include <stdio.h>
#include <string.h>

#include "trace.h"
#include "formatter.h"
#include "syscall_table.h"
#include "syspeek.h"
#include "event.h"

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
*/

static void on_syscall_event(const struct SyscallEvent *ev, void *user)
{
    (void)user;

    if (ev->syscall_name == NULL)
    {
        const char *name = syspeek_syscall_name(ev->syscall_num);

        struct SyscallEvent tmp = *ev;
        tmp.syscall_name = name;

        char line[512];
        if (format_line(&tmp, line, sizeof(line)) == 0)
        {
            puts(line);
        }
        else
        {
            // formatter failed; print minimal fallback
            printf("[%d] syscall(%d) = %lu\n",
                   tmp.pid, tmp.syscall_num, (unsigned long)tmp.ret);
        }
        return;
    }

    // Normal path: syscall_name already present
    char line[512];
    if (format_line(ev, line, sizeof(line)) == 0)
    {
        puts(line);
    }
    else
    {
        printf("[%d] %s(...) = %lu\n",
               ev->pid, ev->syscall_name, (unsigned long)ev->ret);
    }
}

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Usage: %s <program> [args...]\n", argv[0]);
        return 1;
    }
    printf("Syspeek started with %d arguments.\n", argc);

    const char *target_program = argv[1];
    char *const *target_argv = &argv[1];

    struct SyspeekConfig config;
    memset(&config, 0, sizeof(config));
    config.print_pid = 1;
    config.capture_args = 1;
    config.capture_return = 1;
    config.capture_error = 1;
    config.arch = "x86_64";

    int result = syspeek_trace_run(target_program , (char *const *)target_argv , &config , on_syscall_event , NULL);
    if(result != 0){
        printf("Failed to start tracing the program: %s\n", target_program);
        return 2;
    }
    return 0;
}