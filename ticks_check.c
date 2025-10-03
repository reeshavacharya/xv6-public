#include "types.h"
#include "user.h"

int main(void) {
    int pid = getpid();
    printf(1, "My PID: %d\n", pid);

    for(int i = 0; i < 5; i++) {
        // Burn CPU for a short while to generate some ticks
        for(volatile int j = 0; j < 10000000; j++); 
        int ticks = ticks_running(pid);
        printf(1, "Ticks running (iteration %d): %d\n", i+1, ticks);
    }

    exit();
}
