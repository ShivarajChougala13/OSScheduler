#include "../../Process/PCBHeader.h"
#include "../../Main/main.h"

// head of the ready queue is selected (same as FCFS).
// when its time quantum is over the cpu puts it back at the end of the ready queue,
// that is how the queue keeps rotating.
struct PCB* getProcessroundRobbin(){
    struct ReadyQueue* queue = schedulerAccess.readyQueue;
    if(queue == NULL)
        return NULL;

    return queue->pcb;
}
