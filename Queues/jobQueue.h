#ifndef JOBQUEUE.h
#define JOBQUEUE.h
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
extern struct JobQueue jobQueue;

#endif