//formatter.h
//Declarations for formatter functions
#pragma once
#include "event.h"
#include <stddef.h>

// forward declaration to ensure the type name is known here
struct SyscallEvent;


// Formats a SyscallEvent (defined in event.h) into a human-readable string.
// returns 0 on success, -1 on failure.
 format_line(const struct SyscallEvent* ev , char* out , size_t out_sz);