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

## Locking Strategy and Synchronization Approach
--------------------------------------------
🔒 Accessing the process table must be synchronized.

### Recommended approach:
1. Acquire the process table lock before iterating processes.
2. Read process metadata while holding the lock.
3. Build a temporary kernel-side `struct procinfo` entry.
4. Use `copyout()` to move data into the user buffer.
5. Release the lock once iteration is complete.

### Notes:
- Never walk process table entries without the required lock.
- Keep lock hold time reasonable; avoid unnecessary work under lock.
- Ensure parent PID access is done safely when parent pointer can be null.


## Memory Management and `copyout` Logic
-------------------------------------
💾 Kernel code must not directly dereference user pointers.

Safe flow:
1. Validate `max_procs` first (`max_procs > 0`).
2. Validate pointer argument extraction in syscall handler.
3. For each process entry to export:
   - Fill a local kernel `struct procinfo kpi`.
   - Compute destination user virtual address:

         dst = user_pinfo + index * sizeof(struct procinfo)

   - Call `copyout(pagetable, dst, (char *)&kpi, sizeof(kpi))`.
4. If any `copyout` fails, return `-1`.

Additional safety notes:
- Bound writes by `max_procs`.
- Avoid integer overflow in address arithmetic.
- Return exact number successfully copied when complete.


## Testing Approach and Test Cases
-------------------------------
🧪 Test both functionality and failure handling.

A) Functional tests
1. Boot xv6 and run `ps`.
2. Confirm header and fixed-width formatting.
3. Confirm known processes appear (`init`, `sh`, `ps`).
4. Confirm state mapping text is correct.

B) Boundary tests
1. `max_procs = 1` returns a single row.
2. `max_procs` smaller than total processes truncates output safely.
3. `max_procs` larger than total returns only actual count.

C) Error-path tests
1. Invalid user pointer returns `-1`.
2. `max_procs <= 0` returns `-1`.
3. Ensure no kernel panic on bad arguments.

D) Concurrency sanity
1. Spawn/exit processes while repeatedly calling `ps`.
2. Verify stable behavior (no panics, no obvious corruption).