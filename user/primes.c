//Initial method:
//  Inspired from https://swtch.com/~rsc/thread/,I can to it as follows:
//  Enumerate number from 2 to 35,then create 4 pipes
//  The first pipe links 2-3,2th links 3-5,3th for 5-7
//  because 5 <= sqrt(35) <= sqrt(7),so I just need to fork 3 processes;
//  the root process filter out 2,3 deals with 3,etc.
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int main(int argc, char **argv)
{
    // first ,create 4 pipes;
    int two_to_three[2],
        three_to_five[2],
        five_to_seven[2];
    pipe(two_to_three);
    pipe(three_to_five);
    pipe(five_to_seven);
    // the root process

    for (int i = 2; i <= 35; i++)
    {
        if (i == 2 || i % 2 != 0)
            write(two_to_three[1], &i, 4);
    }
    // IMPORTANT:YOU SHOULD NOTICE THAT IF YOUR DO NOT CLOSE THIS WRITE END BEFORE YOU FORKED A NEW PROCESS,THE READ WILL BE BLOCKED FOREVER!!!
    close(two_to_three[1]);

    // send all numbers , and then wait all child process to exit
    if (fork() == 0)
    {
        // close the unneeded fd so that the system won't out of resource
        close(0);
        close(1);
        int receiver;
        //close(two_to_three[1]);
        // the 3 process
        while (read(two_to_three[0], &receiver, 4) != 0)
        {
            if (receiver == 3 || receiver % 3 != 0)
            {
                write(three_to_five[1], &receiver, 4);
            }
        }
        close(three_to_five[1]);

        close(five_to_seven[1]);
        // get all of the number from father , normally exits
        exit(0);
    }
    // IMPORTANT:YOU SHOULD NOTICE THAT IF YOUR DO NOT CLOSE THIS WRITE END BEFORE YOU FORKED A NEW PROCESS,THE READ WILL BE BLOCKED FOREVER!!!
    close(three_to_five[1]);
    if (fork() == 0)
    {
        // close the unneeded fd so that the system won't out of resource
        close(0);
        //close(1);
        int receiver;
        //close(three_to_five[1]);
        // the 5 process
        while (read(three_to_five[0], &receiver, 4) != 0)
        {
            if (receiver == 5 || receiver % 5 != 0)
            {
                write(five_to_seven[1], &receiver, 4);
            }
        }
        close(five_to_seven[1]);
        close(two_to_three[1]);

        exit(0);
        // get all of the number from father , normally exits
    }
    // IMPORTANT:YOU SHOULD NOTICE THAT IF YOUR DO NOT CLOSE THIS WRITE END BEFORE YOU FORKED A NEW PROCESS,THE READ WILL BE BLOCKED FOREVER!!!
    close(five_to_seven[1]);
    if (fork() == 0)
    {
        // close the unneeded fd so that the system won't out of resource
        close(0);
        int receiver;
        //close(five_to_seven[1]);
        // the 7 process
        while (read(five_to_seven[0], &receiver, 4) != 0)
        {
            if (receiver == 7 || receiver % 7 != 0)
            {
                printf("prime %d\n", receiver);
            }
        }
        close(two_to_three[1]);
        close(three_to_five[1]);

        // get all of the number from father , normally exits
    }

    wait((int *)0);
    wait((int *)0);
    wait((int *)0);
    exit(0);
}