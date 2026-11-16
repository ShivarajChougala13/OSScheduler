#ifndef SHORTTERMSCHEDULER
#define SHORTTERMSCHEDULER

#define POLICY_FCFS 1
#define POLICY_SFJ 2
#define POLICY_ROUNDROBBIN 3
#define POLICY_PRIORITY 4

void selectPolicy();
int getPolicy();
struct PCB* getProcess();

#endif
