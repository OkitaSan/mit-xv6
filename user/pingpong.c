#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define PING 0xAA
#define PONG 0xBB
int main(int argc, char **argv){
    int ping_pipe[2], // ping pipe creates a pipe that the parent sends a byte to the child process
        pong_pipe[2]; // pong pipe creates a pipe that the child sends a byte to the parent process
    pipe(ping_pipe);
    pipe(pong_pipe);
    //May it work fine ...
    if(fork() == 0){
        // Child process
        char read_buffer[10];
        read(ping_pipe[1],read_buffer,1); // receive the ping singal ,read 1 bytes
        printf("%d: received ping\n",getpid());
        char write_buffer[10] = {PONG};
        //Now , write pong!
        write(ping_pipe[0],write_buffer,1);
        exit(0);
    }else{
        //Parent process
        char write_buffer[10] = {PING};
        write(ping_pipe[0],write_buffer,1); // write ping to the pipe
        //Now , read pong!
        char read_buffer[10];
        read(ping_pipe[1],read_buffer,1); // receive the pong singal ,read 1 bytes
        wait((int*)0);
        printf("%d: received pong\n",getpid());
        exit(0);
    }


}