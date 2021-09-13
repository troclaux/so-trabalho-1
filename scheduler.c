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
  int diskQuantum;
  Queue* tapeQueue;
  int tapeQuantum;
  Queue* printerQueue;
  int printerQuantum;
  int quantum;
  int quantumChecker;
  int programCounter;
  int runningProcessPID;
  int diskCount;
  int tapeCount;
  int printerCount;
} Scheduler;


Scheduler newScheduler(int quantum, int numberOfProcesses, Process* allProcesses, unsigned diskQuantum, unsigned tapeQuantum, unsigned printerQuantum){
  Scheduler scheduler;
  scheduler.quantum = quantum;
  scheduler.numberOfProcesses = numberOfProcesses;
  scheduler.allProcesses = allProcesses;
  scheduler.highPriority = newQueue(numberOfProcesses);
  scheduler.lowPriority = newQueue(numberOfProcesses);
  scheduler.diskQueue = newQueue(numberOfProcesses);
  scheduler.tapeQueue = newQueue(numberOfProcesses);
  scheduler.printerQueue = newQueue(numberOfProcesses);
  scheduler.programCounter = 0;
  scheduler.runningProcessPID = 0;
  scheduler.diskCount = 0;
  scheduler.tapeCount = 0;
  scheduler.printerCount = 0;
  
  if (diskQuantum == 0) {
    scheduler.diskQuantum = (rand() % 5) + 2;
  } else {
    scheduler.diskQuantum = diskQuantum;
  }
  if (tapeQuantum == 0) {
    scheduler.tapeQuantum = (rand() % 5) + 2;
  } else {
    scheduler.tapeQuantum = tapeQuantum;
  }
  if (printerQuantum == 0) {
    scheduler.printerQuantum = (rand() % 5) + 2;
  } else {
    scheduler.printerQuantum = printerQuantum;
  }

  return scheduler;
}

void processIOQueue(Scheduler *scheduler) {
  int processPid;
  if(!isEmpty(scheduler->diskQueue)) {
    if(scheduler->diskCount == scheduler->diskQuantum) {
      processPid = dequeue(scheduler->diskQueue);
      enqueue(scheduler->lowPriority, processPid);
      scheduler->diskCount = 0;
    } else {
      scheduler->diskCount++;
    }
  }

  if(!isEmpty(scheduler->tapeQueue)) {
    if(scheduler->tapeCount == scheduler->tapeQuantum) {
      processPid = dequeue(scheduler->tapeQueue);
      enqueue(scheduler->highPriority, processPid);
      scheduler->tapeCount = 0;
    }else{
      scheduler->tapeCount++;
    }
  }

  if(!isEmpty(scheduler->printerQueue)) {
    if(scheduler->printerCount == scheduler->printerQuantum) {
      processPid = dequeue(scheduler->printerQueue);
      enqueue(scheduler->highPriority, processPid);
      scheduler->printerCount = 0;
    } else {
      scheduler->printerCount++;
    }
  }
}

void updateRunningProcess(Scheduler *scheduler) {
  Process *process;
  if (!isEmpty(scheduler->highPriority)){
    //computa os processos de alta prioridade
    //desempilha highpriority e define o running process como o processo desempilhado 
    scheduler->runningProcessPID = dequeue(scheduler->highPriority);
    process = getProcessByPID(scheduler->runningProcessPID, scheduler->allProcesses, scheduler->numberOfProcesses);
    process->status = 1;
    process->processedTU++;
    scheduler->quantumChecker++;
  }
  else if (!isEmpty(scheduler->lowPriority)){
    //computa processos de baixa prioridade
    scheduler->runningProcessPID = dequeue(scheduler->lowPriority);
    process = getProcessByPID(scheduler->runningProcessPID, scheduler->allProcesses, scheduler->numberOfProcesses);
    process->status = 1;
    process->processedTU++;
    scheduler->quantumChecker++;
  }
}

void updateScheduler(Scheduler *scheduler) {
  processIOQueue(scheduler);
  Process *process;

  for(int i = 0 ; i < scheduler->numberOfProcesses ; i++) {

    process = getProcessByPID(scheduler->allProcesses[i].pid, scheduler->allProcesses, scheduler->numberOfProcesses);

    if(process->start == scheduler->programCounter){
      enqueue(scheduler->highPriority, process->pid);
    }
  }
  //se o escalonador já tem algum processo computando
  if(scheduler->runningProcessPID) {
    //começo
    process = getProcessByPID(scheduler->runningProcessPID, scheduler->allProcesses, scheduler->numberOfProcesses);
    if(process->processedTU == process->service){
      process->finished = true;
      process->end = scheduler->programCounter;
      process->status = 0;
      scheduler->quantumChecker = 0;
      scheduler->runningProcessPID = 0;
      updateRunningProcess(scheduler);
    } else if (checkIntInArray(process->processedTU, process->diskRequests)) {
      enqueue(scheduler->diskQueue, process->pid);
      process->status = 0;
      scheduler->quantumChecker = 0;
      scheduler->runningProcessPID = 0;
      updateRunningProcess(scheduler);
    } else if (checkIntInArray(process->processedTU, process->tapeRequests)) {
      enqueue(scheduler->tapeQueue, process->pid);
      process->status = 0;
      scheduler->quantumChecker = 0;
      scheduler->runningProcessPID = 0;
      updateRunningProcess(scheduler);
    } else if (checkIntInArray(process->processedTU, process->printerRequests)) {
      enqueue(scheduler->printerQueue, process->pid);
      process->status = 0;
      scheduler->quantumChecker = 0;
      scheduler->runningProcessPID = 0;
      updateRunningProcess(scheduler);
    } else if (scheduler->quantumChecker == scheduler->quantum) { //checa se precisa fazer preempcao
      process->status = 0;
      scheduler->quantumChecker = 0;
      scheduler->runningProcessPID = 0;
      enqueue(scheduler->lowPriority, process->pid);
      updateRunningProcess(scheduler);
    } else {
      process->processedTU++;
      scheduler->quantumChecker++;
    }

  //se nao estiver computando
  } else {
    updateRunningProcess(scheduler);
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

void printScheduler(Scheduler *scheduler) {
  int i, j;
  Process process;
  printf("\e[1;1H\e[2J");
  printf("U.T.: %d\n", scheduler->programCounter);
  printf("Quantum: %d\n", scheduler->quantum);
  printf("PID\tPPID\tStart\tEnd\tStatus\tProcessedTU\tService\t\tDisk\t\tTape\t\tPrinter\n");
  for (i = 0 ; i < scheduler->numberOfProcesses ; i++) {
    process = scheduler->allProcesses[i];
    printf("%d\t%d\t%d\t%d\t%d\t%d\t\t%d\t\t", process.pid, process.ppid, process.start, process.end, process.status, process.processedTU, process.service);
    for (j = 1 ; j <= process.diskRequests[0] ; j++) {
      if (process.diskRequests[j] < 10) {
        printf("0");
      }
      printf("%d ", process.diskRequests[j]);
    }
    if (process.diskRequests[0] > 2) {
      printf("\t");
    } else {
      printf("\t\t");
    }
    for (j = 1 ; j <= process.tapeRequests[0] ; j++) {
      if (process.tapeRequests[j] < 10) {
        printf("0");
      }
      printf("%d ", process.tapeRequests[j]);
    }
    if (process.tapeRequests[0] > 2) {
      printf("\t");
    } else {
      printf("\t\t");
    }
    for (j = 1 ; j <= process.printerRequests[0] ; j++) {
      if (process.printerRequests[j] < 10) {
        printf("0");
      }
      printf("%d ", process.printerRequests[j]);
    }
    printf("\n");
  }
  printf("\n");
  printf("Alta prioridade: ");
  printQueue(scheduler->highPriority);
  printf("\n");
  printf("Baixa prioridade: ");
  printQueue(scheduler->lowPriority);
  printf("\n");
  printf("Fila Disco: ");
  printQueue(scheduler->diskQueue);
  printf("\n");
  printf("Fila Fita: ");
  printQueue(scheduler->tapeQueue);
  printf("\n");
  printf("Fila Impressora: ");
  printQueue(scheduler->printerQueue);
  printf("\n\n");
  printf("Contador Quantum: %d\n", scheduler->quantumChecker);
}

void computeExecutionCycles(Scheduler *scheduler, Process* allProcesses){
  printScheduler(scheduler);
  while(unfinishedProcessesExist(scheduler)){
    getchar();
    updateScheduler(scheduler);
    scheduler->programCounter++;
    printScheduler(scheduler);
  }
  printScheduler(scheduler);
}

void killScheduler(Scheduler *scheduler) {
  int i;
  for (i = 0 ; i < scheduler->numberOfProcesses ; i++) {
    killProcess(&scheduler->allProcesses[i]);
  }
  killQueue(scheduler->highPriority);
  killQueue(scheduler->lowPriority);
  killQueue(scheduler->diskQueue);
  killQueue(scheduler->tapeQueue);
  killQueue(scheduler->printerQueue);
}