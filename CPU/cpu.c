#include "../Algorithms/shortTermScheduler.h"
#include "../Algorithms/Policy/roundRobbin.h"
#include "../Process/PCBHeader.h"
#include "../Process/processTableHeader.h"
#include "../Queues/readyQueue.h"
#include "../Queues/blockedQueue.h"
#include <stdbool.h>
#include "../Clock/clock.h"

struct PCB* runningProcess = NULL;

static int quantumUsed = 0;         // how many ticks the running process used (for roundrobbin)
static int contextSwitches = 0;     // number of times cpu is given to a process
static int busyTime = 0;            // ticks in which cpu was executing something
static unsigned int lastEndTime = 0;

// completed processes are stored here, so the summary can be printed at the end
struct CompletedProcess{
    PCB* pcb;
    struct CompletedProcess* next;
};
static struct CompletedProcess* completedHead = NULL;
static struct CompletedProcess* completedTail = NULL;

static void addCompletedProcess(PCB* pcb){
    struct CompletedProcess* newNode = (struct CompletedProcess*)malloc(sizeof(struct CompletedProcess));
    if(newNode == NULL){
        perror("memory allocation failed in completed list\n");
        exit(EXIT_FAILURE);
    }
    newNode->pcb = pcb;
    newNode->next = NULL;

    if(completedHead == NULL){
        completedHead = newNode;
        completedTail = newNode;
        return;
    }
    completedTail->next = newNode;
    completedTail = newNode;
}

// time which the process spent in the blocked queue doing i/o
static unsigned int getIOTime(PCB* pcb){
    if(pcb->io_done){
        return pcb->io_burst;
    }
    return 0;
}

void checkProcessCompletion(){
    if(runningProcess == NULL)
        return;

    if(runningProcess->remaining_time == 0){
            // process used the cpu during this tick, so it finishes at the end of the tick (clk + 1)
            updateProcessStatus(runningProcess, completedState);
            updateEndTime(runningProcess, *clk + 1);

            // turnaround = end - arrival in system
            // waiting = turnaround - burst - i/o time  (i/o time is not waiting for cpu)
            unsigned int turnaround = runningProcess->end_time - runningProcess->system_arrival_time;
            updateWaitingTime(runningProcess, turnaround - runningProcess->burst_time - getIOTime(runningProcess));
            lastEndTime = runningProcess->end_time;

            printf("[Time %u] Process %d completed\n", runningProcess->end_time, runningProcess->pid);

            bool processTableStatus = removeProcessFromProcessTable(runningProcess);
            if(!processTableStatus){
                perror("Error while removing process from Process Table\n");
                exit(EXIT_FAILURE);
            }
            addCompletedProcess(runningProcess);
            runningProcess = NULL;
    }
}

// process asks for i/o after using io_after cpu ticks -> cpu is released, process goes to blocked queue
static void checkIORequest(){
    if(runningProcess == NULL)
        return;
    if(runningProcess->io_after == 0 || runningProcess->io_done)
        return;

    unsigned int cpuUsed = runningProcess->burst_time - runningProcess->remaining_time;
    if(cpuUsed == runningProcess->io_after){
        printf("[Time %d] Process %d requested I/O (%u ticks), moved to Blocked Queue\n", *clk + 1, runningProcess->pid, runningProcess->io_burst);
        addProcessInBlockedQueue(runningProcess);
        runningProcess = NULL;
    }
}

// roundrobbin : when the quantum is over, running process goes to the end of the ready queue
// and the head of the ready queue gets the cpu
static void checkTimeQuantum(){
    if(runningProcess == NULL)
        return;
    if(getPolicy() != POLICY_ROUNDROBBIN)
        return;
    if(quantumUsed < TIME_QUANTUM)
        return;

    // nobody is waiting so the same process continues
    if(isReadyQueueEmpty()){
        quantumUsed = 0;
        return;
    }

    // first take the next process out (this makes space in the ready queue), then add the old one
    struct PCB* old = runningProcess;
    struct PCB* next = getProcess();

    updateProcessStatus(old, readyState);
    if(!addProcessInReadyQueue(old)){
        perror("Failed to add preempted process in Ready Queue");
        exit(EXIT_FAILURE);
    }
    printf("[Time %d] Process %d preempted (time quantum over), Process %d gets the cpu\n", *clk + 1, old->pid, next->pid);

    runningProcess = next;
    quantumUsed = 0;
    contextSwitches++;
}

void execute(){
    if(runningProcess == NULL){
        runningProcess = getProcess();
        quantumUsed = 0;
        if(runningProcess != NULL){
            contextSwitches++;
        }
    }
    if(runningProcess == NULL){
        printf("[Time %d] CPU idle\n", *clk);
        return;
    }

    // first time on the cpu
    if(runningProcess->remaining_time == runningProcess->burst_time){
        updateStartTime(runningProcess, *clk);
    }
    printf("[Time %d] Process %d running (remaining %u)\n", *clk, runningProcess->pid, runningProcess->remaining_time);

    updateRemainingTime(runningProcess);
    busyTime++;
    quantumUsed++;
    checkProcessCompletion();
    checkIORequest();
    checkTimeQuantum();
}

void printStatistics(){
    char* policyNames[] = {"", "FCFS", "SFJ", "RoundRobbin", "Priority"};
    struct CompletedProcess* cur = completedHead;
    int count = 0;
    unsigned int totalWaiting = 0;
    unsigned int totalTurnaround = 0;
    unsigned int totalResponse = 0;

    printf("\n================ Summary (%s) ================\n", policyNames[getPolicy()]);
    printf("PID\tArrival\tBurst\tI/O\tStart\tEnd\tTurnaround\tWaiting\tResponse\n");
    while(cur != NULL){
        PCB* pcb = cur->pcb;
        unsigned int turnaround = pcb->end_time - pcb->system_arrival_time;
        unsigned int response = pcb->start_time - pcb->system_arrival_time;     // first cpu time - arrival
        printf("%d\t%u\t%u\t%u\t%u\t%u\t%u\t\t%u\t%u\n",
        pcb->pid,
        pcb->system_arrival_time,
        pcb->burst_time,
        getIOTime(pcb),
        pcb->start_time,
        pcb->end_time,
        turnaround,
        pcb->waiting_time,
        response);

        totalWaiting += pcb->waiting_time;
        totalTurnaround += turnaround;
        totalResponse += response;
        count++;

        // process is finished, free its memory
        struct CompletedProcess* old = cur;
        cur = cur->next;
        free(old->pcb);
        free(old);
    }
    completedHead = NULL;
    completedTail = NULL;

    if(count == 0){
        printf("no process completed\n");
        return;
    }
    printf("\nAverage Turnaround Time : %.2f\n", (double)totalTurnaround / count);
    printf("Average Waiting Time    : %.2f\n", (double)totalWaiting / count);
    printf("Average Response Time   : %.2f\n", (double)totalResponse / count);
    if(lastEndTime > 0){
        printf("Throughput              : %.3f processes/time unit\n", (double)count / lastEndTime);
        printf("CPU Utilization         : %.2f%%\n", (busyTime * 100.0) / lastEndTime);
    }
    printf("Context Switches        : %d\n", contextSwitches);
}
