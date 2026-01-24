# syspeek — Specification (SPEC)

## 1) Overview
syspeek is a learning-oriented syscall tracing tool for Linux.
It runs a target program and prints its syscalls in a human-readable format.

## 2) Scope
### In Scope (MVP)
- Trace a newly launched target process (not attach)
- Observe syscalls at entry and exit
- Print:
  - syscall name (or number in early phase)
  - syscall arguments (raw values initially)
  - return value
  - error indication (errno) when applicable

### Out of Scope (initially)
- Attaching to an existing PID (`-p`)
- Following forks/children
- Full decoding of complex structs
- Complete compatibility across all architectures

## 3) Platform & Assumptions
- Linux on x86_64 (initial target)
- Output is text-based to stdout (file output later)

## 4) CLI Behavior
### Basic usage (conceptual)
- `syspeek <command> [args...]`

### Options (planned, not required for MVP)
- `-e trace=<list>` : trace only selected syscalls
- `-c` : summary mode (counts per syscall)
- `-T` : print time spent per syscall
- `-o <file>` : write output to file

## 5) Output Format
### Default (MVP)
One line per syscall (on exit), includes:
- syscall name (or number)
- arguments (raw or partially decoded)
- return value
- errno (if failure)
The default output format is defined in docs/examples/output-format.md.”

Example shape:
- `openat(...raw...) = 3`
- `read(...raw...) = 128`
- `openat(...raw...) = -1 (ENOENT)`

### Formatting rules
- Unknown syscalls use a fallback representation: `syscall(<num>)(arg0, arg1, ...)`
- If decoding fails, print raw safely.

## 6) Syscall Coverage (MVP decoding targets)
Minimum list to aim for (decoding can be partial):
- execve
- openat
- read
- write
- close
- mmap
- munmap
- brk
- exit / exit_group
- clone/fork (optional in MVP, required later)

## 7) Error Handling
- If target program fails to start → print a clear error and exit non-zero
- If tracing is interrupted (signal) → exit gracefully and stop tracing
- If a syscall cannot be decoded → fallback to raw format (never crash)

## 8) Non-Functional Requirements (Learning-first)
- Keep the codebase modular and readable
- Document important decisions in `docs/decisions/`
- Prefer correctness and clarity over supporting every feature
