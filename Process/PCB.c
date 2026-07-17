// process definition
// this contains definition for "create_pcb" and "display pcb"

/*
    notes:
    string inside printf is called format string 
    %d is called placeholder or format specifier 
*/
#include "./PCBHeader.h"

PCB* getNewPCB(){
    PCB* newPCB=(PCB*) malloc(sizeof(PCB));
    PCB* newPCB=NULL;
    return newPCB;
}
PCB* create_PCB( int pid,state state, unsigned int arrival_time, unsigned int system_arrival_time, unsigned int waiting_time, unsigned int burst_time,
    unsigned int start_time, unsigned int remaining_time, unsigned int end_time, size_t start_address, size_t size, int priority){
    
    PCB* newPCB=getNewPCB(); // memory allocate
                    newPCB->pid=pid;
                    newPCB->state=state;
                    newPCB->arrival_time=arrival_time;
                    newPCB->system_arrival_time=system_arrival_time;
                    newPCB->waiting_time=waiting_time;
                    newPCB->burst_time=burst_time;
                    newPCB->start_time=start_time;
                    newPCB->remaining_time=remaining_time;
                    newPCB->end_time=end_time;
                    newPCB->pageTable.start_address=start_address;
                    newPCB->pageTable.size=size;
                    newPCB->priority=priority;     
                return newPCB;
    }
void print_pcb(PCB* pcb){
        PCB* cur=pcb;
        if(cur==NULL){
            printf("PCB haven't created\n");
            return ;
        }
        printf("%d %d %u %u %u %u %u %u %u %zu %zu %d\n",
        cur->pid,
        cur->state,
        cur->arrival_time,
        cur->system_arrival_time,
        cur->waiting_time,
        cur->burst_time,
        cur->start_time,
        cur->remaining_time,
        cur->end_time,
        cur->pageTable.start_address,
        cur->pageTable.size,
        cur->priority
        );
        
        return;
}

void updateProcessStatus(struct PCB* pcb, state state){
    pcb->state=state;
}

void updateArrivalTime(struct PCB* pcb, unsigned int arrivalTime){
    pcb->arrival_time=arrivalTime;
}
void updateRemainingTime(struct PCB* pcb){
    pcb->remaining_time--;
}
void updateEndTime(struct PCB* pcb, unsigned int endTime){
    pcb->end_time=endTime;
}

void updateWaitingTime(struct PCB* pcb, unsigned int waitingTime){
    pcb->waiting_time=waitingTime;
}
void updateSystemArrivalTime(struct PCB* pcb, unsigned int time){
    pcb->system_arrival_time=time;
}

