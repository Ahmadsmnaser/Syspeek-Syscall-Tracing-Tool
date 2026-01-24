//syscall_table_x86_64.c
#include "syscall_table.h"
#include <string.h>

const char *syspeek_syscall_name(int syscall_num){
    switch (syscall_num)
    {
    case 0: return "read";
    case 1: return "write";
    case 3: return "close";
    case 4: return "stat";
    case 5: return "fstat";
    case 9: return "mmap";
    case 12: return "brk";
    case 59: return "execve";
    case 60: return "exit";
    case 257: return "openat";
    default: return NULL;
    }
}

int syspeek_syscall_num(char const *name){
    if(strcmp(name, "read") == 0) return 0;
    if(strcmp(name, "write") == 0) return 1;
    if(strcmp(name, "close") == 0) return 3;
    if(strcmp(name, "stat") == 0) return 4;
    if(strcmp(name, "fstat") == 0) return 5;
    if(strcmp(name, "mmap") == 0) return 9;
    if(strcmp(name, "brk") == 0) return 12;
    if(strcmp(name, "execve") == 0) return 59;
    if(strcmp(name, "exit") == 0) return 60;
    if(strcmp(name, "openat") == 0) return 257;
    return -1;
}

const char *syspeek_arch_name(){
    return "x86_64";
}
