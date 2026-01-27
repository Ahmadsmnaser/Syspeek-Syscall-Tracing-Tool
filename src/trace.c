// trace.c
#include "trace.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>

// IF there is no target program, we will return an error.
// IF cd == NULL, we will return an error.

int syspeek_trace_run(const char *program, char *const argv[], const SyspeekConfig *cfg, syspeek_event_cb cb, void *user)
{
    if (program == NULL || argv == NULL || cfg == NULL)
    {
        return -1; // Invalid arguments
    }
    if (cb == NULL)
    {
        return -1; // Callback is required
    }
    if (argv[0] == NULL)
    {
        return -1; // No target program specified
    }

    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork failed");
        return -1; // Fork failed
    }
    if (pid == 0)
    { // Child process
        // Child process will be traced
        if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) < 0)
        {
            perror("ptrace(PTRACE_TRACEME) failed");
            _exit(1);
        }
        execvp(program, argv);
        perror("execvp failed");
        _exit(1);
    }
    if (pid > 0)
    { // Parent process
        printf("Started tracing process with PID %d\n", pid);

        int status = 0;
        if (waitpid(pid, &status, 0) < 0)
        {
            perror("waitpid failed");
            return -1;
        }

        if (WIFSTOPPED(status))
        {
            printf("Child process stopped, continuing...\n");

            // Set options to trace syscalls
            if (ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD) < 0)
            {
                perror("ptrace(PTRACE_SETOPTIONS) failed");
                return -1;
            }

            // Start tracing syscalls
            if (ptrace(PTRACE_SYSCALL, pid, NULL, NULL) < 0)
            {
                perror("ptrace(PTRACE_SYSCALL) failed");
                return -1;
            }

            int in_syscall = 0; // Flag, 0 => next is Entry, 1 => next is Exit
            struct SyscallEvent ev;
            for (int i = 0; i < 6; i++)
                ev.args[i] = 0;
            ev.pid = (syspeek_pid_t)pid;
            ev.ret = 0;
            ev.err = 0;
            ev.has_error = 0;
            ev.phase = EVENT_PHASE_ENTRY;
            ev.syscall_name = NULL;
            ev.syscall_num = -1;

            // Wait until the child exits
            while (1)
            {
                if (waitpid(pid, &status, 0) < 0)
                {
                    perror("waitpid failed");
                    return -1;
                }

                if (WIFEXITED(status))
                {
                    printf("Child exited with status %d\n", WEXITSTATUS(status));
                    break;
                }

                if (WIFSIGNALED(status))
                {
                    printf("Child killed by signal %d\n", WTERMSIG(status));
                    break;
                }

                if (WIFSTOPPED(status))
                {
                    int sig = WSTOPSIG(status);

                    if (sig == (SIGTRAP | 0x80))
                    {
                        // We need to read the registers to get syscall info
                        struct user_regs_struct regs;
                        if (ptrace(PTRACE_GETREGS, pid, NULL, &regs) < 0)
                        {
                            perror("ptrace(PTRACE_GETREGS) failed");
                            return -1;
                        }

                        if (in_syscall == 0)
                        {
                            ev.pid = (syspeek_pid_t)pid;
                            ev.syscall_num = (int)regs.orig_rax; // x86_64: syscall number at entry
                            ev.syscall_name = NULL;
                            ev.args[0] = (syspeek_word_t)regs.rdi; // Argument 1
                            ev.args[1] = (syspeek_word_t)regs.rsi; // Argument 2
                            ev.args[2] = (syspeek_word_t)regs.rdx; // Argument 3
                            ev.args[3] = (syspeek_word_t)regs.r10; // Argument 4
                            ev.args[4] = (syspeek_word_t)regs.r8;  // Argument 5
                            ev.args[5] = (syspeek_word_t)regs.r9;  // Argument 6

                            ev.phase = EVENT_PHASE_ENTRY;
                            ev.ret = 0;       // No return value at entry
                            ev.has_error = 0; // No error at entry
                            ev.err = 0;       // No error at entry

                            in_syscall = 1;
                        }
                        else
                        {
                            struct SyscallEvent ev1 = ev;
                            ev1.phase = EVENT_PHASE_EXIT;

                            long rax = (long)regs.rax;
                            ev1.ret = (syspeek_word_t)rax; // Return value

                            if (rax < 0 && rax >= -4095)
                            {
                                ev1.has_error = 1;
                                ev1.err = (syspeek_word_t)(-rax); // Error code
                            }
                            else
                            {
                                ev1.has_error = 0;
                                ev1.err = 0;
                            }

                            cb(&ev1, user);
                            in_syscall = 0;
                        }

                        // IMPORTANT: continue to next syscall-stop
                        if (ptrace(PTRACE_SYSCALL, pid, NULL, NULL) < 0)
                        {
                            perror("ptrace(PTRACE_SYSCALL) failed");
                            return -1;
                        }
                    }
                    else
                    {
                        // Other stops (e.g., signals)
                        // IMPORTANT: never deliver SIGTRAP to the child (exec-related traps etc.)
                        // Also reset in_syscall because exec/traps can break entry/exit pairing
                        if (sig == SIGTRAP)
                        {
                            in_syscall = 0;
                            if (ptrace(PTRACE_SYSCALL, pid, NULL, 0) < 0)
                            {
                                perror("ptrace(PTRACE_SYSCALL) failed");
                                return -1;
                            }
                        }
                        else
                        {
                            if (ptrace(PTRACE_SYSCALL, pid, NULL, (void *)(long)sig) < 0)
                            {
                                perror("ptrace(PTRACE_SYSCALL) failed");
                                return -1;
                            }
                        }
                    }
                }
            }
        }
    }
    return 0; // Success
}
