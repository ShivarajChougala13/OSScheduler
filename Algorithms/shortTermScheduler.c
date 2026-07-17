#include "./shortTermScheduler.h"
#include "../Main/main.h"
#include "./Policy/FCFS.h"
#include "./Policy/SFJ.h"
#include "./Policy/priority.h"
#include "./Policy/roundRobbin.h"
#include<stdbool.h>

static int select=-1;

struct PCB* getProcess(){
    struct PCB* process=NULL;
    if(select==-1){
        printf("select policy: 1 - FCFS , 2 - SFJ , 3 - RoundRobbin , 4 - priority \n");
        scanf("%d",&select);
    }
    switch(select){
        case 1: // fcfs
            process=getProcessFCFS();
            break;
        case 2: //sfj
            process=getProcessSFJ();
            break;
        case 3: // roundrobbin
            process=getProcessroundRobbin();
            break;
        case 4: //priority
            process=getProcessPriority();
            break;
        default :
        printf("not a valid Policy \n");
            break;
    }
    bool readyQueueStatus;
    if(process!=NULL)
      readyQueueStatus = removeReadyQueueProcess(process);

    if(!readyQueueStatus)
    {
        perror("Failed to remove process");
        exit(EXIT_FAILURE);
    }
     updateProcessStatus(process,runningState); 
    return process;
}



// this implements different algorithmic policies..  fcfs priority roundrobbin sfj