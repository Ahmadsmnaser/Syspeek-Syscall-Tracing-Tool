# syspeek 👁️🐧
A small strace-like syscall tracing tool for Linux — built for learning low-level OS concepts.

## Goal 🎯
syspeek is a learning-first project.
The main goal is to deeply understand how syscall tracing works (process tracing, registers, syscalls, signals),
and to build a clean, maintainable mini tool along the way.

## What it does (MVP)
- Launches a target program and traces its syscalls
- Prints syscall name + arguments (raw first, decoded later) + return value
- Provides readable output similar in spirit to `strace` (but smaller)

## Planned Features
- Syscall name mapping (number → name)
- Basic decoders for common syscalls (open/read/write/execve/mmap/close)
- Filters (trace only selected syscalls)
- Summary mode (counts per syscall)
- Timing per syscall (optional)

## Non-goals (for now)
- Full `strace` parity
- Deep decoding of every syscall structure
- Complex attach scenarios (later milestone)

## Repo Structure
- `SPEC.md`  → user-visible behavior (CLI/output/requirements)
- `DESIGN.md` → internal architecture and decisions
- `ROADMAP.md` → milestones and progress plan
- `docs/` → examples, diagrams, design decisions
