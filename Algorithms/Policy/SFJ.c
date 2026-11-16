#include "../../Process/PCBHeader.h"
#include "../../Main/main.h"

struct PCB* getProcessSFJ(){
    struct ReadyQueue* queue = schedulerAccess.readyQueue;
    if(queue == NULL)
        return NULL;

    struct ReadyQueue* best = queue;
    struct ReadyQueue* current = queue->next;

    while(current != NULL){
        if(current->pcb->remaining_time < best->pcb->remaining_time){
            best = current;
        }
        current = current->next;
    }

    return best->pcb;
}