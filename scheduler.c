#include <stdio.h>
#include "queue.h"

typedef struct{
  Queue* highPriority;
  Queue* lowPriority;
  int quantum;
  int programCounter;
} Scheduler;

Scheduler newScheduler(int quantum, int maxCapacity){
  Scheduler scheduler;
  //o tamanho da fila tem q ser suficiente para caber todos os process (inclusive os que retornam de io) 
  Queue* emptyQueue = newQueue(maxCapacity);
  scheduler.highPriority = emptyQueue;
  scheduler.lowPriority = emptyQueue;
  scheduler.programCounter = 0;
  scheduler.quantum = quantum;

  return scheduler;
}

