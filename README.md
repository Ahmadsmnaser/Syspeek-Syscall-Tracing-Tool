# 🕵️ syspeek — Lightweight Linux Syscall Tracer

`syspeek` is a lightweight Linux system call tracer written in **C**, built on top of **ptrace**.  
It is inspired by tools like `strace`, but designed to be **simpler, educational, and extensible**, with a strong focus on **low-level systems programming**.

This project demonstrates hands-on understanding of:
- Linux system calls
- Process tracing and control
- Register-level ABI details (x86_64)
- Event-driven tracing design

---

## ✨ Features

- 🔍 Trace Linux **system calls** (entry & exit)
- 🧠 Capture:
  - syscall number
  - syscall name (when known)
  - up to 6 arguments
  - return value
  - errno on failure
- 🧩 Architecture-aware syscall mapping (**x86_64**)
- 🪶 Lightweight (no external dependencies)
- 🧪 Works with real programs (`read`, `write`, `openat`, `execve`, `malloc`, etc.)

---

### What each test covers

| Test              | Purpose               | Expected syscalls (at least) |
| ----------------- | --------------------- | ---------------------------- |
| `t_write`         | Write to stdout       | `write`                      |
| `open_fail`       | Failing file open     | `openat` (errno=2/ENOENT)    |
| `open_read_close` | Open + read + close   | `openat`, `read`, `close`    |
| `malloc_activity` | Heap/mmap activity    | `brk` and/or `mmap`          |
| `execve`          | Execute a new program | `execve`                     |

---

## ⚙️ Syscall Mapping

`syspeek` includes a small syscall table for **x86_64**:

* Known syscalls are printed by name (e.g., `write`, `openat`, `execve`)
* Unknown syscalls are printed as: `syscall(<num>)`

The table is intentionally minimal and easy to extend.

---

## 🚧 Known Limitations

* x86_64 only
* No advanced decoding for pointers/strings/flags (yet)
* No follow-forks/threads (yet)
* Plain text output (no colors / JSON mode)

---

## 🛣️ Optional Future Improvements

* Pretty printing (hex pointers, strings, flags)
* Filtering by syscall name/number
* JSON output mode
* Follow fork/clone/threads
* Support for additional architectures
* Summary statistics (counts, error rate)

> The project is functionally complete for educational and portfolio purposes — further work is optional.

---

## 🎓 Why This Project Matters

This project demonstrates real systems-level skills:

* Linux syscall mechanics
* ptrace-based process control
* register-level ABI reasoning
* kernel ↔ userspace interaction
* robust error handling around exec and signals

---

## 👤 Author

**Ahmad Naser**
B.Sc. Computer Science
Low-Level & Systems Programming Enthusiast

```
```
