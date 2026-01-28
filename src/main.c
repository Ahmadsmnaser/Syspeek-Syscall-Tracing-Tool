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

    // Print only EXIT events (cleaner, one row per syscall)
    if (ev->phase != EVENT_PHASE_EXIT)
    {
        return;
    }

    static int header_printed = 0;
    static unsigned long printed = 0;
    static unsigned long errors = 0;

    char line[512];

    // Print table header once
    if (!header_printed)
    {
        if (format_table_header(line, sizeof(line)) == 0)
        {
            puts(line);
        }
        header_printed = 1;
    }

    // Print one row
    if (format_table_row(ev, line, sizeof(line)) == 0)
    {
        puts(line);
        printed++;
        if (ev->has_error)
            errors++;
    }
    else
    {
        // Minimal fallback (should rarely happen)
        printf("%-7d %-16s 0x%016lx 0x%016lx 0x%016lx 0x%016lx %-6s\n",
               ev->pid,
               "syscall(?)",
               (unsigned long)ev->args[0],
               (unsigned long)ev->args[1],
               (unsigned long)ev->args[2],
               (unsigned long)ev->ret,
               ev->has_error ? "ERR" : "-");
        printed++;
        if (ev->has_error)
            errors++;
    }
}

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Usage: %s <program> [args...]\n", argv[0]);
        return 1;
    }
    //printf("Syspeek started with %d arguments.\n", argc);

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