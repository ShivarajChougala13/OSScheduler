#include "../Algorithms/shortTermScheduler.h"
#include "../Process/PCBHeader.h"
#include<stdbool.h>
#include "../Clock/clock.h"

    struct PCB* runningProcess=NULL;

void checkProcessCompletion(){
    if(runningProcess->remaining_time==0){
            changeProcessStatus(runningProcess,completedState);
            updateEndTime(runningProcess,*clk);
            bool processTableStatus=removeProcessFromProcessTable(runningProcess);
            if(!processTableStatus){
                perror("Error while removing process from Process Table \n");
                exit(EXIT_FAILURE);
            }
            runningProcess=NULL;
    }

}
void execute(){
    if(runningProcess==NULL){
    runningProcess=getProcess();    // from short term scheduler
}
    if(runningProcess!=NULL){
    updateRemainingTime(runningProcess);
    checkProcessCompletion();
    }
}
