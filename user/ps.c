#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/procinfo.h"
#include "user/user.h"

/*
Create a user program ps.c that uses your new system call.
The output must be formatted as a table with fixed-width columns.
for the STATE column, you must map the internal kernel state integers to the following strings:
 UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE.
*/

/*
Defining state cases, numeric values into readable words
*/

static const char*  //returns pointer to a string literal (read-only)

state_name(int state)
{
  switch(state){
  case 0: return "UNUSED";
  case 1: return "EMBRYO";
  case 2: return "SLEEPING";
  case 3: return "RUNNABLE";
  case 4: return "RUNNING";
  case 5: return "ZOMBIE";
  default: return "UNKNOWN";
  }
}

int main(void) {

    // Declaring variables:
    struct procinfo infos[NPROC]; // Array to hold info of all possible processes.
    int count = getprocs(infos, NPROC); // Call to getprocs syscall to fill infos array, returns number of processes.

    if (count < 0) { //If failed, print error and exit.
        fprintf(2, "ps: getprocs failed \n"); // Print error message if getprocs fails
        exit(1);
    }

    // Print header for the output table.
    printf("PID    PPID   STATE      SIZE       NAME\n");
      for(int i = 0; i < count; i++){
    printf("%-6d %-6d %-10s %-10u %s\n",
           infos[i].pid,
           infos[i].ppid,
           state_name(infos[i].state),
           infos[i].sz,
           infos[i].name);
  }

  exit(0);

}


/*

Without the functions, formatting print out like this:

PID    PPID   STATE      SIZE       NAME
%-6d %-6d %-10s %-10u q�����������V�Z�^�b�f�j���

*/
