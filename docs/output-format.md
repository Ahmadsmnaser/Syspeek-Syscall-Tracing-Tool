# syspeek Output Format (Default)

## One line per syscall (on EXIT)
syspeek prints one line per syscall when the syscall finishes (exit phase).

## Default line shape
[PID] SYSCALL(arg0, arg1, arg2, arg3, arg4, arg5) = RET [ERRNO]

### Rules
- PID is always printed inside square brackets.
- SYSCALL is the syscall name if known, otherwise: syscall(<number>).
- Arguments are printed as raw values (6 slots).
- RET is the return value.
- If RET == -1, append errno name (e.g., ENOENT). Otherwise no errno is printed.

## Examples (shape)
[1234] openat(AT_FDCWD, 0x7ffd..., O_RDONLY, 0, 0, 0) = 3
[1234] read(3, 0x7ffd..., 128, 0, 0, 0) = 128
[1234] openat(AT_FDCWD, 0x7ffd..., O_RDONLY, 0, 0, 0) = -1 ENOENT
