#include "jobQueue.h"
#include "../Clock/clock.h"
#include "../Process/PCBHeader.h"
#include "../Process/processTableHeader.h"
#include "../Main/main.h"


static struct JobQueueInformation jobQueueInformation;

// add process to jobqueue

void initJobQueue(){
    jobQueueInformation.front=NULL;
    jobQueueInformation.rear=NULL;
    jobQueueInformation.size=0;
    return;
}
void startJobQueue(){
    initJobQueue();
    return;
}
struct JobQueue* newJobQueue(){
    struct JobQueue* newNode=(struct JobQueue*)malloc(sizeof(struct JobQueue));
    if(newNode==NULL){
        perror("Error while Job queue memory allocation\n");
        exit(EXIT_FAILURE);
    }
    return newNode;
}
void addProcess(PCB* pcb){
    struct JobQueue* temp=access.jobQueue;
    struct JobQueue* newNode=NULL;
    newNode=newJobQueue();

    newNode->pcb=pcb;
    newNode->next=NULL;         // important 

    if(temp==NULL){
        access.jobQueue=newNode;
        jobQueueInformation.front=newNode;
        jobQueueInformation.rear=newNode;
        jobQueueInformation.size++;
        return;
    }
    jobQueueInformation.rear->next=newNode;
    jobQueueInformation.rear=newNode;
    jobQueueInformation.size++;

    // changing process state
    updateProcessStatus(pcb,newState);  // changing process state
    updateSystemArrivalTime(pcb,*clk);  // process entering time 
    return;
}

// remove process from job queue while switching to ready queue
void removeJobQueueProcess(){
    // struct JobQueue* temp=access.jobQueue; 
    // remove process

    // pop head....
    struct JobQueue* temp=access.jobQueue;
    if(temp==NULL) return;
    struct JobQueue* cur=temp;
    temp=temp->next;
    jobQueueInformation.front=temp;
    jobQueueInformation.size--;
    access.jobQueue=temp;   // update access.jobQueue
    if(jobQueueInformation.front==NULL){
        jobQueueInformation.rear=NULL;
    }
    free(cur);


    return;
}
struct PCB* getJobQueueFront(){
    if(jobQueueInformation.front==NULL){
        printf("Job queue is empty\n");
        return NULL;
    }
    struct JobQueue* temp=access.jobQueue;
    return temp->pcb;
}

bool isJobQueueEmpty(){
    if(jobQueueInformation.size==0) return true;
    return false;
}
void printJobQueue(){
    // struct JobQueue* temp=access.jobQueue; 
    // remove process 

    // print jobqueue
}

void jobAdmit(){
    char dir[]="./Input_Outputs/input.txt";
    FILE *fp=fopen(dir,"r");
    if(fp==NULL){
        printf(dir,"file is empty \n");
        exit(EXIT_FAILURE);
    }
    // array to store the processes in the system.

        int pid;
        int state;
        unsigned int arrival_time;
        unsigned int system_arival_time;

        unsigned int waiting_time;
        unsigned int burst_time;
        unsigned int start_time;
        unsigned int remaining_time;
        unsigned int end_time;
        size_t start_address;
        size_t size;
        int priority;
            int i=0;
            while(fscanf(fp,"%d %d %u %u %u %u %u %u %u %zu %zu %d",
            &pid, &state, &arrival_time, &system_arival_time,
            &waiting_time, &burst_time,
            &start_time, &remaining_time,
            &end_time, &start_address,
            &size, &priority
                     )==11){
        //  printf("%u \n",arrival_time);

            PCB* new_pcb=create_PCB(pid,state,arrival_time,system_arival_time,waiting_time,burst_time,start_time,remaining_time,end_time,start_address,size,priority);
            // print_pcb(new_pcb);
            addProcess(new_pcb);
            addNewProcess(new_pcb);
            }
            fclose(fp);
}

/*
enqueueJob
dequueJob
jobAdmit
printJobQueue

*/