# Project 1: System Call Implementation for xv6


## Overview
1. Understand the full system call path from user space to kernel space.
2. Practice safe kernel programming with proper locking and memory isolation.
3. Build confidence navigating and modifying a real operating system codebase.

## Required Features
-----------------
1) `getprocs` system call
   - Signature:

         int getprocs(struct procinfo *pinfo, int max_procs)

   - Parameters:
     - `pinfo`: user pointer to an array of `struct procinfo`
     - `max_procs`: maximum entries to populate

   - Return values:
     - On success: number of entries written
     - On failure: `-1`
       - invalid user pointer
       - invalid `max_procs` (e.g., <= 0)

2) Shared process structure
   Define this in a header shared by kernel and user space:

    struct procinfo {
        int pid;           // Process ID
        int ppid;          // Parent process ID
        int state;         // Numeric process state
        uint sz;           // Size of process memory (bytes)
        char name[16];     // Process name
    };

3) `ps` user utility
   - Fetch process entries using `getprocs()`
   - Print fixed-width table columns
   - Convert state integers to readable labels:
     - UNUSED
     - EMBRYO
     - SLEEPING
     - RUNNABLE
     - RUNNING
     - ZOMBIE

   Required output shape:

```text
PID    PPID   STATE      SIZE       NAME
1      0      SLEEPING   12288      init
2      1      SLEEPING   16384      sh
3      2      RUNNING    8192       ps
```

## Implementation Checklist
------------------------
- Register a new syscall number.
- Add user-space syscall interface/stub for `getprocs`.
- Add kernel syscall dispatcher entry.
- Implement `sys_getprocs` argument validation.
- Implement safe traversal of the process table in kernel code.
- Copy each `procinfo` entry to user memory with `copyout()`.
- Add `ps.c` and include it in the build system.