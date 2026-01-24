// trace.c
#include "trace.h"
#include <stdio.h>


// IF there is no target program, we will return an error.
// IF cd == NULL, we will return an error.

int syspeek_trace_run(const char *program, char *const argv[], const SyspeekConfig *cfg, syspeek_event_cb cb, void *user){
    if(program == NULL || argv == NULL || cfg == NULL){
        return -1; // Invalid arguments
    }
    if(cb == NULL){
        return -1; // Callback is required
    }
    // For demonstration, we will just print the program and arguments.
    printf("Tracing program: %s\n", program);
    for(int i = 0; argv[i] != NULL; i++){
        printf("Arg[%d]: %s\n", i, argv[i]);
    }
    printf("Configuration:\n");
    printf("  print_pid: %d\n", cfg->print_pid);
    printf("  capture_args: %d\n", cfg->capture_args);
    printf("  capture_return: %d\n", cfg->capture_return);
    printf("  capture_error: %d\n", cfg->capture_error);
    printf("  arch: %s\n", cfg->arch);

    // Here we would normally set up ptrace and start tracing the target program.
    // For this example, we will simulate a syscall event callback.

    if(cb != NULL){
        struct SyscallEvent ev;
        ev.pid = 1234;
        ev.syscall_num = 60; // exit syscall
        ev.syscall_name = "exit";
        ev.args[0] = 0; // exit code
        ev.ret = 0;
        ev.err = 0;
        ev.has_error = 0;
        ev.phase = EVENT_PHASE_EXIT;

        // Invoke the callback with the simulated event
        cb(&ev, user);
    }

    return 0; // Success
}
