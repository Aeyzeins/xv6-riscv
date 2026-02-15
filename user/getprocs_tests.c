#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/procinfo.h"
#include "user/user.h"

static int failures = 0;

static void
check(const char *name, int cond)
{
  if(cond){
    printf("PASS: %s\n", name);
  } else {
    printf("FAIL: %s\n", name);
    failures++;
  }
}

int
main(void)
{
  struct procinfo one[1];
  struct procinfo small[3];
  struct procinfo all[NPROC];
  int total;
  int got;

  // Baseline total for comparison in boundary tests.
  total = getprocs(all, NPROC);
  check("baseline getprocs(all, NPROC) succeeds", total >= 1);

  // B1: max_procs = 1 should return exactly one row.
  got = getprocs(one, 1);
  check("max_procs=1 returns 1", got == 1);

  // B2: max_procs smaller than total should truncate to max safely.
  if(total > 3){
    got = getprocs(small, 3);
    check("max_procs smaller than total truncates", got == 3);
  } else {
    // If system currently has <=3 procs, we can only assert upper bound.
    got = getprocs(small, 3);
    check("small max returns <= 3", got >= 0 && got <= 3);
  }

  // B3: max_procs larger than total should return actual count only.
  got = getprocs(all, NPROC);
  check("max_procs larger/equal returns actual count", got >= 1 && got <= NPROC);

  // C2: max_procs <= 0 returns -1.
  got = getprocs(all, 0);
  check("max_procs=0 returns -1", got == -1);

  got = getprocs(all, -1);
  check("max_procs<0 returns -1", got == -1);

  // C1: invalid user pointer returns -1 (no kernel panic).
  got = getprocs((struct procinfo*)0xffffffffffffffffULL, 1);
  check("invalid pointer returns -1", got == -1);

  if(failures == 0){
    printf("All getprocs tests passed.\n");
    exit(0);
  }

  printf("getprocs tests failed: %d\n", failures);
  exit(1);
}
