# syspeek — Design (DESIGN)

## 1) Design Goals
- Learning-first: correctness and clarity come before feature-completeness
- Modular architecture: tracer core separate from formatting/decoding
- Safe output: decoding failures should not crash the tracer

## 2) High-Level Architecture
### Modules
1) Tracer Core
- Responsible for launching and controlling the traced process
- Observes syscall entry/exit and collects raw data

2) Syscall Table
- Maps syscall numbers to names for the target architecture

3) Event Model
- A small internal representation of a syscall event:
  - pid/tid
  - syscall number + name
  - args[0..N] raw
  - return value
  - errno (derived when failure)
  - timestamp (optional)
  - phase (entry/exit) if needed

4) Decoders
- Per-syscall decoding logic
- Starts minimal: raw args only
- Adds safe decoding gradually (strings, flags)

5) Formatter
- Converts an event into one printable line
- Supports multiple modes later (verbose/json)

6) Filters
- Decide whether to print an event based on syscall name/number

## 3) Execution Flow (Conceptual)
1) Start traced program (MVP: new process only)
2) Enter main tracing loop
3) For each syscall:
   - capture entry state (args + syscall number)
   - capture exit state (return value)
   - build SyscallEvent
   - decode (optional)
   - format → print

## 4) Decoder Strategy
- Default fallback is always available: print raw args
- Add decoders gradually:
  - strings: pathname, argv[0..k]
  - flags: open flags, mmap prot/flags
- Safety rules:
  - bounded reads (avoid reading huge memory)
  - if memory read fails → fallback to raw pointer value

## 5) Compatibility Notes
- Syscall numbers and calling convention are architecture-dependent
- syspeek focuses on x86_64 first; add other arches later via:
  - separate syscall tables
  - arch-specific argument extraction logic

## 6) Testing Strategy
- Golden output tests for stable programs:
  - echo, ls, cat
- Decoder tests:
  - validate that failures fallback safely
- Stress tests later:
  - multi-threaded target
  - targets that exec

## 7) Future Extensions
- Attach mode (-p PID)
- Follow forks/children
- JSON output for analysis tooling
- Timing/profiling per syscall
