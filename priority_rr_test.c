#include "types.h"
#include "stat.h"
#include "user.h"

#define PROCS_PER_LEVEL  2
#define NLEVELS          3      
#define ROUNDS           6
#define BURN_ITERS       1200000

static void cpu_burn(int iters) {
  volatile int s = 0;
  for (int i = 0; i < iters; i++) s += i & 7;
}

static void child_worker(int prio, int quantum, int id_in_level, char label) {
  set_sched_priority(prio);
  set_sched_quantum(quantum);

  for (int r = 0; r < ROUNDS; r++) {
    cpu_burn(BURN_ITERS);
    // print just one character to show interleaving
    printf(1, "%c", label);
  }

  // mark completion for this child
  printf(1, "|");
  exit();
}

int
main(void)
{
  // Lower parent's priority so it doesn't get in the way
  set_sched_priority(NLEVELS - 1);
  set_sched_quantum(10);

  printf(1, "\nPRIOROTY RR Scheduler Test\n");
  printf(1, "\n------------------------------\n");
  printf(1, "Legend:\n");
  for (int pr = 0; pr < NLEVELS; pr++) {
    for (int k = 0; k < PROCS_PER_LEVEL; k++) {
      char L = 'A' + pr*PROCS_PER_LEVEL + k;
      printf(1, "  %c = P%d.%d (priority=%d, q=%d)\n", L, pr, k, pr, 3 + k);
    }
  }
  printf(1, "\n------------------------------\n");
  printf(1, "\nOutput stream (letters interleave by RR; higher priority first):\n");

  // Spawn children
  int total = NLEVELS * PROCS_PER_LEVEL;
  for (int pr = 0; pr < NLEVELS; pr++) {
    for (int k = 0; k < PROCS_PER_LEVEL; k++) {
      int q = 3 + k; 
      char L = 'A' + pr*PROCS_PER_LEVEL + k;
      int pid = fork();
      if (pid < 0) {
        printf(1, "\nfork failed\n");
        goto WAIT;
      } else if (pid == 0) {
        child_worker(pr, q, k, L);
      }
      // parent continues to fork others
    }
  }

WAIT:
  // Wait for all children; final newline for clean prompt
  for (int i = 0; i < total; i++) wait();

  printf(1, "\nAll children dead. Priority RR test complete.\n");
  exit();
}
