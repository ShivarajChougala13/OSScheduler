#include "./shortTermScheduler.h"
#include "../Main/main.h"
#include "./Policy/FCFS.h"
#include "./Policy/SFJ.h"
#include "./Policy/priority.h"
#include "./Policy/roundRobbin.h"
#include <stdbool.h>

static int policy = -1;     // (named policy because "select" is already a system function)

// ask the policy once before the clock starts
void selectPolicy(){
    while(policy < 1 || policy > 4){
        printf("select policy: 1 - FCFS , 2 - SFJ , 3 - RoundRobbin , 4 - priority \n");
        if(scanf("%d", &policy) != 1){
            printf("invalid input \n");
            exit(EXIT_FAILURE);
        }
        if(policy < 1 || policy > 4){
            printf("not a valid Policy \n");
        }
    }
}

int getPolicy(){
    return policy;
}

struct PCB* getProcess(){
    struct PCB* process = NULL;
    if(policy == -1){
        selectPolicy();
    }

    switch(policy){
        case POLICY_FCFS:
            process = getProcessFCFS();
            break;
        case POLICY_SFJ:
            process = getProcessSFJ();
            break;
        case POLICY_ROUNDROBBIN:
            process = getProcessroundRobbin();
            break;
        case POLICY_PRIORITY:
            process = getProcessPriority();
            break;
        default:
            printf("not a valid Policy \n");
            return NULL;
    }

    if(process == NULL){
        return NULL;
    }

    bool readyQueueStatus = removeReadyQueueProcess(process);
    if(!readyQueueStatus){
        perror("Failed to remove process");
        exit(EXIT_FAILURE);
    }

    updateProcessStatus(process, runningState);
    return process;
}



// this implements different algorithmic policies..  fcfs priority roundrobbin sfj
// FCFS, SFJ and priority are non preemptive, only roundrobbin is preempted (by time quantum in cpu.c)
// priority : smaller number means higher priority
