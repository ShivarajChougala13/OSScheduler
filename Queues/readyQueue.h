#ifndef READYQUEUE
#define READYQUEUE
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#include "../Process/PCBHeader.h"

struct ReadyQueue{
    PCB* pcb;
    struct ReadyQueue* next;
};

struct ReadyQueueInfo{
    struct ReadyQueue* front;
    struct ReadyQueue* rear;
    int size;
    int capacity;
};

void initReadyQueue(int capacity);
void startReadyQueue();
struct ReadyQueue* getNewNode(); // memory allocation
bool addProcessInReadyQueue(struct PCB* pcb);
bool removeReadyQueueProcess(struct PCB* pcb);

#endif