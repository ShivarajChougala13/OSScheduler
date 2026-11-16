#include "./main.h"
#include "../Clock/clock.h"
#include "../Algorithms/shortTermScheduler.h"

struct SchedulerAccess schedulerAccess = {0};
char* inputFile = "./Input_Outputs/input.txt";

// ./scheduler                              -> uses Input_Outputs/input.txt
// ./scheduler Input_Outputs/input_io.txt   -> uses the given file
int main(int argc, char* argv[]){
    if(argc > 1){
        inputFile = argv[1];
    }
    selectPolicy();     // ask the policy first, then the clock starts
    startSystem();
    return 0;
}
