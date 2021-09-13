#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "scheduler.c"


int test1() {
  int seed = 48271; // http://c-faq.com/lib/rand.html
  srand((time(NULL)*seed)); // should be called just once on the entire code base
  int pid;
  Process *process;
  Process *processes;
  processes = (Process *)calloc(3, sizeof(Process));
  Queue *queue = newQueue(3);

  for(int i = 0; i< 3 ; i++){
    Process temp = newProcess(100 + i, 1);
    processes[i] = temp;
    enqueue(queue, temp.pid);
  }

  for(int i = 0; i< 3 ; i++){
    pid = dequeue(queue);
    process = getProcessByPID(pid, processes, 3);
    //printProcess(process);
    killProcess(process);
  }
  return 0;
}

int main() {
  int seed = 48271; // http://c-faq.com/lib/rand.html
  srand((time(NULL)*seed)); // should be called just once on the entire code base
  Scheduler scheduler;
  Process *processes;
  int numProcessos = 20;

  processes = (Process *)calloc(numProcessos,sizeof(Process));

  for(int i = 0; i< numProcessos ; i++) {
    processes[i] = newProcess(i + 100,1);
  }

  scheduler = newScheduler(5, numProcessos, processes, 2, 3, 4);
  computeExecutionCycles(&scheduler, processes);
  free(processes);
  killScheduler(&scheduler);

  return 0;
}