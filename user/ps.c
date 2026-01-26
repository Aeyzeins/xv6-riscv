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

state_str(int state)
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

int
main(void) {


    
}


