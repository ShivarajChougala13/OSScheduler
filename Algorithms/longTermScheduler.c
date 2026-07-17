#include "../Main/main.h"
#include "../Queues/jobQueue.h"
#include "../Queues/readyQueue.h"
#include "../Clock/clock.h"

void longTermScheduler(){
    // remove process from job queue
    // add process to ready queue

   while(!isJobQueueEmpty()){
    struct PCB* pcb=getJobQueueFront();
    bool status=addProcessInReadyQueue(pcb);    // to ready Queue
    if(status==true){
    removeJobQueueProcess();
    updateProcessStatus(pcb,readyState);    // change process state
    changeArrivalTime(pcb,*clk);

    }
    else{
        break;
    }
   }

}
