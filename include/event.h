// event.h
// Definition of event structure
#pragma once

typedef unsigned long syspeek_word_t; // Word type for syscall arguments and return values
typedef int syspeek_pid_t;            // Process ID type

typedef enum EventPhase
{
    EVENT_PHASE_ENTRY = 0,
    EVENT_PHASE_EXIT = 1
} EventPhase;

typedef struct SyscallEvent
{
    syspeek_pid_t pid;        // Process ID
    int syscall_num;          // Syscall number
    const char *syscall_name; // Syscall name
    syspeek_word_t args[6];   // Up to 6 syscall arguments
    syspeek_word_t ret;       // Return value
    syspeek_word_t err;       // Error code
    int has_error;            // Flag indicating if there was an error
    EventPhase phase;         // 0 for entry, 1 for exit
} SyscallEvent;
