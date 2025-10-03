#include "types.h"
#include "user.h"
   
#define NUM_JOBS 3
    
void burn_cpu(int seconds) { 
    volatile int i, j;    
    for(i = 0; i < seconds; i++)
        for(j = 0; j < 10000000; j++); 
}

int main(void) {
    int pids[NUM_JOBS];
    int predicted_lengths[NUM_JOBS];
 
    printf(1, "Launching %d CPU-bound jobs...\n", NUM_JOBS);

    for(int i = 0; i < NUM_JOBS; i++){
        predicted_lengths[i] = (i+1) * 10; // Example: 10, 20, 30 
    }
 
    // Sort jobs by predicted length (simple bubble sort) 
    for(int i = 0; i < NUM_JOBS-1; i++){
        for(int j = 0; j < NUM_JOBS-1-i; j++){
            if(predicted_lengths[j] > predicted_lengths[j+1]){
                int tmp_len = predicted_lengths[j];
                predicted_lengths[j] = predicted_lengths[j+1];
                predicted_lengths[j+1] = tmp_len;
            }
        } 
    }

    printf(1, "SJF queue order (PID + predicted lengths):\n");
    for(int i = 0; i < NUM_JOBS; i++){
        printf(1, "Job %d: predicted length = %d\n", i, predicted_lengths[i]);
    }
  
    for(int i = 0; i < NUM_JOBS; i++){
        int pid = fork();
        if(pid == 0){
            // Child   
            sleep(1); // allow all jobs to be created
            printf(1, "PID %d: started, predicted length = %d\n", getpid(), predicted_lengths[i]);

            // Burn CPU proportional to predicted length
            burn_cpu(predicted_lengths[i]); 

            printf(1, "PID %d finished, ticks used  = %d\n", getpid(), ticks_running(getpid()));
            exit();
        } else if(pid > 0){
            pids[i] = pid;
        }
    }

    // Wait for all children
    for(int i = 0; i < NUM_JOBS; i++){
        int wpid = wait();  
        printf(1, "Parent: PID %d reaped\n", wpid);
    }

    printf(1, "All jobs finished\n");
    exit();
}
   


