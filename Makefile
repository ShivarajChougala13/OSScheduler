CC = gcc
CFLAGS = -Wall

SRC = Main/main.c \
      Clock/clock.c \
      CPU/cpu.c \
      Process/PCB.c \
      Process/processTable.c \
      Queues/jobQueue.c \
      Queues/readyQueue.c \
      Queues/blockedQueue.c \
      Algorithms/longTermScheduler.c \
      Algorithms/shortTermScheduler.c \
      Algorithms/Policy/FCFS.c \
      Algorithms/Policy/SFJ.c \
      Algorithms/Policy/roundRobbin.c \
      Algorithms/Policy/priority.c

scheduler: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o scheduler

clean:
	rm -f scheduler
