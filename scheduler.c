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
  int quantumChecker;
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

void preempt(Scheduler *schedule, int pid){
  process = getProcessByPID(scheduler->runningProcessPID, scheduler->allProcesses);

  return;
}

//TODO
//falta tratar a preempçao
//falta tratar as filas pros IOs
void updateScheduler(Scheduler *scheduler){
  Process process;

  //se o escalonador já tem algum processo computando
  if(scheduler->runningProcessPID){
    //começo
    process = getProcessByPID(scheduler->runningProcessPID, scheduler->allProcesses, scheduler->numberOfProcesses);
    process.processedTU++;
    scheduler->quantumChecker++;
    if(process.processedTU == process.service){
      finishProcess(process);
    } else if (checkIntInArray(process.processedTU, process.diskRequests)) {
      enqueue(scheduler.diskQueue, process);
    } else if (checkIntInArray(process.processedTU, process.tapeRequests)) {
      enqueue(scheduler.tapeQueue, process);
    } else if (checkIntInArray(process.processedTU, process.printerRequests)) {
      enqueue(scheduler.printerQueue, process);
    }
  //se nao estiver computando
  }else{
    if (!isEmpty(scheduler->highPriority)){
      //computa os processos de alta prioridade
      //desempilha highpriority e define o running process como o processo desempilhado 
      scheduler->runningProcessPID = dequeue(scheduler->highPriority).pid;
    }
    else if (!isEmpty(scheduler->lowPriority)){
      //computa processos de baixa prioridade
      scheduler->runningProcessPID = dequeue(scheduler->lowPriority).pid;
    }else{
      //se ambas as filas estiverem vazias
      return;
    }
  }
}


bool unfinishedProcessesExist(Scheduler *scheduler){
  for(int i = 0 ; i < scheduler->numberOfProcesses ; i++){
    if(!scheduler->allProcesses[i].finished){
      return true;
    }
  }
  return false;
}

void computeExecutionCycles(Scheduler *scheduler, Process* allProcesses){
  while(unfinishedProcessesExist(scheduler)){
    getchar();
    updateScheduler(scheduler);
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
