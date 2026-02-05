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

/* Without changing printf.c, random charactization from output appears
 * I decided to make a print functions to help address the problem.
 */

static int
uint_digits(uint64 value)
{
  int digits = 1;

  while(value >= 10){
    value /= 10;
    digits++;
  }
  return digits;
}

static void
print_padding(int count)
{
  for(int i = 0; i < count; i++){
    printf(" ");
  }
}

static void
print_uint_padded(uint64 value, int width)
{
  int digits = uint_digits(value);

  printf("%lu", value);
  if(width > digits){
    print_padding(width - digits);
  }
}

static void
print_int_padded(int value, int width)
{
  uint64 magnitude = value < 0 ? (uint64)(-value) : (uint64)value;
  int digits = uint_digits(magnitude) + (value < 0 ? 1 : 0);

  printf("%d", value);
  if(width > digits){
    print_padding(width - digits);
  }
}

static void
print_str_padded(const char *value, int width)
{
  int length = 0;

  if(value){
    while(value[length]){
      length++;
    }
  }

  printf("%s", value ? value : "");
  if(width > length){
    print_padding(width - length);
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

    /*
    printf("PID    PPID   STATE      SIZE       NAME\n");
      for(int i = 0; i < count; i++){
    printf("%-6d %-6d %-10s %-10u %s\n",
           infos[i].pid,
           infos[i].ppid,
           state_name(infos[i].state),
           infos[i].sz,
           infos[i].name);
          */
    print_str_padded("PID", 6);
    print_str_padded("PPID", 6);
    print_str_padded("STATE", 10);
    print_str_padded("SIZE", 10);

    printf("NAME\n");
    for(int i = 0; i < count; i++){
      print_int_padded(infos[i].pid, 6);
      print_int_padded(infos[i].ppid, 6);
      print_str_padded(state_name(infos[i].state), 10);
      print_uint_padded(infos[i].sz, 10);
      printf("%s\n", infos[i].name);
  }

  exit(0);

}


/*

Without the functions, formatting print out like this:

PID    PPID   STATE      SIZE       NAME
%-6d %-6d %-10s %-10u q�����������V�Z�^�b�f�j���

*/
