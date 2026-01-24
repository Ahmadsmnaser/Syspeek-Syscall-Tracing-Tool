//syscall_table.h
//Declarations for syscall table functions
#pragma once

// Returns the name of the syscall given its number.
const char *syspeek_syscall_name(int syscall_num);

// Returns the syscall number given its name, or -1 if not found.
int syspeek_syscall_num(char const* name);

// Returns the architecture name used for syscall table.
const char* syspeek_arch_name();