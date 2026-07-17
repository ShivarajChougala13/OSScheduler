#include "./PCBHeader.h"

struct processTable{
    int pid;
    state state;
    int priority;
    // int ppi;
    PCB* pcb;   // store just pcb addres
    struct processTable *next;
};

void addNewProcess(PCB* pcb);
void PrintProcessTable();