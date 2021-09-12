#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue.h"

typedef struct{
  int numberOfProcesses;
  Process* allProcesses;
  Queue* highPriority;
  Queue* lowPriority;
  Queue* diskQueue;
  unsigned diskTimer;
  Queue* tapeQueue;
  unsigned tapeTimer;
  Queue* printerQueue;
  unsigned printerTimer;
  int quantum;
  int programCounter;
  int runningProcessPID;
} Scheduler;


Scheduler newScheduler(int quantum, int numberOfProcesses, Process* allProcesses, unsigned diskTimer, unsigned tapeTimer, unsigned printerTimer){
  Scheduler scheduler;
  scheduler.allProcesses = allProcesses;
  scheduler.highPriority = newQueue(numberOfProcesses);
  scheduler.lowPriority = newQueue(numberOfProcesses);
  scheduler.diskQueue = newQueue(numberOfProcesses);
  scheduler.tapeQueue = newQueue(numberOfProcesses);
  scheduler.printerQueue = newQueue(numberOfProcesses);
  scheduler.programCounter = 0;
  scheduler.quantum = quantum;
  scheduler.runningProcessPID = 0;
  
  if (diskTimer == 0) {
    scheduler.diskTimer = (rand() % 5) + 2;
  } else {
    scheduler.diskTimer = diskTimer;
  }
  if (tapeTimer == 0) {
    scheduler.tapeTimer = (rand() % 5) + 2;
  } else {
    scheduler.tapeTimer = tapeTimer;
  }
  if (printerTimer == 0) {
    scheduler.printerTimer = (rand() % 5) + 2;
  } else {
    scheduler.printerTimer = printerTimer;
  }

  return scheduler;
}


//TODO
void updateProcesses(Scheduler *scheduler){
  Process *process;

  if(scheduler->runningProcessPID){
    //começo
    process = getProcessByPID(scheduler->runningProcessPID, scheduler->allProcesses);
    process->processedTU++;
  }else{
    if (!isEmpty(scheduler->highPriority)){
      //computa os processos de alta prioridade

    }
    else if (!isEmpty(scheduler->lowPriority)){
      //computa processos de baixa prioridade
    }else{

    }
  }
}


bool unfinishedProcessExist(Scheduler *scheduler){
  for(int i = 0 ; i < scheduler->numberOfProcesses ; i++){
    if(!scheduler->allProcesses[i].finished){
      return true;
    }
  }
  return false;
}

void computeExecutionCycles(Scheduler *scheduler, Process* allProcesses){
  while(unfinishedProcessExist(scheduler)){
    updateProcesses(scheduler);
    scheduler->programCounter++;
  }
}

void killScheduler(Scheduler *scheduler) {
  free(scheduler->highPriority);
  free(scheduler->lowPriority);
  free(scheduler->diskQueue);
  free(scheduler->tapeQueue);
  free(scheduler->printerQueue);
}