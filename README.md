# OS Scheduler Simulator (C)

A CPU scheduling simulator that follows the life of a process:

**Input file -> Job Queue -> Long-term scheduler -> Ready Queue (capacity 5) -> Short-term scheduler -> CPU -> Completed**
and, when a process asks for I/O: **CPU -> Blocked Queue -> Ready Queue**

A separate clock process (created with `fork()`) increments a counter kept in **shared memory** every second and
gives a signal on a **semaphore**. The scheduler sleeps on that semaphore, so it wakes up only when a tick happens (no polling).
On every tick it admits new jobs, finishes I/O, fills the ready queue and runs the CPU for one unit.

## Build and run

Run from the project folder.

```
make
./scheduler                                 # uses Input_Outputs/input.txt (no I/O)
./scheduler Input_Outputs/input_io.txt      # same processes, some of them do I/O
```

Choose a policy: `1 - FCFS , 2 - SFJ , 3 - RoundRobbin , 4 - priority`.
The simulation stops by itself when all processes are completed and prints a summary table.
Ctrl+C also works, it releases the shared memory and the semaphore before exiting (check with `ipcs -m` and `ipcs -s`).

## Policies

| Policy | How the next process is chosen | Preemptive |
|---|---|---|
| FCFS | head of the ready queue | No |
| SFJ | smallest remaining time | No |
| RoundRobbin | head of the ready queue, time quantum = 2 (`TIME_QUANTUM` in `roundRobbin.h`) | Yes |
| Priority | smaller number = higher priority | No |

## Input file

First line is the column names, then one process per line:

```
pid state arrival_time system_arrival_time waiting_time burst_time start_time remaining_time end_time start_address size priority io_after io_burst
```

- `state`, `system_arrival_time`, `waiting_time`, `start_time`, `end_time` start as 0 and are filled by the scheduler.
- `remaining_time` should be equal to `burst_time`.
- `io_after` = number of CPU ticks the process runs before it asks for I/O, `io_burst` = how many ticks the I/O takes.
  `0 0` means the process never does I/O. Each process does at most one I/O request.

## Metrics

- Turnaround time = end time - arrival time in the system
- Waiting time = turnaround time - burst time - I/O time (time spent in blocked queue is not waiting for the CPU)
- Response time = first time on the CPU - arrival time in the system
- Throughput = completed processes / total time
- CPU utilization = busy ticks / total time
- Context switches = number of times the CPU is given to a process

## Result on the sample input (10 processes, no I/O)

| Policy | Avg turnaround | Avg waiting | Avg response | Context switches |
|---|---|---|---|---|
| FCFS | 18.60 | 13.30 | 13.30 | 10 |
| SFJ | 15.30 | 10.00 | 10.00 | 10 |
| RoundRobbin (q=2) | 23.20 | 17.90 | 6.10 | 27 |
| Priority | 18.40 | 13.10 | 13.10 | 10 |

Throughput is the same (0.189) for all policies here because the CPU is never idle, so all work finishes at time 53.

## Folder structure

```
Main/          main.c            entry point, asks the policy and starts the system
Clock/         clock.c           shared memory + semaphore clock, fork, main scheduling loop
Process/       PCB.c, processTable.c
Queues/        jobQueue.c, readyQueue.c, blockedQueue.c
Algorithms/    longTermScheduler.c, shortTermScheduler.c, Policy/ (FCFS, SFJ, roundRobbin, priority)
CPU/           cpu.c             executes one unit per tick, quantum, I/O request, statistics
Input_Outputs/ input.txt, input_io.txt
```

## Future work (from plan.png)

Memory manager, dispatcher, preemptive SJF (SRTF), priority aging, multilevel feedback queue.
