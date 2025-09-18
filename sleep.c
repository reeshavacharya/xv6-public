#include "user.h"

int main(int argc, char *argv[]){
    if(argc <2){
        printf(1, "Usage: sleep </ticks>\n");
    }
    int ticks = atoi(argv[1]);
    sleep(ticks);
    exit();
}