#include "jobQueue.h"
#include "../Clock/clock.h"
#include "../Process/PCBHeader.h"
#include "../Process/processTableHeader.h"
#include "../Main/main.h"


static struct JobQueueInformation jobQueueInformation;

// processes which are read from the input file but their arrival time has not come yet
static struct JobQueue* pendingFront=NULL;
static struct JobQueue* pendingRear=NULL;

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
    struct JobQueue* temp=schedulerAccess.jobQueue;
    struct JobQueue* newNode=NULL;
    newNode=newJobQueue();

    newNode->pcb=pcb;
    newNode->next=NULL;

    // changing process state (this is done for every process, including the first one)
    updateProcessStatus(pcb,newState);  // changing process state
    updateSystemArrivalTime(pcb,*clk);  // process entering time 

    if(temp==NULL){
        schedulerAccess.jobQueue=newNode;
        jobQueueInformation.front=newNode;
        jobQueueInformation.rear=newNode;
        jobQueueInformation.size++;
    } else {
        jobQueueInformation.rear->next=newNode;
        jobQueueInformation.rear=newNode;
        jobQueueInformation.size++;
    }
    printf("[Time %d] Process %d arrived in Job Queue\n",*clk,pcb->pid);
    return;
}

// remove process from job queue while switching to ready queue
void removeJobQueueProcess(){
    struct JobQueue* temp=schedulerAccess.jobQueue;
    if(temp==NULL) return;
    struct JobQueue* cur=temp;
    temp=temp->next;
    jobQueueInformation.front=temp;
    jobQueueInformation.size--;
    schedulerAccess.jobQueue=temp;
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
    struct JobQueue* temp=schedulerAccess.jobQueue;
    return temp->pcb;
}

bool isJobQueueEmpty(){
    if(jobQueueInformation.size==0) return true;
    return false;
}
void printJobQueue(){
    struct JobQueue* temp=schedulerAccess.jobQueue;
    if(temp==NULL){
        printf("Job queue is empty\n");
        return;
    }
    printf("Job queue : ");
    while(temp!=NULL){
        printf("P%d ",temp->pcb->pid);
        temp=temp->next;
    }
    printf("\n");
}

// keep the process in pending list until its arrival time comes
static void addPendingProcess(PCB* pcb){
    struct JobQueue* newNode=newJobQueue();
    newNode->pcb=pcb;
    newNode->next=NULL;

    if(pendingFront==NULL){
        pendingFront=newNode;
        pendingRear=newNode;
        return;
    }
    pendingRear->next=newNode;
    pendingRear=newNode;
}

// called on every clock tick
// if arrival time of a pending process is reached then it enters the job queue
void checkNewArrivals(){
    struct JobQueue* current=pendingFront;
    struct JobQueue* previous=NULL;

    while(current!=NULL){
        struct JobQueue* next=current->next;
        if(current->pcb->arrival_time <= (unsigned int)*clk){
            addProcess(current->pcb);

            // remove from pending list
            if(previous==NULL){
                pendingFront=next;
            } else {
                previous->next=next;
            }
            if(pendingRear==current){
                pendingRear=previous;
            }
            free(current);
        } else {
            previous=current;
        }
        current=next;
    }
}

void jobAdmit(){
    char* dir=inputFile;    // default Input_Outputs/input.txt or the file given in command line
    FILE *fp=fopen(dir,"r");
    if(fp==NULL){
        printf("%s file not found (run the program from the project folder) \n",dir);
        exit(EXIT_FAILURE);
    }
    // first line of the file is the column names, so skip it
    char header[256];
    fgets(header,sizeof(header),fp);

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
        unsigned int io_after;
        unsigned int io_burst;
            int i=0;
            while(fscanf(fp,"%d %d %u %u %u %u %u %u %u %zu %zu %d %u %u",
            &pid, &state, &arrival_time, &system_arival_time,
            &waiting_time, &burst_time,
            &start_time, &remaining_time,
            &end_time, &start_address,
            &size, &priority,
            &io_after, &io_burst
                     )==14){
        //  printf("%u \n",arrival_time);

            PCB* new_pcb=create_PCB(pid,state,arrival_time,system_arival_time,waiting_time,burst_time,start_time,remaining_time,end_time,start_address,size,priority);
            setIORequest(new_pcb,io_after,io_burst);    // 0 0 means no i/o
            // print_pcb(new_pcb);
            addNewProcess(new_pcb);         // process table has all the processes in the system
            addPendingProcess(new_pcb);     // job queue gets it only when arrival time comes
            i++;
            }
            fclose(fp);
            printf("%d processes loaded from input file\n",i);
}

/*
enqueueJob
dequueJob
jobAdmit
printJobQueue

*/
