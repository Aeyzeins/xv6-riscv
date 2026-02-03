//1st step of the Project 1: Add the shared struct header so both kernel and user programs can use it
#ifndef _PROCINFO_H_
#define _PROCINFO_H_


/*
 * Structure to hold process information
*/
struct procinfo {
    int pid;                    // Process ID
    int ppid;                   // Parent process ID
    int state;                  // Numeric Process state
    uint sz;                    // Size of process memory in bytes
    char name[16];              // Process name

}

#endif 