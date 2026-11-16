// enqueueReady()
#include "./readyQueue.h"
#include "../Main/main.h"
#include "../Process/PCBHeader.h"

static struct ReadyQueueInfo readyQueueInfo;

void initReadyQueue(int capacity){
    readyQueueInfo.front=NULL;
    readyQueueInfo.rear=NULL;
    readyQueueInfo.size=0;
    readyQueueInfo.capacity=capacity;
    schedulerAccess.readyQueue=NULL;
}

// allocate memory 
struct ReadyQueue* getNewNode(){
    struct ReadyQueue* newNode=(struct ReadyQueue*)malloc(sizeof(struct ReadyQueue));
    if (newNode == NULL)
    {
        perror("memory allocation failed In Ready Queue\n");
        exit(EXIT_FAILURE);
    }
    return newNode;
}

// start ready queue 
void startReadyQueue(){
    int size=5;
    initReadyQueue(size);
}

bool addProcessInReadyQueue(struct PCB* pcb){
    if(readyQueueInfo.size==readyQueueInfo.capacity) return false;
    struct ReadyQueue* newNode = getNewNode();
    newNode->pcb = pcb;
    newNode->next = NULL;

    if(schedulerAccess.readyQueue == NULL){
        schedulerAccess.readyQueue = newNode;
        readyQueueInfo.front = newNode;
        readyQueueInfo.rear = newNode;
        readyQueueInfo.size++;
        return true;
    }

    readyQueueInfo.rear->next = newNode;
    readyQueueInfo.rear = newNode;
    readyQueueInfo.size++;
    return true;
}

bool removeReadyQueueProcess(struct PCB* pcb){
    if(schedulerAccess.readyQueue == NULL) return false;

    struct ReadyQueue* current = schedulerAccess.readyQueue;
    struct ReadyQueue* previous = NULL;

    while(current != NULL){
        if(current->pcb == pcb){
            if(previous == NULL){
                schedulerAccess.readyQueue = current->next;
            } else {
                previous->next = current->next;
            }

            if(readyQueueInfo.rear == current){
                readyQueueInfo.rear = previous;
            }
            if(readyQueueInfo.front == current){
                readyQueueInfo.front = current->next;
            }

            readyQueueInfo.size--;
            free(current);
            return true;
        }
        previous = current;
        current = current->next;
    }

    return false;
}

bool isReadyQueueEmpty(){
    if(readyQueueInfo.size==0) return true;
    return false;
}
