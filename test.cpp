#include <stdio.h>
#include <unistd.h>
#include "MyThreadPool.h"

//g++ test.cpp -o t -lpthread

int main(int argc, char **argv)
{
    printf("begin\n");

    MyThreadPool pool(2);

    pool.enqueue([](){
        printf("run task 1 ... @%ld\n", time(nullptr));
        usleep(1*1000*1000);
        printf("run end 1 @%ld\n", time(nullptr));
    });

    pool.enqueue([](){
        printf("run task 2 ... @%ld\n", time(nullptr));
        usleep(2*1000*1000);
        printf("run end 2 @%ld\n", time(nullptr));
    });

    pool.enqueue([](){
        printf("run task 3 ... @%ld\n", time(nullptr));
        usleep(3*1000*1000);
        printf("run end 3 @%ld\n", time(nullptr));
    });

    pool.enqueue([](){
        printf("run task 1- ... @%ld\n", time(nullptr));
        usleep(1*1000*1000);
        printf("run end 1- @%ld\n", time(nullptr));
    });

    pool.enqueue([](){
        printf("run task 2- ... @%ld\n", time(nullptr));
        usleep(2*1000*1000);
        printf("run end 2- @%ld\n", time(nullptr));
    });

    pool.enqueue([](){
        printf("run task 3- ... @%ld\n", time(nullptr));
        usleep(3*1000*1000);
        printf("run end 3- @%ld\n", time(nullptr));
    });

    printf("end\n");
    return 0;
}

