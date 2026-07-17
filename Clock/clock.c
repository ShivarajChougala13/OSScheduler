#include "clock.h"
#include "../Queues/jobQueue.h"
#include "../Queues/readyQueue.h"
#include "../Algorithms/longTermScheduler.h"

static int shmid;
#define CLOCK_KEY 100
#define ARR_KEY 200

int* getClock(){
    shmid=shmget(CLOCK_KEY,sizeof(int),IPC_CREAT|0666); // create shared memory of size int  this retuns shared memory ID like 1 4 17 100
                                        // create new shared memory blocks if there currently no shared memory blocks
    if(shmid==-1) {
        printf("not memory id created!\n");
        exit(EXIT_FAILURE);
    }
    int* clock = (int*)shmat(shmid,NULL,0);     // attach shared memory. shared memory is inside the kernel attaches it to my process// means give that shmid to a ID from my process 
    if(clock==(void*)-1){
        printf("clock did not initialized\n");
        exit(1);
    }
        return clock;
}

void killClock(){

    shmdt(clk);     // detatch 
    shmctl(shmid, IPC_RMID, NULL);  // remove shared memory 
    printf("clock memory is released\n");
    return ;
}

// if clock checking for continuously then it is called - polling or busy waiting.. (add sleep(1 sec))
void scheduling(){

    // jobAdmit();     // function to admit the jobs
    // start ready queue... 
    // start process generator that has its own loop to continuous process generation

    startJobQueue();
    startReadyQueue();  
    // startLongTermSchedler();
    // startFCFS();

    int prev=-1;
    while(1){
        if(*clk!=prev){
        //check new process arraval 
        //run scheduler
        //process life handling
        
        longTermScheduler();
        FCFS();
        prev=*clk;
        }
        usleep(1000); // 1ms -> ( 1 microsecond is 1 second )
    }
}

void runClock(){
    while(1){
        sleep(1);
        (*clk)++;
    }
}

void startSystem(){
    clk=getClock(); // both parent and child share the attached memory 
    *clk=0;
    int pid=fork();
    if(pid==0){
        // runClock();
        runClock();
    }
    else if(pid>0){
        scheduling();
    }
    else{
        perror("error while fork\n");
        exit(1);
    }
}

int main(){
    printf("Initializing clock\n");
    startSystem();
    return 0;
}
