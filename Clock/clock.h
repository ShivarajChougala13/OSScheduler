#ifndef CLOCK_H
#define CLOCK_H

#include<stdlib.h>
#include<stdio.h>
#include<sys/shm.h>    // create shared memory 
#include<sys/ipc.h>
#include<signal.h>  // click shared memory
#include<unistd.h>  // sleep function

    extern int *clk;
    // extern  int shmid;

    int* getClock();
    void killClock();
#endif