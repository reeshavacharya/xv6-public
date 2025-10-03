#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf(2, "Usage: ticks_check <pid>\n");
        exit();
    }

    int pid = atoi(argv[1]);  // convert argument to integer
    int ticks = ticks_running(pid);

    if (ticks >= 0)
        printf(1, "PID %d has run for %d ticks\n", pid, ticks);
    else
        printf(1, "PID %d does not exist\n", pid);

    exit();
}
