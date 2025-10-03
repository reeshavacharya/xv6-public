#include "types.h"
#include "user.h"

#define NUM_JOBS 3

void burn_cpu(int loops) {
    volatile int i, j;
    for(i = 0; i < loops; i++)
        for(j = 0; j < 1000000; j++);
}

int main(void) {
    int pids[NUM_JOBS];
    int priorities[NUM_JOBS] = {1, 2, 0}; // example priorities: 0=high, 2=low
    int i;

    printf(1, "Launching %d CPU-bound jobs with different priorities...\n", NUM_JOBS);

    for(i = 0; i < NUM_JOBS; i++){
        int pid = fork();
        if(pid == 0){
            // Child process
            set_sched_priority(priorities[i]);
            printf(1, "PID %d: started, priority = %d\n", getpid(), get_sched_priority(getpid()));

            // Burn CPU for a while
            burn_cpu((i+1) * 5); // vary workload

            printf(1, "PID %d: finished, ticks used = %d\n", getpid(), ticks_running(getpid()));
            exit();
        } else if(pid > 0){
            pids[i] = pid;
        }
    }

    // Parent waits for all children
    for(i = 0; i < NUM_JOBS; i++){
        int wpid = wait();
        printf(1, "Parent: PID %d reaped\n", wpid);
    }

    printf(1, "All jobs finished\n");
    exit();
}
