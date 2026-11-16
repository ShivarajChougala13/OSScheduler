#include "../../Process/PCBHeader.h"
#include "../../Main/main.h"

struct PCB* getProcessFCFS(){
    struct ReadyQueue* queue = schedulerAccess.readyQueue;
    if(queue == NULL)
        return NULL;

    return queue->pcb;
}
