// syspeek.h
// Declarations for syspeek configuration
#pragma once

typedef struct SyspeekConfig
{
    int print_pid; // 1 to print PID, 0 otherwise
    // int follow_forks ; // 1 to follow forks, 0 otherwise
    int capture_args;   // 1 to capture syscall arguments, 0 otherwise
    int capture_return; // 1 to print return values, 0 just syscall names
    int capture_error;  // 1 to print error details, 0 print -1 without details
    const char *arch;   // syscall tabale architecture, e.g., "x86_64"
} SyspeekConfig;