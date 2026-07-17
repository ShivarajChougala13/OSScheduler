#ifndef MAIN
#define MAIN

#include<stdio.h>
#include<stdlib.h>
#include "../Process/processTableHeader.h"
#include "../Queues/jobQueue.h"
#include "../Queues/readyQueue.h"


// struct PCB* pcb;
// struct processTable* processTable;

struct access{
    struct processTable* processTable;
    struct JobQueue* jobQueue;
    struct ReadyQueue* readyQueue;
};
extern struct access access;
#endif