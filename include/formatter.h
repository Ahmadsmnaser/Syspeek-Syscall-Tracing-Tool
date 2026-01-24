//formatter.h
//Declarations for formatter functions
#pragma once
#include "event.h"
#include <stddef.h>


// Formats a SyscallEvent into a human-readable string.
// returns 0 on success, -1 on failure.
int format_line(const SyscallEvent* ev , char* out , size_t out_sz);