//sleep system call declaration:int sleep(int)

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char **argv){
    if(argc <= 1){
        fprintf(2,"usage:sleep [number]");
        exit(1);
    }
    int sleep_duration = atoi(argv[1]);
    int return_code = sleep(sleep_duration);
    exit(return_code);
}