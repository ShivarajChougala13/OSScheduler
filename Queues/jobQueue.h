#ifndef JOBQUEUE_H
#define JOBQUEUE_H
#include<stdio.h>
#include<stdlib.h>
#include "../Process/PCBHeader.h"
#include<stdbool.h>

struct JobQueue{
    PCB* pcb;
    struct JobQueue* next;
};

struct JobQueueInformation{
    struct JobQueue* front;
    struct JobQueue* rear;
    int size;
};

void addProcess(PCB* pcb);
void removeJobQueueProcess();
void printJobQueue();
// struct JobQueue* newJobQueue();
// void initJobQueue();
void startJobQueue();
struct PCB* getJobQueueFront();
bool isJobQueueEmpty();
void jobAdmit();            // read all processes from input file
void checkNewArrivals();    // move processes whose arrival time has come into job queue

#endif
