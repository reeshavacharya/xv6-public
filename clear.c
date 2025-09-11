#include "types.h"
#include "user.h"
#include "fcntl.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

int main(int argc, char *argv[]) {
    int i, j;

    // Loop over all screen positions and print space
    for (i = 0; i < SCREEN_HEIGHT; i++) {
        for (j = 0; j < SCREEN_WIDTH; j++) {
            printf(1, " ");
        }
        printf(1, "\n");
    }

    exit();
}
