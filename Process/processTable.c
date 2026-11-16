#include "processTableHeader.h"
#include "../Main/main.h"

// add new process to the table 
void addNewProcess(PCB* pcb){
    struct processTable* new_process=(struct processTable*)malloc(sizeof(struct processTable));
    if(new_process==NULL){
        perror("memory allocation failed in Process Table\n");
        exit(EXIT_FAILURE);
    }
    new_process->pid=pcb->pid;
    new_process->state=pcb->state;
    new_process->priority=pcb->priority;
    new_process->pcb=pcb;
    new_process->next=NULL;

    struct processTable* activeProcesses=schedulerAccess.processTable;

    if(activeProcesses==NULL){
        schedulerAccess.processTable = new_process;
        return;
    }

    struct processTable* cur=activeProcesses;
    while(cur->next){
        cur=cur->next;
    }
    cur->next=new_process;
    return ;
}

// print process table 
   void PrintProcessTable(){
    struct processTable* activeProcesses=schedulerAccess.processTable;

        struct processTable* cur=activeProcesses;
        if(cur==NULL){
            printf("no processes have created \n");
            return ;
        }
       while(cur!=NULL){
        printf("%d %d %u %u %u %u %u %u %u %zu %zu %d\n",
        cur->pcb->pid,
        cur->pcb->state,
        cur->pcb->arrival_time,
        cur->pcb->system_arrival_time,
        cur->pcb->waiting_time,
        cur->pcb->burst_time,
        cur->pcb->start_time,
        cur->pcb->remaining_time,
        cur->pcb->end_time,
        cur->pcb->pageTable.start_address,
        cur->pcb->pageTable.size,
        cur->pcb->priority);
        
        cur=cur->next;
        }
        return;
}

bool removeProcessFromProcessTable(struct PCB* pcb){
    if(schedulerAccess.processTable == NULL || pcb == NULL){
        return false;
    }

    struct processTable* current = schedulerAccess.processTable;
    struct processTable* previous = NULL;

    while(current != NULL){
        if(current->pcb == pcb){
            if(previous == NULL){
                schedulerAccess.processTable = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            return true;
        }
        previous = current;
        current = current->next;
    }

    return false;
}
/*
create process
remove process 
print process
find process
*/