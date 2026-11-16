#include "./blockedQueue.h"
#include "../Main/main.h"
#include "../Clock/clock.h"
#include "../Process/PCBHeader.h"

void startBlockedQueue(){
    schedulerAccess.blockedQueue=NULL;
}

struct BlockedQueue* getNewBlockedNode(){
    struct BlockedQueue* newNode=(struct BlockedQueue*)malloc(sizeof(struct BlockedQueue));
    if(newNode==NULL){
        perror("memory allocation failed In Blocked Queue\n");
        exit(EXIT_FAILURE);
    }
    return newNode;
}

// running process asked for i/o, so it is added at the end of the blocked queue
void addProcessInBlockedQueue(struct PCB* pcb){
    struct BlockedQueue* newNode=getNewBlockedNode();
    newNode->pcb=pcb;
    newNode->next=NULL;

    updateProcessStatus(pcb,waitingState);
    pcb->io_remaining=pcb->io_burst;

    if(schedulerAccess.blockedQueue==NULL){
        schedulerAccess.blockedQueue=newNode;
        return;
    }
    struct BlockedQueue* temp=schedulerAccess.blockedQueue;
    while(temp->next!=NULL){
        temp=temp->next;
    }
    temp->next=newNode;
}

// called on every tick (before the cpu runs)
// io_remaining reached 0 in an earlier tick -> process goes back to ready queue
// otherwise one more tick of i/o is done
void updateBlockedQueue(){
    struct BlockedQueue* current=schedulerAccess.blockedQueue;
    struct BlockedQueue* previous=NULL;

    while(current!=NULL){
        struct BlockedQueue* next=current->next;
        struct PCB* pcb=current->pcb;
        bool removed=false;

        if(pcb->io_remaining==0){
            // if ready queue is full the process stays here and tries again in the next tick
            if(addProcessInReadyQueue(pcb)){
                updateProcessStatus(pcb,readyState);
                pcb->io_done=1;
                printf("[Time %d] Process %d finished I/O, moved to Ready Queue\n",*clk,pcb->pid);

                if(previous==NULL){
                    schedulerAccess.blockedQueue=next;
                } else {
                    previous->next=next;
                }
                free(current);
                removed=true;
            }
        } else {
            pcb->io_remaining--;
        }

        if(!removed){
            previous=current;
        }
        current=next;
    }
}

bool isBlockedQueueEmpty(){
    if(schedulerAccess.blockedQueue==NULL) return true;
    return false;
}
