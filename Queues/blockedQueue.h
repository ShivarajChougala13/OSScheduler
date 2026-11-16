#ifndef BLOCKEDQUEUE_H
#define BLOCKEDQUEUE_H
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#include "../Process/PCBHeader.h"

// processes which are waiting for i/o are kept here (waiting state)
struct BlockedQueue{
    PCB* pcb;
    struct BlockedQueue* next;
};

void startBlockedQueue();
void addProcessInBlockedQueue(struct PCB* pcb);
void updateBlockedQueue();      // called on every clock tick
bool isBlockedQueueEmpty();

#endif
