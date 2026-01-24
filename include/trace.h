// trace.h
// Declarations for tracing functions
#pragma once
#include "event.h"
#include "syspeek.h"

// Callback type for handling syscall events
typedef void (*syspeek_event_cb)(const SyscallEvent *ev, void *user);


// Runs the specified program with arguments under tracing according to the given configuration.
int syspeek_trace_run(const char *program, char *const argv[], const SyspeekConfig *cfg);


// Runs tracing with the given configuration and invokes the callback for each syscall event.
int syspeek_trace_run_cb(const SyspeekConfig *cfg, syspeek_event_cb cb, void *user);