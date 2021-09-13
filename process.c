#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef struct{
  int pid;
  int ppid;
  int start;
  int end;
  int status;
  int processedTU;
  int service;
  int *diskRequests;
  int *tapeRequests;
  int *printerRequests;
  bool finished;
} Process;

static const Process EmptyProcess;

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
  process.start = rand() % 32;
  process.end = -1;
  process.processedTU = 0;

  process.finished = false;
  process.status = 0;
  process.service = rand() % 30 + 2;
  process.diskRequests = (int *)calloc(4, sizeof(int));
  process.tapeRequests = (int *)calloc(4, sizeof(int));
  process.printerRequests = (int *)calloc(4, sizeof(int));
  process.diskRequests = generateIOTime(process.service, process.tapeRequests, process.printerRequests);
  process.tapeRequests = generateIOTime(process.service, process.diskRequests, process.printerRequests);
  process.printerRequests = generateIOTime(process.service, process.diskRequests, process.tapeRequests);

  return process;
}

void printProcess(Process process) {
  int sizePrinterRequests, sizeTapeRequests, sizeDiskRequests, i;
  if (process.diskRequests != NULL) {
    sizeDiskRequests = process.diskRequests[0];
  } else {
    sizeDiskRequests = 0;
  }
  if (process.tapeRequests != NULL) {
    sizeTapeRequests = process.tapeRequests[0];
  } else {
    sizeTapeRequests = 0;
  }
  if (process.printerRequests != NULL) {
    sizePrinterRequests = process.printerRequests[0];
  } else {
    sizePrinterRequests = 0;
  }
  printf("PID: %d | ", process.pid);
  printf("PPID: %d | ", process.ppid);
  printf("status: %d | ", process.status);
  printf("start: %d | ", process.start);
  printf("end: %d | ", process.end);
  printf("processeTU: %d | ", process.processedTU);
  printf("service: %d | ", process.service);
  if (sizeDiskRequests > 0) {
    printf("disk: ");
    printf("[%d]: ", sizeDiskRequests);
    for(i = 1 ; i <= sizeDiskRequests ; i++){
      if (i == sizeDiskRequests) {
        printf("%d", process.diskRequests[i]);
      } else {
        printf("%d, ", process.diskRequests[i]);
      }
    }
    printf(" | ");
  }
  if (sizePrinterRequests > 0) {
    printf("printer: ");
    printf("[%d]: ", sizePrinterRequests);
    for(i = 1 ; i <= sizePrinterRequests ; i++){
      if (i == sizePrinterRequests) {
        printf("%d", process.printerRequests[i]);
      } else {
        printf("%d, ", process.printerRequests[i]);
      }
    }
    printf(" | ");
  }
  if (sizeTapeRequests > 0) {
    printf("tape: ");
    printf("[%d]: ", sizeTapeRequests);
    for(i = 1 ; i <= sizeTapeRequests ; i++){
      if (i == sizeTapeRequests) {
        printf("%d", process.tapeRequests[i]);
      } else {
        printf("%d, ", process.tapeRequests[i]);
      }
    }
    printf(" |");
  }
  printf("\n");
}

void killProcess(Process *process) {
  free(process->diskRequests);
  free(process->tapeRequests);
  free(process->printerRequests);
}

Process *getProcessByPID(int pid, Process *processes, int size){
  for (int i = 0; i < size; i++){
    if (processes[i].pid == pid){
      return &processes[i];
    }
  }
}