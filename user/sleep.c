#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc,char *argv[]) {
    // This is a simple program that sleeps for 5 seconds
    // and then exits.
    if(argc != 2) {
        fprintf(2, "Usage: sleep time\n");
        exit(1);
    }
    unsigned int time = atoi(argv[1]);
    sleep(time);
    return 0;
}