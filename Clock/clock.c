#include "clock.h"
#include "../Main/main.h"
#include "../Queues/jobQueue.h"
#include "../Queues/readyQueue.h"
#include "../Algorithms/longTermScheduler.h"
#include "../CPU/cpu.h"

static int shmid;
static int semid;           // semaphore : clock process gives one signal on every tick
static int clockPid = 0;    // process id of the child which runs the clock
int *clk = NULL;
#define CLOCK_KEY 100
#define ARR_KEY 200
#define SEM_KEY 300

int* getClock(){
    shmid=shmget(CLOCK_KEY,sizeof(int),IPC_CREAT|0666); // create shared memory of size int  this retuns shared memory ID like 1 4 17 100                          // create new shared memory blocks if there currently no shared memory blocks
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

// create semaphore with value 0. parent waits on it, clock process increases it on every tick
void getSemaphore(){
    semid=semget(SEM_KEY,1,IPC_CREAT|0666);
    if(semid==-1){
        perror("semaphore not created\n");
        exit(EXIT_FAILURE);
    }
    // if an old semaphore is left from a previous run make its value 0
    struct sembuf drain={0,-1,IPC_NOWAIT};
    while(semctl(semid,0,GETVAL)>0){
        semop(semid,&drain,1);
    }
}

// clock process : one tick happened (semaphore value + 1)
void sendTick(){
    struct sembuf up={0,1,0};
    semop(semid,&up,1);
}

// scheduler process : sleeps here until next tick comes (semaphore value - 1), no busy waiting
void waitForTick(){
    struct sembuf down={0,-1,0};
    if(semop(semid,&down,1)==-1){
        perror("semaphore wait failed\n");
        stopSystem();
    }
}

void killClock(){

    shmdt(clk);     // detatch 
    shmctl(shmid, IPC_RMID, NULL);  // remove shared memory 
    semctl(semid, 0, IPC_RMID);     // remove semaphore
    printf("clock memory and semaphore are released\n");
    return ;
}

// stop the clock process and release the shared memory
void stopSystem(){
    if(clockPid > 0){
        kill(clockPid, SIGKILL);    // stop the clock child
    }
    killClock();
    exit(0);
}

// Ctrl+C comes here, so the shared memory is not left in the system (check with ipcs -m)
void handleSignal(int sig){
    printf("\nstopped by user\n");
    stopSystem();
}

// parent waits on the semaphore, so it only wakes up when the clock ticks (no polling loop)
void scheduling(){

    startJobQueue();
    startReadyQueue();  
    startBlockedQueue();
    jobAdmit();         // read all the processes from the input file

    int prev=-1;
    while(1){
        if(*clk!=prev){
        //check new process arrival 
        //run scheduler
        //process life handling
        
        checkNewArrivals();     // arrival time reached -> job queue
        updateBlockedQueue();   // i/o finished -> ready queue
        longTermScheduler();    // job queue -> ready queue
        execute();              // short term scheduler gives the cpu to a process
        prev=*clk;

        // process table is empty means every process is completed
        if(schedulerAccess.processTable==NULL){
            printStatistics();
            stopSystem();
        }
        }
        waitForTick();  // sleep until the clock process gives next tick
    }
}

void runClock(){
    while(1){
        sleep(1);
        (*clk)++;
        sendTick();     // wake up the scheduler
    }
}

void startSystem(){
    clk=getClock(); // both parent and child share the attached memory 
    *clk=0;
    getSemaphore();  // created before fork so both processes use the same semaphore
    int pid=fork();
    if(pid==0){
        // runClock();
        runClock();
    }
    else if(pid>0){
        clockPid=pid;
        signal(SIGINT,handleSignal);    // only parent handles Ctrl+C
        scheduling();
    }
    else{
        perror("error while fork\n");
        exit(1);
    }
}

int clock_main(){
    printf("Initializing clock\n");
    startSystem();
    return 0;
}
