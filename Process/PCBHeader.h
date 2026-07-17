#ifndef PCB_H
#define PCB_H

#include<stdio.h>
#include<stdlib.h>

// admitted state...
typedef enum state{
   newState=0,
   readyState=1,
   runningState=2,
   waitingState=3,
   terminatedState=4,
   completedState=5
}state;

// pcb definition
// page table location and information 
typedef struct PageTable{
    size_t start_address;
    size_t size;
} pageTable;

typedef struct PCB{
    int pid;
    state state;
    unsigned int arrival_time;
    unsigned int system_arrival_time;
    unsigned int waiting_time;
    unsigned int burst_time;
    unsigned int start_time;
    unsigned int remaining_time;
    unsigned int end_time; // check whether to include or not.
    int priority;
    pageTable pageTable;

// system_arrival_time
// ready_enter_time
} PCB;

PCB* create_PCB( 

    int pid,
    state state,
    unsigned int arrival_time,
    unsigned int system_arrival_time,
    unsigned int waiting_time,
    unsigned int burst_time,
    unsigned int start_time,
    unsigned int remaining_time,
    unsigned int end_time,
    size_t start_address,   // make a separate structure for this

    size_t size,
    int priority);
void print_pcb(PCB* pcb);
void updateProcessStatus(struct PCB* pcb, state state);
void updateWaitingTime(struct PCB* pcb, unsigned int waitingTime);
void updateEndTime(struct PCB* pcb, unsigned int endTime);
void updateRemainingTime(struct PCB* pcb);
void updateArrivalTime(struct PCB* pcb, unsigned int arrivalTime);
void updateSystemArrivalTime(struct PCB* pcb, unsigned int time);

// unsigned int cpu_time_used;
// unsigned int io_time;

// MemoryInfo memory;
// unsigned int context_switches;
#endif



