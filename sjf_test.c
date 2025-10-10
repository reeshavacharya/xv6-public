#include "types.h"
#include "stat.h"
#include "user.h"

#define PROCESSES 5

int
main(void)
{
    int process_id[PROCESSES];
    int i;

    printf(1, "\nSJF Scheduler Test\n");
    printf(1, "\n------------------------------\n");
    for(i = 0; i < PROCESSES; i++) {
        int pid = fork();

        if(pid == 0) {
            // Child process
            int work = (i + 1) * 5000000;  // increasing workload
            volatile int sum = 0;
            for(int j = 0; j < work; j++) {
                sum += j % 7;
            }
            printf(1, "\nChild %d finished (work = %d)\n", getpid(), work);
            exit();
        } else if(pid > 0) {
            // Parent process
            process_id[i] = pid;
            int len = sjf_job_length(pid);
            printf(1, "\nBirth of child PID %d (predicted job length = %d)\n", pid, len);
        } else {
            printf(1, "\nFork failed!\n");
            exit();
        }
    }

    // Wait for all children to finish
    for(i = 0; i < PROCESSES; i++) {
        wait();
    }

    printf(1, "\nAll children dead. SJF test complete.\n");
    exit();
}
