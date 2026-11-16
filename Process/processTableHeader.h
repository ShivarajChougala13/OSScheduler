#ifndef PROCESSTABLEHEADER_H
#define PROCESSTABLEHEADER_H

#include <stdbool.h>
#include "./PCBHeader.h"

struct processTable{
    int pid;
    state state;
    int priority;
    PCB* pcb;
    struct processTable *next;
};

void addNewProcess(PCB* pcb);
void PrintProcessTable();
bool removeProcessFromProcessTable(struct PCB* pcb);

#endif