#include "../Main/main.h"
#include "../Queues/jobQueue.h"
#include "../Queues/readyQueue.h"
#include "../Clock/clock.h"

void longTermScheduler(){
    while(!isJobQueueEmpty()){
        struct PCB* pcb = getJobQueueFront();
        if(pcb == NULL){
            break;
        }

        bool status = addProcessInReadyQueue(pcb);
        if(status == true){
            removeJobQueueProcess();
            updateProcessStatus(pcb, readyState);
            updateArrivalTime(pcb, *clk);
            printf("[Time %d] Process %d moved to Ready Queue\n", *clk, pcb->pid);
        } else {
            break;      // ready queue is full, process waits in job queue
        }
    }
}
