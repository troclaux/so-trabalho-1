#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef struct{
  int pid;
  int ppid;
  int start; //tempo em que o processo pede para ter acesso ao CPU
  int end;  //tempo em que o processo terminou de ser computado
  int status;  //diz se o processo esta sendo computado pelo CPU ou não
  int service;  //tempo necessario para computar completamente esse processo
  int processedTU;  //unidades de tempo que ja foram computadas do processo ate o momento atual
  int *diskRequests;  //requisicoes dos io's
  int *tapeRequests;
  int *printerRequests;
  bool finished;
} Process;

bool checkIntInArray(int number, int *array) {
  int i, length;
  length = array[0];
  for (i = 1; i <= length; i++) {
    if (array[i] == number) {
      return true;
    }
  }
  return false;
}

//funcao para definir a duracao das operacoes de entrar e saida aleatoriamente
int *generateIOTime(int serviceTime, int *IOArray1, int *IOArray2) {
  if (serviceTime == 0) {
    return NULL;
  }
  bool exists;
  int temp, quantity;
  int *IOTimeArray;
  if (serviceTime <= 3) {
    quantity = rand() % 1;
  } else if (serviceTime <= 6) {
    quantity = rand() % 2;
  } else if (serviceTime <= 10) {
    quantity = rand() % 3;
  } else {
    quantity = rand() % 4;
  }
  IOTimeArray = (int *)calloc(quantity + 1, sizeof(int));
  IOTimeArray[0] = quantity;
  for (int j = 1; j < quantity + 1; j++) {
    temp = rand() % serviceTime;
    if (checkIntInArray(temp, IOTimeArray) || 
        checkIntInArray(temp, IOArray1) || 
        checkIntInArray(temp, IOArray2)) {
      exists = true;
      while (exists) {
        temp = rand() % serviceTime;
        if (checkIntInArray(temp, IOTimeArray) || 
            checkIntInArray(temp, IOArray1) || 
            checkIntInArray(temp, IOArray2)) {
          continue;
        }
        exists = false;
      }
    }
    IOTimeArray[j] = temp;
  }
  return IOTimeArray;
}

Process newProcess(int pid, int ppid) {
  Process process;
  process.pid = pid;
  process.ppid = ppid;
  process.start = rand() % 45;
  process.end = -1;
  process.processedTU = 0;

  process.finished = false;
  process.status = 0;
  process.service = rand() % 43 + 2;
  process.diskRequests = (int *)calloc(4, sizeof(int));
  process.tapeRequests = (int *)calloc(4, sizeof(int));
  process.printerRequests = (int *)calloc(4, sizeof(int));
  process.diskRequests = generateIOTime(process.service, process.tapeRequests, process.printerRequests);
  process.tapeRequests = generateIOTime(process.service, process.diskRequests, process.printerRequests);
  process.printerRequests = generateIOTime(process.service, process.diskRequests, process.tapeRequests);

  return process;
}

//funcao para liberar a memoria alocada para as requisicoes das operacoes de entrada e saida
void killProcess(Process *process) {
  free(process->diskRequests);
  free(process->tapeRequests);
  free(process->printerRequests);
}

//funcao que vai retornar o endereco de um processo especifico em uma fila de processos
Process *getProcessByPID(int pid, Process *processes, int size){
  for (int i = 0; i < size; i++){
    if (processes[i].pid == pid){
      return &processes[i];
    }
  }
}