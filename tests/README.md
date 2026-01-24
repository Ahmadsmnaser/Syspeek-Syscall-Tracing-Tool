# syspeek Tests

This folder contains small C programs designed to trigger specific syscalls.
They are used to validate that `syspeek` prints syscalls in the expected order
and in the default output format.

## Structure
- `programs/` : test programs (each triggers a small set of syscalls)
- `expected/` : notes/patterns we expect to see in syspeek output (not strict)
- `README.md` : this file

## How to run
Build syspeek first, then run:

- `./syspeek ./tests/programs/t_write`
- `./syspeek ./tests/programs/t_open_fail`
- `./syspeek ./tests/programs/t_open_read_close`
- `./syspeek ./tests/programs/t_execve`
- `./syspeek ./tests/programs/t_malloc_free`

## Important Notes
- The tracer output may include extra syscalls due to the runtime loader, libc, or the program doing I/O (printf).
- `open()` may appear as `openat()` in the traced syscalls (this is normal).
- For `t_open_read_close`, make sure `testfile.txt` exists in the working directory.
- For `t_execve`, the traced program will execute `/bin/ls`, which produces many syscalls (expected noise).

## Test Programs
### 1) t_write
Goal: ensure `write` appears.
Must include:
- `write(...) = <non-negative>`

### 2) t_open_fail
Goal: ensure a failing open shows errno.
Must include:
- `openat(...) = -1 ENOENT` (or equivalent)

### 3) t_open_read_close
Goal: see open -> read -> close sequence.
Must include (in order, with possible syscalls in between):
- `openat(...) = <fd>`
- `read(<fd>, ...) = <n>`
- `close(<fd>) = 0`

### 4) t_execve
Goal: ensure execve appears.
Must include:
- `execve(...) = 0` (or the relevant return behavior)
Note: after execve, many syscalls will appear from `/bin/ls`.

### 5) t_malloc_free
Goal: observe allocator-related syscalls (best-effort).
May include:
- `brk(...) = ...` and/or `mmap(...) = ...`
Note: allocator behavior depends on libc and current process state.
