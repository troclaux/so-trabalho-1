#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "scheduler.c"

int main() {

  int seed = 48271; // http://c-faq.com/lib/rand.html
  srand((time(NULL)*seed)); // should be called just once on the entire code base

  Scheduler scheduler;
  Process *processes;

  int quantum;
  int numProcessos;

  printf("Insira o numero de processos desejado: ");
  scanf("%d", &numProcessos);

  while(numProcessos > 100){
    printf("Por favor insira um número menor ou igual a 100: ");
    scanf("%d", &numProcessos);
  }

  printf("Insira o quantum desejado para o escalonador:");
  scanf("%d", &quantum);

  processes = (Process *)calloc(numProcessos,sizeof(Process));
  
  //gera tempos aleatorios para operacoes de io
  int quantumDisk = (rand() % 3) + 1;
  int quantumTape = (rand() % 4) + 1;
  int quantumPrinter = (rand() % 5) + 1;

  for(int i = 0; i< numProcessos ; i++) {
    processes[i] = newProcess(i + 100,1);
  }

  scheduler = newScheduler(quantum, numProcessos, processes, quantumDisk, quantumTape, quantumPrinter);
  computeExecutionCycles(&scheduler, processes);
  //libera memoria
  free(processes);
  killScheduler(&scheduler);

  return 0;
}