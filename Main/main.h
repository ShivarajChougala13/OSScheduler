#ifndef MAIN
#define MAIN

#include<stdio.h>
#include<stdlib.h>
#include "../Process/processTableHeader.h"
#include "../Queues/jobQueue.h"
#include "../Queues/readyQueue.h"
#include "../Queues/blockedQueue.h"

struct SchedulerAccess{
    struct processTable* processTable;
    struct JobQueue* jobQueue;
    struct ReadyQueue* readyQueue;
    struct BlockedQueue* blockedQueue;
};
extern struct SchedulerAccess schedulerAccess;
extern char* inputFile;     // input file path (can be given as command line argument)
#endif