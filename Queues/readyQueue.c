// enqueueReady()
#include "./readyQueue.h"
#include "../Main/main.h"
#include "../Process/PCBHeader.h"

static struct ReadyQueueInfo readyQueueInfo;
// initialize ready queue  
void initReadyQueue(int capacity){
    readyQueueInfo.front=NULL;
    readyQueueInfo.rear=NULL;
    readyQueueInfo.size=0;
    readyQueueInfo.capacity=capacity;
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
    struct ReadyQueueInfo readyQueueInfo;
    int size=5;
    initReadyQueue(size);
}

// add process to ready queue by long term scheduler
bool addProcessInReadyQueue(struct PCB* pcb){
    if(readyQueueInfo.size==readyQueueInfo.capacity) return false;
    struct ReadyQueue* readyQueue=access.readyQueue;

    struct ReadyQueue* newNode=NULL;
    newNode= getNewNode();
    newNode->pcb=pcb;
    newNode->next=NULL;

    if(readyQueue==NULL){
        readyQueue=newNode;
        readyQueueInfo.size++;
        readyQueueInfo.front=newNode;
        readyQueueInfo.rear=newNode;
        return true;
    }
    readyQueue->next=newNode;
    readyQueueInfo.rear=newNode;
    readyQueueInfo.size++;
    
    return true;
}



// remove process from ready queue
bool removeReadyQueueProcess(struct PCB* pcb){
    
}

